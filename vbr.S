#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   VBR boot loader
#         entry
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒11 T
#*******************************************************************************
.equiv Q_memory_S_vbr,              0x7c00
# “Q_memory_S_file_loader” musi być na wielokrotności 512.
.equiv Q_memory_S_file_loader,      0x7e00
#-------------------------------------------------------------------------------
.equiv Q_offset_S_file_loader_data, 224
.equiv Q_offset_S_second_mbr_part,  228
.equiv Q_offset_S_signature,        510
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_light_gray,    7
#===============================================================================
.code16
.text
    cli
    xor     %ax, %ax
    mov     %ax, %ss
    mov     $Q_memory_S_vbr, %sp
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    mov     %sp, %bp
    ljmp    $0, $0f
0:  push    %dx                             # Numer dysku, z którego ‘bootuje’.
    mov     $Q_print_S_logo, %si
    push    %bp
    call    Q_vga_I_print
    pop     %bp
    cld
    mov     $Q_memory_S_vbr + Q_offset_S_file_loader_data, %si
    lodsb
    mov     %al, %dh                        # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_print_no_file_loader
    push    %ax
    push    %cx
    push    %dx
    mov     $Q_print_S_booting_file, %si
    push    %bp
    call    Q_vga_I_print
    pop     %bp
    movzbw  -4(%bp), %ax
    mov     $512, %si
    mov     $Q_memory_S_file_loader, %bx
    mul     %si                             # Rozmiar pliku w “dx:ax”.

    inc     %dx                             # DBG

    push    %dx
1:  push    %ax
    mov     $1, %al
    mov     -7(%bp), %dh
    mov     -6(%bp), %cx
    mov     -2(%bp), %dl
    mov     $2, %ah
    push    %bp
    int     $0x13
    jc      I_print_io_error
    pop     %bp
    mov     -6(%bp), %cl
    and     $~( ~0 << 6 ), %cl
    inc     %cl
    testb   $0x80, -2(%bp)
    jnz     0f
    cmp     $19, %cl
    jb      2f
    mov     $1, %cl
    incb    -7(%bp)
    testb   $~0x1, -7(%bp)
    jz      2f
    movb    $0, -7(%bp)
    jmp     2f
0:  test    $~0 << 6, %cl
    jz      2f
    inc     %cl
    and     $~( ~0 << 6 ), %cl
    incb    -7(%bp)
    testb   $~0xf, -7(%bp)
    jz      2f
#    addb    $1, -7(%bp)
#    jnc     2f
    movb    $0, -7(%bp)
0:  addb    $1, -5(%bp)
    jnc     2f
    addb    $1 << 6, -6(%bp)
2:  andb    $~0 << 6, -6(%bp)
    or      %cl, -6(%bp)
    pop     %ax
    dec     %al
    jz      0f
    add     $512, %bx
    jnc     1b
    mov     %es, %cx
    pop     %dx
    add     $1 << 12, %cx
    test    %dx, %dx
    mov     %cx, %es
    jz      0f
    dec     %dx
    push    %dx
    pushw   $0x10000 / 512
    jmp     1b
0:  
#-------------------------------------------------------------------------------
    jmp     0f
.org Q_offset_S_second_mbr_part
#-------------------------------------------------------------------------------
0:  add     $3 * 2, %sp
    pop     %dx
    mov     $Q_memory_S_file_loader, %sp    # Ustawienie większego stosu.
    jmp     Q_memory_S_file_loader
I_print_no_file_loader:
    mov     $Q_print_S_no_file_loader, %si
    jmp     0f
I_print_io_error:
    xor     %ax, %ax
    mov     %ax, %es
    mov     $Q_print_S_io_error, %si
0:  call    Q_vga_I_print
    cli
0:  hlt
    jmp 0b
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
    .asciz  "OUX/C+ OS boot loader. Volume Boot Record.\r\n"
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_no_file_loader:
    .asciz  "No file loader."
Q_print_S_booting_file:
    .asciz  "Booting file...\r\n"
#===============================================================================
.org Q_offset_S_signature
.byte   0x55, 0xaa
#*******************************************************************************
