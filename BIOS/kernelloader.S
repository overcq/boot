#*******************************************************************************
#   ___   public
#  ¦OUX¦  assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS kernel boot loader
#         IA-32e mode initialization
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
#*******************************************************************************
// Nie można z jakiegoś powodu włączyć stronicowania, bo błąd.
#===============================================================================
.equiv Q_offset_Z_base,                     0x10000
#-------------------------------------------------------------------------------
.equiv Q_int_10_4f00_S_magic_call,          0x32454256
.equiv Q_int_10_4f00_S_magic_ret,           0x41534556
.equiv Q_int_15_e820_S_magic,               0x534d4150
.equiv Q_vga_color_S_light_gray,            7
#-------------------------------------------------------------------------------
.equiv Z_msr_S_ia32_efer,                   0xc0000080
.equiv Z_msr_Z_ia32_efer_S_lme,             ( 1 << 8 )
#-------------------------------------------------------------------------------
.equiv Z_cr0_S_pe,                          ( 1 << 0 )
.equiv Z_cr0_S_mp,                          ( 1 << 1 )
.equiv Z_cr0_S_em,                          ( 1 << 2 )
.equiv Z_cr0_S_ts,                          ( 1 << 3 )
.equiv Z_cr0_S_pg,                          ( 1 << 31 )
.equiv Z_cr4_S_pae,                         ( 1 << 5 )
.equiv Z_cr4_S_osfxsr,                      ( 1 << 9 )
.equiv Z_cr4_S_osxmmexcpt,                  ( 1 << 10 )
.equiv Z_cr4_S_osxsave,                     ( 1 << 18 )
#-------------------------------------------------------------------------------
.equiv Z_descriptor_Z_type_data_code_S_a,   ( 1 << 8 )
.equiv Z_descriptor_Z_type_data_S_w,        ( 1 << 9 )
.equiv Z_descriptor_Z_type_data_code_S_code,( 1 << 11 )
.equiv Z_descriptor_Z_type_system_S_ldt,    ( 1 << 9 )
.equiv Z_descriptor_Z_type_system_S_tss,    ( 9 << 8 )
.equiv Z_descriptor_S_s,                    ( 1 << 12 )
.equiv Z_descriptor_S_p,                    ( 1 << 15 )
.equiv Z_descriptor_S_l,                    ( 1 << 21 )
.equiv Z_descriptor_S_db,                   ( 1 << 22 )
.equiv Z_descriptor_S_g,                    ( 1 << 23 )
#-------------------------------------------------------------------------------
.equiv Z_page_entry_S_p,                    ( 1 << 0 )
.equiv Z_page_entry_S_rw,                   ( 1 << 1 )
#===============================================================================
.global S_gd
.global S_id
.global S_gdt
.global S_ldt
#===============================================================================
.text
.code16
    mov     $Q_print_S_logo - Q_offset_Z_base,%si
    call    Q_vga_I_print
# Odczytanie obszarów pamięci RAM,jeśli dostępne.
0:  mov     %ss,%ax
    mov     $Q_int_15_e820_S_magic,%edx
    mov     %ax,%es
    mov     $0xe000 - 28,%di                 # Pozostawienie na stos 8 KiB.
    mov     $20,%ecx
    xor     %ebx,%ebx
    mov     $0xe820,%eax
    int     $0x15
    jc      I_hlt_16
    cmp     $Q_int_15_e820_S_magic,%eax
    jne     I_hlt_16
0:  mov     $20,%ecx
    mov     %eax,%edx
    sub     $28,%di
    jnc     1f
    mov     %es,%si
    lea     -0x1000(%si),%si
    mov     %si,%es
1:  mov     $0xe820,%eax
    int     $0x15
    jc      3f
    cmp     $20,%ecx
    jne     I_hlt_16
1:  test    %ebx,%ebx
    jnz     0b
    jmp     2f
3:  add     $28,%di
    jnc     2f
    mov     %es,%si
    lea     0x1000(%si),%si
    mov     %si,%es
2:  mov     %es,%ax
    shr     $12,%ax
    pushl   $0
    pushw   %ax
    push    %di                             # Zachowanie na później 64-bitowego adresu tablicy.
    mov     %ss,%ax
    mov     %ax,%es
