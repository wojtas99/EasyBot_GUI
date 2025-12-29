#ifndef LUABINDINGS_H
#define LUABINDINGS_H
#include <lua.hpp>
#include <luabridge3/LuaBridge/LuaBridge.h>
#include "../../const.h"
#include "../../proto_functions_client.h"

namespace LuaBindings {
    inline void registerAll(lua_State* L) {
        luabridge::getGlobalNamespace(L)
            .beginClass<Position>("Position")
                .addConstructor<void(*)()>()
                .addProperty("x", &Position::x)
                .addProperty("y", &Position::y)
                .addProperty("z", &Position::z)
            .endClass()

            .beginClass<BotClient>("BotClient")
                // Container
                .addFunction("getItem", &BotClient::getItem)
                .addFunction("getItems", &BotClient::getItems)
                .addFunction("getItemsCount", &BotClient::getItemsCount)
                .addFunction("getSlotPosition", &BotClient::getSlotPosition)
                .addFunction("getContainerName", &BotClient::getContainerName)
                .addFunction("getContainerId", &BotClient::getContainerId)
                .addFunction("getContainerItem", &BotClient::getContainerItem)
                .addFunction("hasParent", &BotClient::hasParent)
                .addFunction("getSize", &BotClient::getSize)
                .addFunction("getFirstIndex", &BotClient::getFirstIndex)
                // Creature
                .addFunction("getCreatureName", &BotClient::getCreatureName)
                .addFunction("getHealthPercent", &BotClient::getHealthPercent)
                .addFunction("getDirection", [](BotClient* client, uintptr_t creature) { return (int)client->getDirection(creature); })
                .addFunction("isDead", &BotClient::isDead)
                .addFunction("canBeSeen", &BotClient::canBeSeen)
                .addFunction("isCovered", &BotClient::isCovered)
                .addFunction("canShoot", &BotClient::canShoot)
                // Game
                .addFunction("walk", [](BotClient* client, int dir) { client->walk((Otc::Direction)dir); })
                .addFunction("autoWalkGame", &BotClient::autoWalkGame)
                .addFunction("turn", [](BotClient* client, int dir) { client->turn((Otc::Direction)dir); })
                .addFunction("stop", &BotClient::stop)
                .addFunction("move", &BotClient::move)
                .addFunction("moveToParentContainer", &BotClient::moveToParentContainer)
                .addFunction("use", &BotClient::use)
                .addFunction("useWith", &BotClient::useWith)
                .addFunction("useInventoryItem", &BotClient::useInventoryItem)
                .addFunction("useInventoryItemWith", &BotClient::useInventoryItemWith)
                .addFunction("findItemInContainers", &BotClient::findItemInContainers)
                .addFunction("open", &BotClient::open)
                .addFunction("openParent", &BotClient::openParent)
                .addFunction("close", &BotClient::close)
                .addFunction("refreshContainer", &BotClient::refreshContainer)
                .addFunction("attack", &BotClient::attack)
                .addFunction("cancelAttack", &BotClient::cancelAttack)
                .addFunction("follow", &BotClient::follow)
                .addFunction("cancelAttackAndFollow", &BotClient::cancelAttackAndFollow)
                .addFunction("talk", &BotClient::talk)
                .addFunction("talkChannel", [](BotClient* client, int mode, uint16_t channelId, const std::string& message) { client->talkChannel((Otc::MessageMode)mode, channelId, message); })
                .addFunction("talkPrivate", [](BotClient* client, int mode, const std::string& receiver, const std::string& message) { client->talkPrivate((Otc::MessageMode)mode, receiver, message); })
                .addFunction("openPrivateChannel", &BotClient::openPrivateChannel)
                .addFunction("setChaseMode", [](BotClient* client, int mode) { client->setChaseMode((Otc::ChaseModes)mode); })
                .addFunction("setFightMode", [](BotClient* client, int mode) { client->setFightMode((Otc::FightModes)mode); })
                .addFunction("buyItem", &BotClient::buyItem)
                .addFunction("sellItem", &BotClient::sellItem)
                .addFunction("equipItem", &BotClient::equipItem)
                .addFunction("equipItemId", &BotClient::equipItemId)
                .addFunction("mount", &BotClient::mount)
                .addFunction("changeMapAwareRange", &BotClient::changeMapAwareRange)
                .addFunction("canPerformGameAction", &BotClient::canPerformGameAction)
                .addFunction("isOnline", &BotClient::isOnline)
                .addFunction("isAttacking", &BotClient::isAttacking)
                .addFunction("isFollowing", &BotClient::isFollowing)
                .addFunction("getContainer", &BotClient::getContainer)
                .addFunction("getContainers", &BotClient::getContainers)
                .addFunction("getAttackingCreature", &BotClient::getAttackingCreature)
                .addFunction("getFollowingCreature", &BotClient::getFollowingCreature)
                .addFunction("getLocalPlayer", &BotClient::getLocalPlayer)
                .addFunction("getClientVersion", &BotClient::getClientVersion)
                .addFunction("getCharacterName", &BotClient::getCharacterName)
                // Item
                .addFunction("getCount", &BotClient::getCount)
                .addFunction("getSubType", &BotClient::getSubType)
                .addFunction("getItemId", &BotClient::getItemId)
                .addFunction("getTooltip", &BotClient::getTooltip)
                .addFunction("getDurationTime", &BotClient::getDurationTime)
                .addFunction("getItemName", &BotClient::getItemName)
                .addFunction("getDescription", &BotClient::getDescription)
                .addFunction("getTier", &BotClient::getTier)
                .addFunction("getText", &BotClient::getText)
                // LocalPlayer
                .addFunction("isWalkLocked", &BotClient::isWalkLocked)
                .addFunction("getStates", &BotClient::getStates)
                .addFunction("getHealth", &BotClient::getHealth)
                .addFunction("getMaxHealth", &BotClient::getMaxHealth)
                .addFunction("getFreeCapacity", &BotClient::getFreeCapacity)
                .addFunction("getLevel", &BotClient::getLevel)
                .addFunction("getMana", &BotClient::getMana)
                .addFunction("getMaxMana", &BotClient::getMaxMana)
                .addFunction("getManaShield", &BotClient::getManaShield)
                .addFunction("getSoul", &BotClient::getSoul)
                .addFunction("getStamina", &BotClient::getStamina)
                .addFunction("hasEquippedItemId", &BotClient::hasEquippedItemId)
                .addFunction("getInventoryCount", &BotClient::getInventoryCount)
                .addFunction("hasSight", &BotClient::hasSight)
                .addFunction("isAutoWalking", &BotClient::isAutoWalking)
                .addFunction("stopAutoWalk", &BotClient::stopAutoWalk)
                .addFunction("autoWalk", &BotClient::autoWalk)
                // Map
                .addFunction("getTile", &BotClient::getTile)
                .addFunction("getSpectators", &BotClient::getSpectators)
                .addFunction("findPath", &BotClient::findPath)
                .addFunction("isWalkable", &BotClient::isWalkable)
                .addFunction("isSightClear", &BotClient::isSightClear)
                // Thing
                .addFunction("getId", &BotClient::getId)
                .addFunction("getPosition", &BotClient::getPosition)
                .addFunction("getParentContainer", &BotClient::getParentContainer)
                .addFunction("isItem", &BotClient::isItem)
                .addFunction("isMonster", &BotClient::isMonster)
                .addFunction("isNpc", &BotClient::isNpc)
                .addFunction("isCreature", &BotClient::isCreature)
                .addFunction("isPlayer", &BotClient::isPlayer)
                .addFunction("isLocalPlayer", &BotClient::isLocalPlayer)
                .addFunction("isContainer", &BotClient::isContainer)
                .addFunction("isUsable", &BotClient::isUsable)
                .addFunction("isLyingCorpse", &BotClient::isLyingCorpse)
                // Tile
                .addFunction("getTopThing", &BotClient::getTopThing)
                .addFunction("getTopUseThing", &BotClient::getTopUseThing)
                // Custom
                .addFunction("getMessages", &BotClient::getMessages)
                .addFunction("clearMessages", &BotClient::clearMessages)
                .addFunction("dropMessages", &BotClient::dropMessages)
            .endClass();

        // Otc constants
        luabridge::getGlobalNamespace(L)
            .beginNamespace("Otc")
                .addProperty("North", []{ return (int)Otc::North; })
                .addProperty("East", []{ return (int)Otc::East; })
                .addProperty("South", []{ return (int)Otc::South; })
                .addProperty("West", []{ return (int)Otc::West; })
                .addProperty("NorthEast", []{ return (int)Otc::NorthEast; })
                .addProperty("SouthEast", []{ return (int)Otc::SouthEast; })
                .addProperty("SouthWest", []{ return (int)Otc::SouthWest; })
                .addProperty("NorthWest", []{ return (int)Otc::NorthWest; })
                // Inventory Slots
                .addProperty("InventorySlotHead", []{ return (int)Otc::InventorySlotHead; })
                .addProperty("InventorySlotNecklace", []{ return (int)Otc::InventorySlotNecklace; })
                .addProperty("InventorySlotBackpack", []{ return (int)Otc::InventorySlotBackpack; })
                .addProperty("InventorySlotArmor", []{ return (int)Otc::InventorySlotArmor; })
                .addProperty("InventorySlotRight", []{ return (int)Otc::InventorySlotRight; })
                .addProperty("InventorySlotLeft", []{ return (int)Otc::InventorySlotLeft; })
                .addProperty("InventorySlotLegs", []{ return (int)Otc::InventorySlotLegs; })
                .addProperty("InventorySlotFeet", []{ return (int)Otc::InventorySlotFeet; })
                .addProperty("InventorySlotRing", []{ return (int)Otc::InventorySlotRing; })
                .addProperty("InventorySlotAmmo", []{ return (int)Otc::InventorySlotAmmo; })
            .endNamespace();

        // Register proto instance as a global variable
        luabridge::setGlobal(L, BotClient::getInstance(), "proto");

        // Helper: msleep
        luabridge::getGlobalNamespace(L)
            .addFunction("msleep", [](int ms) {
                QThread::msleep(ms);
            })
            .addFunction("sleep", [](int ms) {
                QThread::msleep(ms);
            })
            .addFunction("getInventorySlotPosition", [](int slot) {
                Position p;
                p.x = 0xFFFF;
                p.y = slot;
                p.z = 0;
                return p;
            });
    }
}

#endif // LUABINDINGS_H
