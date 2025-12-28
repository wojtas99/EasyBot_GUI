#include "ScriptsController.h"

ScriptsController::ScriptsController(QObject *parent)
    : QObject(parent) {
    m_model = new ScriptsModel(this);
    m_view = new ScriptsView();
    m_view->show();

    // Scripts View requests
    connect(m_view, &ScriptsView::addItem_signal, this, &ScriptsController::addItem_View);
}

ScriptsController::~ScriptsController() {
}

void ScriptsController::showView() {
    m_view->showNormal();
    m_view->show();
}

QJsonArray ScriptsController::saveSettings() const {
    if (m_model) return m_model->toJson();
    return QJsonArray();
}

void ScriptsController::loadSettings(const QJsonArray& json) {
    if (m_model) m_model->fromJson(json);
}

void ScriptsController::addItem_View(const int &id, bool state, const QString &script_text) {
    m_model->addItem(id, state, script_text);
}

void ScriptsController::startScripts_slot(bool state) {
    m_model->startScripts(state);
}
