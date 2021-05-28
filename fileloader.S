#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   file boot loader
#         IA-32e mode initialization
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
#*******************************************************************************
.equiv Q_int_10_4f00_S_magic_call,          0x32454256
.equiv Q_int_10_4f00_S_magic_ret,           0x41534556
.equiv Q_int_15_e820_S_magic,               0x534d4150
.equiv Q_vga_color_S_light_gray,            7
.equiv Q_vga_S_default_mode,                0x11a
.equiv Q_vga_S_bpp,                         32
.equiv Q_vga_S_height,                      1050
.equiv Q_vga_S_width,                       1680
.equiv Z_map_to_address,                    0x80000
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
.org 0x7e00
.text
.code16
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
    push    %di
    int     $0x15
    pop     %di
    jc      0f
    cmp     $Q_int_15_e820_S_magic, %eax
    je      1f
0:  mov     $80000, %eax
    jmp     2f
0:  mov     $24, %ecx
    mov     %eax, %edx
    sub     %cx, %di
    mov     $0xe820, %eax
    push    %di
    int     $0x15
    pop     %di
    jc      0f
1:  cmp     $20, %cl
    jne     1f
    movl    $1, %es:20(%di)
1:  test    %ebx, %ebx
    jz      1f
    jmp     0b
0:  lea     24(%di), %di
1:  xor     %eax, %eax
    mov     %es, %ax
    shl     $4, %eax
    mov     %di, %ax
2:  pushl   $0                              # Zachowanie na później 64-bitowego adresu tablicy.
    push    %eax
    xor     %ax, %ax
    mov     %ax, %es
# Włączenie trybu graficznego ekranu.
    mov     %sp, %bx
    mov     $0x4f00, %ax
    lea     -512(%bx),%sp
    mov     %sp, %di
    movl    $Q_int_10_4f00_S_magic_call, (%di)
    int     $0x10
    cmp     $0x4f, %ax
    jne     Hlt_16
    mov     %sp, %bx
    cmpl    $Q_int_10_4f00_S_magic_ret, (%bx)
    jne     4f
    mov     0xe(%bx), %si
    pushw   $0                              # Tryb.
    pushw   $0x8000                         # Ujemna różnica szerokości, wysokości i ‘bpp’.
    lea     -(2*2+256)(%bx),%sp
0:  mov     (%si), %cx
    cmp     $0xffff, %cx
    je      3f
    mov     %sp, %di
    mov     $0x4f01, %ax
    push    %si
    int     $0x10
    cmp     $0x4f, %al
    jne     Hlt_16
    pop     %si
    test    %ah, %ah
    jnz     3f
    mov     %sp, %bx
    testb   $1 << 7, (%bx)
    jz      1f
    mov     0x12(%bx), %ax
    mov     0x14(%bx), %dx
    mov     0x19(%bx), %cl
    xor     %bp, %bp
    sub     $Q_vga_S_width, %ax
    ja      1f
    jne     2f
    inc     %bp
2:  sub     $Q_vga_S_height, %dx
    ja      1f
    jne     2f
    inc     %bp
2:  sub     $Q_vga_S_bpp, %cl
    ja      1f
    jne     2f
    inc     %bp
2:  xor     %ch, %ch
    add     %dx, %ax
    add     %cx, %ax
    cmp     %ax, 256(%bx)
    jg      1f
    mov     %ax, 256(%bx)
    mov     (%si), %ax
    mov     %ax, 256+2(%bx)
    mov     0x12(%bx), %ax
    mov     0x14(%bx), %dx
    mov     0x19(%bx), %cl
    mov     %ax, E_main_Z_video_S_width
    mov     %dx, E_main_Z_video_S_height
    mov     0x28(%bx), %edx
    mov     %cl, E_main_Z_video_S_bpp
    mov     %edx, E_main_Z_video_S_framebuffer
    mov     0x10(%bx), %ax
    mov     0x1f(%bx), %dx
    mov     0x21(%bx), %cx
    mov     %ax, E_main_Z_video_S_line_width
    mov     %dl, E_main_Z_video_S_red_size
    mov     %dh, E_main_Z_video_S_red_start
    mov     0x23(%bx), %ax
    mov     %cl, E_main_Z_video_S_green_size
    mov     %ch, E_main_Z_video_S_green_start
    mov     %al, E_main_Z_video_S_blue_size
    mov     %ah, E_main_Z_video_S_blue_start
1:  lea     2(%si), %si
    cmp     $3, %bp                         # Czy idealnie dopasowany do żądania?
    jne     0b
