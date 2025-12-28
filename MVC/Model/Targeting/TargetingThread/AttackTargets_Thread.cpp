#include "AttackTargets_Thread.h"
#include <algorithm>

void AttackTargets_Thread::run()
{
    if (m_targets.empty()) return;

    Position lastTargetPos{};
    uintptr_t currentTarget = 0;
    size_t index = 0;

    while (!isInterruptionRequested()) {
        // If we are not attacking seek for creature.
        if (!proto->isAttacking()) {
            auto target = m_targets[index];
            index = (index + 1) % m_targets.size();
            auto localPlayer = proto->getLocalPlayer();
            auto playerPos = proto->getPosition(localPlayer);
            auto spectators = proto->getSpectators(playerPos, false);
            for (auto spectator : spectators) {
                if (attackCondition(target, spectator)) {
                    currentTarget = spectator;
                    proto->attack(spectator, false);
                    msleep(100);
                    break;
                }
            }
        } else {
            if (proto->isDead(currentTarget)) {
                std::cout << "Current Target is Dead" << std::endl;
            }
        }
        if (currentTarget) {
            if (proto->isDead(currentTarget)) {
                auto targetPos = proto->getPosition(currentTarget);
                std::cout << std::endl;
                std::cout << "Target Dead Pos" << std::endl;
                std::cout << targetPos.x <<std::endl;
                std::cout << targetPos.y <<std::endl;
                std::cout << targetPos.z <<std::endl;
                std::cout << std::endl;
                if (m_openCorpseState) {
                    std::cout << std::endl;
                    std::cout << lastTargetPos.x <<std::endl;
                    std::cout << lastTargetPos.y <<std::endl;
                    std::cout << lastTargetPos.z <<std::endl;
                    std::cout << std::endl;
                    auto tile = proto->getTile(lastTargetPos);
                    auto topUseThing = proto->getTopUseThing(tile);
                    proto->open(topUseThing, 0);
                    currentTarget = 0;
                }

            } else {
                lastTargetPos = proto->getPosition(currentTarget);
            }
        }
        msleep(50);
    }
}

bool AttackTargets_Thread::attackCondition(Target target, uintptr_t spectator) {
    if (proto->isPlayer(spectator)) return false;
    auto specName = proto->getCreatureName(spectator);
    std::string tName = target.targetName;
    std::transform(specName.begin(), specName.end(), specName.begin(), ::tolower);
    if (specName != tName && tName != "*") return false;
    auto localPlayer = proto->getLocalPlayer();
    auto pPos = proto->getPosition(localPlayer);
    auto sPos = proto->getPosition(spectator);
    int dist = std::max(std::abs(static_cast<int>(pPos.x) - static_cast<int>(sPos.x)),
        std::abs(static_cast<int>(pPos.y) - static_cast<int>(sPos.y)));
    if (target.dist > 0 && dist > target.dist) return false;
    if (m_shootableState && !proto->canShoot(spectator, 7)) return false;
    if (m_reachableState) {
        auto path = proto->findPath(pPos, sPos, 100, Otc::PathFindIgnoreCreatures | Otc::PathFindAllowNonPathable);
        if (path.empty() && dist > 1) return false;
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
