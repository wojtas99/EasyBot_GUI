#include "AttackTargets_Thread.h"
#include "../../BotEngine.h"
#include <algorithm>


void AttackTargets_Thread::run()
{
    if (m_targets.empty()) return;

    Position lastTargetPos{};
    uintptr_t currentTarget = 0;
    size_t index = 0;
    engine->hasTarget = false;
    while (!isInterruptionRequested()) {
        auto target = m_targets[index];
        auto localPlayer = proto->getLocalPlayer();
        auto playerPos = proto->getPosition(localPlayer);
        // Attacking target logic
        if (!proto->isAttacking()) {
            auto spectators = proto->getSpectators(playerPos, false);
            std::vector<uintptr_t> monsters;
            for (auto spectator : spectators) {
                if (attackCondition(target, spectator)) {
                    monsters.emplace_back(spectator);
                }
            }
            currentTarget = 0;
            int best_match = 999;
            // Attack closest target
            for (auto monster : monsters) {
                auto monsterPos = proto->getPosition(monster);
                int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(monsterPos.x)),
                    std::abs(static_cast<int>(playerPos.y) - static_cast<int>(monsterPos.y)));
                if (dist < best_match) {
                    best_match = dist;
                    currentTarget = monster;
                }
            }
            if (currentTarget == 0) {
                engine->hasTarget = false;
                index = (index + 1) % m_targets.size();
            } else {
                engine->hasTarget = true;
                proto->attack(currentTarget, false);
                msleep(100);
            }
        }

        if (currentTarget) {
            if (proto->isDead(currentTarget)) {
                currentTarget = 0;
            } else {
                if (!attackCondition(target, currentTarget)) {
                    currentTarget = 0;
                    proto->cancelAttack();
                    msleep(100);
                    proto->cancelAttackAndFollow();
                    msleep(100);
                } else {
                    auto monsterPos = proto->getPosition(currentTarget);
                    desiredStance(playerPos, monsterPos, target.desiredStance);
                    monstersAttacks(playerPos, monsterPos, target.monstersAttacks);
                }
            }
        }


        msleep(50);
    }
}

bool AttackTargets_Thread::attackCondition(Target target, uintptr_t spectator) {
    if (!proto->isMonster(spectator)) return false;
    auto specName = proto->getCreatureName(spectator);
    std::string tName = target.targetName;
    std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
    if (specName != tName && tName != "*") return false;
    auto localPlayer = proto->getLocalPlayer();
    auto playerPos = proto->getPosition(localPlayer);
    auto spectatorPos = proto->getPosition(spectator);
    int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(spectatorPos.x)),
        std::abs(static_cast<int>(playerPos.y) - static_cast<int>(spectatorPos.y)));
    if (target.dist > 0 && dist > target.dist) return false;
    // Can shoot the monster
    if (m_shootableState && !proto->canShoot(spectator, dist)) return false;
    // There is a path to monster
    if (m_reachableState) {
        auto path = proto->findPath(playerPos, spectatorPos, 100, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
        if (path.empty()) return false;
    }
    return true;
}

