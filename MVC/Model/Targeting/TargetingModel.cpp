#include "TargetingModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


TargetingModel::TargetingModel(QObject* parent)
    : QObject(parent) {

}

TargetingModel::~TargetingModel() {
    if (attackTargetsThread) {
        attackTargetsThread->quit();
        attackTargetsThread->wait();
        delete attackTargetsThread;
    }
}

void TargetingModel::addItem(const QString &targetName, const int &dist, const int &count,
    const QString &desiredStance, const QString &monstersAttacks) {
    auto item = QStringLiteral("%1").arg(targetName);
    auto tmp_targetName = targetName.toStdString();
    std::transform(tmp_targetName.begin(), tmp_targetName.end(), tmp_targetName.begin(), ::tolower);
    Target target;
    target.targetName = tmp_targetName;
    target.desiredStance = desiredStance.toStdString();
    target.monstersAttacks = monstersAttacks.toStdString();
    target.dist = dist;
    target.count = count;
    targets.push_back(target);
    emit addItem_signal(item);
}

void TargetingModel::shootableState(bool state) {
    m_shootable = state;
    if (attackTargetsThread) {
        emit shootableStateChanged_signal(state);
    }
}


void TargetingModel::reachableState(bool state) {
    m_reachable = state;
    if (attackTargetsThread) {
        emit reachableStateChanged_signal(state);
    }
}


void TargetingModel::openCorpseState(bool state) {
    m_openCorpse = state;
    if (attackTargetsThread) {
        emit openCorpseStateChanged_signal(state);
    }
}

void TargetingModel::stayAwayDist(int currentDist) {
    m_stayAwayDist = currentDist;
    if (attackTargetsThread) {
        emit stayAwayDistChanged_signal(currentDist);
    }
}


void TargetingModel::startTargeting(bool state) {
    if (state) {
        if (!attackTargetsThread) {
            attackTargetsThread = new AttackTargets_Thread(targets, m_reachable, m_shootable, m_openCorpse, m_stayAwayDist, this);
            connect(this, &TargetingModel::shootableStateChanged_signal,attackTargetsThread, &AttackTargets_Thread::shootableStateChange);
            connect(this, &TargetingModel::reachableStateChanged_signal,attackTargetsThread, &AttackTargets_Thread::reachableStateChange);
            connect(this, &TargetingModel::openCorpseStateChanged_signal,attackTargetsThread, &AttackTargets_Thread::openCorpseStateChange);
            connect(this, &TargetingModel::stayAwayDistChanged_signal,attackTargetsThread, &AttackTargets_Thread::stayAwayDistChange);
            connect(attackTargetsThread, &QThread::finished, attackTargetsThread, &QObject::deleteLater);
            connect(attackTargetsThread, &QThread::finished, this, [this]() {
                this->attackTargetsThread = nullptr;
            });
            attackTargetsThread->start();
        }
    } else {
        if (attackTargetsThread) {
            attackTargetsThread->requestInterruption();
            attackTargetsThread->quit();
        }
    }
}

QJsonArray TargetingModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &target : targets) {
        QJsonObject jsonObj;
        jsonObj["name"] = QString::fromStdString(target.targetName);
        jsonObj["dist"] = target.dist;
        jsonObj["count"] = target.count;
        jsonObj["stance"] = QString::fromStdString(target.desiredStance);
        jsonObj["attack"] = QString::fromStdString(target.monstersAttacks);
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void TargetingModel::fromJson(const QJsonArray &json) {
    targets.clear();
    emit clearListWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString name = obj["name"].toString();
        int dist = obj["dist"].toInt();
        int count = obj["count"].toInt();
        QString stance = obj["stance"].toString();
        QString attack = obj["attack"].toString();
        addItem(name, dist, count, stance, attack);
    }
}

void TargetingModel::deleteItem(const int& index) {
    targets.erase(targets.begin() + index);
}


