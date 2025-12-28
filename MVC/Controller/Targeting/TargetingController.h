//
// Created by blazz on 12.10.2025.
//

#ifndef TARGETINGCONTROLLER_H
#define TARGETINGCONTROLLER_H
#include "Targeting/TargetingModel.h"
#include "Targeting/TargetingView.h"


class TargetingController : public QObject {
    Q_OBJECT
    public:
    explicit TargetingController(QObject *parent = nullptr);
    ~TargetingController();

    void showView();
    
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);


    private slots:
    // TargetingView Requests
    void addItem_View(const QString &targetName, const int &dist, const int &count, const QString &desiredStance, const QString &monstersAttacks);
    void shootableState_View(bool state);
    void reachableState_View(bool state);
    void openCorpseState_View(bool state);
    void stayAwayDist_View(int value);
    void deleteItem_View(const int &index);


    // TargetingModel Requests
    void addItem_Model(const QString &item);
    void clearListWidget_Model();

public slots:
    void startTargeting_slot(bool state);

private:
    TargetingView *m_view;
    TargetingModel *m_model;
};



#endif //TARGETINGCONTROLLER_H
