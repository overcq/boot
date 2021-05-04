#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   file boot loader
#         IA-32e mode initialization
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
#*******************************************************************************
.equiv Q_int_15_e820_S_magic,               0x534d4150
.equiv Q_vga_color_S_light_gray,            7
.equiv Q_vesa_S_mode,                       0x11a
#-------------------------------------------------------------------------------
.equiv Z_cr0_S_pe,                          ( 1 << 0 )
.equiv Z_cr0_S_pg,                          ( 1 << 31 )
#-------------------------------------------------------------------------------
.equiv Z_msr_S_ia32_efer,                   0xc0000080
.equiv Z_msr_Z_ia32_efer_S_lme,             ( 1 << 8 )
#-------------------------------------------------------------------------------
.equiv Z_cr4_S_pae,                         ( 1 << 5 )
.equiv Z_cr4_S_osfxsr,                      ( 1 << 9 )
.equiv Z_cr4_S_osxmmexcpt,                  ( 1 << 10 )
.equiv Z_cr4_S_osxsave,                     ( 1 << 18 )
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
# Odczytanie obszarów pamięci RAM, jeśli dostępne.
    mov     $7 << 12, %ax
    mov     $Q_int_15_e820_S_magic, %edx
    mov     %ax, %es
    mov     $0 - 24, %di                    # Wrap around.
    mov     $24, %ecx
    xor     %ebx, %ebx
    mov     $0xe820, %eax
    int     $0x15
    jc      0f
    cmp     $Q_int_15_e820_S_magic, %eax
    je      1f
0:  mov     $80000, %eax
    jmp     2f
1:  cmp     $20, %cl
    jne     0f
    movl    $1, 20(%di)
0:  mov     $24, %ecx
    sub     %cx, %di
    mov     $0xe820, %eax
    int     $0x15
    mov     $0, %ax
    adc     $0, %ax
    test    %ebx, %ebx
    jz      0f
    test    %ax, %ax
    jz      0b
    lea     24(%di), %di
0:  xor     %eax, %eax
    mov     %es, %ax
    shl     $4, %eax
    mov     %di, %ax
2:  pushl   $0                              # Zachowanie na później 64-bitowego adresu tablicy.
    push    %eax
# Włączenie trybu graficznego ekranu.
    xor     %ax, %ax
    mov     %ax, %es
    mov     $0x4f01, %ax
    mov     %sp, %bx
    lea     -256(%bx),%sp
    mov     $Q_vesa_S_mode, %cx
    mov     %sp, %di
    int     $0x10
    mov     %sp, %bx
    lea     256(%bx),%sp
    cmp     $0x4f, %ax
    jne     Hlt_16
    pushl   $0
    pushl   0x28(%bx)
    mov     $0x4f02, %ax
    mov     $Q_vesa_S_mode | ( 1 << 14 ), %bx
    int     $0x10
    cmp     $0x4f, %ax
    jne     Hlt_16
# Inicjowanie ‘page tables’ trybu chronionego.
    mov     $S_pt_32, %bx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $0x80000, %eax
    ja      0f
    mov     %eax, (%bx)
    lea     0x1000(%eax), %eax
    lea     4(%bx), %bx
    jmp     0b
0:  mov     $S_pt_32, %eax
    or      $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
    mov     %eax, S_pdt_32
    mov     $S_pdt_32, %eax
    mov     %eax, %cr3
    cli                                     #TODO Blokować zewnętrzne przerwania (APIC).
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
    movzwl  %sp, %esp
    ljmpl   $1 << 3, $0f
Hlt_16:
    hlt
    jmp     Hlt_16
.code32
0:  orl     $S_ldt_32, S_gdt_32 + 3 * 8 + 2 # Adres “S_ldt_32” mniejszy niż 1000000.
    mov     $3 << 3, %ax
    lldt    %ax
    lidt    S_id
# Inicjowanie ‘page tables’ trybu IA-32e.
    mov     $S_pt, %ebx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $0x80000, %eax
    ja      0f
    mov     %eax, (%ebx)
    movl    $0, 4(%ebx)
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
    or      $Z_cr4_S_pae | Z_cr4_S_osfxsr | Z_cr4_S_osxmmexcpt | Z_cr4_S_osxsave, %eax
    mov     %eax, %cr4
    mov     $S_pml4, %eax
    mov     %eax, %cr3
    mov     $Z_msr_S_ia32_efer, %ecx
    rdmsr
    or      $Z_msr_Z_ia32_efer_S_lme, %eax
    wrmsr
    mov     %cr0, %eax
    or      $Z_cr0_S_pg, %eax
    mov     %eax, %cr0
    lgdt    S_gd
    orl     $S_ldt, S_gdt + 3 * 8 + 2       # Adres “S_ldt” mniejszy niż 1000000.
    mov     $3 << 3, %ax
    lldt    %ax
    lidt    S_id
    mov     $2 << 3, %ax
    mov     %ax, %ds
    mov     %ax, %es
    mov     %ax, %ss
    xor     %ax, %ax
    mov     %ax, %fs
    mov     %ax, %gs
    ljmp    $1 << 3, $0f
.code64
0:  mov     %esp, %esp
    pop     %rsi                            # Przekazanie adresu bufora pamięci video.
    pop     %rdi                            # Przekazanie adresu tablicy obszarów pamięci RAM.
    jmp     main
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
    .asciz  "OUX/C+ OS boot loader. File Boot Loader.\r\n"
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
    .8byte  0
#-------------------------------------------------------------------------------
.macro page_entry_32
    .4byte  0
.endm
.p2align 12
S_pt_32:
    page_entry_32
.p2align 12
S_pdt_32:
    page_entry_32
.p2align 12
S_pdpt_32:
    page_entry_32
#-------------------------------------------------------------------------------
.macro page_entry
    .8byte  0
.endm
.p2align 12
S_pt:
    page_entry
.p2align 12
S_pd:
    page_entry
.p2align 12
S_pdpt:
    page_entry
.p2align 12
S_pml4:
    page_entry
#*******************************************************************************
