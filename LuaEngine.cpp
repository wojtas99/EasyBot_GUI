#include "LuaEngine.h"


void LuaEngine::run() {
    initLua();
    executeLuaScript(m_script);
    closeLua();
}

void LuaEngine::initLua() {
    std::cout << "Creating Lua" << std::endl;
    L = luaL_newstate();
    luaL_openlibs(L);
    LuaBindings::registerAll(L);

    // Globalize proto functions
    const char* globalizeSnippet =
        "setmetatable(_G, {"
        "  __index = function(_, key)"
        "    local val = proto[key]"
        "    if type(val) == 'function' then"
        "      return function(...) return val(proto, ...) end"
        "    end"
        "    return val"
        "  end"
        "})";

    if (luaL_dostring(L, globalizeSnippet) != LUA_OK) {
        std::cerr << "Globalize Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}

void LuaEngine::closeLua() {
    if (L) {
        lua_close(L);
        L = nullptr;
    }
    std::cout << "Closing Lua" << std::endl;
}



void LuaEngine::executeLuaScript(const std::string &scriptText) {
    if (!L) return;
    if (luaL_dostring(L, scriptText.c_str()) != LUA_OK) {
        std::cerr << "Lua Error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
    }
}
