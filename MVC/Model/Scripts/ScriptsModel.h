#ifndef SCRIPTSMODEL_H
#define SCRIPTSMODEL_H
#include <QObject>
#include <QVariant>
#include <QJsonArray>
#include <vector>
#include "../../const.h"
#include "ScriptsThread/RunScripts_Thread.h"


class ScriptsModel : public QObject {
    Q_OBJECT
public:
    explicit ScriptsModel(QObject *parent = nullptr);
    ~ScriptsModel();


    void addItem(const int &id, bool state, const QString &script_text);
    void startScripts(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);
private:
    std::vector<Script> scripts;
    RunScripts_Thread *runScriptsThread = nullptr;
};

#endif // SCRIPTSMODEL_H
