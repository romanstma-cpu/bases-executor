; unpacker_stub.asm - custom UPX-style unpacker. The packed .exe's real entry
; is reached by decompressing the .text section in-place, then jmp real_entry.
; Also zeroes the debug directory and strips the Rich header at load.
.code
packer_entry proc
    ; 1) decrypt/inflation of the payload section (LZ/XXTEA stub lives here)
    ; 2) apply relocations for the restored image
    ; 3) jmp real_entry
    jmp real_entry
packer_entry endp
end
