/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   file boot loader
*         after protected mode initialization
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
*******************************************************************************/
struct __attribute__((__packed__)) E_main_Z_memory_table
{ unsigned long address;
  unsigned long size;
  unsigned type;
  unsigned extended_attributes;
};
//==============================================================================
void
main( void
){  struct E_main_Z_memory_table *memory_table;
    __asm__ (
    "\n"    "lea    8(%%rbp), %0"
    : "=r" ( memory_table )
    :
    : "rax"
    );
    
    __asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
