// syscall.h - direct syscalls.
#pragma once
#include <cstdint>
#include <cstddef>

namespace sys {
    using NTSTATUS = long;
    bool init();
    std::uint32_t ssn(const char* zfname);
    NTSTATUS syscall_raw(std::uint32_t num, std::uintptr_t a1, std::uintptr_t a2,
                         std::uintptr_t a3, std::uintptr_t a4, std::uintptr_t a5,
                         std::uintptr_t a6, std::uintptr_t a7, std::uintptr_t a8);
}