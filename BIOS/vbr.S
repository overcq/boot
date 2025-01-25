#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS VBR boot loader
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
.org Q_memory_S_vbr
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
    mov     %al, -1(%bp)                    # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_print_S_no_file_loader
    push    %cx
    push    %ax
    mov     $Q_print_S_booting_file, %si
    push    %bp
    call    Q_vga_I_print
    pop     %bp
    mov     $Q_memory_S_file_loader, %bx
1:  mov     -2(%bp), %dx
    mov     -4(%bp), %cx
    mov     $0x201, %ax
    push    %bp
    push    %bx
    int     $0x13
    jc      I_print_S_io_error
    pop     %bx
    pop     %bp
    mov     -4(%bp), %cl
    and     $~( ~0 << 6 ), %cl
    inc     %cl
    testb   $0x80, -2(%bp)
    jnz     0f
    cmp     $19, %cl
    jb      3f
    mov     $1, %cl
    incb    -1(%bp)
    cmpb    $2, -1(%bp)
    jmp     2f
0:  cmp     $64, %cl
    jb      3f
    mov     $1, %cl
    incb    -1(%bp)
    cmpb    $16, -1(%bp)
2:  jb      3f
    movb    $0, -1(%bp)
    addb    $1, -3(%bp)
    jnc     3f
    addb    $1 << 6, -4(%bp)
    jc      I_hlt
3:  andb    $~0 << 6, -4(%bp)
    or      %cl, -4(%bp)
    pop     %ax
    dec     %al
    jz      0f
    push    %ax
    add     $512, %bx
    jnc     1b
    mov     %es, %cx
    add     $1 << 12, %cx
    mov     %cx, %es
    jmp     1b
0:  add     $3 * 2, %sp
    pop     %dx
    mov     $Q_memory_S_file_loader, %sp    # Ustawienie większego stosu.
    jmp     Q_memory_S_file_loader
#-------------------------------------------------------------------------------
#    jmp     0f
.org Q_memory_S_vbr + Q_offset_S_second_mbr_part
#-------------------------------------------------------------------------------
I_print_S_no_file_loader:
    mov     $Q_print_S_no_file_loader, %si
    jmp     0f
I_print_S_io_error:
    xor     %ax, %ax
    mov     $Q_print_S_io_error, %si
    mov     %ax, %es
0:  call    Q_vga_I_print
I_hlt:
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
.org Q_memory_S_vbr + Q_offset_S_signature
.byte   0x55, 0xaa
#*******************************************************************************
