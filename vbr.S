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
    mov     Q_memory_S_vbr, %sp
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    mov     %sp, %bp
    ljmp    $0, $0f
0:  push    %dx                             # Numer dysku, z którego ‘bootuje’.
    mov     $Q_print_S_logo, %si
    call    Q_vga_I_print
    cld
    mov     $Q_memory_S_vbr + Q_offset_S_file_loader_data, %si
    lodsb
    mov     %al, %dh                        # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_print_no_file_loader
    pusha
    mov     $Q_print_S_booting_file, %si
    call    Q_vga_I_print
    popa
    mov     -2(%bp), %dl
    mov     $Q_memory_S_file_loader, %bx
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
    pop     %dx
    jmp     Q_memory_S_file_loader
I_print_no_file_loader:
    mov     $Q_print_S_no_file_loader, %si
    jmp     0f
I_print_io_error:
    mov     $Q_print_S_io_error, %si
0:  call    Q_vga_I_print
    cli
0:  hlt
    jmp 0b
#===============================================================================
.org Q_offset_S_second_mbr_part
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
    .asciz  "OUX/C+ boot loader. Volume Boot Record.\r\n"
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
