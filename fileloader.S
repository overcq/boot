#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   file boot loader
#         IA-32e mode initialization
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
#*******************************************************************************
.equiv Q_vga_color_S_light_gray,    7
#-------------------------------------------------------------------------------
.equiv Z_cr0_S_pe,                          ( 1 << 0 )
.equiv Z_cr0_S_pg,                          ( 1 << 31 )
#-------------------------------------------------------------------------------
.equiv Z_msr_S_ia32_efer,                   0xc0000080
.equiv Z_msr_Z_ia32_efer_S_lme,             ( 1 << 8 )
#-------------------------------------------------------------------------------
.equiv Z_cr4_S_pae,                         ( 1 << 5 )
#-------------------------------------------------------------------------------
.equiv Z_descriptor_Z_type_data_code_S_a,   ( 1 << 8 )
.equiv Z_descriptor_Z_type_data_S_w,        ( 1 << 9 )
.equiv Z_descriptor_Z_type_data_code_S_code,( 1 << 11 )
.equiv Z_descriptor_Z_type_system_S_ldt,    ( 1 << 9 )
.equiv Z_descriptor_S_s,                    ( 1 << 12 )
.equiv Z_descriptor_S_p,                    ( 1 << 15 )
.equiv Z_descriptor_S_l,                    ( 1 << 21 )
.equiv Z_descriptor_S_db,                   ( 1 << 22 )
.equiv Z_descriptor_S_g,                    ( 1 << 23 )
#-------------------------------------------------------------------------------
.equiv Z_page_entry_S_p,                    ( 1 << 0 )
.equiv Z_page_entry_S_rw,                   ( 1 << 1 )
#===============================================================================
.code16
.text
    mov     $Q_print_S_logo, %si
    call    Q_vga_I_print
# Inicjowanie ‘page tables’ trybu chronionego.
    mov     $S_pt_32, %ebx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $0x80000, %eax
    ja      0f
    mov     %eax, (%ebx)
    lea     0x1000(%eax), %eax
    lea     4(%ebx), %ebx
    jmp     0b
0:  mov     $S_pt_32, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pdt_32
    mov     $S_pdt_32, %eax
    mov     %eax, %cr3
    cli
# TODO Blokować zewnętrzne przerwania (APIC).
    mov     $S_gd, %eax
    lgdtl   S_gd
# Przełączenie do trybu chronionego.
    mov     %cr0, %eax
    or      $Z_cr0_S_pe | Z_cr0_S_pg, %eax
    mov     %eax, %cr0
    mov     $2 << 3, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %ss
    xor     %ax, %ax
    mov     %ax, %fs
    mov     %ax, %gs
    ljmpl   $1 << 3, $0f
.code32
0:  #mov     $3 << 3, %ax
    #lldt    %ax
    lidt    S_id
# Inicjowanie ‘page tables’ trybu IA-32e.
    mov     $S_pt, %ebx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $0x80000, %eax
    ja      0f
    mov     %eax, (%ebx)
    lea     0x1000(%eax), %eax
    lea     8(%ebx), %ebx
    jmp     0b
0:  mov     $S_pt, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pd
    mov     $S_pd, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pdpt
    mov     $S_pdpt, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pml4
# Inicjowanie tymczasowych deskryptorów ‘gdt’ i ‘idt’ trybu IA-32e.
    movl    $S_gdt, S_gd + 2
    movl    $S_idt, S_id + 2
# Przełączenie do trybu IA-32e.
    mov     %cr0, %eax
    and     $~Z_cr0_S_pg, %eax
    mov     %eax, %cr0
    mov     %cr4, %eax
    or      $Z_cr4_S_pae, %eax
    mov     %eax, %cr4
    mov     $S_pml4, %eax
    mov     %eax, %cr3
    mov     $Z_msr_S_ia32_efer, %ecx
    rdmsr
    or      $Z_msr_Z_ia32_efer_S_lme, %eax
    wrmsr
    mov     %cr0, %eax
    or      $Z_cr0_S_pg, %eax
    mov     %eax, %cr0 # Ta instrukcja powoduje #GP.

0:  hlt
    jmp 0b

    lgdt    S_gd
.code64
    #mov     $3 << 3, %ax
    #lldt    %ax
    lidt    S_id
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
.data
Q_print_S_logo:
    .asciz  "OUX/C+ boot loader. File Boot Loader.\r\n"
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.p2align 1
S_gd:
    .2byte  4 * 8 - 1
    .4byte  S_gdt_32
S_id:
    .2byte  8 - 1
    .4byte  S_idt_32

#-------------------------------------------------------------------------------
.macro descriptor base=0, limit=~0, flags
    .2byte  \limit & ~( ~0 << 16 )
    .2byte  \base & ~( ~0 << 16 )
    .4byte  (( \base >> 16 ) & 0xff ) | ( \limit & ( 0xf << 16 )) | ( \base & ( 0xff << 24 )) | Z_descriptor_S_p | Z_descriptor_S_g | \flags
.endm
.p2align 3
S_gdt_32:
    .8byte  0
    descriptor , , Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_s | Z_descriptor_S_db
    descriptor , , Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s | Z_descriptor_S_db
    descriptor , , Z_descriptor_Z_type_system_S_ldt
S_ldt_32:
    .8byte  0
S_idt_32:
    .8byte  0
S_gdt:
    .8byte  0
    descriptor , , Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_s | Z_descriptor_S_l
    descriptor , , Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s
    descriptor , , Z_descriptor_Z_type_system_S_ldt
S_ldt:
    .8byte  0
S_idt:
    .8byte  0
#-------------------------------------------------------------------------------
.macro page_entry_32
    .4byte  0
.endm
.macro pte_flat_memory_32 from, to
    .p2align 0
    page_entry_32
    .if \to - \from
        pte_flat_memory_32 "(\from + 0x1000 )", \to
    .endif
.endm
.p2align 12
S_pt_32:
    page_entry_32
    pte_flat_memory_32 0x1000, 0x60000
    pte_flat_memory_32 0x60000, 0x80000
.p2align 12
S_pdt_32:
    page_entry_32
S_pdpt_32:
    page_entry_32
#-------------------------------------------------------------------------------
.macro page_entry
    .8byte  0
.endm
.macro pte_flat_memory from, to
    .p2align 0
    page_entry
    .if \to - \from
        pte_flat_memory "(\from + 0x1000 )", \to
    .endif
.endm
.p2align 12
S_pt:
    page_entry
    pte_flat_memory 0x1000, 0x60000
    pte_flat_memory 0x60000, 0x80000
.p2align 12
S_pd:
    page_entry
S_pdpt:
    page_entry
S_pml4:
    page_entry
#===============================================================================
.text
.code64
I_start:
#*******************************************************************************
