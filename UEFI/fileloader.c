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
void
E_main_Q_memory_map_I_sort( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N n
){  while( n > 1 )
    {   struct H_uefi_Z_memory_descriptor *entry_prev = memory_map;
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   struct H_uefi_Z_memory_descriptor *entry = ( struct H_uefi_Z_memory_descriptor * )( (Pc)memory_map + i * descriptor_l );
            if( entry_prev->physical_start > entry->physical_start )
            {   J_swap( struct H_uefi_Z_memory_descriptor, *entry_prev, *entry );
                new_n = i;
            }
            entry_prev = entry;
        }
        n = new_n;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
S
H_uefi_I_print( struct H_uefi_Z_system_table *system_table
, N v
, N v_l
, N base
){  Pc16 s;
    N n = ( base == 2 || base == 16 ? 2 : base == 8 ? 1 : 0 ) + E_text_Z_n_N_s_G( v, v_l, base ) + 1;
    S status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, n * sizeof( C16 ), ( P * )&s );
    if( status < 0 )
        return status;
    s[ n - 1 ] = L'\0';
    E_text_Z_n_N_s( s + n - 1, v, v_l, base );
    if( base == 2
    || base == 8
    || base == 16
    )
    {   if( base == 2
        || base == 16
        )
            s[1] = base == 2 ? L'b' : L'x';
        s[0] = L'0';
    }
    status = system_table->output->output( system_table->output, s );
    if( status < 0 )
        return status;
    status = system_table->boot_services->W_pool(s);
    return status;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
S
H_uefi_Z_api
H_uefi_I_main(
  P image_handle
, struct H_uefi_Z_system_table *system_table
){  S status = system_table->output->output( system_table->output, L"OUX/C+ OS boot loader\r\n" );
    if( status < 0 )
        return status;
    N memory_map_l = 0;
    struct H_uefi_Z_memory_descriptor *memory_map;
    N map_key, descriptor_l;
    N32 descriptor_version;
    status = system_table->boot_services->R_memory_map( &memory_map_l, memory_map, &map_key, &descriptor_l, &descriptor_version );
    if( status < 0
    && status != H_uefi_Z_error_S_buffer_too_small
    )
        return status;
    memory_map_l += ( 2 + 6 * memory_map_l / descriptor_l ) * descriptor_l;
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, memory_map_l, ( P * )&memory_map );
    if( status < 0 )
        return status;
    N memory_map_l_ = memory_map_l;
    status = system_table->boot_services->R_memory_map( &memory_map_l_, memory_map, &map_key, &descriptor_l, &descriptor_version );
    if( status < 0 )
        return status;
    if(yes)
    {   status = system_table->output->output( system_table->output, L"Initial memory map: type, physical, virtual, pages, attribute\r\n" );
        if( status < 0 )
            return status;
        struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
        for_n( i, memory_map_l_ / descriptor_l )
        {   status = H_uefi_I_print( system_table, i, sizeof(i), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L". " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->type, sizeof( memory_map_->type ), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->physical_start, sizeof( memory_map_->physical_start ), 16 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->virtual_start, sizeof( memory_map_->virtual_start ), 16 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->pages, sizeof( memory_map_->pages ), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->attribute, sizeof( memory_map_->attribute ), 2 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L"\r\n" );
            if( status < 0 )
                return status;
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
            if( i % 24 == 0 )
            {   struct H_uefi_Z_input_key key;
                while(( status = system_table->input->read_key_stroke( system_table->input, &key )) == H_uefi_Z_error_S_not_ready );
                if( status < 0 )
                    return status;
            }
        }
        memory_map_l_ = memory_map_l;
        status = system_table->boot_services->R_memory_map( &memory_map_l_, memory_map, &map_key, &descriptor_l, &descriptor_version );
        if( status < 0 )
            return status;
    }
    E_main_Q_memory_map_I_sort( memory_map, descriptor_l, memory_map_l_ / descriptor_l );
    N64 next_virtual_address = 0x1000;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_l_ / descriptor_l )
    {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
        && memory_map_->type != H_uefi_Z_memory_Z_loader_data
        && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
        && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
        && memory_map_->type != H_uefi_Z_memory_Z_conventional
        )
        {   memory_map_->virtual_start = next_virtual_address;
            next_virtual_address += memory_map_->pages * H_uefi_S_page_size;
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    //N64 first_address = next_virtual_address;
    memory_map_ = memory_map;
    for_n_( i, memory_map_l_ / descriptor_l )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code
        || memory_map_->type == H_uefi_Z_memory_Z_loader_data
        || memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
        || memory_map_->type == H_uefi_Z_memory_Z_boot_services_data
        || memory_map_->type == H_uefi_Z_memory_Z_conventional
        )
        {   memory_map_->virtual_start = next_virtual_address;
            next_virtual_address += memory_map_->pages * H_uefi_S_page_size;
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    //N64 last_address = next_virtual_address;
    if(yes)
    {   status = system_table->output->output( system_table->output, L"Virtual memory map: type, physical, virtual, pages, attribute\r\n" );
        if( status < 0 )
            return status;
        struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
        for_n( i, memory_map_l_ / descriptor_l )
        {   status = H_uefi_I_print( system_table, i, sizeof(i), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L". " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->type, sizeof( memory_map_->type ), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->physical_start, sizeof( memory_map_->physical_start ), 16 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->virtual_start, sizeof( memory_map_->virtual_start ), 16 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->pages, sizeof( memory_map_->pages ), 10 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L", " );
            if( status < 0 )
                return status;
            status = H_uefi_I_print( system_table, memory_map_->attribute, sizeof( memory_map_->attribute ), 2 );
            if( status < 0 )
                return status;
            status = system_table->output->output( system_table->output, L"\r\n" );
            if( status < 0 )
                return status;
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
            if( i % 24 == 0 )
            {   struct H_uefi_Z_input_key key;
                while(( status = system_table->input->read_key_stroke( system_table->input, &key )) == H_uefi_Z_error_S_not_ready );
                if( status < 0 )
                    return status;
            }
        }
        memory_map_l_ = memory_map_l;
        status = system_table->boot_services->R_memory_map( &memory_map_l_, memory_map, &map_key, &descriptor_l, &descriptor_version );
        if( status < 0 )
            return status;
        E_main_Q_memory_map_I_sort( memory_map, descriptor_l, memory_map_l_ / descriptor_l );
    }
    status = system_table->boot_services->exit_boot_services( image_handle, map_key );
    if( status < 0 )
        return status;
    status = system_table->runtime_services->P_virtual_address_map( memory_map_l_, descriptor_l, descriptor_version, memory_map );
    if( status < 0 )
        system_table->runtime_services->reset_system( H_uefi_Z_reset_Z_shutdown, status, sizeof( L"virtual_address_map" ), L"virtual_address_map" );
    __asm__ (
        "0: jmp 0b"
    );
    __builtin_unreachable();
}
/******************************************************************************/
