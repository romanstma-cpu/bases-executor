// manualmap.h - manual-map a position-independent payload via early-bird APC.
// No WriteProcessMemory of a raw DLL; imports resolved in-target.
#pragma once
#include <cstdint>

namespace map {
    // Maps 'payload' (a PE/DLL buffer) into 'pid' and queues an APC in a
    // alertable thread. Returns true on queued.
    bool inject(uintptr_t pid, const uint8_t* payload, size_t len);
}
