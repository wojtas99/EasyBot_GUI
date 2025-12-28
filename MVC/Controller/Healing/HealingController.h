//
// Created by Wojciech on 11.10.2025.
//

#ifndef HEALINGCONTROLLER_H
#define HEALINGCONTROLLER_H
#include <QObject>

#include "Healing/HealingModel.h"
#include "Healing/HealingView.h"


class HealingController : public QObject {
    Q_OBJECT
public:
    explicit HealingController(QObject *parent = nullptr);
    ~HealingController();

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);


    private slots:
    // HealingView Requests
    void addItem_View(const QString &action, const QString &heal, const QString &condition, const int &random,
        const int &below, const int &above, const int &minimum);
    void deleteItem_View(const int &index);


    // HealingModel Requests
    void addItem_Model(const QString &item);
    void clearListWidget_Model();

public slots:
void startHealing_slot(bool state);

private:
    HealingView *m_view;
    HealingModel *m_model;
};


#endif //HEALINGCONTROLLER_H
