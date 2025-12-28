#include "LootingController.h"
#include <QJsonArray>



LootingController::LootingController(QObject *parent)
    : QObject(parent) {
    m_model = new LootingModel(this);
    m_view = new LootingView();

    // Looting View requests
    connect(m_view, &LootingView::addItem_signal, this, &LootingController::addItem_View);
    connect(m_view, &LootingView::nextBpState_signal, this, &LootingController::nextBpState_View);
    connect(m_view, &LootingView::corpseBpState_signal, this, &LootingController::corpseBpState_View);
    connect(m_view, &LootingView::deleteItem_signal, this, &LootingController::deleteItem_View);



    // Looting Model requests
    connect(m_model, &LootingModel::addItem_signal, this, &LootingController::addItem_Model);
    connect(m_model, &LootingModel::clearListWidget_signal, this, &LootingController::clearListWidget_Model);


    m_view->show();
}

LootingController::~LootingController() {
}

void LootingController::showView() {
    if (m_view) {
        m_view->show();
    }
}

void LootingController::addItem_View(const int& itemID, const QString& destination, const int& lootingSpeed) {
    m_model->addItem(itemID, destination, lootingSpeed);
}

void LootingController::nextBpState_View(bool state) {
    m_model->nextBpState(state);
}

void LootingController::corpseBpState_View(bool state) {
    m_model->corpseBpState(state);
}

void LootingController::deleteItem_View(const int& index){
    m_model->deleteItem(index);
}

void LootingController::addItem_Model(const QString& item) {
    m_view->addItem(item);
}

void LootingController::clearListWidget_Model() {
    m_view->clearListWidget();
}

void LootingController::startLooting_slot(bool state) {
    m_model->startLooting(state);
}

QJsonArray LootingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void LootingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}


