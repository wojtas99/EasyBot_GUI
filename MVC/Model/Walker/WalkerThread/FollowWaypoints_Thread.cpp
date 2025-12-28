#include "FollowWaypoints_Thread.h"
#include <QElapsedTimer>


void FollowWaypoints_Thread::run() {
    if (waypoints.empty()) return;
    size_t index = findClosest();
    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto wpt = waypoints[index];
        auto playerPos = proto->getPosition(localPlayer);
        if (playerPos.x == wpt.position.x && playerPos.y == wpt.position.y && playerPos.z == wpt.position.z) {
            index = (index + 1) % waypoints.size();
            emit indexUpdate_signal(static_cast<int>(index));
            continue;
        }
        while (proto->isAttacking() && wpt.option != "Lure")
        {
            msleep(4000);
        }
        if (!proto->isAutoWalking(localPlayer)) {
            proto->autoWalk(localPlayer, wpt.position, false);
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


