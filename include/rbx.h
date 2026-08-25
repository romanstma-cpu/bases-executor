// rbx.h - live Roblox VM access.
#pragma once
#include <cstdint>

namespace rbx {
    using lua_State = void*;
    uintptr_t get_datamodel();
    uintptr_t get_scheduler();
    uintptr_t resolve(const char* symbol);
    int safe_lua(lua_State* L, const char* chunk, size_t len);
    void refresh_patterns(const char* json_blob);
}