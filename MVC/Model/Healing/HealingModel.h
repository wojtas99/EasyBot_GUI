//
// Created by Wojciech on 11.10.2025.
//

#ifndef HEALINGMODEL_H
#define HEALINGMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"
#include "HealingThread/StartHealing_Thread.h"


class HealingModel : public QObject {
    Q_OBJECT
public:
    explicit HealingModel(QObject *parent = nullptr);
    ~HealingModel();

    void addItem(const QString &action, const QString &heal, const QString &condition, const int &random,
        const int &below, const int &above, const int &minimum);
    void deleteItem(const int &index);

    void startHealing(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    signals:
    void addItem_signal(const QString &item);
    void clearListWidget_signal();

private:
    std::vector<Heal> heals;
    StartHealing_Thread *startHealingThread = nullptr;

};



#endif //HEALINGMODEL_H
