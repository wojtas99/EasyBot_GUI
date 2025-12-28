#include "ScriptsModel.h"

#include <iostream>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>


ScriptsModel::ScriptsModel(QObject *parent)
    : QObject(parent) {
}

ScriptsModel::~ScriptsModel() {
    if (runScriptsThread) {
        runScriptsThread->quit();
        runScriptsThread->wait();
        delete runScriptsThread;
    }
}

void ScriptsModel::addItem(const int &id, bool state, const QString &script_text) {
    Script script;
    script.text = script_text.toStdString();
    script.enabled = state;
    if (id < scripts.size()) {
        scripts[id] = script;
    } else {
        while (scripts.size() < id) {
            scripts.push_back(Script{});
        }
        scripts.push_back(script);
    }
}

void ScriptsModel::startScripts(bool state) {
    if (state) {
        if (!runScriptsThread) {
            runScriptsThread = new RunScripts_Thread(scripts, this);
            connect(runScriptsThread, &QThread::finished, runScriptsThread, &QObject::deleteLater);
            connect(runScriptsThread, &QThread::finished, this, [this]() {
                this->runScriptsThread = nullptr;
            });
            runScriptsThread->start();
        }
    } else {
        if (runScriptsThread) {
            runScriptsThread->requestInterruption();
            runScriptsThread->quit();
        }
    }
}


QJsonArray ScriptsModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &script : scripts) {
        QJsonObject obj;
        obj["text"] = script.text.c_str();
        obj["enabled"] = script.enabled;
        jsonArray.append(obj);
    }
    return jsonArray;
}

void ScriptsModel::fromJson(const QJsonArray &json) {
    scripts.clear();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        Script script;
        script.text = obj["text"].toString().toStdString();
        script.enabled = obj["enabled"].toBool();
        scripts.push_back(script);
    }
}
