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
#define E_cpu_Z_cr0_S_pg                ( 1ULL << 31 )
#define E_cpu_Z_cr4_S_pae               ( 1ULL << 5 )
#define E_cpu_Z_cr4_S_osfxsr            ( 1ULL << 9 )
#define E_cpu_Z_cr4_S_osxmmexcpt        ( 1ULL << 10 )
#define E_cpu_Z_msr_Z_ia32_efer         0xc0000080
#define E_cpu_Z_msr_Z_ia32_efer_S_lme   ( 1ULL << 8 )
#define E_cpu_Z_page_entry_S_present    ( 1ULL << 0 )
#define E_cpu_Z_page_entry_S_write      ( 1ULL << 1 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_code       ( 1ULL << ( 32 + 11 ))
#define E_cpu_Z_gdt_S_code_data         ( 1ULL << ( 32 + 12 ))
#define E_cpu_Z_gdt_S_present           ( 1ULL << ( 32 + 15 ))
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1ULL << ( 32 + 21 ))
#define E_cpu_Z_gdt_S_db                ( 1ULL << ( 32 + 22 ))
#define E_cpu_Z_gdt_S_granularity       ( 1ULL << ( 32 + 23 ))
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_tss        ( 011ULL << ( 32 + 8 ))
//==============================================================================
N E_main_S_loader_stack;
struct H_uefi_Z_system_table *E_main_S_system_table;
N64 gdt[3], ldt[2], idt[2];
//==============================================================================
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
void
H_uefi_Z_api
E_main_I_virtual_address_change( P event
, P context
){  struct H_uefi_Z_system_table *system_table = E_main_S_system_table;
    struct H_uefi_Z_runtime_services *runtime_services = system_table->runtime_services;
    runtime_services->convert_pointer( 0, ( P * )&E_main_S_loader_stack );
    runtime_services->convert_pointer( 0, ( P * )&E_main_S_system_table );
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_sort_physical( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  while( memory_map_n > 1 )
    {   struct H_uefi_Z_memory_descriptor *entry_prev = memory_map;
        N new_memory_map_n = 0;
        for( N i = 1; i != memory_map_n; i++ )
        {   struct H_uefi_Z_memory_descriptor *entry = ( struct H_uefi_Z_memory_descriptor * )( (Pc)memory_map + i * descriptor_l );
            if( entry_prev->physical_start > entry->physical_start )
            {   J_swap( struct H_uefi_Z_memory_descriptor, *entry_prev, *entry );
                new_memory_map_n = i;
            }
            entry_prev = entry;
        }
        memory_map_n = new_memory_map_n;
    }
}
void
E_main_Q_memory_map_I_sort_virtual( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  while( memory_map_n > 1 )
    {   struct H_uefi_Z_memory_descriptor *entry_prev = memory_map;
        N new_memory_map_n = 0;
        for( N i = 1; i != memory_map_n; i++ )
        {   struct H_uefi_Z_memory_descriptor *entry = ( struct H_uefi_Z_memory_descriptor * )( (Pc)memory_map + i * descriptor_l );
            if( entry_prev->virtual_start > entry->virtual_start )
            {   J_swap( struct H_uefi_Z_memory_descriptor, *entry_prev, *entry );
                new_memory_map_n = i;
            }
            entry_prev = entry;
        }
        memory_map_n = new_memory_map_n;
    }
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_R_loader_location( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
, N *loader_begin
, N *loader_end
){  struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code )
        {   *loader_begin = memory_map_->physical_start;
            *loader_end = memory_map_->physical_start + memory_map_->pages * E_memory_S_page_size;
            break;
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
}
N
E_main_Q_memory_map_R_reserved_size( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N size = 0;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
        && memory_map_->type != H_uefi_Z_memory_Z_loader_data
        && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
        && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
        && memory_map_->type != H_uefi_Z_memory_Z_conventional
        )
            size += memory_map_->pages * E_memory_S_page_size;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_size( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N size = E_memory_S_page_size;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   size += memory_map_->pages * E_memory_S_page_size;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return size;
}
void
E_main_Q_memory_map_I_set_virtual_I_entry_from_end( struct H_uefi_Z_memory_descriptor *memory_map_
, struct H_uefi_Z_memory_descriptor *memory_map_end
, N descriptor_l
, N loader_begin
, N loader_end
, B *has_memory_map_new_entry
, N *next_virtual_address
, B *loader_computed
){  *next_virtual_address -= memory_map_->pages * H_uefi_S_page_size;
    if( !*loader_computed
    && *next_virtual_address <= loader_end
    )
    {   if( *next_virtual_address != loader_end )
        {   *has_memory_map_new_entry = yes;
            struct H_uefi_Z_memory_descriptor *memory_map_new_entry = memory_map_end;
            memory_map_new_entry->type = memory_map_->type;
            memory_map_new_entry->physical_start = memory_map_->physical_start;
            memory_map_new_entry->pages = ( loader_end - *next_virtual_address ) / H_uefi_S_page_size;
            memory_map_->physical_start += loader_end - *next_virtual_address;
            memory_map_->pages -= memory_map_new_entry->pages;
            memory_map_->virtual_start = loader_end;
            memory_map_new_entry->virtual_start = *next_virtual_address -= loader_end - loader_begin;
        }else
            *next_virtual_address -= loader_end - loader_begin;
        *loader_computed = yes;
    }else
        memory_map_->virtual_start = *next_virtual_address;
}
void
E_main_Q_memory_map_I_set_virtual_I_entry_from_start( struct H_uefi_Z_memory_descriptor *memory_map_
, struct H_uefi_Z_memory_descriptor *memory_map_end
, N descriptor_l
, N loader_begin
, N loader_end
, B *has_memory_map_new_entry
, N *next_virtual_address
, B *loader_computed
){  memory_map_->virtual_start = *next_virtual_address;
    *next_virtual_address += memory_map_->pages * H_uefi_S_page_size;
    if( !*loader_computed
    && *next_virtual_address >= loader_begin
    )
    {   if( *next_virtual_address != loader_begin )
        {   *has_memory_map_new_entry = yes;
            struct H_uefi_Z_memory_descriptor *memory_map_new_entry = memory_map_end;
            memory_map_new_entry->type = memory_map_->type;
            memory_map_new_entry->pages = ( *next_virtual_address - loader_begin ) / H_uefi_S_page_size;
            memory_map_->pages -= memory_map_new_entry->pages;
            memory_map_new_entry->physical_start = memory_map_->physical_start + memory_map_->pages * H_uefi_S_page_size;
            memory_map_new_entry->virtual_start = loader_end;
            *next_virtual_address += loader_end - loader_begin;
        }else
            *next_virtual_address += loader_end - loader_begin;
        *loader_computed = yes;
    }
}
void
E_main_Q_memory_map_I_set_virtual( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
, B reserved_from_end
, N loader_begin
, N loader_end
, N memory_size
, B *has_memory_map_new_entry
){  *has_memory_map_new_entry = no;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code )
            memory_map_->virtual_start = memory_map_->physical_start;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    B loader_computed = no;
    N next_virtual_address;
    if( reserved_from_end )
    {   next_virtual_address = memory_size;
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
            || memory_map_->type == H_uefi_Z_memory_Z_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        //DFN Stos jest w pamięci typu “H_uefi_Z_memory_Z_boot_services_data”.
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_boot_services_data )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }else
    {   next_virtual_address = E_memory_S_page_size;
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
            || memory_map_->type == H_uefi_Z_memory_Z_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        //DFN Stos jest w pamięci typu “H_uefi_Z_memory_Z_boot_services_data”.
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_boot_services_data )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_begin, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }
}
//------------------------------------------------------------------------------
S
E_main_I_allocate_page_table_I_next_page( struct H_uefi_Z_memory_descriptor **entry
, N descriptor_l
, P memory_map_end
, B reserved_from_end
, N *pages
){  if( reserved_from_end )
    {   if( !*pages-- )
        {   *entry = (P)( (Pc)*entry + descriptor_l );
            if( *entry == memory_map_end )
                return ~0;
            if( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
            && (*entry)->type != H_uefi_Z_memory_Z_conventional
            )
            {   while( *entry != memory_map_end
                && ( (*entry)->type == H_uefi_Z_memory_Z_loader_code
                  || (*entry)->type == H_uefi_Z_memory_Z_loader_data
                ))
                    *entry = (P)( (Pc)*entry + descriptor_l );
                if( *entry == memory_map_end
                || ( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
                  && (*entry)->type != H_uefi_Z_memory_Z_conventional
                ))
                    return ~0;
            }
            *pages = (*entry)->pages - 1;
        }
    }else
        if( ++*pages == (*entry)->pages )
        {   *entry = (P)( (Pc)*entry + descriptor_l );
            if( *entry == memory_map_end )
                return ~0;
            if( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
            && (*entry)->type != H_uefi_Z_memory_Z_conventional
            )
            {   while( *entry != memory_map_end
                && ( (*entry)->type == H_uefi_Z_memory_Z_loader_code
                  || (*entry)->type == H_uefi_Z_memory_Z_loader_data
                ))
                    *entry = (P)( (Pc)*entry + descriptor_l );
                if( *entry == memory_map_end
                || ( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
                  && (*entry)->type != H_uefi_Z_memory_Z_conventional
                ))
                    return ~0;
            }
            *pages = 0;
        }
    return 0;
}
S
E_main_I_allocate_page_table_I_next_physical_address( struct H_uefi_Z_memory_descriptor **entry
, N descriptor_l
, P memory_map_end
, N *pages
){  if( ++*pages == (*entry)->pages )
    {   *entry = (P)( (Pc)*entry + descriptor_l );
        if( *entry == memory_map_end )
            return ~0;
        *pages = 0;
    }
    return 0;
}
S
E_main_I_allocate_page_table( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_l
, N max_memory
, B reserved_from_end
, N *pml4_
, N *first_last_address
){  P memory_map_end = (Pc)memory_map + memory_map_l;
    struct H_uefi_Z_memory_descriptor *memory_map_entry_conventional = memory_map;
    while( memory_map_entry_conventional->type != H_uefi_Z_memory_Z_boot_services_code
    && memory_map_entry_conventional->type != H_uefi_Z_memory_Z_conventional
    )
        memory_map_entry_conventional = (P)( (Pc)memory_map_entry_conventional + descriptor_l );
    N pages = reserved_from_end ? memory_map_entry_conventional->pages - 1 : 0;
    N physical_pages = -1;
    Pn pml4 = (P)memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size;
    B end = no;
    const N table_n = E_memory_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
    {   if( !end )
        {   S status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, reserved_from_end, &pages );
            if( status < 0 )
                return status;
            Pn pdpt = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
            for_n( pdpt_i, table_n )
            {   if( !end )
                {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, reserved_from_end, &pages );
                    if( status < 0 )
                        return status;
                    Pn pd = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
                    for_n( pd_i, table_n )
                    {   if( !end )
                        {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, reserved_from_end, &pages );
                            if( status < 0 )
                                return status;
                            Pn pt = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i * ( 1ULL << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                                    if( virtual_address == max_memory - E_memory_S_page_size )
                                        end = yes;
                                    if( !pml4_i
                                    && !pdpt_i
                                    && !pd_i
                                    && !pt_i
                                    )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   status = E_main_I_allocate_page_table_I_next_physical_address( &memory_map, descriptor_l, memory_map_end, &physical_pages );
                                        if( status < 0 )
                                            return status;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | memory_map->physical_start + physical_pages * E_memory_S_page_size;
                                    }
                                }else
                                    pt[ pt_i ] = 0;
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pt;
                        }else
                            pd[ pd_i ] = 0;
                    }
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pd;
                }else
                    pdpt[ pdpt_i ] = 0;
            }
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pdpt;
        }else
            pml4[ pml4_i ] = 0;
    }
    *pml4_ = (N)pml4;
    *first_last_address = memory_map_entry_conventional->virtual_start + ( reserved_from_end ? 0 : ( pages + 1 ) * E_memory_S_page_size );
    return 0;
}
//------------------------------------------------------------------------------
S
H_uefi_Z_api
H_uefi_I_main(
  P image_handle
, struct H_uefi_Z_system_table *system_table
){  S status = system_table->output->output( system_table->output, L"OUX/C+ OS boot loader\r\n" );
    if( status < 0 )
        return status;
    P event;
    status = system_table->boot_services->M_event( 0x60000202, 8, E_main_I_virtual_address_change, 0, &event );
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
    memory_map_l += ( 2 + 1 ) * descriptor_l; // 2 na możliwość wstawienia w następującym “M_pool”, 1 na możliwość podziału wirtualnych adresów przez blok tego programu pozostający w mapowaniu identycznym do fizycznych adresów.
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_boot_services_data, memory_map_l, ( P * )&memory_map );
    if( status < 0 )
        return status;
    status = system_table->boot_services->R_memory_map( &memory_map_l, memory_map, &map_key, &descriptor_l, &descriptor_version );
    if( status < 0 )
        return status;
    E_main_Q_memory_map_I_sort_physical( memory_map, descriptor_l, memory_map_l / descriptor_l );
    N loader_begin, loader_end;
    E_main_Q_memory_map_R_loader_location( memory_map, descriptor_l, memory_map_l / descriptor_l, &loader_begin, &loader_end );
    N reserved_size = E_main_Q_memory_map_R_reserved_size( memory_map, descriptor_l, memory_map_l / descriptor_l );
    B reserved_from_end = loader_begin < E_memory_S_page_size + reserved_size;
    //reserved_from_end = yes;
    N memory_size = E_main_Q_memory_map_R_size( memory_map, descriptor_l, memory_map_l / descriptor_l );
    B has_memory_map_new_entry;
    E_main_Q_memory_map_I_set_virtual( memory_map, descriptor_l, memory_map_l / descriptor_l, reserved_from_end, loader_begin, loader_end, memory_size, &has_memory_map_new_entry );
    if( has_memory_map_new_entry )
        memory_map_l += descriptor_l;
    E_main_Q_memory_map_I_sort_virtual( memory_map, descriptor_l, memory_map_l / descriptor_l );
    /*struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_l / descriptor_l )
    {   H_uefi_I_print( system_table, i, sizeof(i), 10 );
        system_table->output->output( system_table->output, L". " );
        H_uefi_I_print( system_table, memory_map_->type, sizeof( memory_map_->type ), 10 );
        system_table->output->output( system_table->output, L", " );
        H_uefi_I_print( system_table, memory_map_->virtual_start, sizeof( memory_map_->virtual_start ), 16 );
        system_table->output->output( system_table->output, L", " );
        H_uefi_I_print( system_table, memory_map_->physical_start, sizeof( memory_map_->physical_start ), 16 );
        system_table->output->output( system_table->output, L", " );
        H_uefi_I_print( system_table, memory_map_->pages, sizeof( memory_map_->pages ), 10 );
        system_table->output->output( system_table->output, L"\r\n" );
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    system_table->output->output( system_table->output, L"has_new_entry=" );
    H_uefi_I_print( system_table, has_memory_map_new_entry, sizeof( has_memory_map_new_entry ), 10 );
    system_table->output->output( system_table->output, L", from_end=" );
    H_uefi_I_print( system_table, reserved_from_end, sizeof( reserved_from_end ), 10 );
    struct H_uefi_Z_input_key key;
    while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready );
    goto End;*/
    status = system_table->boot_services->exit_boot_services( image_handle, map_key );
    if( status < 0 )
        return status;
