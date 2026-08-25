// process.h - locate Roblox client process.
#pragma once
#include <cstdint>

namespace proc {
    std::uintptr_t find_verified(const char* class_name);
}