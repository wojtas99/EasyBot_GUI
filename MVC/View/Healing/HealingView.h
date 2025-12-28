//
// Created by blazz on 12.10.2025.
//

#ifndef HEALINGVIEW_H
#define HEALINGVIEW_H

#include <QMainWindow>
#include "ui_HealingView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Healing_View; }
QT_END_NAMESPACE

class HealingView : public QMainWindow {
Q_OBJECT

public:
    explicit HealingView(QWidget *parent = nullptr);
    ~HealingView() override;

    void addItem(const QString &item);
    void clearListWidget();

    signals:
    void addItem_signal(const QString &action, const QString &heal, const QString &condition, const int &random,
        const int &below, const int &above, const int &minimum);
    void deleteItem_signal(const int &index);

private:
    Ui::Healing_View *ui;
};


#endif //HEALINGVIEW_H
