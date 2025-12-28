#include "LootItems_Thread.h"

void LootItems_Thread::run() {
    if (items.empty()) return;
    for (auto &item : items) {
        auto destination = item.destination;
        auto containers = proto->getContainers();
        for (auto container : containers) {
            auto container_name = proto->getContainerName(container);
            std::transform(container_name.begin(), container_name.end(), container_name.begin(), ::tolower);
            if (container_name == destination) {
                auto containerPos = proto->getSlotPosition(container, 0);
                item.itemPosition = containerPos;
            }
        }
    }
    while (!isInterruptionRequested()) {
        for (auto item : items) {
            auto destination = item.destination;
            auto itemID = item.itemID;
            auto destPos = item.itemPosition;
            auto sleepTime = item.lootingSpeed;
            performLooting(itemID, sleepTime, destPos, destination);
        }
        Sleep(50);
    }
}

void LootItems_Thread::nextBpStateChange(bool state) {
    m_nextBpState = state;
}

void LootItems_Thread::corpseBpStateChange(bool state) {
    m_corpseBpState = state;
}

bool LootItems_Thread::performLooting(int itemID, int sleepTime, Position destPos, std::string dest_name) {
    auto containers = proto->getContainers();
    for (auto container : containers) {
        auto container_name = proto->getContainerName(container);
        std::transform(container_name.begin(), container_name.end(), container_name.begin(), ::tolower);
        auto containerPos = proto->getSlotPosition(container, 0);
        if (containerPos.x == destPos.x && containerPos.y == destPos.y && containerPos.z == destPos.z)
        {
            if (m_nextBpState) {
                auto itemCount = proto->getItemsCount(container);
                if (itemCount == 20) {
                    auto itemsInContainer = proto->getItems(container);
                    for (auto item : itemsInContainer) {
                        if (proto->isContainer(item))
                        {
                            proto->open(item, container);
                            Sleep(sleepTime);
                            break;
                        }
                    }
                }
            }
            continue;
        }
        auto itemsInContainer = proto->getItems(container);
        uintptr_t foundContainer = 0;
        for (auto item : itemsInContainer) {
            if (proto->isContainer(item))
            {
                foundContainer = item;
                continue;
            }
            if (itemID == proto->getItemId(item) || itemID == 0) {
                auto itemCount = proto->getCount(item);
                proto->move(item, destPos, itemCount);
                Sleep(sleepTime);
            }
        }
        if (foundContainer && m_corpseBpState) {
            proto->open(foundContainer, container);
            Sleep(sleepTime);
        }
    }
    return true;
}

