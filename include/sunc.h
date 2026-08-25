// sunc.h - full Solara UNC function table.
#pragma once
#include <cstdint>

namespace sunc {
    using lua_State = void*;
    int open(lua_State* L);

    int l_cloneref(lua_State* L);
    int l_compareinstances(lua_State* L);
    int l_getrawmetatable(lua_State* L);
    int l_setrawmetatable(lua_State* L);
    int l_getclosedmethods(lua_State* L);
    int l_getgc(lua_State* L);
    int l_getgenv(lua_State* L);
    int l_getrenv(lua_State* L);
    int l_getsenv(lua_State* L);
    int l_getloadedmodules(lua_State* L);
    int l_getrunningscript(lua_State* L);
    int l_getnamecallmethod(lua_State* L);
    int l_getcallingscript(lua_State* L);
    int l_checkcaller(lua_State* L);
    int l_hookfunction(lua_State* L);
    int l_hookmetamethod(lua_State* L);
    int l_islclosure(lua_State* L);
    int l_iscclosure(lua_State* L);
    int l_isexecutorclosure(lua_State* L);
    int l_loadstring(lua_State* L);
    int l_getscriptbytecode(lua_State* L);
    int l_dumpbytecode(lua_State* L);
    int l_setrbxclipboard(lua_State* L);
    int l_getrbxclipboard(lua_State* L);
    int l_setsimulationradius(lua_State* L);
    int l_getnilinstances(lua_State* L);
    int l_fireproximityprompt(lua_State* L);
    int l_firesignal(lua_State* L);
    int l_fireclickdetector(lua_State* L);
    int l_isnetworkowner(lua_State* L);
    int l_getexploitidentity(lua_State* L);
    int l_queue_on_teleport(lua_State* L);
    int l_setfflag(lua_State* L);
    int l_saveinstance(lua_State* L);
    int l_crypt_encrypt(lua_State* L);
    int l_crypt_decrypt(lua_State* L);
    int l_crypt_hash(lua_State* L);
    int l_crypt_base64encode(lua_State* L);
    int l_request(lua_State* L);
    int l_websocket_connect(lua_State* L);
    int l_debug_info(lua_State* L);
    int l_debug_getupvalues(lua_State* L);
    int l_debug_setupvalue(lua_State* L);
    int l_debug_getregistry(lua_State* L);
    int l_debug_getmetatable(lua_State* L);
    int l_debug_setmetatable(lua_State* L);
}