# Włączenie trybu graficznego ekranu.
    mov     %sp,%bx
    mov     $0x4f00,%ax
    lea     -512(%bx),%di
    mov     %di,%sp
    movl    $Q_int_10_4f00_S_magic_call,%ss:(%di)
    int     $0x10
    cmp     $0x4f,%ax
    jne     I_hlt_16
    mov     %sp,%bp
    pushw   $0                              # Tryb.
    movw    $0,E_main_Z_video_S_width - Q_offset_Z_base
    movw    $0,E_main_Z_video_S_height - Q_offset_Z_base
    cmpl    $Q_int_10_4f00_S_magic_ret,(%bp)
    jne     I_hlt_16
    mov     0xe(%bp),%si
    mov     0xe + 2(%bp),%fs
    lea     -( 256 + 2 )(%bp),%sp
0:  mov     %fs:(%si),%cx
    cmp     $0xffff,%cx
    je      3f
    mov     %sp,%di
    mov     $0x4f01,%ax
    push    %si
    push    %fs
    int     $0x10
    cmp     $0x4f,%ax
    jne     I_hlt_16
    pop     %fs
    pop     %si
    mov     %sp,%bp
    mov     (%bp),%ax
    xor     $( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 3 ) | ( 1 << 4 ) | ( 1 << 7 ),%ax # Włączalny, opisany, kolorowy, graficzny, ma ‘framebuffer’.
    test    $( 1 << 0 ) | ( 1 << 1 ) | ( 1 << 3 ) | ( 1 << 4 ) | ( 1 << 7 ),%ax
    jnz     1f
    mov     0x19(%bp),%cl                   # Bits per pixel.
    cmp     $16,%cl                         # Pomija tryby nie 16 i 32-bitowe.
    jb      1f
    cmpb    $6,0x1b(%bp)                    # Direct color.
    jne     1f
    mov     0x12(%bp),%ax                   # Szerokość.
    mov     0x14(%bp),%dx                   # Wysokość.
    cmp     %ax,E_main_Z_video_S_width - Q_offset_Z_base
    jb      2f
    cmp     %dx,E_main_Z_video_S_height - Q_offset_Z_base
    jnb     1f
2:  mov     %ax,E_main_Z_video_S_width - Q_offset_Z_base
    mov     %dx,E_main_Z_video_S_height - Q_offset_Z_base
    mov     %cl,E_main_Z_video_S_bits_per_pixel - Q_offset_Z_base
    mov     $Q_offset_Z_base >> 4,%ax
    mov     %ax,%ds
    mov     0x28(%bp),%edx
    mov     %edx,E_main_Z_video_S_framebuffer - Q_offset_Z_base
    mov     0x10(%bp),%ax
    mov     %ax,E_main_Z_video_S_line_width - Q_offset_Z_base
    mov     0x1f(%bp),%dx
    mov     0x21(%bp),%cx
    mov     0x23(%bp),%ax
    mov     %dx,E_main_Z_video_S_red_size_start - Q_offset_Z_base
    mov     %cx,E_main_Z_video_S_green_size_start - Q_offset_Z_base
    mov     %ax,E_main_Z_video_S_blue_size_start - Q_offset_Z_base
    mov     %fs:(%si),%ax
    mov     %ax,256(%bp)
    jmp     0b
1:  lea     2(%si),%si
    jmp     0b
3:  mov     %sp,%bp
    testb   $1 << 3,256 + 2 + 0xa(%bp)      # Rozszerzenia VBE/AF v1.0P.
    jz      0f
    andb    $~( 1 << 3 ),256 + 2 + 0xa(%bp)
    mov     256 + 2 + 0x24(%bp),%si         # Lista przyspieszanych trybów.
    mov     256 + 2 + 0x24 + 2(%bp),%fs
    jmp     0b
0:  mov     256(%bp),%bx                    # Tryb.
    lea     256 + 2 + 512(%bp),%sp
    test    %bx,%bx
    jz      I_hlt_16
    mov     $0x4f02,%ax
    or      $1 << 14,%bx                    # Włączenie ‘linear frame buffer’.
    int     $0x10
    cmp     $0x4f,%ax
    jne     I_hlt_16
    mov     $Q_offset_Z_base >> 4,%ax
    mov     %ax,%ds
