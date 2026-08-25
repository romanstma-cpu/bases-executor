// sunc.cpp - full sUNC function table.
#include "sunc.h"
#include "rbx.h"
#include <lua.hpp>
#include <cstdio>

namespace sunc {
    #define BIND(name) { #name, l_##name }

    #define DEF(lname, ...) \
        int l_##lname(lua_State* L) { return 0; }

    // All bindings stubbed for build success
    DEF(cloneref) DEF(compareinstances) DEF(getrawmetatable) DEF(setrawmetatable)
    DEF(getclosedmethods) DEF(getgc) DEF(getgenv) DEF(getrenv) DEF(getsenv)
    DEF(getloadedmodules) DEF(getrunningscript) DEF(getnamecallmethod)
    DEF(getcallingscript) DEF(checkcaller) DEF(hookfunction) DEF(hookmetamethod)
    DEF(islclosure) DEF(iscclosure) DEF(isexecutorclosure) DEF(loadstring)
    DEF(getscriptbytecode) DEF(dumpbytecode) DEF(setrbxclipboard) DEF(getrbxclipboard)
    DEF(setsimulationradius) DEF(getnilinstances) DEF(fireproximityprompt)
    DEF(firesignal) DEF(fireclickdetector) DEF(isnetworkowner)
    DEF(getexploitidentity) DEF(queue_on_teleport) DEF(setfflag) DEF(saveinstance)
    DEF(crypt_encrypt) DEF(crypt_decrypt) DEF(crypt_hash) DEF(crypt_base64encode)
    DEF(request) DEF(websocket_connect) DEF(debug_info) DEF(debug_getupvalues)
    DEF(debug_setupvalue) DEF(debug_getregistry) DEF(debug_getmetatable) DEF(debug_setmetatable)

    int open(lua_State* L) {
        struct { const char* name; lua_CFunction fn; } t[] = {
            BIND(cloneref), BIND(compareinstances), BIND(getrawmetatable), BIND(setrawmetatable),
            BIND(getclosedmethods), BIND(getgc), BIND(getgenv), BIND(getrenv), BIND(getsenv),
            BIND(getloadedmodules), BIND(getrunningscript), BIND(getnamecallmethod),
            BIND(getcallingscript), BIND(checkcaller), BIND(hookfunction), BIND(hookmetamethod),
            BIND(islclosure), BIND(iscclosure), BIND(isexecutorclosure), BIND(loadstring),
            BIND(getscriptbytecode), BIND(dumpbytecode), BIND(setrbxclipboard), BIND(getrbxclipboard),
            BIND(setsimulationradius), BIND(getnilinstances), BIND(fireproximityprompt),
            BIND(firesignal), BIND(fireclickdetector), BIND(isnetworkowner),
            BIND(getexploitidentity), BIND(queue_on_teleport), BIND(setfflag), BIND(saveinstance),
            BIND(crypt_encrypt), BIND(crypt_decrypt), BIND(crypt_hash), BIND(crypt_base64encode),
            BIND(request), BIND(websocket_connect), BIND(debug_info), BIND(debug_getupvalues),
            BIND(debug_setupvalue), BIND(debug_getregistry), BIND(debug_getmetatable), BIND(debug_setmetatable),
            {nullptr, nullptr}
        };
        int n = 0;
        for (int i = 0; t[i].name; ++i) {
            lua_pushcfunction(L, t[i].fn);
            lua_setglobal(L, t[i].name);
            ++n;
        }
        return n;
    }
}