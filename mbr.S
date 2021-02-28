################################################################################
#   ___   [obszar]
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  [rodzaj składnika]
#   ---   [nazwa składnika]
#         [tytuł elementu]
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
################################################################################
.equiv S_reloc_from,        0x7c00
.equiv S_reloc_to,          0x7a00
.equiv S_partiton,          S_reloc_to + 0x1be
.equiv S_signature,         510
.equiv S_second_mbr_part,   0xe0
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_black,         0
.equiv Q_vga_color_S_blue,          1
.equiv Q_vga_color_S_green,         2
.equiv Q_vga_color_S_cyan,          3
.equiv Q_vga_color_S_red,           4
.equiv Q_vga_color_S_magenta,       5
.equiv Q_vga_color_S_brown,         6
.equiv Q_vga_color_S_light_gray,    7
.equiv Q_vga_color_S_dark_gray,     8
.equiv Q_vga_color_S_light_blue,    9
.equiv Q_vga_color_S_light_green,   10
.equiv Q_vga_color_S_light_cyan,    11
.equiv Q_vga_color_S_light_red,     12
.equiv Q_vga_color_S_light_magenta, 13
.equiv Q_vga_color_S_light_brown,   14
.equiv Q_vga_color_S_white,         15
#===============================================================================
.code16
.text
0:  cli
    xor     %ax, %ax
    mov     %ax, %ss
    mov     $S_reloc_to, %sp
    mov     %sp, %di
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    cld
    mov     $S_reloc_from, %si
    mov     $512 / 2, %cx
    rep movsw
    ljmp    $0, $S_reloc_to + 0f - 0b
0:  call    main
    cli
0:  hlt
    jmp 0b
main:
    push    %dx                             # Numer dysku, z którego ‘bootuje’.
    push    $Q_print_S_logo
    call    Q_vga_I_print
    cld
    mov     $4, %cx
    mov     $S_partiton - 15, %si
0:  add     $15, %si
    lodsb
    cmp     $0x80, %al
    loopne  0b
    pop     %dx
    jne     0f
    lodsb
    mov     %al, %dh
    lodsw
    movw    %ax, %cx
    mov     $S_reloc_from, %bx
    mov     $0x201, %ax
    int     $0x13
    jc      1f
    mov     $S_reloc_from + S_signature, %bx
    mov     (%bx), %ax
    cmp     $0x55aa, %ax
    jne     2f
    pop     %ax                             # Zdjęcie adresu powrotu z “main”.
    jmp     *S_reloc_from
0:  push    $Q_print_S_no_bootable_partition
    jmp     3f
1:  push    $Q_print_S_io_error
    jmp     3f
2:  push    $Q_print_S_not_bootable_sector
3:  call    Q_vga_I_print
    ret
#===============================================================================
.org S_second_mbr_part
Q_vga_I_print:
    pop     %ax
    pop     %si
    push    %ax
0:  cld
    lodsb
    test    %al, %al
    jz      0f
    mov     $0xe, %ah
    mov     $Q_vga_color_S_white, %bx
    push    %si
    int     $0x10
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ boot loader. Master Boot Record.\r\n"
Q_print_S_no_bootable_partition:
    .asciz  "No bootable partition."
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_not_bootable_sector:
    .asciz  "Not bootable sector."
#===============================================================================
.org S_signature
.byte   0x55
.byte   0xaa
################################################################################
