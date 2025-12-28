//
// Created by Wojciech on 10.10.2025.
//

#ifndef BOTVIEW_H
#define BOTVIEW_H

#include <QSystemTrayIcon>

#include "ui_BotView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Bot_View; }
QT_END_NAMESPACE

class BotView : public QMainWindow {
Q_OBJECT

public:
    explicit BotView(QWidget *parent = nullptr);
    ~BotView() override;

    signals:

    void openWalkerView();
    void openTargetingViewRequested();
    void openLootingViewRequested();
    void openHealingViewRequested();
    void openSpellsViewRequested();

    void startWalker_signal(bool state);
    void startTargeting_signal(bool state);
    void startLooting_signal(bool state);
    void startHealing_signal(bool state);
    void startSpells_signal(bool state);


    void saveRequested(const QString &name, const QString &category);
    void loadRequested(const QString &name, const QString &category);


private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void restoreFromTray();
    void quitApp();

    void onSaveClicked();
    void onLoadClicked();

    void showAboutQt();

private:
    Ui::Bot_View *ui;
    QSystemTrayIcon *m_tray = nullptr;
    QMenu *m_trayMenu = nullptr;
    QAction *m_restoreAction = nullptr;
    QAction *m_quitAction = nullptr;
};


#endif //BOTVIEW_H
