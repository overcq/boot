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
#define E_cpu_Z_cr0_S_pg                ( 1LL << 31 )
#define E_cpu_Z_cr4_S_pae               ( 1LL << 5 )
#define E_cpu_Z_cr4_S_osfxsr            ( 1LL << 9 )
#define E_cpu_Z_cr4_S_osxmmexcpt        ( 1LL << 10 )
#define E_cpu_Z_msr_Z_ia32_efer         0xc0000080
#define E_cpu_Z_msr_Z_ia32_efer_S_lme   ( 1LL << 8 )
#define E_cpu_Z_page_entry_S_present          ( 1LL << 0 )
#define E_cpu_Z_page_entry_S_write         ( 1LL << 1 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1LL << 41 )
#define E_cpu_Z_gdt_S_code              ( 1LL << 43 )
#define E_cpu_Z_gdt_S_code_data         ( 1LL << 44 )
#define E_cpu_Z_gdt_S_present           ( 1LL << 47 )
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1LL << 53 )
#define E_cpu_Z_gdt_S_db                ( 1LL << 54 )
#define E_cpu_Z_gdt_S_granularity       ( 1LL << 55 )
#define E_cpu_Z_gdt_Z_type_S_local      ( 1LL << 41 )
//==============================================================================
N E_main_S_loader_stack;
struct H_uefi_Z_system_table *E_main_S_system_table;
N64 gdt[7], ldt[2], idt[2];
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
/*N32
I_crc32( P data_
, N l
){  Pc data = data_;
    N32 crc32 = 0;
    Pc end = data + l;
    N pre = (N)data % sizeof( N64 );
    if( pre > l )
        pre = l;
    while(pre)
    {   crc32 = __builtin_ia32_crc32di( crc32, *data );
        data++;
        pre--;
    }
    while( data != end )
    {   crc32 = __builtin_ia32_crc32qi( crc32, *( N64 * )data );
        data += sizeof( N64 );
    }
    while( data != end )
    {   crc32 = __builtin_ia32_crc32di( crc32, *data );
        data++;
    }
    return crc32;
}*/
void
H_uefi_Z_api
E_main_I_virtual_address_change( P event
, P context
){  struct H_uefi_Z_system_table *system_table = E_main_S_system_table;
    struct H_uefi_Z_runtime_services *runtime_services = system_table->runtime_services;
    runtime_services->convert_pointer( 0, ( P * )&E_main_S_loader_stack );
    /*runtime_services->convert_pointer( 0, ( P * )&system_table->firmware_vendor );
    runtime_services->convert_pointer( 0, ( P * )&system_table->runtime_services );
    runtime_services->convert_pointer( 0, ( P * )&system_table->configuration_table );
    system_table->header.crc32 = 0;
    system_table->header.crc32 = I_crc32( (P)system_table, system_table->header.header_l );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_time );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->P_time );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_wakeup_time );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->P_wakeup_time );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_variable );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_next_variable_name );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->P_variable );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_next_high_monotonic_count );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->reset_system );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->update_capsule );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_capsule_capabilities );
    runtime_services->convert_pointer( 0, ( P * )&runtime_services->R_variable_info );
    runtime_services->header.crc32 = 0;
    runtime_services->header.crc32 = I_crc32( runtime_services, runtime_services->header.header_l );*/
    runtime_services->convert_pointer( 0, ( P * )&E_main_S_system_table );
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_sort( struct H_uefi_Z_memory_descriptor *memory_map
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
S
E_main_I_allocate_page_table_I_next_page( struct H_uefi_Z_memory_descriptor **entry
, N descriptor_l
, P memory_map_end
, N *pages
){  if( ++*pages == (*entry)->pages )
    {   *entry = (P)( (Pc)*entry + descriptor_l );
        while( *entry != memory_map_end
        && (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
        && (*entry)->type != H_uefi_Z_memory_Z_conventional
        )
            *entry = (P)( (Pc)*entry + descriptor_l );
        if( *entry == memory_map_end )
            return ~0;
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
){  P memory_map_end = (Pc)memory_map + memory_map_l;
    N pages = 0, physical_pages = -1;
    struct H_uefi_Z_memory_descriptor *memory_map_entry_conventional = memory_map;
    while( memory_map_entry_conventional->type != H_uefi_Z_memory_Z_boot_services_code
    && memory_map_entry_conventional->type != H_uefi_Z_memory_Z_conventional
    )
        memory_map_entry_conventional = (P)( (Pc)memory_map_entry_conventional + descriptor_l );
    Pn pml4 = (P)memory_map_entry_conventional->physical_start;
    B end = no;
    for_n( pml4_i, E_memory_S_page_size / sizeof(N) )
    {   if( !end )
        {   S status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, &pages );
            if( status < 0 )
                return status;
            Pn pdpt = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
            for_n( pdpt_i, E_memory_S_page_size / sizeof(N) )
            {   if( !end )
                {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, &pages );
                    if( status < 0 )
                        return status;
                    Pn pd = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
                    for_n( pd_i, E_memory_S_page_size / sizeof(N) )
                    {   if( !end )
                        {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_end, &pages );
                            if( status < 0 )
                                return status;
                            Pn pt = (P)( memory_map_entry_conventional->physical_start + pages * E_memory_S_page_size );
                            for_n( pt_i, E_memory_S_page_size / sizeof(N) )
                                if( !end )
                                {   if((( pml4_i * ( 1LL << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size )) == max_memory - E_memory_S_page_size )
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
    memory_map_l += 2 * descriptor_l;
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, memory_map_l, ( P * )&memory_map );
    if( status < 0 )
        return status;
    status = system_table->boot_services->R_memory_map( &memory_map_l, memory_map, &map_key, &descriptor_l, &descriptor_version );
    if( status < 0 )
        return status;
    N next_virtual_address = E_memory_S_page_size;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_l / descriptor_l )
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
    B conventional = no;
    register N pml4;
    memory_map_ = memory_map;
    for_n_( i, memory_map_l / descriptor_l )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
        || memory_map_->type == H_uefi_Z_memory_Z_conventional
        )
        {   memory_map_->virtual_start = next_virtual_address;
            next_virtual_address += memory_map_->pages * H_uefi_S_page_size;
            if( !conventional )
            {   conventional = yes;
                pml4 = memory_map_->physical_start;
            }
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    if( !conventional )
        return ~0;
    //DFN Stos jest w pamięci typu “H_uefi_Z_memory_Z_boot_services_data”.
    memory_map_ = memory_map;
    for_n_( i, memory_map_l / descriptor_l )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code
        || memory_map_->type == H_uefi_Z_memory_Z_loader_data
        || memory_map_->type == H_uefi_Z_memory_Z_boot_services_data
        )
        {   memory_map_->virtual_start = next_virtual_address;
            next_virtual_address += memory_map_->pages * H_uefi_S_page_size;
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    N last_address = next_virtual_address;
    E_main_Q_memory_map_I_sort( memory_map, descriptor_l, memory_map_l / descriptor_l );
    /*memory_map_ = memory_map;
    for_n_( i, memory_map_l / descriptor_l )
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
    N16 ds, es, ss, fs, gs;
    __asm__ volatile (
    "\n"    "mov    %%ds,%0"
    "\n"    "mov    %%es,%1"
    "\n"    "mov    %%ss,%2"
    "\n"    "mov    %%fs,%3"
    "\n"    "mov    %%gs,%4"
    : "=g" (ds), "=g" (es), "=g" (ss), "=g" (fs), "=g" (gs)
    );
    H_uefi_I_print( system_table, ds, sizeof(ds), 10 );
    system_table->output->output( system_table->output, L", " );
    H_uefi_I_print( system_table, es, sizeof(es), 10 );
    system_table->output->output( system_table->output, L", " );
    H_uefi_I_print( system_table, ss, sizeof(ss), 10 );
    system_table->output->output( system_table->output, L", " );
    H_uefi_I_print( system_table, fs, sizeof(fs), 10 );
    system_table->output->output( system_table->output, L", " );
    H_uefi_I_print( system_table, gs, sizeof(gs), 10 );
    struct H_uefi_Z_input_key key;
    while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready );
    goto End;*/
    status = system_table->boot_services->exit_boot_services( image_handle, map_key );
    if( status < 0 )
        return status;
    gdt[1] = E_cpu_Z_gdt_S_code | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_Z_code_S_64bit | E_cpu_Z_gdt_S_db;
    //gdt[1] = (( 1 << 16 ) - 1 ) | E_cpu_Z_gdt_S_code | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_Z_code_S_64bit | E_cpu_Z_gdt_S_db | E_cpu_Z_gdt_S_granularity | ((( 1LL << 4 ) - 1 ) << 48 );
    gdt[2] = 0;
    gdt[3] = E_cpu_Z_gdt_Z_data_S_write | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present;
    gdt[4] = 0;
    gdt[5] = (( sizeof(ldt) - 1 ) & (( 1 << 16 ) - 1 )) | (( (N)&ldt[0] & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_local | E_cpu_Z_gdt_S_present | (((( sizeof(ldt) - 1 ) >> 16 ) & (( 1 << 4 ) - 1 )) << 48 ) | ((( (N)&ldt[0] >> 24 ) & (( 1 << 8 ) - 1 )) << 56 );
    gdt[6] = (N)&ldt[0] >> 32;
    ldt[0] = 0;
    idt[0] = 0;
    struct __attribute__ ((packed))
    { N16 limit;
      N base;
    }gd, id;
    gd.limit = sizeof(gdt) - 1;
    gd.base = (N)&gdt[0];
    id.limit = sizeof(idt) - 1;
    id.base = (N)&idt[0];
    __asm__ volatile (
    "\n"    "lgdt   %0"
    "\n"    "mov    $5 << 3,%%ax"
    "\n"    "lldt   %%ax"
    "\n"    "lidt   %1"
    "\n"    "mov    $3 << 3,%%ax"
    "\n"    "mov    %%ax,%%ds"
    "\n"    "mov    %%ax,%%es"
    "\n"    "mov    %%ax,%%ss"
    "\n"    "mov    %%ax,%%fs"
    "\n"    "mov    %%ax,%%gs"
    //"\n"    "movw   $1 << 3,-8(%%rsp)"
    //"\n"    "movq   $0f,-16(%%rsp)"
    //"\n"    "ljmp   *-16(%%rsp)"
    //"\n0:"
    :
    : "p" ( &gd ), "p" ( &id )
    : "ax"
    );
    status = E_main_I_allocate_page_table( memory_map, descriptor_l, memory_map_l, last_address );
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
    gdt[5] = (( sizeof(ldt) - 1 ) & (( 1 << 16 ) - 1 )) | (( (N)&ldt[0] & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_local | E_cpu_Z_gdt_S_present | (((( sizeof(ldt) - 1 ) >> 16 ) & (( 1 << 4 ) - 1 )) << 48 ) | ((( (N)&ldt[0] >> 24 ) & (( 1 << 8 ) - 1 )) << 56 );
    gdt[6] = (N)&ldt[0] >> 32;
    __asm__ volatile (
    "\n"    "mov    %0,%%cr3"
    //"\n"    "mov    %1,%%rsp"
    //"\n"    "lgdt   %2"
    //"\n"    "mov    $5 << 3,%%ax"
    //"\n"    "lldt   %%ax"
    //"\n"    "lidt   %3"
    //"\n"    "mov    $3 << 3,%%ax"
    //"\n"    "mov    %%ax,%%ds"
    //"\n"    "mov    %%ax,%%es"
    //"\n"    "mov    %%ax,%%ss"
    //"\n"    "mov    %%ax,%%fs"
    //"\n"    "mov    %%ax,%%gs"
    //"\n"    "movw   $1 << 3,-8(%%rsp)"
    //"\n"    "movq   $0f,-16(%%rsp)"
    //"\n"    "ljmp   *-16(%%rsp)"
    //"\n0:"
    :
    : "r" (pml4), "m" ( E_main_S_loader_stack ), "p" ( &gd ), "p" ( &id )
    );
    system_table = E_main_S_system_table;
    goto End;
    //system_table->runtime_services->reset_system( H_uefi_Z_reset_Z_shutdown, status, sizeof( L"test" ), L"test" );
End:
    __asm__ volatile (
    "\n0:"  "jmp    0b"
    );
    __builtin_unreachable();
}
/******************************************************************************/
