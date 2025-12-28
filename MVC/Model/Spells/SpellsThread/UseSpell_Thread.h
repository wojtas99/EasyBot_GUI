#ifndef USESPELL_THREAD_H
#define USESPELL_THREAD_H
#include <QThread>
#include "../../const.h"
#include "../../proto_functions_client.h"


class UseSpell_Thread : public QThread {
    Q_OBJECT
    public:
    explicit UseSpell_Thread(const std::vector<Spell> &spells, QObject *parent = nullptr)
    : QThread(parent), m_spells(spells) {}

protected:
    void run() override;


private:
    std::vector<Spell> m_spells;

};



#endif //USESPELL_THREAD_H
