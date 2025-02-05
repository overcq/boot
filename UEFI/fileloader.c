/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         boot loader
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 d
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
S
H_efi_Z_api
H_efi_I_main(
  P image_handle
, struct H_efi_Z_system_table *system_table
){  S status = system_table->output->output( system_table->output, L"OUX/C+ boot loader\r\n" );
    if( status < 0 )
        return status;
    N memory_map_n = 0;
    struct H_efi_Z_memory_descriptor *memory_map = 0;
    N map_key, descriptor_size;
    N32 descriptor_version;
    status = system_table->boot_services->R_memory_map( &memory_map_n, memory_map, &map_key, &descriptor_size, &descriptor_version );
    if( status < 0 )
        return status;
    struct H_efi_Z_input_key key;
    while(( status = system_table->input->read_key_stroke( system_table->input, &key )) == H_efi_S_error(6) );
    return status;
}
/******************************************************************************/
