//
// Created by Wojciech on 11.10.2025.
//

#ifndef WALKERMODEL_H
#define WALKERMODEL_H
#include <QObject>
#include <QVariant>
#include "../../proto_functions_client.h"
#include "../../const.h"
#include "WalkerThread/FollowWaypoints_Thread.h"
#include "WalkerThread/RecordWaypoints_Thread.h"

class WalkerModel : public QObject {
    Q_OBJECT
public:
    explicit WalkerModel(QObject *parent = nullptr);
    ~WalkerModel() override;

    void addItem(const QString &direction, const QString &option, const QString &action);

    void recordWaypoints(bool state, int sqmDist, const QString &direction, const QString &option);
    void startWalker(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);

public slots:
    void addItem_slot(Position position, const QString &direction, const QString &option);
    void indexUpdate_slot(int index);
signals:
    void addItem_signal(const QString &item);
    void indexUpdate_signal(int index);
    void clearListWidget_signal();

    private:
    std::vector<Waypoint> waypoints;
    RecordWaypoints_Thread *recordWaypointsThread = nullptr;
    FollowWaypoints_Thread *followWaypointsThread = nullptr;
};



#endif //WALKERMODEL_H
