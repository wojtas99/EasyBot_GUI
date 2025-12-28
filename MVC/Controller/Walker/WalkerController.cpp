//
// Created by Wojciech on 11.10.2025.
//


#include "WalkerController.h"
#include <QJsonArray>


WalkerController::WalkerController(QObject *parent)
    : QObject(parent) {
    m_model = new WalkerModel(this);
    m_view = new WalkerView();

    // Walker View requests
    connect(m_view, &WalkerView::addItem_signal, this, &WalkerController::addItem_View);
    connect(m_view, &WalkerView::recordWaypoints_signal, this, &WalkerController::recordWaypoints_View);
    connect(m_view, &WalkerView::deleteItem_signal, this, &WalkerController::deleteItem_View);

    // Walker Model requests
    connect(m_model, &WalkerModel::addItem_signal, this, &WalkerController::addItem_Model);
    connect(m_model, &WalkerModel::indexUpdate_signal, this, &WalkerController::indexUpdate_Model);
    connect(m_model, &WalkerModel::clearListWidget_signal, this, &WalkerController::clearListWidget_Model);


    m_view->show();
}


void WalkerController::showView() {
    m_view->show();
}

void WalkerController::startWalker_slot(bool state) {
    m_model->startWalker(state);
}

QJsonArray WalkerController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void WalkerController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}


// Walker View Requests
void WalkerController::addItem_View(const QString &direction, const QString &option, const QString &action)
{
    m_model->addItem(direction, option, action);
}
void WalkerController::recordWaypoints_View(bool state, int sqmDist, const QString &direction, const QString &option)
{
    m_model->recordWaypoints(state, sqmDist, direction, option);
}

void WalkerController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

// Walker Model Requests
void WalkerController::addItem_Model(const QString &item)
{
    m_view->addItem(item);
}

void WalkerController::indexUpdate_Model(int index) {
    m_view->indexUpdate(index);
}

void WalkerController::clearListWidget_Model() {
    m_view->clearListWidget();
}



