// syscall.h - Hell's Gate / Halos Gate direct syscall resolver.
// Parses ntdll in-process, recovers hooked syscalls by walking the
// syscall instruction, returns syscall numbers resolved at runtime.
// No win32 memory APIs are used past this boundary.
#pragma once
#include <cstdint>
#include <cstddef>

namespace sys {
    using NTSTATUS = long;
    using HANDLE   = void*;
    using PVOID    = void*;
    using SIZE_T   = uintptr_t;
    using ULONG    = unsigned long;

    // Initialize: map ntdll, find the SSN for the named Zw routine.
    bool        init();
    ULONG       ssn(const char* zfname);                 // resolve "NtAllocateVirtualMemory"
    // Generic syscall thunks (caller sets RCX..R9 + stack as needed).
    NTSTATUS    syscall_raw(ULONG num, uintptr_t a1, uintptr_t a2,
                            uintptr_t a3, uintptr_t a4,
                            uintptr_t a5, uintptr_t a6,
                            uintptr_t a7, uintptr_t a8);
}
