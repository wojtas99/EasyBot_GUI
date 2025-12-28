#ifndef SPELLSVIEW_H
#define SPELLSVIEW_H

#include <QMainWindow>
#include "ui_SpellsView.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Spells_View; }
QT_END_NAMESPACE

class SpellsView : public QMainWindow {
Q_OBJECT

public:
    explicit SpellsView(QWidget *parent = nullptr);
    ~SpellsView() override;

    void addItem(const QString &item);
    void clearListWidget();

    signals:
    void addItem_signal(const QString &target_name, const int &dist, const int &count, const QString &spell,
        const QString &spell_name, const int &type, const int &from, const int &to, const int &minHp, const int &minMp);
    void deleteItem_signal(const int &index);


private:
    Ui::Spells_View *ui;
};


#endif //SPELLSVIEW_H
