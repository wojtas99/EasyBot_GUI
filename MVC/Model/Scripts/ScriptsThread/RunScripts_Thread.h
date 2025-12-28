#ifndef SCRIPTSTHREAD_H
#define SCRIPTSTHREAD_H
#include <QThread>
#include "../../const.h"
#include "../../proto_functions_client.h"

class RunScripts_Thread : public QThread {
    Q_OBJECT
    public:
    explicit RunScripts_Thread(const std::vector<Script>& scripts, QObject *parent = nullptr)
        : QThread(parent), m_scripts(scripts) {}

protected:
    void run() override;
private:
    std::vector<Script> m_scripts;
};

#endif // SCRIPTSTHREAD_H
