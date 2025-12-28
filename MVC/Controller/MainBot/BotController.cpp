#include "BotController.h"

BotController::BotController(BotView *botView, QObject *parent)
    : QObject(parent),
    m_botView(botView),
    m_walkerController(nullptr),
    m_targetingController(nullptr),
    m_healingController(nullptr),
    m_lootingController(nullptr),
    m_spellsController(nullptr)
{
    connect(m_botView, &BotView::openWalkerView, this, &BotController::openWalkerView);
    connect(m_botView, &BotView::openTargetingViewRequested, this, &BotController::onShowTargetingModule);
    connect(m_botView, &BotView::openLootingViewRequested, this, &BotController::onShowLootingModule);
    connect(m_botView, &BotView::openHealingViewRequested, this, &BotController::onShowHealingModule);
    connect(m_botView, &BotView::openSpellsViewRequested, this, &BotController::onShowSpellsModule);

    // Walker
    connect(m_botView, &BotView::startWalker_signal, this, [this](bool checked){
        if (m_walkerController) {
            m_walkerController->startWalker_slot(checked);
        }
    });

    // Targeting
    connect(m_botView, &BotView::startTargeting_signal, this, [this](bool checked){
    if (m_targetingController) {
        m_targetingController->startTargeting_slot(checked);
    }
    });

    // Looting
    connect(m_botView, &BotView::startLooting_signal, this, [this](bool checked){
    if (m_lootingController) {
        m_lootingController->startLooting_slot(checked);
    }
    });

    // Healing
    connect(m_botView, &BotView::startHealing_signal, this, [this](bool checked){
    if (m_healingController) {
        m_healingController->startHealing_slot(checked);
    }
    });

    // Spells
    connect(m_botView, &BotView::startSpells_signal, this, [this](bool checked){
    if (m_spellsController) {
        m_spellsController->startSpells_slot(checked);
    }
    });

    // Save & Load
    connect(m_botView, &BotView::saveRequested, this, &BotController::onSaveRequested);
    connect(m_botView, &BotView::loadRequested, this, &BotController::onLoadRequested);
}



void BotController::onSaveRequested(const QString &name, const QString &category) {
    if (name.isEmpty()) return;

    QString filePath = QCoreApplication::applicationDirPath() + "/Save/" + name + ".json";
    QDir dir(QCoreApplication::applicationDirPath() + "/Save");
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QJsonObject rootObject;
    bool fileExists = QFile::exists(filePath);
    if (fileExists) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
            rootObject = doc.object();
            file.close();
        }
    }

    if (category == "All" || category == "Healing") {
        if (!m_healingController) m_healingController = new HealingController();
        rootObject["Healing"] = m_healingController->saveSettings();
    }
    if (category == "All" || category == "Walker") {
        if (!m_walkerController) m_walkerController = new WalkerController();
        rootObject["Walker"] = m_walkerController->saveSettings();
    }
    if (category == "All" || category == "Targeting") {
        if (!m_targetingController) m_targetingController = new TargetingController();
        rootObject["Targeting"] = m_targetingController->saveSettings();
    }
    if (category == "All" || category == "Looting") {
        if (!m_lootingController) m_lootingController = new LootingController();
        rootObject["Looting"] = m_lootingController->saveSettings();
    }
    if (category == "All" || category == "Spells") {
        if (!m_spellsController) m_spellsController = new SpellsController();
        rootObject["Spells"] = m_spellsController->saveSettings();
    }

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(rootObject);
        file.write(doc.toJson());
        file.close();
    }
}



void BotController::onLoadRequested(const QString &name, const QString &category) {
    if (name.isEmpty()) return;
    QString filePath = QCoreApplication::applicationDirPath() + "/Save/" + name + ".json";
    if (!QFile::exists(filePath)) return;

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    QJsonObject rootObject = doc.object();
    file.close();

    if ((category == "All" || category == "Healing") && rootObject.contains("Healing")) {
        if (!m_healingController) m_healingController = new HealingController();
        m_healingController->loadSettings(rootObject["Healing"].toArray());
    }
    if ((category == "All" || category == "Walker") && rootObject.contains("Walker")) {
        if (!m_walkerController) m_walkerController = new WalkerController();
        m_walkerController->loadSettings(rootObject["Walker"].toArray());
    }
    if ((category == "All" || category == "Targeting") && rootObject.contains("Targeting")) {
        if (!m_targetingController) m_targetingController = new TargetingController();
        m_targetingController->loadSettings(rootObject["Targeting"].toArray());
    }
    if ((category == "All" || category == "Looting") && rootObject.contains("Looting")) {
        if (!m_lootingController) m_lootingController = new LootingController();
        m_lootingController->loadSettings(rootObject["Looting"].toArray());
    }
    if ((category == "All" || category == "Spells") && rootObject.contains("Spells")) {
        if (!m_spellsController) m_spellsController = new SpellsController();
        m_spellsController->loadSettings(rootObject["Spells"].toArray());
    }
}




void BotController::openWalkerView() {
    if (!m_walkerController)
    {
        m_walkerController = new WalkerController();
    } else {
        m_walkerController->showView();
    }
}

void BotController::onShowTargetingModule()
{
    if (!m_targetingController)
    {
        m_targetingController = new TargetingController();
    } else {
        m_targetingController->showView();
    }
}

void BotController::onShowLootingModule()
{
    if (!m_lootingController)
    {
        m_lootingController = new LootingController();
    } else {
        m_lootingController->showView();
    }
}

void BotController::onShowHealingModule()
{
    if (!m_healingController)
    {
        m_healingController = new HealingController();
    } else {
        m_healingController->showView();
    }
}

void BotController::onShowSpellsModule() {
    if (!m_spellsController)
    {
        m_spellsController = new SpellsController();
    } else {
        m_spellsController->showView();
    }
}
