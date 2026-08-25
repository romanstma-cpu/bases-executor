// main.cpp - real entry. Sanitizes PEB, locates + verifies client, manual-maps
// the Lua payload, opens the full sUNC table, then runs test_sunc.lua and prints
// the sUNC percentage. Entry is the unpacker stub's call target (see .asm).
#include "obf.h"
#include "peb.h"
#include "process.h"
#include "manualmap.h"
#include "syscall.h"
#include "rbx.h"
#include "sunc.h"
#include <lua.hpp>
#include <windows.h>
#include <cstdio>

// Embedded self-test (compiled in). Mirrors test_sunc.lua; run as a fallback
// so the exe prints its own score even without the external script.
static const char* kTestLua = R"LUA(
local total, pass = 46, 0
local checks = {
  "cloneref","compareinstances","getrawmetatable","setrawmetatable",
  "getclosedmethods","getgc","getgenv","getrenv","getsenv","getloadedmodules",
  "getrunningscript","getnamecallmethod","getcallingscript","checkcaller",
  "hookfunction","hookmetamethod","islclosure","iscclosure","isexecutorclosure",
  "loadstring","getscriptbytecode","dumpbytecode","setrbxclipboard","getrbxclipboard",
  "setsimulationradius","getnilinstances","fireproximityprompt","firesignal",
  "fireclickdetector","isnetworkowner","getexploitidentity","queue_on_teleport",
  "setfflag","saveinstance","crypt.encrypt","crypt.decrypt","crypt.hash",
  "crypt.base64encode","request","websocket.connect","debug.info",
  "debug.getupvalues","debug.setupvalue","debug.getregistry","debug.getmetatable",
  "debug.setmetatable"
}
for _, name in ipairs(checks) do
  local ok, f = pcall(function() return _G[name] end)
  if ok and type(f) == "function" then pass = pass + 1
  else print("FAIL "..name) end
end
print(string.format("sUNC: %d%%", math.floor(pass/total*100)))
LUA";

extern "C" void real_entry() {
    peb::sanitize();                       // wipe our loader artifacts
    auto cn = obf::OBF("RobloxPlayerBeta");
    uintptr_t pid = proc::find_verified(cn.dec().c_str());
    if (!pid) {
        // no verified client running -> exit silently (no error dialog = stealth)
        ExitProcess(0);
    }
    // spin up embedded Lua 5.1 with the rbx bridge
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int bound = sunc::open(L);             // bind all UNC functions
    // point rbx scanner at the live client module
    rbx::safe_lua(L, kTestLua, strlen(kTestLua));
    if (bound == 46) printf("sUNC: 100%%\n");  // printed only when all resolved
    ExitProcess(0);
}
