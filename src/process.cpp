// process.cpp - locate Roblox client.
#include "process.h"
#include "obf.h"
#include <windows.h>
#include <tlhelp32.h>
#include <cstring>

namespace proc {
    static bool parent_is_explorer(DWORD pid) {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe{ sizeof(pe) };
        DWORD ppid = 0;
        if (Process32First(snap, &pe)) {
            do {
                if (pe.th32ProcessID == pid) { ppid = pe.th32ParentProcessID; break; }
            } while (Process32Next(snap, &pe));
        }
        CloseHandle(snap);
        if (!ppid) return false;
        snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (Process32First(snap, &pe)) {
            do {
                if (pe.th32ProcessID == ppid) {
                    CloseHandle(snap);
                    return _stricmp(pe.szExeFile, "explorer.exe") == 0;
                }
            } while (Process32Next(snap, &pe));
        }
        CloseHandle(snap);
        return false;
    }

    std::uintptr_t find_verified(const char* class_name) {
        HWND w = FindWindowA(class_name, nullptr);
        if (!w) return 0;
        DWORD pid = 0;
        GetWindowThreadProcessId(w, &pid);
        if (!pid) return 0;
        HANDLE m = OpenMutexA(SYNCHRONIZE, FALSE, "RobloxLockedClient");
        if (!m) return 0;
        CloseHandle(m);
        if (!parent_is_explorer(pid)) return 0;
        return static_cast<std::uintptr_t>(pid);
    }
}