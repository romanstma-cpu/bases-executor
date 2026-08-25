// peb.cpp - PEB self-sanitization.
#include "peb.h"
#include <windows.h>
#include <winternl.h>

namespace peb {
    void sanitize() {
        PEB* p = reinterpret_cast<PEB*>(__readgsqword(0x60));
        // 1) Unlink from loader linked list (InLoadOrderModuleList).
        LIST_ENTRY* head = &p->Ldr->InLoadOrderModuleList;
        for (LIST_ENTRY* e = head->Flink; e != head; e = e->Flink) {
            LDR_DATA_TABLE_ENTRY* l =
                CONTAINING_RECORD(e, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            if (l->DllBase == GetModuleHandleA(nullptr)) {
                e->Blink->Flink = e->Flink;
                e->Flink->Blink = e->Blink;
                break;
            }
        }
        // 2) Zero the process command line.
        if (p->ProcessParameters) {
            p->ProcessParameters->CommandLine.Length = 0;
            p->ProcessParameters->CommandLine.Buffer = nullptr;
            // 3) Clear current working directory string.
            p->ProcessParameters->CurrentDirectory.DosPath.Length = 0;
        }
    }
}