# Przełączenie do trybu chronionego.
    cli
    lgdtl   S_gd - Q_offset_Z_base
    mov     %cr0,%eax
    or      $Z_cr0_S_pe,%eax
    mov     %eax,%cr0
    ljmpl   $1 << 3,$0f
I_hlt_16:
    cli
9:  hlt
    jmp     9b
.code32
0:  mov     $2 << 3,%ax
    xor     %ebp,%ebp
    mov     %ss,%bp
    shl     $4,%ebp
    mov     %sp,%bp
    mov     %ebp,%esp
    mov     %ax,%ds
    mov     %ax,%es
    mov     %ax,%ss
    xor     %ax,%ax
    mov     %ax,%fs
    mov     %ax,%gs
    mov     $S_gdt_32,%ebx
    mov     $S_ldt_32,%eax
    mov     %eax,%ecx
    mov     %eax,%edx
    and     $0xff000000,%edx
    and     $0xff0000,%ecx
    shr     $16,%ecx
    mov     %ax,3 * 8 + 2(%ebx)
    or      %ecx,%edx
    or      %edx,3 * 8 + 4(%ebx)
    mov     $S_tss_32,%eax
    mov     %eax,%ecx
    mov     %eax,%edx
    and     $0xff000000,%edx
    and     $0xff0000,%ecx
    shr     $16,%ecx
    mov     %ax,4 * 8 + 2(%ebx)
    or      %ecx,%edx
    or      %edx,4 * 8 + 4(%ebx)
    mov     $3 << 3,%ax
    lldt    %ax
    mov     $S_tss_32,%ebx
    movl    $0x80000,4(%ebx)
    mov     %ss,%ax
    mov     %ax,8(%ebx)
    mov     $4 << 3,%ax
    ltr     %ax
    lidt    S_id
# Inicjowanie ‘page tables’ trybu IA-32e.
    xor     %eax,%eax
    mov     $0xc000,%edi
    xor     %eax,%eax
    mov     $Z_page_entry_S_p | Z_page_entry_S_rw,%edx
    mov     $0x1000 / 8,%ecx
0:  cmp     $0x80000,%edx
    ja      0f
    mov     %edx,(%edi)
    lea     4(%edi),%edi
    stosl
    add     $0x1000,%edx
    dec     %ecx
    jnz     0b
0:  shl     $1,%ecx
    rep stosl
    mov     $0xc000 | Z_page_entry_S_p | Z_page_entry_S_rw,%eax
    stosl
    xor     %eax,%eax
    mov     $0x1000 / 8 * 2 - 1,%ecx
    rep stosl
    mov     $0xd000 | Z_page_entry_S_p | Z_page_entry_S_rw,%eax
    stosl
    xor     %eax,%eax
    mov     $0x1000 / 8 * 2 - 1,%ecx
    rep stosl
    mov     $0xe000 | Z_page_entry_S_p | Z_page_entry_S_rw,%eax
    stosl
    xor     %eax,%eax
    mov     $0x1000 / 8 * 2 - 1,%ecx
    rep stosl
# Inicjowanie tymczasowych deskryptorów ‘gdt’ i ‘idt’ trybu IA-32e, przygotowanie deskryptora ‘ldt’.
    mov     $S_gdt,%ebx
    mov     $S_gd,%esi
    mov     $S_id,%edi
    mov     %ebx,2(%esi)
    movl    $S_idt,2(%edi)
    mov     $S_ldt,%eax
    mov     %eax,%ecx
    mov     %eax,%edx
    and     $0xff000000,%edx
    and     $0xff0000,%ecx
    shr     $16,%ecx
    mov     %ax,3 * 8 + 2(%ebx)
    or      %ecx,%edx
    or      %edx,3 * 8 + 4(%ebx)
# Przełączenie do trybu IA-32e.
    mov     %cr4,%eax
    or      $Z_cr4_S_pae | Z_cr4_S_osfxsr | Z_cr4_S_osxmmexcpt | Z_cr4_S_osxsave,%eax
    mov     %eax,%cr4
    mov     $0xf000,%eax
    mov     %eax,%cr3
    mov     $Z_msr_S_ia32_efer,%ecx
    rdmsr
    or      $Z_msr_Z_ia32_efer_S_lme,%eax
    wrmsr
    mov     %cr0,%eax
    and     $~( Z_cr0_S_em | Z_cr0_S_ts ),%eax
    or      $Z_cr0_S_pg | Z_cr0_S_mp,%eax
    mov     %eax,%cr0
    lgdt    S_gd
    mov     $2 << 3,%ax
    mov     %ax,%ds
    mov     %ax,%es
    mov     %ax,%ss
    xor     %ax,%ax
    mov     %ax,%fs
    mov     %ax,%gs
    mov     $3 << 3,%ax
    lldt    %ax
    lidt    S_id
    ljmp    $1 << 3,$0f
