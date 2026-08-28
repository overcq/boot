#*******************************************************************************
#   ___   public
#  ¦OUX¦  x86_64 assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   UEFI boot loader
#         jump procedure located in identity mapped code
# (c)overcq              on WSL\Debian (Linux 6.18.33.2)             2026-8-28 Z
#*******************************************************************************
.global E_remap_jump_I
.global E_remap_jump_I_end
#===============================================================================
.code64
E_remap_jump_I:
    pop     %rax
    mov     %rcx,%rsp
    mov     %rdx,%cr3
    lea     (%rax,%r8),%rax
    jmp     *%rax
E_remap_jump_I_end:
#*******************************************************************************
