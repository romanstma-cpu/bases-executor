// process.cpp - locate + verify Roblox client.
#include "process.h"
#include "obf.h"
#include <windows.h>
#include <tlhelp32.h>

namespace proc {
    static bool parent_is_explorer(DWORD pid) {
        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe{sizeof(pe)};
        DWORD ppid = 0;
        for (Process32First(snap, &pe); Process32Next(snap, &pe);)
            if (pe.th32ProcessID == pid) { ppid = pe.th32ParentProcessID; break; }
        CloseHandle(snap);
        if (!ppid) return false;
        HANDLE snap2 = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        for (Process32First(snap2, &pe); Process32Next(snap2, &pe);)
            if (pe.th32ProcessID == ppid) {
                CloseHandle(snap2);
                return _stricmp(pe.szExeFile, "explorer.exe") == 0;
            }
        CloseHandle(snap2);
        return false;
    }

    uintptr_t find_verified(const char* class_name) {
        auto cn = obf::OBF("RobloxPlayerBeta");
        HWND w = FindWindowA(cn.dec().c_str(), nullptr);
        if (!w) return 0;
        DWORD pid = 0; GetWindowThreadProcessId(w, &pid);
        if (!pid) return 0;
        // Client-set mutex check: Roblox opens a named mutex early.
        auto mtx = obf::OBF("RobloxLockedClient");
        HANDLE m = OpenMutexA(SYNCHRONIZE, FALSE, mtx.dec().c_str());
        if (!m) return 0;  // no mutex => not a real client
        CloseHandle(m);
        if (!parent_is_explorer(pid)) return 0;  // injected into a weird parent
        return static_cast<uintptr_t>(pid);
    }
}
