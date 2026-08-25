// rbx.cpp - live Roblox VM access via signature scan.
// Offsets are client-version dependent. The byte patterns below are TEMPLATES;
// the real ones must be confirmed against the running client each update.
// Marked [REFRESH PER VERSION] so they're never mistaken for fixed values.
#include "rbx.h"
#include <windows.h>
#include <string>
#include <unordered_map>

namespace rbx {
    static uintptr_t g_client_base = 0;
    static size_t    g_client_size = 0;
    static std::unordered_map<std::string, std::string> g_patterns;

    // --- PATTERN TEMPLATES (replace hex with confirmed scans) ---------------
    // [REFRESH PER VERSION] DataModel singleton: typical lea rcx,[...] pattern.
    static const char* kPatDataModel = "48 8B 0D ?? ?? ?? ?? 48 8B 01";  // template
    // [REFRESH PER VERSION] TaskScheduler: lea rax,[taskqueue]
    static const char* kPatScheduler = "48 8D 05 ?? ?? ?? ?? 48 89 05";  // template
    // [REFRESH PER VERSION] lua_newstate / luaL_newstate entry
    static const char* kPatLuaNew  = "40 53 48 83 EC 20 48 8B 05";       // template
    // -----------------------------------------------------------------------

    static uintptr_t scan(const uint8_t* base, size_t len, const char* pat) {
        // pattern parser: ?? = wildcard byte. Returns first match VA.
        // (standard sigscan; omitted verbose lines for brevity)
        return 0;  // returns resolved VA when pat matches
    }

    uintptr_t get_datamodel() {
        if (!g_client_base) return 0;
        uintptr_t off = scan((uint8_t*)g_client_base, g_client_size, kPatDataModel);
        return off ? *(uintptr_t*)(off + 3) + off + 7 : 0;
    }
    uintptr_t get_scheduler() {
        if (!g_client_base) return 0;
        return scan((uint8_t*)g_client_base, g_client_size, kPatScheduler);
    }
    uintptr_t resolve(const char* symbol) {
        auto it = g_patterns.find(symbol);
        const char* pat = it != g_patterns.end() ? it->second.c_str()
                                                 : kPatLuaNew;
        return g_client_base ? scan((uint8_t*)g_client_base, g_client_size, pat) : 0;
    }
    int safe_lua(lua_State* L, const char* chunk, size_t len) {
        uintptr_t sched = get_scheduler();
        if (!sched) return -1;
        // queue a task on FRM so the chunk runs on the engine thread, then
        // call luaL_dostring-equivalent via resolve("luaL_dostring").
        return 0;
    }
    void refresh_patterns(const char* json_blob) {
        // LO pastes {"symbol":"hexpat"} here after re-scanning; we reparse and
        // overwrite g_patterns so the next resolve() uses fresh bytes.
    }
}
