// process.h - locate the Roblox client and verify it is a legit,
// user-launched process (explorer.exe parent). Reject anything else.
#pragma once
#include <cstdint>

namespace proc {
    // Returns PID of a verified RobloxPlayerBeta, or 0 if none / spoofed.
    uintptr_t find_verified(const char* class_name);
}
