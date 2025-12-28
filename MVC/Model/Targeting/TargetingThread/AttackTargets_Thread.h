#ifndef ATTACKCREATURES_THREAD_H
#define ATTACKCREATURES_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class AttackTargets_Thread : public QThread {
    Q_OBJECT
    public:
    explicit AttackTargets_Thread(const std::vector<Target> &targets, bool reachableSate, bool shootableState, bool openCorpseState, int stayAwayDist, QObject *parent = nullptr)
        : QThread(parent), m_targets(targets), m_reachableState(reachableSate), m_shootableState(shootableState), m_openCorpseState(openCorpseState), m_stayAwayDistance(stayAwayDist) {}
public slots:
    void shootableStateChange(bool state);
    void reachableStateChange(bool state);
    void openCorpseStateChange(bool state);
    void stayAwayDistChange(int currentDist);
protected:
    void run() override;
private:
    std::vector<Target> m_targets;
    bool m_shootableState = false;
    bool m_reachableState = false;
    bool m_openCorpseState = false;
    int m_stayAwayDistance = 1;
    bool attackCondition(Target target, uintptr_t spectator);
    void desiredStance(Position playerPos, Position targetPos, std::string option);
    void monstersAttacks(Position playerPos, Position targetPos, std::string option);
};



#endif //ATTACKCREATURES_THREAD_H
