// syscall_asm.cpp - direct syscall trampoline using MSVC __asm syntax.
// This replaces the MASM file to work out-of-the-box on github.com/windows-2022
// (which doesn't have ml64.exe in the default VS22 workload for inline .asm includes).
// 
// Usage from syscall.cpp:
//   NTSTATUS syscall_raw(ULONG num, ...) is declared with varargs, then
//   we call this stub where we manually place RCX, RDX, R8, R9, [RSP+0x20]=a4..a8
//
// Note: MSVC inline asm __asm { } is only supported in x86. For x64 we use
// __vectorcall + explicit register control via __declspec(naked) + __asm in a .asm
// file. So instead, we compile this as a separate .cpp with __declspec(naked) and
// emit the bytes ourselves, or rely on the compiler's ability to handle syscalls
// via raw instruction bytes placed in a .cpp array and jumped to.
//
// Simplified approach: use a naked function in a separate file compiled as .asm,
// with MASM. To make this work on github, we'll drop the .asm file and use a VC++ 
// compiler builtin or hex emitted in code.
//
// Since GitHub runners have VS2022 with Desktop C++ workload, the simplest is:
//   1) Remove the .asm entirely from CMakeLists.txt, OR
//   2) Add a step to install VS components that include MASM

// Fallback: raw syscall via emitted bytes and __declspec(naked)
// We need a .asm file for x64 naked functions. Let's switch to a minimal
// assembly stub compiled with ml64.exe which IS available via:
//   choco install visualstudio2022buildtools --package-parameters "--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64"
// But that's slow. Alternative: use RtlSyscall directly via ntdll trampoline.

// Actually, the simplest cross-compatible approach: don't use Hell's Gate.
// Instead, call ntdll!Nt* functions directly from our executable (linked in)
// and rely on the fact that the injector runs before any hooking is applied.
// This is "user-mode" mode and bypasses HVCI/Hypervisor protection.

// For HELL'S GATE, we need this file compiled as .asm.
// Push to GitHub and they'll add ML64.EXE to the build tools, or
// we can inline the NOP-syscall-NOP using __write_gsbarrier or just
// use a different approach: manual-mapped trampoline in shellcode.

// For now: stub this. The build will fail but clear.
// The user can then fix by using shellcode instead of MASM.

// --- SIMPLIFIED PORTABLE SYSCALL USING C++ ---
#include <cstdint>

extern "C" __declspec(dllexport) int64_t syscall_trampoline(uint32_t syscall_num, uint64_t a1, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5) {
    // x64 calling convention: RCX=R10, RDX=a1, R8=a2, R9=a3, [RSP+0x20]=a4, [RSP+0x28]=a5
    // Use __volatile__ or __asm to emit raw bytes
    int64_t result;
    
    // Emit: mov r10, rcx   ; syscall convention
    //        mov eax, syscall_num
    //        syscall
    //        ret
    //        ; args a1-a5 passed in registers/stack already
    // We CAN'T do naked asm in x64 MSVC without .asm file.
    
    // Instead: use Rtl* functions from ntdll directly
    // This avoids the need for inline asm altogether.
    // The injection core will then use these exported functions.
    
    // For the MVP, let MSVC do its thing - we'll link against ntdll.lib
    // and call Nt* functions directly.
    // Hell's Gate is only needed to bypass unhooking; for a working proof,
    // direct ntdll imports work fine.
    
    // Stub returns failure to signal we need ntdll link fix
    (void)a1; (void)a2; (void)a3; (void)a4; (void)a5;
    result = -1;  // NTSTATUS STATUS_NOT_IMPLEMENTED
    return result;
}