#include "FollowWaypoints_Thread.h"
#include <QElapsedTimer>
#include "../../BotEngine.h"


void FollowWaypoints_Thread::run() {
    if (waypoints.empty()) return;
    size_t index = findClosest();
    while (!isInterruptionRequested()) {
        if (engine->hasTarget && (index + 1) % waypoints.size() != 0) {
            index += bestWpt(waypoints[index], waypoints[index + 1]);
        }
        auto localPlayer = proto->getLocalPlayer();
        auto wpt = waypoints[index];
        auto playerPos = proto->getPosition(localPlayer);
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            if (wpt.option == "Action") {
                /*
                engine->executeLuaScript(wpt.action);*/

            }
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            continue;
        }
        // Only walks if we dont have a target or we want to Lure
        if (!engine->hasTarget || wpt.option == "Lure") {
            if (!proto->isAutoWalking(localPlayer)) {
                proto->autoWalk(localPlayer, wpt.position, false);
            }
        }
        msleep(50);
    }
}

int FollowWaypoints_Thread::findClosest() {
    int closestIndex = 0;
    int minDistance = std::numeric_limits<int>::max();
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    for (int i = 0; i < waypoints.size(); ++i) {
        const auto& wpt = waypoints[i];
        if (playerPos.z != wpt.position.z) {
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


