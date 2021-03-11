#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   MBR boot loader
#         entry
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
.equiv S_relocation_from,           0x7c00
.equiv S_relocation_to,             0x7a00
.equiv S_file_loader,               0x7e00
#-------------------------------------------------------------------------------
.equiv Q_offset_S_second_mbr_part,  0xe0
.equiv Q_offset_S_partiton,         0x1be
.equiv Q_offset_S_file_loader_data, 0x1b3
.equiv Q_offset_S_signature,        510
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_light_gray,    7
#===============================================================================
.code16
.text
0:  cli
    xor     %ax, %ax
    mov     %ax, %ss
    mov     $S_relocation_to, %sp
    mov     %sp, %di
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    cld
    mov     $Q_offset_S_file_loader_data, %si
    lodsb
    mov     %al, %dh                        # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_next_boot_loader
    mov     $S_file_loader, %bx
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
    jmp     *S_file_loader
I_next_boot_loader:
    mov     $S_relocation_from, %si
    mov     $512 / 2, %cx
    rep movsw
    ljmp    $0, $S_relocation_to + 0f - 0b
0:  push    %dx                             # Numer dysku, z którego ‘bootuje’.
    push    $Q_print_S_logo
    call    Q_vga_I_print
    cld
    mov     $4, %cx
    mov     $S_relocation_to + Q_offset_S_partiton - 15, %si
0:  add     $15, %si
    lodsb
    cmp     $0x80, %al
    loopne  0b
    pop     %dx
    jne     I_print_no_bootable_partition
    lodsb
    mov     %al, %dh
    lodsw
    movw    %ax, %cx
    mov     $S_relocation_from, %bx
    mov     $0x201, %ax
    int     $0x13
    jc      I_print_io_error
    mov     $S_relocation_from + Q_offset_S_signature, %bx
    mov     (%bx), %ax
    cmp     $0x55aa, %ax
    jne     I_print_not_bootable_sector
    jmp     *S_relocation_from
I_print_no_bootable_partition:
    push    $Q_print_S_no_bootable_partition
    jmp     0f
I_print_io_error:
    push    $Q_print_S_io_error
    jmp     0f
I_print_not_bootable_sector:
    push    $Q_print_S_not_bootable_sector
0:  call    Q_vga_I_print
    cli
0:  hlt
    jmp 0b
#===============================================================================
.org Q_offset_S_second_mbr_part
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
    .asciz  "OUX/C+ boot loader. Master Boot Record.\r\n"
Q_print_S_no_bootable_partition:
    .asciz  "No bootable partition."
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_not_bootable_sector:
    .asciz  "Not bootable sector."
#===============================================================================
.org Q_offset_S_signature
.byte   0x55
.byte   0xaa
#*******************************************************************************
