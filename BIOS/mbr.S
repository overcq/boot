#*******************************************************************************
#   ___   public
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS MBR boot loader
#         wczytanie drugiego etapu
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
.equiv Q_offset_S_first_mbr_part_end,   218
.equiv Q_offset_S_kernel_loader_data,   224
.equiv Q_offset_S_second_mbr_part,      228
.equiv Q_offset_S_second_mbr_part_end,  440
.equiv Q_offset_S_signature,            510
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_light_gray,        7
#===============================================================================
.text
.code16
    cli
    cld
    mov     $0x7000,%cx
    mov     $0x7fc0,%ax
    xor     %bp,%bp
    mov     %cx,%ss
    mov     %bp,%sp
    mov     %bp,%ds
    mov     $0x7c00,%si
    mov     %ax,%es
    mov     %bp,%di
    mov     $512 / 2,%cx
    rep movsw
    ljmp    $0x7fc0,$0f
0:  sti
    pusha
    push    %es
    pop     %ds
    mov     $Q_print_S_logo,%si
    call    Q_vga_I_print
    popa
    mov     $Q_offset_S_kernel_loader_data,%si
    lodsw
    mov     %ax,%di                         # Pozycja LBA.
    lodsw                                   # Liczba sektorów do przeczytania.
    test    %ax,%ax
    jnz     1f
    mov     $Q_print_S_no_location_error,%si
    call    Q_vga_I_print
I_hlt:
    cli
0:  hlt
    jmp     0b
1:  pusha
    mov     $Q_print_S_booting_second,%si
    call    Q_vga_I_print
    popa
    pusha
    push    %bp
    push    %es
    xor     %di,%di
    mov     %di,%es
    mov     $8,%ah
    int     $0x13
    jc      I_hlt
    pop     %es
    pop     %bp
    and     $~( ~0 << 6 ),%cl
    mov     %dh,%ch
    inc     %cl
    mov     %cx,-9 * 2(%bp)
    popa
    push    -9 * 2(%bp)
    mov     $0x1000,%si
    xor     %bx,%bx
    xor     %dh,%dh
    mov     $1,%cx
0:  cmp     $0x5000,%si
    jae     1f
    cmp     $0x3000,%si
    jnb     2f
1:  mov     %si,%es
    test    %di,%di
    jnz     2f
    pusha
    mov     $0x201,%ax
    int     $0x13
    jnc     1f
    mov     $Q_print_S_io_error,%si
    call    Q_vga_I_print
    jmp     I_hlt
1:  popa
2:  push    %ax
    mov     %cl,%al
    and     $~( ~0 << 6 ),%al
    inc     %al
1:  cmp     -2(%bp),%al
    jne     1f
    mov     $1,%al
    inc     %dh
    cmp     -1(%bp),%dh
    jne     1f
    xor     %dh,%dh
    add     $1,%ch
    jnc     1f
    add     $1 << 6,%cl
    jc      I_hlt
1:  and     $~0 << 6,%cl
    or      %al,%cl
    pop     %ax
    test    %di,%di
    jz      1f
    dec     %di
    jmp     0b
1:  dec     %ax
    jz      0f
    add     $512,%bx
    jnc     0b
    lea     0x1000(%si),%si
    jmp     0b
0:  mov     $0x1000,%ax
    pop     %cx
    mov     %ax,%ds
    mov     %ax,%es
    jmp     0f
#-------------------------------------------------------------------------------
.org Q_offset_S_first_mbr_part_end
.org Q_offset_S_second_mbr_part
#-------------------------------------------------------------------------------
0:  ljmp    $0x1000,$0
Q_vga_I_putchar:
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    push    %es
    push    %ds
    int     $0x10
    pop     %ds
    pop     %es
    ret
Q_vga_I_print:
0:  lodsb
    test    %al,%al
    jz      0f
    push    %si
    call    Q_vga_I_putchar
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ OS boot loader. Master Boot Record.\r\n"
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_no_location_error:
    .asciz  "Second loader not set."
Q_print_S_booting_second:
    .asciz  "Booting second loader...\r\n"
#===============================================================================
.org Q_offset_S_second_mbr_part_end
.org Q_offset_S_signature
.byte   0x55,0xaa
#*******************************************************************************
