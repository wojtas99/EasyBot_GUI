//
// Created by blazz on 12.10.2025.
//

#ifndef TARGETINGMODEL_H
#define TARGETINGMODEL_H
#include <QObject>
#include "../../const.h"
#include "TargetingThread/AttackTargets_Thread.h"
#include <QJsonObject>
#include <QJsonArray>


class TargetingModel : public QObject {
    Q_OBJECT
    public:
    explicit TargetingModel(QObject *parent = nullptr);
    ~TargetingModel();

    void addItem(const QString &targetName, const int &dist, const int &count, const QString &desiredStance, const QString &monstersAttacks);
    void shootableState(bool state);
    void reachableState(bool state);
    void openCorpseState(bool state);
    void stayAwayDist(int currentDist);
    void startTargeting(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);



    signals:
    void addItem_signal(const QString &item);
    void shootableStateChanged_signal(bool state);
    void reachableStateChanged_signal(bool state);
    void openCorpseStateChanged_signal(bool state);
    void stayAwayDistChanged_signal(int currentDist);
    void clearListWidget_signal();

private:
    std::vector<Target> targets;
    AttackTargets_Thread *attackTargetsThread = nullptr;
    bool m_shootable = false;
    bool m_reachable = false;
    bool m_openCorpse = false;
    int m_stayAwayDist = 2;

};



#endif //TARGETINGMODEL_H
