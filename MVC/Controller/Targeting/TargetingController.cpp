//
// Created by blazz on 12.10.2025.
//

#include "TargetingController.h"
#include <QJsonArray>



TargetingController::TargetingController(QObject* parent)
    : QObject(parent) {
    m_model = new TargetingModel(this);
    m_view = new TargetingView();

    // Targeting View requests
    connect(m_view, &TargetingView::addItem_signal, this, &TargetingController::addItem_View);
    connect(m_view, &TargetingView::shootableState_signal, this, &TargetingController::shootableState_View);
    connect(m_view, &TargetingView::reachableState_signal, this, &TargetingController::reachableState_View);
    connect(m_view, &TargetingView::openCorpseState_signal, this, &TargetingController::openCorpseState_View);
    connect(m_view, &TargetingView::stayAwayDist_signal, this, &TargetingController::stayAwayDist_View);
    connect(m_view, &TargetingView::deleteItem_signal, this, &TargetingController::deleteItem_View);

    // Targeting Model requests
    connect(m_model, &TargetingModel::addItem_signal, this, &TargetingController::addItem_Model);
    connect(m_model, &TargetingModel::clearListWidget_signal, this, &TargetingController::clearListWidget_Model);


    m_view->show();
}

TargetingController::~TargetingController()
{
}

void TargetingController::showView()
{
    if (m_view) {
        m_view->show();
    }
}

void TargetingController::addItem_View(const QString &targetName, const int &dist, const int &count,
    const QString &desiredStance, const QString &monstersAttacks) {
    m_model->addItem(targetName, dist, count, desiredStance, monstersAttacks);
}

void TargetingController::shootableState_View(bool state) {
    m_model->shootableState(state);
}

void TargetingController::reachableState_View(bool state) {
    m_model->reachableState(state);
}

void TargetingController::openCorpseState_View(bool state) {
    m_model->openCorpseState(state);
}

void TargetingController::stayAwayDist_View(int value) {
    m_model->stayAwayDist(value +1);
}

void TargetingController::deleteItem_View(const int& index) {
    m_model->deleteItem(index);
}

void TargetingController::addItem_Model(const QString &item) {
    m_view->addItem(item);
}

void TargetingController::clearListWidget_Model() {
    m_view->clearListWidget();
}

void TargetingController::startTargeting_slot(bool state) {
    m_model->startTargeting(state);
}

QJsonArray TargetingController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void TargetingController::loadSettings(const QJsonArray &json) {
    if (m_model) m_model->fromJson(json);
}


