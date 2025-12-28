//
// Created by blazz on 06.11.2025.
//

#ifndef FOLLOWWAYPINTS_THREAD_H
#define FOLLOWWAYPINTS_THREAD_H
#include <QThread>
#include "../../const.h"
#include "../../proto_functions_client.h"

class FollowWaypoints_Thread : public QThread {
    Q_OBJECT
    public:
    explicit FollowWaypoints_Thread(const std::vector<Waypoint> &waypoints, QObject *parent = nullptr)
        : QThread(parent), waypoints(waypoints) {}
protected:
    void run() override;
    signals:
    void indexUpdate_signal(int index);
private:
    std::vector<Waypoint> waypoints;
    int findClosest();
};



#endif //FOLLOWWAYPINTS_THREAD_H
