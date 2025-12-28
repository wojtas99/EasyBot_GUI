#ifndef LOOTITEMS_THREAD_H
#define LOOTITEMS_THREAD_H
#include <Qthread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class LootItems_Thread : public QThread {
    Q_OBJECT
    public:
    explicit LootItems_Thread(const std::vector<Item> &items, bool nextBpState, bool corpseBpState, QObject *parent = nullptr)
        : QThread(parent), items(items), m_nextBpState(nextBpState), m_corpseBpState(corpseBpState) {}
public slots:
    void nextBpStateChange(bool state);
    void corpseBpStateChange(bool state);
protected:
    void run() override;
private:
    std::vector<Item> items;
    bool m_nextBpState = false;
    bool m_corpseBpState = false;

    bool performLooting(int itemID, int sleepTime, Position destPos, std::string dest_name);
};



#endif //LOOTITEMS_THREAD_H
