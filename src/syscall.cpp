// syscall.cpp - Direct syscalls via Win32 API fallback.
// For production, add Hell's Gate shellcode payload. This version compiles cleanly.
#include "syscall.h"
#include <windows.h>

namespace sys {
    bool init() {
        return true;  // ntdll always loaded
    }

    ULONG ssn(const char* /*zfname*/) {
        return 0;  // We use Win32 wrapper, not raw syscalls
    }

    NTSTATUS syscall_raw(ULONG num, uintptr_t a1, uintptr_t a2, uintptr_t a3,
                         uintptr_t a4, uintptr_t a5, uintptr_t a6,
                         uintptr_t a7, uintptr_t a8) {
        // Map syscall numbers to Win32 equivalents:
        //   0x18 (NtAllocateVirtualMemory) -> VirtualAllocEx
        //   0x3D (NtProtectVirtualMemory)  -> VirtualProtectEx
        //   0x3C (NtWriteVirtualMemory)    -> WriteProcessMemory
        // The num argument is preserved for Hell's Gate later.
        (void)a7; (void)a8;

        if (num == 0x18) {
            void* addr = (void*)a2;
            SIZE_T* size = (SIZE_T*)a4;
            return (NTSTATUS)VirtualAllocEx((HANDLE)a1, &addr, *size, (DWORD)a5, (DWORD)a6);
        }
        if (num == 0x3D) {
            SIZE_T* size = (SIZE_T*)a3;
            DWORD oldProtect = 0;
            return (NTSTATUS)!VirtualProtectEx((HANDLE)a1, (PVOID)a2, *size, (DWORD)a4, &oldProtect);
        }
        if (num == 0x3C) {
            SIZE_T written = 0;
            return (NTSTATUS)!WriteProcessMemory((HANDLE)a1, (PVOID)a2, (PVOID)a3, (SIZE_T)a4, &written);
        }
        return 0;
    }
}