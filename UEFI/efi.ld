OUTPUT_FORMAT( "elf64-x86-64", "elf64-x86-64", "elf64-x86-64" )
OUTPUT_ARCH( i386:x86-64 )
ENTRY( H_efi_I_main )
SECTIONS
{
    .hash : { *(.hash) }
    .gnu.hash : { *(.gnu.hash) }
    . = ALIGN(4096);
    .text :
    {
        *(.text)
        *( .text.* )
        *( .gnu.linkonce.t.* )
        *(.plt)
        . = ALIGN(16);
    }
    . = ALIGN(4096);
    .data :
    {
        *(.got.plt)
        *(.got)
        *( .data* )
        *(.sdata)
        *(.sbss)
        *(.scommon)
        *(.dynbss)
        *( .bss* )
        *(COMMON)
        *(.rel.local)
        LONG(0); LONG(0);
    }
    . = ALIGN(4096);
    .reloc :
    {   KEEP( *(.reloc) )
        LONG(0); LONG(0);
    }
}