void AttackTargets_Thread::desiredStance(Position playerPos, Position targetPos, std::string option) {
    int dist = std::max(std::abs(static_cast<int>(playerPos.x) - static_cast<int>(targetPos.x)),
    std::abs(static_cast<int>(playerPos.y) - static_cast<int>(targetPos.y)));
    if (option == "Chase") {
        if (dist <= 1) return;
        auto path = proto->findPath(playerPos, targetPos, 100, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
        if (!path.empty()) {
            proto->walk(path.at(0));
        }
    }
    else if (option == "Stay Away") {
        if (dist >= m_stayAwayDistance) return;
        auto distFromTarget = 1;
        Otc::Direction neededDir = Otc::InvalidDirection;
        if (targetPos.y < playerPos.y) neededDir = Otc::North;
        else if (targetPos.y > playerPos.y) neededDir = Otc::South;
        else if (targetPos.x < playerPos.x) neededDir = Otc::West;
        else if (targetPos.x > playerPos.x) neededDir = Otc::East;
        auto newPos = playerPos;
        int randomNum = rand() % 4; // Random from 0 to 3
        if (neededDir == Otc::North) {
            if (randomNum == Otc::South) newPos.y += distFromTarget;
            if (randomNum == Otc::West) newPos.x -= distFromTarget;
            if (randomNum == Otc::East) newPos.x += distFromTarget;
        } else if (neededDir == Otc::South) {
            if (randomNum == Otc::North) newPos.y -= distFromTarget;
            if (randomNum == Otc::West) newPos.x -= distFromTarget;
            if (randomNum == Otc::East) newPos.x += distFromTarget;
        } else if (neededDir == Otc::East) {
            if (randomNum == Otc::North) newPos.y -= distFromTarget;
            if (randomNum == Otc::South) newPos.y += distFromTarget;
            if (randomNum == Otc::West) newPos.x -= distFromTarget;
        } else if (neededDir == Otc::West) {
            if (randomNum == Otc::North) newPos.y -= distFromTarget;
            if (randomNum == Otc::South) newPos.y += distFromTarget;
            if (randomNum == Otc::East) newPos.x += distFromTarget;
        }
        if (newPos.x == playerPos.x && newPos.y == playerPos.y) return;
        auto path = proto->findPath(playerPos, newPos, 25, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
        if (!path.empty()) {
            proto->walk(path.at(0));
        }
    }
}

void AttackTargets_Thread::monstersAttacks(Position playerPos, Position targetPos, std::string option) {
    auto abs_x = std::abs(static_cast<int>(playerPos.x) - static_cast<int>(targetPos.x));
    auto abs_y = std::abs(static_cast<int>(playerPos.y) - static_cast<int>(targetPos.y));
    int dist_max = std::max(abs_x, abs_y);
    if (dist_max > 1) return;
    if (option == "Face Target") {
        int dist_min = std::min(abs_x, abs_y);
        if (dist_min == 0) {
            Otc::Direction neededDir = Otc::InvalidDirection;
            if (targetPos.y < playerPos.y) neededDir = Otc::North;
            else if (targetPos.y > playerPos.y) neededDir = Otc::South;
            else if (targetPos.x < playerPos.x) neededDir = Otc::West;
            else if (targetPos.x > playerPos.x) neededDir = Otc::East;
            proto->turn(neededDir);
        } else {
            Otc::Direction neededDir = Otc::InvalidDirection;
            if (targetPos.x < playerPos.x && targetPos.y < playerPos.y) neededDir = Otc::NorthWest;
            else if (targetPos.x > playerPos.x && targetPos.y < playerPos.y) neededDir = Otc::NorthEast;
            else if (targetPos.x < playerPos.x && targetPos.y > playerPos.y) neededDir = Otc::SouthWest;
            else if (targetPos.x > playerPos.x && targetPos.y > playerPos.y) neededDir = Otc::SouthEast;
            auto newPos = playerPos;
            int randomNum = rand() % 4; // Random from 0 to 3
            if (neededDir == Otc::NorthWest) {
                if (randomNum == Otc::North) newPos.y -= 1;
                if (randomNum == Otc::West) newPos.x -= 1;
            } else if (neededDir == Otc::NorthEast) {
                if (randomNum == Otc::North) newPos.y -= 1;
                if (randomNum == Otc::East) newPos.x += 1;
            } else if (neededDir == Otc::SouthWest) {
                if (randomNum == Otc::South) newPos.y += 1;
                if (randomNum == Otc::West) newPos.x -= 1;
            } else if (neededDir == Otc::SouthEast) {
                if (randomNum == Otc::South) newPos.y += 1;
                if (randomNum == Otc::East) newPos.x += 1;
            }
            if (newPos.x == playerPos.x && newPos.y == playerPos.y) return;
            auto path = proto->findPath(playerPos, newPos, 20, 0);
            if (path.size() > 0) {
                proto->walk(static_cast<Otc::Direction>(randomNum));
            }
        }
    } else if (option == "Avoid Waves") {
        if (abs_x == 1 && abs_y == 1) return; // Already stays diagonal
        Otc::Direction neededDir = Otc::InvalidDirection;
        if (targetPos.y < playerPos.y) neededDir = Otc::North;
        else if (targetPos.y > playerPos.y) neededDir = Otc::South;
        else if (targetPos.x < playerPos.x) neededDir = Otc::West;
        else if (targetPos.x > playerPos.x) neededDir = Otc::East;
        auto newPos = playerPos;
        int randomNum = rand() % 4; // Random from 0 to 3
        if (neededDir == Otc::North) {
            if (randomNum == Otc::West) newPos.x -= 1;
            if (randomNum == Otc::East) newPos.x += 1;
        } else if (neededDir == Otc::South) {
            if (randomNum == Otc::West) newPos.x -= 1;
            if (randomNum == Otc::East) newPos.x += 1;
        } else if (neededDir == Otc::East) {
            if (randomNum == Otc::North) newPos.y -= 1;
            if (randomNum == Otc::South) newPos.y += 1;
        } else if (neededDir == Otc::West) {
            if (randomNum == Otc::North) newPos.y -= 1;
            if (randomNum == Otc::South) newPos.y += 1;
        }
        if (newPos.x == playerPos.x && newPos.y == playerPos.y) return;
        auto path = proto->findPath(playerPos, newPos, 20, 0);
        if (path.size() > 0) {
            proto->walk(static_cast<Otc::Direction>(randomNum));
        }
    }
}

void AttackTargets_Thread::shootableStateChange(bool state) { m_shootableState = state; }
void AttackTargets_Thread::reachableStateChange(bool state) { m_reachableState = state; }
void AttackTargets_Thread::openCorpseStateChange(bool state) { m_openCorpseState = state; }
void AttackTargets_Thread::stayAwayDistChange(int currentDist){ m_stayAwayDistance = currentDist; }
