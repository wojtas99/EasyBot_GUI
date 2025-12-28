#include "LootingModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>




LootingModel::LootingModel(QObject *parent)
    : QObject(parent) {
}

LootingModel::~LootingModel() {
    if (lootItemsThread) {
        lootItemsThread->quit();
        lootItemsThread->wait();
        delete lootItemsThread;
    }
}

void LootingModel::addItem(const int& itemID, const QString& destination, const int& lootingSpeed) {
    auto str = QStringLiteral("%1 -> %2").arg(itemID).arg(destination);
    Item item;
    item.itemID = itemID;
    auto dest= destination.toStdString();
    std::transform(dest.begin(), dest.end(), dest.begin(), ::tolower);
    item.destination = dest;
    item.lootingSpeed = lootingSpeed;
    items.push_back(item);
    emit addItem_signal(str);
}

void LootingModel::startLooting(bool state) {
    if (state) {
        if (!lootItemsThread) {
            lootItemsThread = new LootItems_Thread(items, m_nextBpState, m_corpseBpState, this);
            connect(this, &LootingModel::nextBpStateChanged_signal,
                    lootItemsThread, &LootItems_Thread::nextBpStateChange);
            connect(this, &LootingModel::corpseBpStateChanged_signal,
                    lootItemsThread, &LootItems_Thread::corpseBpStateChange);
            connect(lootItemsThread, &QThread::finished, lootItemsThread, &QObject::deleteLater);
            connect(lootItemsThread, &QThread::finished, this, [this]() {
                this->lootItemsThread = nullptr;
            });
            lootItemsThread->start();
        }
    } else {
        if (lootItemsThread) {
            lootItemsThread->requestInterruption();
            lootItemsThread->quit();
        }
    }
}

void LootingModel::nextBpState(bool state) {
    m_nextBpState = state;
}

void LootingModel::corpseBpState(bool state) {
    m_corpseBpState = state;
}

QJsonArray LootingModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &item : items) {
        QJsonObject jsonObj;
        jsonObj["id"] = item.itemID;
        jsonObj["destination"] = QString::fromStdString(item.destination);
        jsonObj["speed"] = item.lootingSpeed;
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void LootingModel::fromJson(const QJsonArray &json) {
    items.clear();
    emit clearListWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        int id = obj["id"].toInt();
        QString dest = obj["destination"].toString();
        int speed = obj["speed"].toInt();
        addItem(id, dest, speed);
    }
}

void LootingModel::deleteItem(const int& index) {
    items.erase(items.begin() + index);
}


