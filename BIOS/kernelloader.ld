SECTIONS
{ . = 0x10000;
  .boot_text :
  { a.out(.text)
    a.out(.data)
  }
  .text : { *(EXCLUDE_FILE(*a.out) .text) }
  .data : { *(EXCLUDE_FILE(*a.out) .data) }
  .rodata : { *(.rodata) *(.rodata.*) }
  E_main_S_relocation_data = .;
}