.code64
0:  mov     %esp,%esp
    fninit
    pop     %rdi                            # Przekazanie adresu tablicy obszarów pamięci RAM.
    lea     -4*8(%rsp),%rsp
    lea     E_main_S_video(%rip),%rsi
    call    main
#===============================================================================
.code16
Q_vga_I_print_hex_word:
    push    %ds
    push    %es
    push    %ax
    mov     %sp,%bp
    mov     $16,%cl
0:  sub     $4,%cl
    mov     (%bp),%dx
    shr     %cl,%dx
    and     $0xf,%dl
    cmp     $9,%dl
    ja      2f
    mov     $'0',%al
    jmp     1f
2:  mov     $'a',%al
    sub     $10,%dl
1:  add     %dl,%al
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    push    %bp
    push    %cx
    int     $0x10
    pop     %cx
    pop     %bp
    test    %cl,%cl
    jnz     0b
    mov     $',',%al
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    int     $0x10
    mov     $' ',%al
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    int     $0x10
    pop     %ax
    pop     %es
    pop     %ds
    ret
Q_vga_I_print:
0:  lodsb
    test    %al,%al
    jz      0f
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    push    %si
    push    %es
    push    %ds
    int     $0x10
    pop     %ds
    pop     %es
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
.data
Q_print_S_logo:
    .asciz  "OUX/C+ OS boot loader. Second Loader.\r\n"
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
E_main_Z_video_S_bits_per_pixel:
    .byte  0
E_main_Z_video_S_blue_size_start:
    .2byte  0
E_main_Z_video_S_green_size_start:
    .2byte  0
E_main_Z_video_S_red_size_start:
    .2byte  0
#~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
.p2align 1
S_gd:
    .2byte  5 * 8 - 1
    .4byte  S_gdt_32
    .4byte  0
S_id:
    .2byte  8 - 1
    .4byte  S_idt_32
    .4byte  0
.p2align 2
S_tss_32:
    .4byte 0,0,0,0,0,0,0
    .4byte 0,0,0,0,0,0,0,0,0,0,0
    .4byte 0,0,0,0,0,0,0
    .2byte 0
    .2byte ~0
    .4byte 0
S_tss_32_end:
#-------------------------------------------------------------------------------
.macro descriptor base=0,limit=~0,flags
    .2byte  \limit & ~( ~0 << 16 )
    .2byte  \base & ~( ~0 << 16 )
    .4byte  (( \base >> 16 ) & 0xff ) | ( \limit & ( 0xf << 16 )) | ( \base & ( 0xff << 24 )) | Z_descriptor_S_p | \flags
.endm
.p2align 3
S_gdt_32:
    .8byte  0
    descriptor ,,Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_s | Z_descriptor_S_db | Z_descriptor_S_g
    descriptor ,,Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s | Z_descriptor_S_db | Z_descriptor_S_g
    descriptor ,(8 - 1),Z_descriptor_Z_type_system_S_ldt
    descriptor ,(S_tss_32_end - S_tss_32 - 1),Z_descriptor_Z_type_system_S_tss
S_ldt_32:
    .8byte  0
S_idt_32:
    .8byte  0
S_gdt:
    .8byte  0
    descriptor ,,Z_descriptor_Z_type_data_code_S_code | Z_descriptor_Z_type_data_code_S_a | Z_descriptor_S_s | Z_descriptor_S_l | Z_descriptor_S_g
    descriptor ,,Z_descriptor_Z_type_data_code_S_a | Z_descriptor_Z_type_data_S_w | Z_descriptor_S_s | Z_descriptor_S_g
    descriptor ,(8 - 1),Z_descriptor_Z_type_system_S_ldt
    .8byte  0
S_idt:
    .8byte  0
    .8byte  0
S_ldt:
    .8byte  0
#*******************************************************************************