#define E_main_J_code_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_code | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_Z_code_S_64bit | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_data_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_data_S_write | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
//#define E_main_J_local_descriptor_1( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_ldt | E_cpu_Z_gdt_S_present | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
    gdt[1] = E_main_J_code_descriptor( 0, ~0ULL );
    gdt[2] = E_main_J_data_descriptor( 0, ~0ULL );
    //gdt[3] = E_main_J_local_descriptor_1( (N)&ldt[0], sizeof(ldt) - 1 );
    //gdt[4] = (N)&ldt[0] >> 32;
    //ldt[0] = 0;
    idt[0] = 0;
    struct __attribute__ ((packed))
    { N32 pad_1;
      N16 pad_2;
      N16 limit;
      N base;
    }gd, id;
    gd.base = (N)&gdt[0];
    gd.limit = sizeof(gdt) - 1;
    id.base = (N)&idt[0];
    id.limit = sizeof(idt) - 1;
    __asm__ volatile (
    "\n"    "lgdt   %0"
    //"\n"    "mov    $3 << 3,%%ax"
    //"\n"    "lldt   %%ax"
    "\n"    "lidt   %1"
    "\n"    "mov    $2 << 3,%%ax"
    "\n"    "mov    %%ax,%%ds"
    "\n"    "mov    %%ax,%%es"
    "\n"    "mov    %%ax,%%ss"
    "\n"    "mov    %%ax,%%fs"
    "\n"    "mov    %%ax,%%gs"
    "\n"    "movw   $1 << 3,-8(%%rsp)"
    "\n"    "movq   $0f,-16(%%rsp)"
    //"\n"    ".byte  0x48"
    //"\n"    "ljmp   *-16(%%rsp)"
    "\n0:"
    :
    : "p" ( &gd.limit ), "p" ( &id.limit )
    : "ax"
    );
    N pml4, first_last_address;
    status = E_main_I_allocate_page_table( memory_map, descriptor_l, memory_map_l, memory_size, reserved_from_end, &pml4, &first_last_address );
    if( status < 0 )
        goto End;
    __asm__ volatile (
    "\n"    "mov    %%rsp,%0"
    : "=m" ( E_main_S_loader_stack )
    );
    E_main_S_system_table = system_table;
    status = system_table->runtime_services->P_virtual_address_map( memory_map_l, descriptor_l, descriptor_version, memory_map );
    if( status < 0 )
        goto End;
    __asm__ volatile (
    "\n"    "mov    %0,%%rsp"
    :
    : "m" ( E_main_S_loader_stack )
    );
    system_table = E_main_S_system_table;
    __asm__ volatile (
    "\n"    "mov    %0,%%cr3"
    :
    : "r" (pml4)
    );
    system_table->runtime_services->reset_system( H_uefi_Z_reset_Z_shutdown, status, 0, 0 );
End:
    __asm__ volatile (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
    __builtin_unreachable();
}
/******************************************************************************/
