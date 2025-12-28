//
// Created by Wojciech on 11.10.2025.
//

#ifndef WALKERCONTROLLER_H
#define WALKERCONTROLLER_H
#include <QObject>

#include "Walker/WalkerModel.h"
#include "Walker/WalkerView.h"


class WalkerController : public QObject {
    Q_OBJECT
public:
    explicit WalkerController(QObject *parent = nullptr);

    void showView();
    QJsonArray saveSettings() const;
    void loadSettings(const QJsonArray &json);

private slots:
    // WalkerView Requests
    void addItem_View(const QString &direction, const QString &option, const QString &action);
    void recordWaypoints_View(bool state, int sqmDist, const QString &direction, const QString &option);
    void deleteItem_View(const int &index);

    // WalkerModel Requests
    void addItem_Model(const QString &item);
    void indexUpdate_Model(int index);
    void clearListWidget_Model();



public slots:
    void startWalker_slot(bool state);

private:
    WalkerView *m_view;
    WalkerModel *m_model;
};


#endif //WALKERCONTROLLER_H
