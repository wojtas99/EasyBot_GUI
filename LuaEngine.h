#ifndef LUAENGINE_H
#define LUAENGINE_H
#include <QThread>
#include <string>
#include <lua.hpp>
#include "LuaBindings.h"

class LuaEngine : public QThread {
    Q_OBJECT
public:
    explicit LuaEngine(const std::string &script, QObject* parent)
        : QThread(parent), m_script(script) {}

protected:
    void run() override;

private:
    void executeLuaScript(const std::string &scriptText);
    void initLua();
    void closeLua();

    lua_State* L = nullptr;
    std::string m_script;
};




#endif //LUAENGINE_H
