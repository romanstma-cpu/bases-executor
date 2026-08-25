; syscall_asm.asm - the actual syscall trampoline (MASM, x64).
; RCX=num, RDX=a1, R8=a2, R9=a3, [rsp+0x20]=a4, +0x28=a5... ; we pass 8 args.
.code
syscall_asm proc
    ; args: RCX num, RDX a1, R8 a2, R9 a3, [rsp+20h]=a4..a8
    mov r10, rcx            ; mov r10, rcx  (syscall convention)
    mov eax, ecx            ; eax = syscall number
    ; a4..a8 already on stack at [rsp+20h..]; fastcall has them placed by caller
    syscall
    ret
syscall_asm endp
end
