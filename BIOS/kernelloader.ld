OUTPUT_FORMAT("binary")
SECTIONS
{ . = 0x10000;
  .boot_text :
  { a.out(.text)
    a.out(.data)
  }
  .rela.dyn :
  { E_main_S_relocation_data = .;
    *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
    *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
    *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
    *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
    *(.rela.got)
    *(.rela.ldata .rela.ldata.* .rela.gnu.linkonce.l.*)
    *(.rela.lrodata .rela.lrodata.* .rela.gnu.linkonce.lr.*)
    *(.rela.ifunc)
  }
  .text : { *(EXCLUDE_FILE(*a.out) .text) }
  .data : { *(EXCLUDE_FILE(*a.out) .data) }
}
