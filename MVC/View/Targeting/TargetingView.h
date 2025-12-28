//
// Created by blazz on 12.10.2025.
//

#ifndef TARGETINGVIEW_H
#define TARGETINGVIEW_H

#include <QMainWindow>
#include "ui_TargetingView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Targeting_View; }
QT_END_NAMESPACE

class TargetingView : public QMainWindow {
Q_OBJECT

public:
    explicit TargetingView(QWidget *parent = nullptr);
    ~TargetingView() override;

    void addItem(const QString &item);
    void clearListWidget();

    signals:
    void addItem_signal(const QString &targetName, const int &dist, const int &count, const QString &desiredStance, const QString &monstersAttacks);

    void shootableState_signal(bool state);
    void reachableState_signal(bool state);
    void openCorpseState_signal(bool state);
    void stayAwayDist_signal(int distance);
    void deleteItem_signal(const int &index);

private:
    Ui::Targeting_View *ui;
};


#endif //TARGETINGVIEW_H
