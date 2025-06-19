#*******************************************************************************
#   ___   public
#  ¦OUX¦  x86_64 assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   kernel
#         multiple processors initialization
# ©overcq                on ‟Gentoo Linux 23.0” “x86_64”             2025‒6‒17 K
#*******************************************************************************
.global E_mp_init_I
.global E_mp_init_I_end
.global E_mp_init_I_reloc_1
.global E_mp_init_I_reloc_2
.global E_mp_init_I_reloc_3
.global E_mp_init_I_reloc_4
.global E_mp_init_I_reloc_5
.global E_mp_init_I_reloc_6
.global E_mp_init_S_gdt_32
.global E_mp_init_S_gdt
.global E_mp_init_S_gd_32
.global E_mp_init_S_gd
#===============================================================================
.equiv Z_cr0_S_pe,                          ( 1 << 0 )
.equiv Z_cr0_S_pg,                          ( 1 << 31 )
.equiv Z_cr4_S_pae,                         ( 1 << 5 )
#-------------------------------------------------------------------------------
.equiv Z_msr_S_ia32_efer,                   0xc0000080
.equiv Z_msr_Z_ia32_efer_S_lme,             ( 1 << 8 )
#-------------------------------------------------------------------------------
.equiv Z_descriptor_Z_type_data_S_w,        ( 1 << 9 )
.equiv Z_descriptor_Z_type_data_code_S_code,( 1 << 11 )
.equiv Z_descriptor_S_s,                    ( 1 << 12 )
.equiv Z_descriptor_S_p,                    ( 1 << 15 )
.equiv Z_descriptor_S_l,                    ( 1 << 21 )
.equiv Z_descriptor_S_db,                   ( 1 << 22 )
.equiv Z_descriptor_S_g,                    ( 1 << 23 )
#===============================================================================
.macro J_descriptor base=0, limit=~0, flags
    .2byte  \limit & ~( ~0 << 16 )
    .2byte  \base & ~( ~0 << 16 )
    .4byte  (( \base >> 16 ) & 0xff ) | ( \limit & ( 0xf << 16 )) | ( \base & ( 0xff << 24 )) | Z_descriptor_S_p | Z_descriptor_S_g | \flags
.endm
.text
.code16
E_mp_init_I:
    cli
E_mp_init_I_reloc_1:
    mov     $0,%ax                      # $0 do zastąpienia selektorem segmentu “E_mp_init_I”.
    mov     %ax,%ds
    lgdtl   E_mp_init_S_gd_32-E_mp_init_I
    mov     %cr0,%eax
    or      $Z_cr0_S_pe,%eax
    mov     %eax,%cr0
E_mp_init_I_reloc_2:
    ljmpl   $1 << 3,$0                  # $0 do zastąpienia adresem 0f.
.code32
0:  mov     $2 << 3,%ax
    mov     %ax,%ds
    mov     %ax,%es
    mov     %ax,%ss
    mov     %cr4,%eax
    or      $Z_cr4_S_pae,%eax
    mov     %eax,%cr4
E_mp_init_I_reloc_3:
    mov     $0x12345678,%eax            # Do zastąpienia wartością “pml4”.
    mov     %eax,%cr3
    mov     $Z_msr_S_ia32_efer,%ecx
    rdmsr
    or      $Z_msr_Z_ia32_efer_S_lme,%eax
    wrmsr
    mov     %cr0,%eax
    or      $Z_cr0_S_pg,%eax
    mov     %eax,%cr0
E_mp_init_I_reloc_4:
    lgdt    0                           # 0 do zastępienia adresem “E_mp_init_S_gd”.
E_mp_init_I_reloc_5:
    ljmp    $1 << 3,$0                  # $0 do zastąpienia adresem 0f.
.code64
0:  fninit
    mov     $1,%eax
    cpuid                               #TODO Zmienić na odczyt z APIC.
    shr     $24,%ebx
    dec     %ebx
E_mp_init_I_reloc_6:
    mov     $0x1234567812345678,%rbp    # Do zastąpienia wartością “E_main_S_kernel_args.processor_proc”.
    incq    (%rbp,%rbx,8)
0:  pause
    mov     (%rbp,%rbx,8),%rax
    test    %rax,%rax
    jz      0b
    jmp     *%rax
.p2align 3
E_mp_init_S_gdt_32:
    .8byte  0
    J_descriptor , , Z_descriptor_Z_type_data_code_S_code | Z_descriptor_S_s | Z_descriptor_S_db
    J_descriptor , , Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s | Z_descriptor_S_db
E_mp_init_S_gdt:
    .8byte  0
    J_descriptor , , Z_descriptor_Z_type_data_code_S_code | Z_descriptor_S_s | Z_descriptor_S_l
    J_descriptor , , Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s
.p2align 1
E_mp_init_S_gd_32:
    .2byte  3 * 8 - 1
    .8byte  0                           # Do zastąpienia adresem “E_mp_init_S_gdt_32”.
E_mp_init_S_gd:
    .2byte  3 * 8 - 1
    .8byte  0                           # Do zastąpienia adresem “E_mp_init_S_gdt”.
E_mp_init_I_end:
#*******************************************************************************
