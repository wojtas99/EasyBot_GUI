#include "RunScripts_Thread.h"
#include "../../../../LuaEngine.h"
#include <iostream>
#include <regex>
#include <QRegularExpression>

void RunScripts_Thread::run() {
    LuaEngine *luaEngine = nullptr;
    for (const auto& script : m_scripts) {
        if (script.enabled) {
            luaEngine = new LuaEngine(script.text, nullptr);
            luaEngine->moveToThread(QThread::currentThread());
            connect(luaEngine, &QThread::finished, luaEngine, &QObject::deleteLater);
            luaEngine->start();
        }
    }
    std::cout << "Staring Thread" << std::endl;
    while (!isInterruptionRequested()) {
        msleep(10);
    }
    std::cout << "End" << std::endl;
}
