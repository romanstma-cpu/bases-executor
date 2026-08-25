// peb.cpp - PEB self-sanitization
#include "peb.h"
#include <windows.h>

namespace peb {
    void sanitize() {
        PEB* p = reinterpret_cast<PEB*>(__readgsqword(0x60));
        LIST_ENTRY* head = &p->Ldr->InLoadOrderModuleList;
        for (LIST_ENTRY* e = head->Flink; e != head; e = e->Flink) {
            LDR_DATA_TABLE_ENTRY* l = CONTAINING_RECORD(e, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
            if (l->DllBase == GetModuleHandleA(nullptr)) {
                e->Flink->Blink = e->Blink;
                e->Blink->Flink = e->Flink;
                break;
            }
        }
        if (p->ProcessParameters) {
            p->ProcessParameters->CommandLine.Length = 0;
            p->ProcessParameters->CommandLine.Buffer = nullptr;
            p->ProcessParameters->CurrentDirectory.DosPath.Length = 0;
        }
    }
}