#include "WalkerModel.h"
#include <QThread>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


WalkerModel::WalkerModel(QObject *parent)
    : QObject(parent) {

}

WalkerModel::~WalkerModel() {
    if (recordWaypointsThread) {
        recordWaypointsThread->quit();
        recordWaypointsThread->wait();
        delete recordWaypointsThread;
    }
    if (followWaypointsThread) {
        followWaypointsThread->quit();
        followWaypointsThread->wait();
        delete followWaypointsThread;
    }
}


void WalkerModel::addItem(const QString &direction, const QString &option, const QString &action) {
    auto localPlayer = proto->getLocalPlayer();
    auto position = proto->getPosition(localPlayer);
    QString item = "";
    if (option == "Label") {
        item = QStringLiteral("%1 %2 %3 %4 %5").arg(action, direction).arg(position.x).arg(position.y).arg(position.z);
    } else if (option == "Action") {
        item = QStringLiteral("Action %2 %3 %4 %5").arg(direction).arg(position.x).arg(position.y).arg(position.z);
    } else {
        item = QStringLiteral("%1 %2 %3 %4 %5").arg(option, direction).arg(position.x).arg(position.y).arg(position.z);
    }
    Waypoint wpt;
    wpt.direction = direction.toStdString();
    wpt.position = position;
    wpt.option = option.toStdString();
    wpt.action = action.toStdString();
    waypoints.push_back(wpt);
    emit addItem_signal(item);
}

void WalkerModel::recordWaypoints(bool state, int sqmDist, const QString &direction, const QString &option) {
    if (state && sqmDist) {
        if (!recordWaypointsThread) {
            recordWaypointsThread = new RecordWaypoints_Thread(sqmDist, direction, option, this);
            connect(recordWaypointsThread, &RecordWaypoints_Thread::addWaypoint_signal,
                    this, &WalkerModel::addItem);
            connect(recordWaypointsThread, &QThread::finished, recordWaypointsThread, &QObject::deleteLater);
            connect(recordWaypointsThread, &QThread::finished, this, [this]() {
                this->recordWaypointsThread = nullptr;
            });
            recordWaypointsThread->start();
        }
    } else {
        if (recordWaypointsThread) {
            recordWaypointsThread->requestInterruption();
            recordWaypointsThread->quit();
        }
    }
}

void WalkerModel::startWalker(bool state) {
    if (state) {
        if (!followWaypointsThread) {
            followWaypointsThread = new FollowWaypoints_Thread(waypoints, this);
            connect(followWaypointsThread, &FollowWaypoints_Thread::indexUpdate_signal, this, &WalkerModel::indexUpdate_slot);
            connect(followWaypointsThread, &QThread::finished, followWaypointsThread, &QObject::deleteLater);
            connect(followWaypointsThread, &QThread::finished, this, [this]() {
                this->followWaypointsThread = nullptr;
            });
            followWaypointsThread->start();
        }
    } else {
        if (followWaypointsThread) {
            followWaypointsThread->requestInterruption();
            followWaypointsThread->quit();
        }
    }
}

void WalkerModel::indexUpdate_slot(int index) {
    emit indexUpdate_signal(index);
}

QJsonArray WalkerModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &wpt : waypoints) {
        QJsonObject jsonObj;
        jsonObj["option"] = QString::fromStdString(wpt.option);
        jsonObj["action"] = QString::fromStdString(wpt.action);
        jsonObj["direction"] = QString::fromStdString(wpt.direction);
        QJsonObject posObj;
        posObj["x"] = static_cast<int>(wpt.position.x);
        posObj["y"] = static_cast<int>(wpt.position.y);
        posObj["z"] = static_cast<int>(wpt.position.z);
        jsonObj["position"] = posObj;

        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void WalkerModel::fromJson(const QJsonArray &json) {
    waypoints.clear();
    emit clearListWidget_signal();
    for (auto val : json) {
        QJsonObject obj = val.toObject();
        QString option = obj["option"].toString();
        QString action = obj["action"].toString();
        QString direction = obj["direction"].toString();
        QJsonObject posObj = obj["position"].toObject();
        Position pos{};
        pos.x = posObj["x"].toInt();
        pos.y = posObj["y"].toInt();
        pos.z = posObj["z"].toInt();
        Waypoint wpt;
        wpt.position = pos;
        wpt.option = option.toStdString();
        wpt.action = action.toStdString();
        wpt.direction = direction.toStdString();
        waypoints.push_back(wpt);
        QString item = "";
        if (option == "Label") {
            item = QStringLiteral("%1 %2 %3 %4 %5").arg(action, direction).arg(pos.x).arg(pos.y).arg(pos.z);
        } else if (option == "Action") {
            item = QStringLiteral("Action %2 %3 %4 %5").arg(direction).arg(pos.x).arg(pos.y).arg(pos.z);
        } else {
            item = QStringLiteral("%1 %2 %3 %4 %5").arg(option, direction).arg(pos.x).arg(pos.y).arg(pos.z);
        }
        emit addItem_signal(item);
    }
}

void WalkerModel::deleteItem(const int& index) {
    waypoints.erase(waypoints.begin() + index);
}






