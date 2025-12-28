#include "UseSpell_Thread.h"


void UseSpell_Thread::run() {
    if (m_spells.empty()) return;
    auto client_version = proto->getClientVersion();
    auto second_found = false;
    while (!isInterruptionRequested()) {
        if (proto->isAttacking()) {
            auto localPlayer = proto->getLocalPlayer();
            auto current_hp = proto->getHealth(localPlayer);
            auto max_hp = proto->getMaxHealth(localPlayer);
            auto current_mp = proto->getMana(localPlayer);
            double current_hp_pc = (current_hp / max_hp) * 100;
            Position playerPos = proto->getPosition(localPlayer);
            for (auto spell : m_spells) {
                auto spectators = proto->getSpectators(playerPos, false);
                for (auto spectator : spectators) {
                    if (proto->isPlayer(spectator)) continue;
                    auto specName = proto->getCreatureName(spectator);
                    std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
                    if (specName != spell.targets && spell.targets != "*") continue;
                    auto targetPos = proto->getPosition(spectator);
                    int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(targetPos.x)),
                        std::abs(static_cast<int>(playerPos.y) - static_cast<int>(targetPos.y)));
                    if (spell.dist > 0 && dist > spell.dist) continue;
                    if (current_hp_pc < spell.minHp || current_mp < spell.minMp) continue;
                    if (spell.count > 1) {
                        spell.count -=1;
                    }
                    if (spell.count <= 1)
                    {
                        if (spell.option == "Say")  proto->talk(spell.name);
                        if (spell.option == "Use on Target") {
                            int itemId = std::stoi(spell.name);
                            if (client_version >= 800) { // Hotkeys Available
                                proto->useInventoryItemWith(itemId, spectator);
                            } else { // No hotkeys
                                auto containers = proto->getContainers();
                                bool found = false;
                                for (auto container : containers) {
                                    auto items = proto->getItems(container);
                                    for (auto item : items) {
                                        if (proto->getItemId(item) == itemId) {
                                            auto tile = proto->getTile(targetPos);
                                            auto topThing = proto->getTopUseThing(tile);
                                            auto specPos = proto->getPosition(spectator);
                                            found = true;
                                            second_found = true;
                                            if (specPos.x != targetPos.x || specPos.y != targetPos.y) break;
                                            proto->useWith(item, topThing);
                                            break;
                                        }
                                    }
                                    if (found) break;
                                }
                            }
                        }
                        if (spell.option == "Use on Yourself") {
                            int itemId = std::stoi(spell.name);
                            if (client_version >= 800) { // Hotkeys Available
                                proto->useInventoryItem(itemId);
                            } else { // No hotkeys
                                bool found = false;
                                auto containers = proto->getContainers();
                                for (auto container : containers) {
                                    auto items = proto->getItems(container);
                                    for (auto item : items) {
                                        if (proto->getItemId(item) == itemId) {
                                            proto->useWith(item, localPlayer);
                                            found = true;
                                            second_found = true;
                                            break;
                                        }
                                    }
                                    if (found) break;
                                }
                            }
                        }
                        break;
                    }
                    if (second_found) {
                        second_found = false;
                        break;
                    }
                }
            }
        }
        msleep(100);
    }
}
