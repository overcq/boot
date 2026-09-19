SECTIONS
{ . = 0x10000;
  .text :
  { a.out(.text)
    a.out(.ltext)
    a.out(.data)
    a.out(.ldata)
    *(EXCLUDE_FILE(a.out) .text)
    *(EXCLUDE_FILE(a.out) .ltext)
    FILL(0);
    . = 0x30000;
    FILL(0);
    . = 0x50000;
    *(EXCLUDE_FILE(a.out) .data)
    *(EXCLUDE_FILE(a.out) .ldata)
    *(.rodata) *(.rodata.*)
    *(.lrodata) *(.lrodata.*)
  }
  E_main_S_relocation_data = .;
}
