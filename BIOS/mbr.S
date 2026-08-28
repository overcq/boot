#*******************************************************************************
#   ___   public
#  ¦OUX¦  16-bit assembly
#  ¦/C+¦  OUX/C+ OS
#   ---   BIOS MBR boot loader
#         wczytanie drugiego etapu
# ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒2‒27 f
#*******************************************************************************
.equiv Q_offset_S_kernel_loader_data,   224
.equiv Q_offset_S_second_mbr_part,      228
.equiv Q_offset_S_signature,            510
#-------------------------------------------------------------------------------
.equiv Q_vga_color_S_light_gray,        7
#===============================================================================
.code16
.text
    cld
    cli
    mov     $0x70e0,%cx
    xor     %ax,%ax
    mov     %cx,%ss
    mov     %ax,%sp
    mov     %ax,%ds
    mov     $0x7c00,%si
    mov     %cx,%es
    mov     %ax,%di
    mov     $512 / 2,%cx
    rep movsw
    ljmp    $0x70e0,$0f
0:  sti
    push    %es
    pop     %ds
    mov     %sp,%bp
    push    %dx                             # Numer dysku,z którego ‘bootuje’.
    push    %bp
    mov     $Q_print_S_logo,%si
    call    Q_vga_I_print
    mov     $1,%eax
    cpuid
# PAT, CMOV, APIC, MSR, TSC, FPU
    xor     $( 1 << 16 ) | ( 1 << 15 ) | ( 1 << 9 ) | ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 0 ),%edx
    test    $( 1 << 16 ) | ( 1 << 15 ) | ( 1 << 9 ) | ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 0 ),%edx
    jnz     I_hlt
# x2APIC
    test    $1 << 21,%ecx
    jz      I_hlt
    mov     $Q_offset_S_kernel_loader_data,%si
    lodsb
    mov     %al,%dh                         # head
    lodsw
    mov     %ax,%cx                         # sector,cylinder
    lodsb                                   # Liczba sektorów do przeczytania.
    test    %al,%al
    jz      Q_print_I_second_loader_not_set
    push    %ax
    push    %cx
    push    %dx
    mov     $Q_print_S_booting_second,%si
    call    Q_vga_I_print
    pop     %dx
    pop     %cx
    pop     %ax
    pop     %bp
    mov     $0x1000,%si
    push    %si
    mov     %si,%es
    mov     -2(%bp),%dl
    xor     %bx,%bx
    mov     $2,%ah
    int     $0x13
    jc      Q_print_I_io_error
    pop     %ax
    mov     %ax,%ds
    mov     %ax,%es
    pop     %dx
    mov     $0x7000,%ax
    mov     %ax,%ss
    ljmp    $0x1000,$0
#-------------------------------------------------------------------------------
.org Q_offset_S_second_mbr_part
#-------------------------------------------------------------------------------
Q_print_I_io_error:
    mov     $Q_print_S_io_error,%si
    call    Q_vga_I_print
    jmp     I_hlt
Q_print_I_second_loader_not_set:
    mov     $Q_print_S_no_location_error,%si
    call    Q_vga_I_print
I_hlt:
    cli
0:  hlt
    jmp     0b
Q_vga_I_putchar:
    mov     $0xe,%ah
    mov     $Q_vga_color_S_light_gray,%bx
    push    %es
    push     %ds
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
.org Q_offset_S_signature
.byte   0x55,0xaa
#*******************************************************************************
