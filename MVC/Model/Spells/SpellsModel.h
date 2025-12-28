#ifndef SPELLSMODEL_H
#define SPELLSMODEL_H
#include <QObject>
#include <QVariant>
#include "../../const.h"
#include "SpellsThread/UseSpell_Thread.h"

class SpellsModel : public QObject {
    Q_OBJECT
public:
    explicit SpellsModel(QObject *parent = nullptr);
    ~SpellsModel();

    void addItem(const QString &target_name, const int &dist, const int &count, const QString &spell_option,
        const QString &spell_name, const int &type, const int &from, const int &to, const int &minHp, const int &minMp);
    void startSpells(bool state);

    QJsonArray toJson() const;
    void fromJson(const QJsonArray &json);

    void deleteItem(const int &index);

    signals:
    void addItem_signal(const QString &item);
    void clearListWidget_signal();



private:
    std::vector<Spell> spells;
    UseSpell_Thread *useSpellThread = nullptr;

};



#endif //SPELLSMODEL_H
