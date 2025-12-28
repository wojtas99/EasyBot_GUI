//
// Created by Wojciech on 11.10.2025.
//

#ifndef LOOTINGMODEL_H
#define LOOTINGMODEL_H
#include <QObject>
#include "../../const.h"
#include "LootingThread/LootItems_Thread.h"

class LootingModel : public QObject {
    Q_OBJECT
public:
    explicit LootingModel(QObject *parent = nullptr);
    ~LootingModel();

    void addItem(const int &itemID, const QString &destination, const int &lootingSpeed);
    void startLooting(bool state);
    void nextBpState(bool state);
    void corpseBpState(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);

    signals:
    void addItem_signal(const QString &item);
    void nextBpStateChanged_signal(bool state);
    void corpseBpStateChanged_signal(bool state);
    void clearListWidget_signal();


private:
    std::vector<Item> items;
    LootItems_Thread *lootItemsThread = nullptr;
    bool m_nextBpState = false;
    bool m_corpseBpState = false;
};



#endif //LOOTINGMODEL_H
