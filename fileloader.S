#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   file boot loader
#         protected mode initialization
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
#*******************************************************************************
.equiv Q_vga_color_S_light_gray,    7
#-------------------------------------------------------------------------------
.equiv Z_cr0_S_pe,                          ( 1 << 0 )
.equiv Z_cr0_S_pg,                          ( 1 << 31 )
#-------------------------------------------------------------------------------
.equiv Z_msr_S_ia32_efer,                   0xc0000080
.equiv Z_msr_S_ia32_efer_S_lme,             ( 1 << 8 )
#-------------------------------------------------------------------------------
.equiv Z_cr4_S_pae,                         ( 1 << 5 )
.equiv Z_cr4_S_la57,                        ( 1 << 12 )
#-------------------------------------------------------------------------------
.equiv Z_descriptor_Z_type_data_code_S_a,   ( 1 << 8 )
.equiv Z_descriptor_Z_type_data_S_w,        ( 1 << 9 )
.equiv Z_descriptor_Z_type_data_code_S_code,( 1 << 11 )
.equiv Z_descriptor_Z_type_system_S_ldt,    ( 1 << 9 )
.equiv Z_descriptor_S_s,                    ( 1 << 12 )
.equiv Z_descriptor_S_p,                    ( 1 << 15 )
.equiv Z_descriptor_S_l,                    ( 1 << 21 )
.equiv Z_descriptor_S_db,                   ( 1 << 22 )
#-------------------------------------------------------------------------------
.equiv Z_page_entry_S_p,                    ( 1 << 0 )
.equiv Z_page_entry_S_rw,                   ( 1 << 1 )
#===============================================================================
.code16
.text
    mov     $Q_print_S_logo, %si
    call    Q_vga_I_print
    mov     $S_pt_32, %ebx
    xor     %eax, %eax
0:  mov     %eax, %edx
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %edx
    mov     %edx, (%ebx,%eax)
    lea     0x1000(%eax), %eax
    cmp     $0x80000, %eax
    jne     0b
    mov     $S_pd_32, %eax
    mov     %eax, %cr3
    cli
# TODO Blokować zewnętrzne przerwania (APIC).
    lgdt    S_gd
    mov     %cr0, %eax
    or      $Z_cr0_S_pe | Z_cr0_S_pg, %eax
    mov     %eax, %cr0
.code32
    ljmp    $1, $0f
0:  mov     $3 << 3, %ax
    lldt    %ax
    xor     %ax, %ax
    mov     %ax, %es
    mov     %ax, %fs
    mov     %ax, %gs
    mov     $2 << 3, %ax
    mov     %ax, %ds
    mov     %ax, %ss
    lidt    S_id
    mov     %cr0, %eax
    and     $~Z_cr0_S_pg, %eax
    mov     %eax, %cr0
    mov     %cr4, %eax
    or      $Z_cr4_S_pae | Z_cr4_S_la57, %eax
    mov     %eax, %cr4
    mov     $S_pt, %ebx
    xor     %eax, %eax
0:  mov     %eax, %edx
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %edx
    mov     %edx, (%ebx,%eax)
    lea     0x1000(%eax), %eax
    cmp     $0x80000, %eax
    jne     0b
    mov     $S_pt, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pd
    mov     $S_pd, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pml4
    mov     $S_pml4, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pml5
    mov     $S_pml5, %eax
    mov     %eax, %cr3
    mov     $Z_msr_S_ia32_efer, %ecx
    rdmsr
    or      $Z_msr_S_ia32_efer_S_lme, %eax
    wrmsr
    movw    $4 * 16 - 1, S_gd
    movl    $S_gdt, S_gd + 2
    lgdt    S_gd
    movl    $S_idt, S_id + 2
    lidt    S_id
    mov     %cr0, %eax
    or      $Z_cr0_S_pg, %eax
    mov     %eax, %cr0
.code64
    sti
    jmp     I_start
    cli
0:  hlt
    jmp 0b
#===============================================================================
.code16
Q_vga_I_print:
0:  cld
    lodsb
    test    %al, %al
    jz      0f
    push    %si
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    int     $0x10
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ boot loader. File Boot Loader.\r\n"
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.p2align 1
S_gd:
    .2byte  4 * 8 - 1
    .4byte  S_gdt_32
S_ld:
    .2byte  8 - 1
    .4byte  S_ldt_32
S_id:
    .2byte  8 - 1
    .4byte  S_idt_32

#-------------------------------------------------------------------------------
.macro descriptor base=0, limit=~0, flags
    .2byte  \base & ~( ~0 << 16 )
    .2byte  \limit & ~( ~0 << 16 )
    .4byte  (( \base >> 16 ) & 0xff ) | ( \limit & ( 0xf << 16 )) | ( \base & ( 0xff << 24 )) | Z_descriptor_S_p | \flags
.endm
.p2align 3
S_gdt_32:
    .8byte  0
    descriptor , , Z_descriptor_S_l | Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_db
    descriptor , , Z_descriptor_S_l | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_db
    descriptor , , Z_descriptor_S_s | Z_descriptor_Z_type_system_S_ldt
S_ldt_32:
    .8byte  0
S_idt_32:
    .8byte  0
#-------------------------------------------------------------------------------
.p2align 4
S_gdt:
    .8byte  0
    descriptor , , Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_db
    descriptor , , Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_db
    descriptor , , Z_descriptor_S_s | Z_descriptor_Z_type_system_S_ldt
S_ldt:
    .8byte  0
S_idt:
    .8byte  0
#-------------------------------------------------------------------------------
.p2align 12
.macro page_entry_32 flags
    .4byte  Z_page_entry_S_p | \flags
.endm
.macro pte_flat_memory_32 from, to
    page_entry_32 Z_page_entry_S_rw
    .if \to - \from
        pte_flat_memory_32 "(\from + 0x1000 )", \to
    .endif
.endm
S_pt_32:
    pte_flat_memory_32 0, 0x60000
    pte_flat_memory_32 0x60000, 0x80000
S_pd_32:
    page_entry_32 Z_page_entry_S_rw
#-------------------------------------------------------------------------------
.macro page_entry flags
    .8byte  Z_page_entry_S_p | \flags
.endm
.macro pte_flat_memory from, to
    page_entry Z_page_entry_S_rw
    .if \to - \from
        pte_flat_memory "(\from + 0x1000 )", \to
    .endif
.endm
S_pt:
    pte_flat_memory 0, 0x60000
    pte_flat_memory 0x60000, 0x80000
S_pd:
    page_entry Z_page_entry_S_rw
S_pml4:
    page_entry Z_page_entry_S_rw
S_pml5:
    page_entry Z_page_entry_S_rw
#===============================================================================
.code64
I_start:
#*******************************************************************************
