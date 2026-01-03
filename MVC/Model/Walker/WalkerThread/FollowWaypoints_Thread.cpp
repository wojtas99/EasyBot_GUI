#include "FollowWaypoints_Thread.h"
#include <QElapsedTimer>
#include <iostream>
#include "../../BotEngine.h"
#include "../../../../LuaEngine.h"

void FollowWaypoints_Thread::run() {
    if (waypoints.empty()) return;


    // Start the Lua script if one is set
    if (!luaScriptText.empty()) {
        luaScriptEngine = new LuaEngine(luaScriptText, nullptr);
        luaScriptEngine->start();
        std::cout << "Started Lua script for walker" << std::endl;
    }
    
    size_t index = findClosest();
    std::cout << index << std::endl;

    QElapsedTimer stuckTimer;
    stuckTimer.start();
    size_t lastIndex = index;


    while (!isInterruptionRequested()) {

        if (index != lastIndex) {
            lastIndex = index;
            stuckTimer.restart();
        } else if (stuckTimer.hasExpired(25000)) {
            index = findClosest();
            lastIndex = index;
            stuckTimer.restart();
            emit indexUpdate_signal(static_cast<int>(index));
        }
        if (engine->hasTarget) stuckTimer.restart();
        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);
        auto wpt = waypoints[index];
        // Only walks if we dont have a target or we want to Lure
        if (!engine->hasTarget || wpt.option == "Lure") {
            if (!proto->isAutoWalking(localPlayer)) {
                if (wpt.option == "Stand" || wpt.option == "Lure") performWalk(wpt, localPlayer);
                if (wpt.option == "Use") performUse(wpt, localPlayer);
                if (wpt.option == "Action") {
                    index = performAction(wpt, index, localPlayer);
                    if (index == -1) return;
                    wpt = waypoints[index];
                }
            }
        }
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
        }
        msleep(50);
    }

    // Stop the Lua script if running
    if (luaScriptEngine) {
        std::cout << "Stopping walker Lua script..." << std::endl;
        luaScriptEngine->requestStop();
        
        if (!luaScriptEngine->wait(1000)) { // 1 second timeout
            std::cerr << "Warning: Walker Lua script did not finish in time, terminating forcefully" << std::endl;
            luaScriptEngine->terminate();
            luaScriptEngine->wait();
        }
        
        delete luaScriptEngine;
        luaScriptEngine = nullptr;
        std::cout << "Walker Lua script stopped" << std::endl;
    }
}

void FollowWaypoints_Thread::performWalk(Waypoint wpt, uintptr_t localPlayer) {
    if (wpt.direction != "C") {
        auto direction = getDirection(wpt.direction);
        proto->walk(direction);
        return;
    }
    proto->autoWalk(localPlayer, wpt.position, false);
}

size_t FollowWaypoints_Thread::performAction(Waypoint wpt, size_t index, uintptr_t localPlayer) {
    auto playerPos = proto->getPosition(localPlayer);
    if ((playerPos.x != wpt.position.x || playerPos.y != wpt.position.y) && playerPos.z == wpt.position.z) {
        proto->autoWalk(localPlayer, wpt.position, true);
        return index;
    }
    auto* actionEngine = new LuaEngine(wpt.action, nullptr);
    actionEngine->start();
    while (actionEngine->isRunning()) {
        if (isInterruptionRequested()) {
            actionEngine->requestStop();
            actionEngine->wait(1000);
            delete actionEngine;
            return -1;
        }
        msleep(100);
    }
    std::string returnedLabel = actionEngine->getReturnedString();
    delete actionEngine;
    if (!returnedLabel.empty()) {
        for (size_t i = 0; i < waypoints.size(); ++i) {
            if (waypoints[i].action == returnedLabel) {
                index = i;
                return index;
            }
        }
    }
    return index;
}

void FollowWaypoints_Thread::performUse(Waypoint wpt, uintptr_t localPlayer) {
    auto playerPos = proto->getPosition(localPlayer);
    if ((playerPos.x != wpt.position.x || playerPos.y != wpt.position.y) && playerPos.z == wpt.position.z) {
        proto->autoWalk(localPlayer, wpt.position, true);
        return;
    }
    int itemId = std::stoi(wpt.action);
    auto wptPos = wpt.position;
    auto direction = getDirection(wpt.direction);
    if (direction == Otc::North) wptPos.y -=1;
    if (direction == Otc::East) wptPos.x +=1;
    if (direction == Otc::South) wptPos.y +=1;
    if (direction == Otc::West) wptPos.x -=1;
    if (direction == Otc::NorthEast) wptPos.x +=1, wptPos.y -=1;
    if (direction == Otc::SouthEast) wptPos.x -=1, wptPos.y +=1;
    if (direction == Otc::SouthWest) wptPos.x -=1, wptPos.y +=1;
    if (direction == Otc::NorthWest) wptPos.x -=1, wptPos.y -=1;
    if (itemId == 0) {
        auto tile = proto->getTile(wptPos);
        auto topThing = proto->getTopUseThing(tile);
        proto->use(topThing);
        msleep(1000);
    } else {
        auto containers = proto->getContainers();
        for (auto container : containers) {
            auto items = proto->getItems(container);
            for (auto item : items) {
                if (proto->getItemId(item) == itemId) {
                    auto tile = proto->getTile(wptPos);
                    auto topThing = proto->getTopUseThing(tile);
                    proto->useWith(item, topThing);
                    msleep(1000);
                    return;
                }
            }
        }
    }
}

Otc::Direction FollowWaypoints_Thread::getDirection(const std::string& wpt_direction) {
    if (wpt_direction == "N") return Otc::North;
    if (wpt_direction == "E") return Otc::East;
    if (wpt_direction == "S") return Otc::South;
    if (wpt_direction == "W") return Otc::West;
    if (wpt_direction == "NW") return Otc::NorthWest;
    if (wpt_direction == "NE") return Otc::NorthEast;
    if (wpt_direction == "SW") return Otc::SouthWest;
    if (wpt_direction == "SE") return Otc::SouthEast;
    return Otc::InvalidDirection;
}

int FollowWaypoints_Thread::findClosest() {
    int closestIndex = 0;
    int minDistance = std::numeric_limits<int>::max();
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    for (int i = 0; i < waypoints.size(); ++i) {
        const auto& wpt = waypoints[i];
        if (playerPos.z != wpt.position.z || wpt.direction != "C") {
            continue;
        }
        int dist = std::max(
            std::abs(static_cast<int>(playerPos.x) - static_cast<int>(wpt.position.x)),
            std::abs(static_cast<int>(playerPos.y) - static_cast<int>(wpt.position.y))
        );
        if (dist < minDistance) {
            minDistance = dist;
            closestIndex = i;
        }
    }
    return closestIndex;
}


int FollowWaypoints_Thread::bestWpt(Waypoint first_wpt, Waypoint second_wpt) {
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);

    auto f = first_wpt.position;
    auto s = second_wpt.position;
    if (f.z != s.z)
        return 0;

    float distFirst =
        (f.x - playerPos.x) * (f.x - playerPos.x) +
        (f.y - playerPos.y) * (f.y - playerPos.y);

    float distSecond =
        (s.x - playerPos.x) * (s.x - playerPos.x) +
        (s.y - playerPos.y) * (s.y - playerPos.y);

    return (distFirst < distSecond) ? 0 : 1;
}


