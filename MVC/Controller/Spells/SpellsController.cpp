#include "SpellsController.h"




SpellsController::SpellsController(QObject *parent)
    : QObject(parent) {
    m_model = new SpellsModel(this);
    m_view = new SpellsView();
    m_view->show();

    // Targeting View requests
    connect(m_view, &SpellsView::addItem_signal, this, &SpellsController::addItem_View);
    connect(m_view, &SpellsView::deleteItem_signal, this, &SpellsController::deleteItem_View);

    // Targeting Model requests
    connect(m_model, &SpellsModel::addItem_signal, this, &SpellsController::addItem_Model);
    connect(m_model, &SpellsModel::clearListWidget_signal, this, &SpellsController::clearListWidget_Model);
}

SpellsController::~SpellsController() {
}

void SpellsController::showView() {
    if (m_view) {
        m_view->show();
    }
}

QJsonArray SpellsController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void SpellsController::loadSettings(const QJsonArray& json) {
    if (m_model) m_model->fromJson(json);
}

void SpellsController::startSpells_slot(bool state) {
    m_model->startSpells(state);
}

void SpellsController::addItem_View(const QString& target_name, const int& dist, const int& count,
    const QString& spell, const QString& spell_name, const int& type, const int &from, const int &to, const int &minHp, const int &minMp) {
    m_model->addItem(target_name, dist, count, spell, spell_name, type, from, to, minHp, minMp);
}

void SpellsController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

void SpellsController::addItem_Model(const QString& item) {
    m_view->addItem(item);
}

void SpellsController::clearListWidget_Model() {
    m_view->clearListWidget();
}

