SECTIONS
{ . = 0x10000;
  .text :
  { a.out(.text)
    a.out(.data)
    /*kernelloader.o(.text)
    acpi.o(.text)
    disc.o(.text)
    font.o(.text)
    interrupt.o(.text)
    mem-blk.o(.text)
    mem-mask.o(.text)
    ouxfs.o(.text)
    pci.o(.text)
    sata.o(.text)
    text.o(.text)
    vga.o(.text)*/
    *(EXCLUDE_FILE(a.out /*kernelloader.o acpi.o disc.o font.o interrupt.o mem-blk.o mem-mask.o ouxfs.o pci.o sata.o text.o vga.o*/) .text)
    FILL(0);
    . = 0x30000;
    FILL(0);
    . = 0x50000;
    *(EXCLUDE_FILE(a.out) .data)
    *(.rodata) *(.rodata.*)
  }
  E_main_S_relocation_data = .;
}
