#ifndef SPELLSCONTROLLER_H
#define SPELLSCONTROLLER_H
#include <QObject>
#include "Spells/SpellsModel.h"
#include "Spells/SpellsView.h"
#include <QJsonArray>


class SpellsController : public QObject {
    Q_OBJECT
public:
    explicit SpellsController(QObject *parent = nullptr);
    ~SpellsController();

    void showView();

    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);



public slots:
    // SpellsView Requests
    void startSpells_slot(bool state);
    void addItem_View(const QString &target_name, const int &dist, const int &count, const QString &spell,
        const QString &spell_name, const int &type, const int &from, const int &to, const int &minHp, const int &minMp);
    void deleteItem_View(const int &index);

    // SpellsModel Requests
    void addItem_Model(const QString &item);
    void clearListWidget_Model();




private:
    SpellsView *m_view;
    SpellsModel *m_model;
};


#endif //SPELLSCONTROLLER_H
