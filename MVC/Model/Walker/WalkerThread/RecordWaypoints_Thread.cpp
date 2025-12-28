//
// Created by blazz on 06.11.2025.
//

#include "RecordWaypoints_Thread.h"


void RecordWaypoints_Thread::run()
{
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    auto oldPos = playerPos;
    emit addWaypoint_signal(playerPos, direction, option);
    while (!isInterruptionRequested()) {
        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);
        auto dif_x = abs(static_cast<int>(playerPos.x - oldPos.x));
        auto dif_y = abs(static_cast<int>(playerPos.y - oldPos.y));
        auto dif = dif_x + dif_y;
        if (dif >= sqmDist && playerPos.z == oldPos.z)
        {
            oldPos = playerPos;
            emit addWaypoint_signal(playerPos, direction, option);
        }
        msleep(50);
    }
}
