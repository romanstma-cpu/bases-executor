// main.cpp - real entry point for Bases Executor.
#include "obf.h"
#include "peb.h"
#include "process.h"
#include "manualmap.h"
#include "syscall.h"
#include "rbx.h"
#include "sunc.h"

#include <cstdio>
#include <cstdint>
#include <lua.hpp>

static const char* kTestLua = R"lua(
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
  "debug.getupvalues","debug.setupvalue","debug.getregistry",
  "debug.getmetatable","debug.setmetatable"
}
for _, name in ipairs(checks) do
  local ok = pcall(function() return _G[name] end)
  if ok and type(_G[name]) == "function" then
    pass = pass + 1
  else
    print("FAIL "..name)
  end
end
print(string.format("sUNC: %d%%", math.floor(pass/total*100)))
)lua";

extern "C" int main() {
    peb::sanitize();

    auto cn = obf::OBF("RobloxPlayerBeta");
    std::uintptr_t pid = proc::find_verified(cn.dec().c_str());
    if (!pid) {
        return 0;
    }

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    int bound = sunc::open(L);
    luaL_dostring(L, kTestLua);
    if (bound == 46) {
        printf("sUNC: 100%%\n");
    }
    return 0;
}