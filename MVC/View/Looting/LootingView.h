//
// Created by blazz on 12.10.2025.
//

#ifndef LOOTINGVIEW_H
#define LOOTINGVIEW_H

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class Looting_View; }
QT_END_NAMESPACE

class LootingView : public QMainWindow {
Q_OBJECT

public:
    explicit LootingView(QWidget *parent = nullptr);
    ~LootingView() override;

    void addItem(const QString &item);
    void clearListWidget();

    signals:
    void addItem_signal(const int &itemID, const QString &destination, const int &lootingSpeed);
    void nextBpState_signal(bool state);
    void corpseBpState_signal(bool state);
    void deleteItem_signal(const int &index);


private:
    Ui::Looting_View *ui;
};


#endif //LOOTINGVIEW_H
