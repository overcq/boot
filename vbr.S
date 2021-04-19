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
    sti
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
    push    %ax
    push    %cx
    push    %dx
    mov     $Q_print_S_booting_file, %si
    call    Q_vga_I_print
    xor     %dx, %dx
    movzbw  -4(%bp), %ax
    mov     $512, %si
    mov     $Q_memory_S_file_loader, %bx
    mul     %si                             # Rozmiar pliku w “dx:ax”.
    add     %bx, %ax
    adc     $0,%dx
    jz      2f                              # Czy nie zostanie przekroczony blok 0x10000?
    dec     %dx
    push    %dx
    xor     %dx,%dx
    div     %si                             # Liczba sektorów pozostałych w ostatnim bloku 0x10000.
    mov     %ax, -4(%bp)
    mov     $Q_memory_S_file_loader, %ax
    xor     %dx, %dx
    neg     %ax
    div     %si
    push    %ax                             # Liczba sektorów w pierwszym bloku 0x10000.
1:  mov     -7(%bp), %dh
    mov     -6(%bp), %cx
    mov     -2(%bp), %dl
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
# Zamiana CHS na LBA.
    mov     -7(%bp), %bl
    mov     -6(%bp), %ax
    movzbl  %bl, %ebx                       # head
    mov     %al, %cl
    shr     $6, %al
    xchg    %al, %ah
    movzwl  %ax, %eax                       # cylinder
    and     $~( ~0 << 6 ), %cl
    dec     %cl
    movzbl  %cl, %ecx                       # sector from 0
    mov     $255, %esi
    xor     %edx, %edx
    mul     %esi
    add     %ebx, %eax
    mov     $63, %esi
    xor     %edx, %edx
    mul     %esi
    add     %ecx, %eax
    pop     %dx
    movzwl  %dx, %edx                       # Liczba odczytanych sektorów.
    add     %edx, %eax
# Zamiana LBA na CHS.
    mov     $255 * 63, %esi
    xor     %edx, %edx
    div     %esi
    xchg    %al, %ah
    shl     $6, %al
    mov     %ax, %cx                        # cylinder without sector
    mov     %dx, %ax
    mov     $63, %si
    xor     %dx, %dx
    div     %si
    mov     %al, -7(%bp)                    # head
    inc     %dl
    or      %dl, %cl                        # sector
#-------------------------------------------------------------------------------
    jmp     0f
.org Q_offset_S_second_mbr_part
#-------------------------------------------------------------------------------
0:  mov     %cx, -6(%bp)                    # cylinder with sector
    mov     %es, %ax
    pop     %dx
    add     $1 << 12, %ax
    xor     %bx, %bx
    mov     %ax, %es
    test    %dx, %dx
    jz      2f
    dec     %dx
    push    %dx
    mov     $0x10000 / 512, %ax
    push    %ax
    jmp     1b
2:  pop     %dx
    pop     %cx
    pop     %ax
    test    %ax, %ax
    jz      0f
    mov     -2(%bp), %dl
    mov     $2, %ah
    int     $0x13
    jc      I_print_io_error
0:  pop     %dx
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
