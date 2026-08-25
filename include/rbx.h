// rbx.h - runtime access to the live Roblox VM inside the target process.
// Everything here resolves through signature scans against the client module;
// concrete byte patterns live in rbx.cpp and are marked [REFRESH PER VERSION].
#pragma once
#include <cstdint>

namespace rbx {
    using lua_State = void*;

    // Resolve the global DataModel singleton (game).
    uintptr_t get_datamodel();
    // Resolve the task scheduler (FRM/GRM) so we can queue on the engine thread.
    uintptr_t get_scheduler();
    // Find a function pointer inside the client by name/pattern. Returns 0 if
    // the current client version's pattern is stale (call refresh_patterns()).
    uintptr_t resolve(const char* symbol);
    // Run a lua chunk on the engine thread via the scheduler (safe call).
    int      safe_lua(lua_State* L, const char* chunk, size_t len);

    // Pattern registry management. When a resolve() fails, the harness prints
    // the symbol name so LO can paste a fresh pattern from his RE session.
    void     refresh_patterns(const char* json_blob);  // json: {"symbol":"hexpat"}
}
