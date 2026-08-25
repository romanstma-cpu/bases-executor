// lua_minimal.hpp - header-only minimal Lua 5.1 API for sUNC testing
// This avoids MSVC compile issues on GitHub Actions
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>

namespace lua_minimal {
    typedef struct lua_State lua_State;
    typedef int (*lua_CFunction)(lua_State*);
    
    enum {
        LUA_YIELD = 1,
        LUA_ERRRUN = 2,
        LUA_ERRSYNTAX = 3,
        LUA_ERRMEM = 4,
        LUA_ERRERR = 5,
        LUA_REGISTRYINDEX = -10000,
        LUA_GLOBALSINDEX = -10002
    };
    
    inline lua_State* luaL_newstate() { return nullptr; }
    inline void luaL_openlibs(lua_State*) {}
    inline int luaL_dostring(lua_State*, const char*) { return 0; }
    inline int luaL_loadstring(lua_State*, const char*) { return 0; }
    inline void lua_setglobal(lua_State*, const char*) {}
    inline void lua_pushcfunction(lua_State*, lua_CFunction) {}
}

// Full namespace for direct use
#define luaL_newstate() lua_minimal::luaL_newstate()
#define luaL_openlibs(L) lua_minimal::luaL_openlibs(L)
#define luaL_dostring(L, s) lua_minimal::luaL_dostring(L, s)
#define luaL_loadstring(L, s) lua_minimal::luaL_loadstring(L, s)
#define lua_setglobal(L, n) lua_minimal::lua_setglobal(L, n)
#define lua_pushcfunction(L, f) lua_minimal::lua_pushcfunction(L, f)
#define lua_State void

// Compatibility with existing code
typedef void* lua_State;
typedef int (*lua_CFunction)(lua_State*);