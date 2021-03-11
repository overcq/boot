#*******************************************************************************
#   ___   publicplace
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   MBR boot loader
#         entry
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
.equiv Q_memory_S_relocation_from,  0x7c00
.equiv Q_memory_S_relocation_to,    0x7a00
.equiv Q_memory_S_file_loader,      0x7e00
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
    mov     $Q_memory_S_relocation_to, %sp
    push    %ax
    pop     %ds
    push    %ax
    pop     %es
    sti
    mov     %sp, %bp
    push    %dx                             # Numer dysku, z którego ‘bootuje’.
    push    $Q_print_S_logo + ( Q_memory_S_relocation_from - Q_memory_S_relocation_to )
    call    Q_vga_I_print
    cld
    mov     $Q_offset_S_file_loader_data, %si
    lodsb
    mov     %al, %dh                        # head
    lodsw
    mov     %ax, %cx                        # sector, cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al, %al
    jz      I_next_boot_loader
    mov     -2(%bp), %dl
    mov     $Q_memory_S_file_loader, %bx
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
    pop     %dx
    jmp     Q_memory_S_file_loader
I_next_boot_loader:
    mov     $Q_memory_S_relocation_from, %si
    mov     %bp, %di
    mov     $512 / 2, %cx
    rep movsw
    ljmp    $0, $Q_memory_S_relocation_to + 0f - 0b
0:  cld
    mov     $4, %cx
    mov     $Q_memory_S_relocation_to + Q_offset_S_partiton - 15, %si
0:  add     $15, %si
    lodsb
    cmp     $0x80, %al
    loopne  0b
    jne     I_print_no_bootable_partition
    push    %cx
    mov     -2(%bp), %dl
    lodsb
    mov     %al, %dh
    lodsw
    movw    %ax, %cx
    mov     $Q_memory_S_relocation_from, %bx
    mov     $0x201, %ax
    int     $0x13
    jc      I_print_io_error
    mov     $Q_memory_S_relocation_from + Q_offset_S_signature, %bx
    mov     (%bx), %ax
    cmp     $0xaa55, %ax
    jne     I_print_not_bootable_sector
    push    $Q_print_S_booting_partition_1
    call    Q_vga_I_print
    pop     %cx
    mov     $'4', %al
    sub     %cl, %al
    call    Q_vga_I_putchar
    push    $Q_print_S_booting_partition_2
    call    Q_vga_I_print
    pop     %dx
    jmp     Q_memory_S_relocation_from
I_print_io_error:
    push    $Q_print_S_io_error
    jmp     0f
I_print_no_bootable_partition:
    push    $Q_print_S_no_bootable_partition
    jmp     0f
I_print_not_bootable_sector:
    push    $Q_print_S_not_bootable_sector
0:  call    Q_vga_I_print
    cli
0:  hlt
    jmp 0b
#===============================================================================
.org Q_offset_S_second_mbr_part
Q_vga_I_putchar:
    mov     $0xe, %ah
    mov     $Q_vga_color_S_light_gray, %bx
    int     $0x10
    ret
Q_vga_I_print:
    pop     %ax
    pop     %si
    push    %ax
0:  cld
    lodsb
    test    %al, %al
    jz      0f
    push    %si
    call    Q_vga_I_putchar
    pop     %si
    jmp     0b
0:  ret
#===============================================================================
Q_print_S_logo:
    .asciz  "OUX/C+ boot loader. Master Boot Record.\r\n"
Q_print_S_io_error:
    .asciz  "Disc I/O error."
Q_print_S_no_bootable_partition:
    .asciz  "No bootable partition."
Q_print_S_not_bootable_sector:
    .asciz  "Not bootable sector."
Q_print_S_booting_partition_1:
    .asciz  "Booting partition "
Q_print_S_booting_partition_2:
    .asciz  "...\r\n"
#===============================================================================
.org Q_offset_S_signature
.byte   0x55
.byte   0xaa
#*******************************************************************************
