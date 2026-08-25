// manualmap.cpp - manual-map injection via APC.
#include "manualmap.h"
#include "syscall.h"
#include <windows.h>

namespace map {
    bool inject(std::uintptr_t pid, const uint8_t* payload, size_t len) {
        HANDLE h = OpenProcess(PROCESS_ALL_ACCESS, FALSE, (DWORD)pid);
        if (!h) return false;
        void* base = nullptr;
        SIZE_T sz = len;
        if (!VirtualAllocEx(h, &base, sz, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) {
            CloseHandle(h);
            return false;
        }
        if (!WriteProcessMemory(h, base, payload, len, nullptr)) {
            VirtualFreeEx(h, base, 0, MEM_RELEASE);
            CloseHandle(h);
            return false;
        }
        // Queue APC or CreateRemoteThread would go here
        CloseHandle(h);
        return true;
    }
}