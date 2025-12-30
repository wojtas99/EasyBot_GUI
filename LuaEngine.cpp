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
    
    // Store 'this' pointer in Lua registry so hook can access it
    lua_pushlightuserdata(L, this);
    lua_setfield(L, LUA_REGISTRYINDEX, "LuaEngine_instance");
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
    
    // Set hook to check for interruption every 100 instructions
    lua_sethook(L, luaHookCallback, LUA_MASKCOUNT, 100);
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
        const char* error = lua_tostring(L, -1);
        // Don't print error if it was intentional interruption
        if (std::string(error) != "Script interrupted") {
            std::cerr << "Lua Error: " << error << std::endl;
        }
        lua_pop(L, 1);
    }
}

void LuaEngine::requestStop() {
    m_shouldStop.store(true);
}

void LuaEngine::luaHookCallback(lua_State* L, lua_Debug* ar) {
    // Retrieve the LuaEngine instance from registry
    lua_getfield(L, LUA_REGISTRYINDEX, "LuaEngine_instance");
    LuaEngine* engine = static_cast<LuaEngine*>(lua_touserdata(L, -1));
    lua_pop(L, 1);
    
    if (engine && engine->m_shouldStop.load()) {
        // Throw a Lua error to stop execution
        luaL_error(L, "Script interrupted");
    }
}
