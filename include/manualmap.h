// manualmap.h - manual-map injection.
#pragma once
#include <cstdint>

namespace map {
    bool inject(std::uintptr_t pid, const uint8_t* payload, size_t len);
}