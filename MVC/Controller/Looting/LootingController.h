//
// Created by Wojciech on 11.10.2025.
//

#ifndef LOOTINGCONTROLLER_H
#define LOOTINGCONTROLLER_H
#include <QObject>

#include "Looting/LootingModel.h"
#include "Looting/LootingView.h"


class LootingController : public QObject {
    Q_OBJECT
public:
    explicit LootingController(QObject *parent = nullptr);
    ~LootingController();

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);


    private slots:
    // LootingView Requests
    void addItem_View(const int &itemID, const QString &destination, const int &lootingSpeed);
    void nextBpState_View(bool state);
    void corpseBpState_View(bool state);
    void deleteItem_View(const int &index);

    // LootingModel Requests
    void addItem_Model(const QString &item);
    void clearListWidget_Model();

public slots:
    void startLooting_slot(bool state);

private:
    LootingView *m_view;
    LootingModel *m_model;
};


#endif //LOOTINGCONTROLLER_H
