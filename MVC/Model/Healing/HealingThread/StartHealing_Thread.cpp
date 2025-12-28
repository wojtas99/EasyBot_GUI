#include "StartHealing_Thread.h"


void StartHealing_Thread::run()
{
    if (heals.empty()) return;
    size_t index = 0;
    auto client_version = proto->getClientVersion();
    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto current_hp = proto->getHealth(localPlayer);
        auto current_mp = proto->getMana(localPlayer);
        auto max_hp = proto->getMaxHealth(localPlayer);
        auto max_mp = proto->getMaxMana(localPlayer);

        double current_hp_pc = current_hp / max_hp * 100;
        double current_mp_pc = current_mp / max_mp * 100;

        auto heal = heals[index];
        if (heal.condition == "HP%") {
            if (heal.below >= current_hp_pc && current_hp_pc >= heal.above &&
                current_mp >= heal.minimum) {
                if (heal.action == "Say") {
                    proto->talk(heal.heal);
                }
                else if (heal.action == "Use") {
                    int itemId = std::stoi(heal.heal);
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
                                    break;
                                }
                            }
                            if (found) break;
                        }
                    }
                }
            }
        }
        if (heal.condition == "MP%") {
            if (heal.below >= current_mp_pc && current_mp_pc >= heal.above &&
                current_hp >= heal.minimum) {
                if (heal.action == "Say") {
                    proto->talk(heal.heal);
                } else if (heal.action == "Use") {
                    int itemId = std::stoi(heal.heal);
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
                                    break;
                                }
                            }
                            if (found) break;
                        }
                    }
                }
            }
        }
        index = (index + 1) % heals.size();
        msleep(50);
    }
}
