#include "HealingController.h"
#include <QJsonArray>


HealingController::HealingController(QObject *parent)
    : QObject(parent) {
    m_model = new HealingModel(this);
    m_view = new HealingView();
    m_view->show();

    // Healing View requests
    connect(m_view, &HealingView::addItem_signal, this, &HealingController::addItem_View);
    connect(m_view, &HealingView::deleteItem_signal, this, &HealingController::deleteItem_View);

    // Healing Model requests
    connect(m_model, &HealingModel::addItem_signal, this, &HealingController::addItem_Model);
    connect(m_model, &HealingModel::clearListWidget_signal, this, &HealingController::clearListWidget_Model);


}

HealingController::~HealingController() {
}

void HealingController::showView() {
    if (m_view) {
        m_view->show();
    }
}

void HealingController::addItem_View(const QString &action, const QString &heal, const QString &condition, const int &random,
        const int &below, const int &above, const int &minimum) {
    m_model->addItem(action, heal, condition, random, below, above, minimum);
}

void HealingController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

void HealingController::addItem_Model(const QString& item) {
    m_view->addItem(item);
}

void HealingController::clearListWidget_Model() {
    m_view->clearListWidget();
}

void HealingController::startHealing_slot(bool state) {
    m_model->startHealing(state);
}

QJsonArray HealingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void HealingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}

