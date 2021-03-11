#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   VBR boot loader
#         entry
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒11 T
#*******************************************************************************
.equiv Q_memory_S_vbr,              0x7c00
.equiv Q_memory_S_file_loader,      0x7e00
#-------------------------------------------------------------------------------
.equiv Q_offset_S_file_loader_data, 506
.equiv Q_offset_S_signature,        510
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_light_gray,    7
#===============================================================================
.code16
.text
0:  cli
    xor     %ax, %ax
    mov     %ax, %ss
    mov     Q_memory_S_vbr, %sp
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    mov     %sp, %bp
    ljmp    $0, $Q_memory_S_vbr + 0f - 0b
0:  push    %dx                             # Numer dysku, z którego ‘bootuje’.
    push    $Q_print_S_logo
    call    Q_vga_I_print
    cld
    mov     $Q_offset_S_file_loader_data, %si
    lodsb
    mov     %al, %dh                        # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_print_no_file_loader
    mov     -2(%bp), %dl
    mov     $Q_memory_S_file_loader, %bx
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
    pop     %dx
    jmp     Q_memory_S_file_loader
I_print_no_file_loader:
    push    $Q_print_S_no_file_loader
    jmp     0f
I_print_io_error:
    push    $Q_print_S_io_error
0:  call    Q_vga_I_print
    cli
0:  hlt
    jmp 0b
#===============================================================================
Q_vga_I_print:
    pop     %ax
    pop     %si
    push    %ax
0:  cld
    lodsb
    test    %al, %al
    jz      0f
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    push    %si
    int     $0x10
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ boot loader. Volume Boot Record.\r\n"
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_no_file_loader:
    .asciz  "No file loader."
#===============================================================================
.org Q_offset_S_signature
.byte   0x55
.byte   0xaa
#*******************************************************************************
