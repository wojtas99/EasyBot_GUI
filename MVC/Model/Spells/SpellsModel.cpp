#include "SpellsModel.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>



SpellsModel::SpellsModel(QObject *parent)
    : QObject(parent) {
}

SpellsModel::~SpellsModel() {
    if (useSpellThread) {
        useSpellThread->quit();
        useSpellThread->wait();
        delete useSpellThread;
    }
}

void SpellsModel::addItem(const QString& target_name, const int& dist, const int& count, const QString& spell_option,
    const QString& spell_name, const int& type, const int &from, const int &to, const int &minHp, const int &minMp) {
    auto item = QStringLiteral("Targets: %1 %2 \"%3\"").arg(target_name).arg(spell_option).arg(spell_name);
    auto tmp_targetName = target_name.toStdString();
    std::transform(tmp_targetName.begin(), tmp_targetName.end(), tmp_targetName.begin(), ::tolower);
    Spell spell;
    spell.targets = tmp_targetName;
    spell.dist = dist;
    spell.count = count;
    spell.option = spell_option.toStdString();
    spell.name = spell_name.toStdString();
    spell.type = type;
    spell.from = from;
    spell.to = to;
    spell.minHp = minHp;
    spell.minMp = minMp;
    spells.push_back(spell);
    emit addItem_signal(item);
}

void SpellsModel::startSpells(bool state) {
    if (state) {
        if (!useSpellThread) {
            useSpellThread = new UseSpell_Thread(spells, this);
            connect(useSpellThread, &QThread::finished, useSpellThread, &QObject::deleteLater);
            connect(useSpellThread, &QThread::finished, this, [this]() {
                this->useSpellThread = nullptr;
            });
            useSpellThread->start();
        }
    } else {
        if (useSpellThread) {
            useSpellThread->requestInterruption();
            useSpellThread->quit();
        }
    }
}

QJsonArray SpellsModel::toJson() const {
    QJsonArray jsonArray;
    for (const auto &spell : spells) {
        QJsonObject jsonObj;
        jsonObj["targets"] = QString::fromStdString(spell.targets);
        jsonObj["dist"] = spell.dist;
        jsonObj["count"] = spell.count;
        jsonObj["option"] = QString::fromStdString(spell.option);
        jsonObj["name"] = QString::fromStdString(spell.name);
        jsonObj["type"] = spell.type;
        jsonObj["from"] = spell.from;
        jsonObj["to"] = spell.to;
        jsonObj["minHp"] = spell.minHp;
        jsonObj["minMp"] = spell.minMp;
        jsonArray.append(jsonObj);
    }
    return jsonArray;
}

void SpellsModel::fromJson(const QJsonArray &json) {
    spells.clear();
    emit clearListWidget_signal();
    for (const auto &val : json) {
        QJsonObject obj = val.toObject();
        QString targets = obj["targets"].toString();
        int dist = obj["dist"].toInt();
        int count = obj["count"].toInt();
        QString option = obj["option"].toString();
        QString name = obj["name"].toString();
        int type = obj["type"].toInt();
        int from = obj["from"].toInt();
        int to = obj["to"].toInt();
        int minHp = obj["minHp"].toInt();
        int minMp = obj["minMp"].toInt();
        addItem(targets, dist, count, option, name, type, from, to, minHp, minMp);
    }
}

void SpellsModel::deleteItem(const int& index) {
    spells.erase(spells.begin() + index);
}

