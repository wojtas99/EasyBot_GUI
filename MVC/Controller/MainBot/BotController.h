//
// Created by Wojciech on 11.10.2025.
//

#ifndef BOTCONTROLLER_H
#define BOTCONTROLLER_H
#include <QPointer>
#include <QFile>
#include <QDir>

#include "Walker/WalkerController.h"
#include "Healing/HealingController.h"
#include "Looting/LootingController.h"
#include "MainBot/BotView.h"
#include "Spells/SpellsController.h"
#include "Targeting/TargetingController.h"


class BotController : public QObject
{
    Q_OBJECT

public:
    explicit BotController(BotView *botView, QObject *parent = nullptr);

private slots:
    void openWalkerView();
    void onShowTargetingModule();
    void onShowLootingModule();
    void onShowHealingModule();
    void onShowSpellsModule();

    void onSaveRequested(const QString &name, const QString &category);
    void onLoadRequested(const QString &name, const QString &category);





private:
    BotView *m_botView;
    WalkerController *m_walkerController;
    TargetingController *m_targetingController;
    LootingController *m_lootingController;
    HealingController *m_healingController;
    SpellsController *m_spellsController;
};



#endif //BOTCONTROLLER_H
