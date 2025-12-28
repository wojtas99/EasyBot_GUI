//
// Created by blazz on 06.11.2025.
//

#ifndef RECORDWAYPOINTS_THREAD_H
#define RECORDWAYPOINTS_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class RecordWaypoints_Thread : public QThread {
    Q_OBJECT
    public:
    explicit RecordWaypoints_Thread(int sqmDist, const QString &direction, const QString &option, QObject *parent = nullptr)
        : QThread(parent), sqmDist(sqmDist), direction(direction), option(option) {}
    protected:
    void run() override;
    signals:
    void addWaypoint_signal(Position position, const QString &direction, const QString &option);
private:
    int sqmDist;
    QString direction, option;
};



#endif //RECORDWAYPOINTS_THREAD_H
