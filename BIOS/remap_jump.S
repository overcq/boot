#*******************************************************************************
#   ___   public
#  ¦OUX¦  x86_64 assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS boot loader
#         jump procedure located in identity mapped code
# (c)overcq              on WSL\Debian (Linux 6.18.33.2)             2026-8-28 Z
#*******************************************************************************
.global E_remap_jump_I
.global E_remap_jump_I_end
#===============================================================================
.code64
E_remap_jump_I:
    pop     %rax
    mov     %rdi,%cr3
    lea     (%rax,%rsi),%rax
    lea     (%rsp,%rsi),%rsp
    jmp     *%rax
E_remap_jump_I_end:
#*******************************************************************************
