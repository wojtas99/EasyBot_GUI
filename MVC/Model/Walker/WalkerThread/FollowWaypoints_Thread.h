#ifndef FOLLOWWAYPINTS_THREAD_H
#define FOLLOWWAYPINTS_THREAD_H
#include <QThread>
#include <string>
#include "../../const.h"
#include "../../proto_functions_client.h"

class LuaEngine; // Forward declaration

class FollowWaypoints_Thread : public QThread {
    Q_OBJECT
    public:
    explicit FollowWaypoints_Thread(const std::vector<Waypoint> &waypoints, QObject *parent = nullptr)
        : QThread(parent), waypoints(waypoints), luaScriptEngine(nullptr) {}
    
protected:
    void run() override;
    signals:
    void indexUpdate_signal(int index);
private:
    std::vector<Waypoint> waypoints;
    std::string luaScriptText;
    LuaEngine* luaScriptEngine;
    
    int findClosest();
    void performWalk(Waypoint wpt, uintptr_t localPlayer);
    size_t performAction(Waypoint wpt, size_t index, uintptr_t localPlayer);
    void performUse(Waypoint wpt, uintptr_t localPlayer);
    Otc::Direction getDirection(const std::string& wpt_direction);
    int bestWpt(Waypoint first_wpt, Waypoint second_wpt);
};



#endif //FOLLOWWAYPINTS_THREAD_H
