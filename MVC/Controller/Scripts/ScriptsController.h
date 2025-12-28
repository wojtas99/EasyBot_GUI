#ifndef SCRIPTSCONTROLLER_H
#define SCRIPTSCONTROLLER_H
#include <QObject>
#include <QJsonArray>
#include "Scripts/ScriptsModel.h"
#include "Scripts/ScriptsView.h"
#include "../../Model/Scripts/ScriptsThread/RunScripts_Thread.h"


class ScriptsController : public QObject {
    Q_OBJECT
public:
    explicit ScriptsController(QObject *parent = nullptr);
    ~ScriptsController();

    void showView();

    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);

private slots:
    // ScriptsView Requests
    void addItem_View(const int &id, bool state, const QString &script_text);

public slots:
    void startScripts_slot(bool state);

private:
    ScriptsView *m_view;
    ScriptsModel *m_model;
};


#endif // SCRIPTSCONTROLLER_H
