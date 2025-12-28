#ifndef WALKERVIEW_H
#define WALKERVIEW_H

#include "ui_WalkerView.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Walker_View; }
QT_END_NAMESPACE

class WalkerView : public QMainWindow {
Q_OBJECT

public:
    explicit WalkerView(QWidget *parent = nullptr);
    ~WalkerView() override;

    void addItem(const QString &item);
    void indexUpdate(int index);
    void clearListWidget();

signals:
    void addItem_signal(const QString &direction, const QString &option, const QString &action);
    void recordWaypoints_signal(bool state, int sqmDist, const QString &direction, const QString &option);
    void deleteItem_signal(const int &index);

private:
    Ui::Walker_View *ui;
};


#endif //WALKERVIEW_H
