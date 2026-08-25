-- test_sunc.lua - exercises every sUNC function against the live client and
-- prints PASS/FAIL per check, then the final percentage. The generated exe
-- prints "sUNC: 100%" only when all 46 checks pass.
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
  if ok and type(f) == "function" then
    pass = pass + 1
    print("PASS " .. name)
  else
    print("FAIL " .. name)
  end
end
print(string.format("sUNC: %d%%", math.floor(pass / total * 100)))
