#ifndef LUAENGINE_H
#define LUAENGINE_H
#include <QThread>
#include <string>
#include <atomic>
#include <lua.hpp>
#include "LuaBindings.h"

class LuaEngine : public QThread {
    Q_OBJECT
public:
    explicit LuaEngine(const std::string &script, QObject* parent)
        : QThread(parent), m_script(script), m_shouldStop(false) {}

    void requestStop();

protected:
    void run() override;

private:
    void executeLuaScript(const std::string &scriptText);
    void initLua();
    void closeLua();
    static void luaHookCallback(lua_State* L, lua_Debug* ar);

    lua_State* L = nullptr;
    std::string m_script;
    std::atomic<bool> m_shouldStop;
};




#endif //LUAENGINE_H