3:  mov     %sp, %bp
    testb   $1 << 3, 256+2*2+0xa(%bp)
    jz      0f
    andb    $~( 1 << 3 ), 256+2*2+0xa(%bp)
    mov     256+2*2+0x24(%bp), %si
    jmp     0b
0:  mov     256+2(%bp), %bx                 # Tryb.
    lea     256+2*2+512(%bp),%sp
    test    %bx, %bx
    jnz     0f
4:  mov     %sp, %bx
    mov     $Q_vga_S_default_mode | ( 1 << 14 ), %cx
    lea     -256(%bx),%sp
    mov     $0x4f01, %ax
    mov     %sp, %di
    int     $0x10
    cmp     $0x4f, %ax
    jne     Hlt_16
    mov     %sp, %bx
    testb   $1 << 7, (%bx)
    jz      Hlt_16
    mov     0x12(%bx), %ax
    mov     0x14(%bx), %dx
    mov     0x19(%bx), %cl
    mov     %ax, E_main_Z_video_S_width
    mov     %dx, E_main_Z_video_S_height
    mov     %cl, E_main_Z_video_S_bpp
    mov     0x28(%bx), %ecx
    mov     0x10(%bx), %ax
    mov     %ecx, E_main_Z_video_S_framebuffer
    mov     0x1f(%bx), %dx
    mov     0x21(%bx), %cx
    mov     %ax, E_main_Z_video_S_line_width
    mov     %dl, E_main_Z_video_S_red_size
    mov     %dh, E_main_Z_video_S_red_start
    mov     0x23(%bx), %ax
    mov     %cl, E_main_Z_video_S_green_size
    mov     %ch, E_main_Z_video_S_green_start
    mov     %al, E_main_Z_video_S_blue_size
    mov     %ah, E_main_Z_video_S_blue_start
    lea     256(%bx),%sp
    mov     $Q_vga_S_default_mode, %bx
0:  mov     $0x4f02, %ax
    or      $1 << 14, %bx                   # Włączenie ‘linear video buffer’.
    int     $0x10
    cmp     $0x4f, %ax
    jne     Hlt_16
# Inicjowanie ‘page tables’ trybu chronionego.
    mov     $S_pt_32, %bx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $Z_map_to_address, %eax
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
.code32
0:  orl     $S_ldt_32, S_gdt_32 + 3 * 8 + 2 # Adres “S_ldt_32” mniejszy niż 1000000.
    mov     $3 << 3, %ax
    lldt    %ax
    lidt    S_id
# Inicjowanie ‘page tables’ trybu IA-32e.
    mov     $S_pt, %ebx
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw, %eax
0:  cmp     $Z_map_to_address, %eax
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
    or      $Z_cr4_S_pae | Z_cr4_S_osfxsr | Z_cr4_S_osxmmexcpt /* | Z_cr4_S_osxsave */, %eax
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
    mov     $E_main_S_video, %rsi
    pop     %rdi                            # Przekazanie adresu tablicy obszarów pamięci RAM.
    jmp     main
#===============================================================================
.code16
Q_vga_I_print_hex_word:
    push    %ax
    mov     %sp, %bp
    mov     $16, %cl
0:  sub     $4, %cl
    mov     (%bp), %dx
    shr     %cl, %dx
    and     $0xf, %dl
    cmp     $9, %dl
    ja      2f
    mov     $'0', %al
    jmp     1f
2:  mov     $'a', %al
    sub     $10, %dl
1:  add     %dl, %al
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    push    %cx
    int     $0x10
    pop     %cx
    test    %cl, %cl
    jnz     0b
    mov     $'\r', %al
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    int     $0x10
    mov     $'\n', %al
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    int     $0x10
    pop     %ax
    ret
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
Hlt_16:
    cli
0:  hlt
    jmp     0b
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ OS boot loader. File Boot Loader.\r\n"
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.p2align 2
E_main_S_video:
E_main_Z_video_S_framebuffer:
    .4byte  0
E_main_Z_video_S_line_width:
    .2byte  0
E_main_Z_video_S_width:
    .2byte  0
E_main_Z_video_S_height:
    .2byte  0
E_main_Z_video_S_bpp:
    .byte  0
E_main_Z_video_S_red_start:
    .byte  0
E_main_Z_video_S_red_size:
    .byte  0
E_main_Z_video_S_green_start:
    .byte  0
E_main_Z_video_S_green_size:
    .byte  0
E_main_Z_video_S_blue_start:
    .byte  0
E_main_Z_video_S_blue_size:
    .byte  0
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.p2align 1
S_gd:
    .2byte  4 * 8 - 1
    .4byte  S_gdt_32
    .4byte  0
S_id:
    .2byte  8 - 1
    .4byte  S_idt_32
    .4byte  0
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
