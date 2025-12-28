//
// Created by Wojciech on 10.10.2025.
//

// You may need to build the project (run Qt uic code generator) to get "ui_BotView.h" resolved

#include "BotView.h"



#include <QMenu>

#include "MainBot/BotController.h"

BotView::BotView(QWidget *parent)
    : ui(new Ui::Bot_View) {

    ui->setupUi(this);

    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);

    connect(ui->walker_pushButton, &QPushButton::clicked, this, &BotView::openWalkerView);
    connect(ui->targeting_pushButton, &QPushButton::clicked, this, &BotView::openTargetingViewRequested);
    connect(ui->looting_pushButton, &QPushButton::clicked, this, &BotView::openLootingViewRequested);
    connect(ui->healing_pushButton, &QPushButton::clicked, this, &BotView::openHealingViewRequested);
    connect(ui->spells_pushButton, &QPushButton::clicked, this, &BotView::openSpellsViewRequested);

    connect(ui->walker_checkBox, &QCheckBox::toggled, this, &BotView::startWalker_signal);
    connect(ui->targeting_checkBox, &QCheckBox::toggled, this, &BotView::startTargeting_signal);
    connect(ui->looting_checkBox, &QCheckBox::toggled, this, &BotView::startLooting_signal);
    connect(ui->healing_checkBox, &QCheckBox::toggled, this, &BotView::startHealing_signal);
    connect(ui->spells_checkBox, &QCheckBox::toggled, this, &BotView::startSpells_signal);

    connect(ui->save_pushButton, &QPushButton::clicked, this, &BotView::onSaveClicked);
    connect(ui->load_pushButton, &QPushButton::clicked, this, &BotView::onLoadClicked);

    for (const auto & entry : std::filesystem::directory_iterator("Save")) {
        std::filesystem::path p = entry.path();
        ui->profiles_listWidget->addItem(p.stem().string().c_str());
    }

    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_tray = new QSystemTrayIcon(this);
        m_tray->setIcon(QIcon(":/Icon.jpg"));
        m_tray->setToolTip(tr("EasyBot"));

        m_trayMenu = new QMenu(this);
        m_restoreAction = m_trayMenu->addAction(tr("Restore"));
        m_quitAction = m_trayMenu->addAction(tr("Exit"));

        connect(m_restoreAction, &QAction::triggered, this, &BotView::restoreFromTray);
        connect(m_quitAction, &QAction::triggered, this, &BotView::quitApp);
        connect(m_tray, &QSystemTrayIcon::activated, this, &BotView::iconActivated);

        connect(ui->actionAbout_QT, &QAction::triggered, this, &BotView::showAboutQt);

        m_tray->setContextMenu(m_trayMenu);
        m_tray->show();
    }

}

BotView::~BotView() {
    delete ui;
}


void BotView::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
        case QSystemTrayIcon::Trigger:
        case QSystemTrayIcon::DoubleClick:
            restoreFromTray();
            break;
        default:
            break;
    }
}

void BotView::restoreFromTray() {
    show();
    raise();
    activateWindow();
}

void BotView::quitApp() {
    if (m_tray) {
        m_tray->hide();
    }
    qApp->quit();
}

void BotView::onSaveClicked() {
    emit saveRequested(ui->name_lineEdit->text(), ui->saveLoad_comboBox->currentText());
    ui->profiles_listWidget->addItem(ui->name_lineEdit->text());
    ui->name_lineEdit->clear();
}

void BotView::onLoadClicked() {
    if (ui->profiles_listWidget->currentItem()) {
        emit loadRequested(ui->profiles_listWidget->currentItem()->text(), ui->saveLoad_comboBox->currentText());
    }
}

void BotView::showAboutQt() {
    QApplication::aboutQt();
}




