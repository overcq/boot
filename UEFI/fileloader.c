/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         main
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 d
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
#define E_cpu_Z_page_entry_S_present    ( 1ULL << 0 )
#define E_cpu_Z_page_entry_S_write      ( 1ULL << 1 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_code       ( 1ULL << ( 32 + 11 ))
#define E_cpu_Z_gdt_S_code_data         ( 1ULL << ( 32 + 12 ))
#define E_cpu_Z_gdt_S_present           ( 1ULL << ( 32 + 15 ))
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1ULL << ( 32 + 21 ))
#define E_cpu_Z_gdt_S_granularity       ( 1ULL << ( 32 + 23 ))
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1ULL << ( 32 + 9 ))
//==============================================================================
struct __attribute__ (( __packed__ )) Q_elf_Z_rela_entry
{ N64 offset;
  N32 type, sym;
  N64 addend;
};
struct __attribute__ (( __packed__ )) Q_exe_Z_rela_plt_entry
{ N64 offset;
  N32 sym;
};
struct __attribute__ (( __packed__ )) Q_exe_Z_export_entry
{ N32 sym;
  N64 offset;
};
struct E_main_Z_kernel_data
{ struct Q_elf_Z_rela_entry *rela;
  struct Q_exe_Z_rela_plt_entry *rela_plt;
  struct Q_exe_Z_export_entry *exports;
  Pc dynstr;
  Pc got;
  Pc text;
  Pc data;
  P entry;
};
//==============================================================================
struct E_main_Z_kernel_args E_main_S_kernel_args;
struct H_uefi_Z_system_table *E_main_S_system_table;
struct H_uefi_Z_memory_descriptor *E_main_S_memory_map;
N E_main_S_descriptor_l;
N E_main_S_loader_stack;
N64 gdt[5], ldt[2], idt[2];
//==============================================================================
__attribute__ (( __warn_unused_result__ ))
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
B
E_main_I_acpi_I_checksum( P table
, N size
){  Pc table_ = table;
    C checksum = 0;
    for_n( i, size )
        checksum += table_[i];
    return !checksum;
}
__attribute__ (( __warn_unused_result__ ))
S
E_main_I_acpi( struct H_uefi_Z_system_table *system_table
){  _0( &E_main_S_kernel_args.acpi, sizeof( E_main_S_kernel_args.acpi ));
    struct H_uefi_Z_guid guid_;
#define Z_guid_T_eq( variable, guid ) guid_ = ( struct H_uefi_Z_guid )guid; if( E_mem_Q_blk_T_eq( &variable, &guid_, sizeof(variable) ))
    for_n( i, system_table->configuration_table_n )
    {   Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_acpi_table )
        {   struct H_acpi_Z_rsdp *rsdp = system_table->configuration_table[i].vendor_table;
            if( !E_mem_Q_blk_T_eq( &rsdp->signature[0], "RSD PTR ", sizeof( rsdp->signature ))
            || rsdp->revision < 2
            || !E_main_I_acpi_I_checksum( rsdp, (Pc)&rsdp->length - (Pc)rsdp )
            || rsdp->length < sizeof( *rsdp )
            || !E_main_I_acpi_I_checksum( rsdp, sizeof( *rsdp ))
            )
                return ~0;
            struct H_acpi_Z_xsdt *xsdt = (P)rsdp->XSDT_address;
            if( xsdt->header.length < sizeof( xsdt->header ) + sizeof( xsdt->table_address[0] )
            || !E_main_I_acpi_I_checksum( xsdt, xsdt->header.length )
            || !E_mem_Q_blk_T_eq( &xsdt->header.signature[0], "XSDT", sizeof( xsdt->header.signature ))
            )
                return ~0;
            N table_n = ( xsdt->header.length - sizeof( xsdt->header )) / sizeof( xsdt->table_address[0] );
            for_n( table_i, table_n )
            {   struct H_acpi_Z_table_header *header = (P)xsdt->table_address[ table_i ];
                if( header->length <= sizeof(header)
                || !E_main_I_acpi_I_checksum( header, header->length )
                )
                    return ~0;
                C16 s[7];
                for_n( i, 4 )
                    s[i] = header->signature[i];
                s[4] = ',';
                s[5] = ' ';
                s[6] = '\0';
                S status = system_table->output->output( system_table->output, &s[0] );
                if( status < 0 )
                    return status;
                if( E_mem_Q_blk_T_eq( &header->signature[0], "APIC", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_apic *apic = (P)header;
                    E_main_S_kernel_args.acpi.apic_content = (Pc)apic + sizeof( *apic );
                    E_main_S_kernel_args.acpi.apic_content_l = apic->header.length - sizeof( *apic );
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "DMAR", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_dmar *dmar = (P)header;
                    E_main_S_kernel_args.acpi.dmar_content = (Pc)dmar + sizeof( *dmar );
                    E_main_S_kernel_args.acpi.dmar_content_l = dmar->header.length - sizeof( *dmar );
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "FACP", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_facs *facs;
                    if( header->revision == 6 )
                    {   struct H_acpi_Z_fadt *fadt = (P)header;
                        if( header->length != sizeof( *fadt )
                        || fadt->table_minor_version != 5
                        || !fadt->ex_dsdt
                        )
                            return ~0;
                        header = (P)fadt->ex_dsdt;
                        facs = (P)fadt->ex_facs;
                    }else if( header->revision == 3
                    || header->revision == 4
                    )
                    {   struct H_acpi_Z_fadt_v3 *fadt = (P)header;
                        if( header->length != sizeof( *fadt )
                        || !fadt->ex_dsdt
                        )
                            return ~0;
                        header = (P)fadt->ex_dsdt;
                        facs = (P)fadt->ex_facs;
                    }else
                        return ~0;
                    if( header->length <= sizeof( *header )
                    || !E_main_I_acpi_I_checksum( header, header->length )
                    || !E_mem_Q_blk_T_eq( &header->signature[0], "DSDT", sizeof( xsdt->header.signature ))
                    )
                        return ~0;
                    E_main_S_kernel_args.acpi.dsdt_content = (Pc)header + sizeof( *header );
                    E_main_S_kernel_args.acpi.dsdt_content_l = header->length - sizeof( *header );
                    if( facs
                    && ( facs->length != sizeof( *facs )
                      || facs->version > 3
                    ))
                        return ~0;
                    E_main_S_kernel_args.acpi.facs = facs;
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "HPET", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_hpet *hpet = (P)header;
                    if( header->length != sizeof( *hpet ))
                        return ~0;
                    E_main_S_kernel_args.acpi.hpet.comparator_count = hpet->comparator_count;
                    E_main_S_kernel_args.acpi.hpet.counter_size = hpet->counter_size;
                    E_main_S_kernel_args.acpi.hpet.legacy_replacement = hpet->legacy_replacement;
                    E_main_S_kernel_args.acpi.hpet.address = hpet->address;
                    E_main_S_kernel_args.acpi.hpet.hpet_number = hpet->hpet_number;
                    E_main_S_kernel_args.acpi.hpet.minimum_tick = hpet->minimum_tick;
                    E_main_S_kernel_args.acpi.hpet.page_protection = hpet->page_protection;
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "MCFG", sizeof( xsdt->header.signature )))
                {   E_main_S_kernel_args.acpi.mcfg_content = (P)( (Pc)header + sizeof( *header ));
                    E_main_S_kernel_args.acpi.mcfg_content_n = ( header->length - sizeof( *header )) / sizeof( struct H_acpi_Z_mcfg_entry );
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "SSDT", sizeof( xsdt->header.signature )))
                {   if( E_main_S_kernel_args.acpi.ssdt_contents_n == J_a_R_n( E_main_S_kernel_args.acpi.ssdt_contents ))
                        return ~0;
                    E_main_S_kernel_args.acpi.ssdt_contents[ E_main_S_kernel_args.acpi.ssdt_contents_n ].address = (Pc)header + sizeof( *header );
                    E_main_S_kernel_args.acpi.ssdt_contents[ E_main_S_kernel_args.acpi.ssdt_contents_n ].l = header->length - sizeof( *header );
                    E_main_S_kernel_args.acpi.ssdt_contents_n++;
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WAET", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_waet *waet = (P)header;
                    if( header->length != sizeof( *waet ))
                        return ~0;
                    E_main_S_kernel_args.acpi.virt_guest_rtc_good = waet->flags & ( 1 << 0 );
                    E_main_S_kernel_args.acpi.virt_guest_pm_good = waet->flags & ( 1 << 1 );
                }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WSMT", sizeof( xsdt->header.signature )))
                {    struct H_acpi_Z_wsmt *wsmt = (P)header;
                    if( header->length != sizeof( *wsmt ))
                        return ~0;
                    E_main_S_kernel_args.acpi.smm_validate_fixed_comm_buffers = wsmt->flags & ( 1 << 0 );
                    E_main_S_kernel_args.acpi.smm_validate_nested_ptr = wsmt->flags & ( 1 << 1 );
                    E_main_S_kernel_args.acpi.smm_system_resource_protection = wsmt->flags & ( 1 << 2 );
                }
            }
            break;
        }
    }
#undef Z_guid_T_eq
    if( !E_main_S_kernel_args.acpi.dsdt_content_l )
        return ~0;
    return 0;
}
void
E_main_I_convert_pointer( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
, P *p
){  for_n( i, memory_map_n )
    {   if( (N)*p >= memory_map->physical_start
        && (N)*p < memory_map->physical_start + memory_map->pages * H_oux_E_mem_S_page_size
        )
        {   *p = (P)(( memory_map->virtual_start + (( (N)*p & ~0xfff ) - memory_map->physical_start )) | ( (N)*p & 0xfff ));
            break;
        }
        memory_map = (P)( (Pc)memory_map + descriptor_l );
    }
}
void
E_main_I_virtual_address_change_I_convert_pointer( struct H_uefi_Z_runtime_services *runtime_services
, struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
, P *p
){  S status = runtime_services->convert_pointer( 0, p );
    if( status < 0 )
        E_main_I_convert_pointer( memory_map, descriptor_l, memory_map_n, p );
}
void
H_uefi_Z_api
E_main_I_virtual_address_change( P event
, P context
){  struct H_uefi_Z_system_table *system_table = E_main_S_system_table;
    struct H_uefi_Z_runtime_services *runtime_services = system_table->runtime_services;
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , &E_main_S_kernel_args.acpi.apic_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , &E_main_S_kernel_args.acpi.dmar_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , &E_main_S_kernel_args.acpi.dsdt_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , &E_main_S_kernel_args.acpi.facs
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , ( P * )&E_main_S_kernel_args.acpi.mcfg_content
    );
    for_n( i, E_main_S_kernel_args.acpi.ssdt_contents_n )
        E_main_I_virtual_address_change_I_convert_pointer( runtime_services
        , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
        , &E_main_S_kernel_args.acpi.ssdt_contents[i].address
        );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , ( P * )&E_main_S_kernel_args.kernel
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , ( P * )&E_main_S_system_table
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , ( P * )&E_main_S_loader_stack
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_kernel_args.memory_map_n
    , ( P * )&E_main_S_memory_map
    );
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
, N *loader_start
, N *loader_end
){  struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code )
        {   *loader_start = memory_map_->physical_start;
            *loader_end = memory_map_->physical_start + memory_map_->pages * H_oux_E_mem_S_page_size;
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
            size += memory_map_->pages * H_oux_E_mem_S_page_size;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_reserved_size_from_start( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N size = 0;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_reserved )
            size += memory_map_->pages * H_oux_E_mem_S_page_size;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_size( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N size = H_oux_E_mem_S_page_size;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   size += memory_map_->pages * H_oux_E_mem_S_page_size;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_saved_n( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N n = 0;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_reserved //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_runtime_services_code //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_runtime_services_data //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_acpi_reclaim //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_acpi_nvs
        || memory_map_->type == H_uefi_Z_memory_Z_memory_mapped_io
        || memory_map_->type == H_uefi_Z_memory_Z_memory_mapped_io_port_space
        || memory_map_->type == H_uefi_Z_memory_Z_pal_code //TODO Czy potrzebne?
        )
            n++;
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    return n;
}
void
E_main_Q_memory_map_I_set_virtual_I_entry_from_end( struct H_uefi_Z_memory_descriptor *memory_map_
, struct H_uefi_Z_memory_descriptor *memory_map_end
, N descriptor_l
, N loader_start
, N loader_end
, B *has_memory_map_new_entry
, N *next_virtual_address
, B *loader_computed
){  *next_virtual_address -= memory_map_->pages * H_oux_E_mem_S_page_size;
    if( !*loader_computed
    && *next_virtual_address <= loader_end
    )
    {   if( *next_virtual_address != loader_end )
        {   *has_memory_map_new_entry = yes;
            struct H_uefi_Z_memory_descriptor *memory_map_new_entry = memory_map_end;
            memory_map_new_entry->type = memory_map_->type;
            memory_map_new_entry->physical_start = memory_map_->physical_start;
            memory_map_new_entry->pages = ( loader_end - *next_virtual_address ) / H_oux_E_mem_S_page_size;
            memory_map_->physical_start += loader_end - *next_virtual_address;
            memory_map_->pages -= memory_map_new_entry->pages;
            memory_map_new_entry->virtual_start = *next_virtual_address -= loader_end - loader_start;
        }else
            *next_virtual_address -= loader_end - loader_start;
        memory_map_->virtual_start = loader_end;
        *loader_computed = yes;
    }else
        memory_map_->virtual_start = *next_virtual_address;
}
void
E_main_Q_memory_map_I_set_virtual_I_entry_from_start( struct H_uefi_Z_memory_descriptor *memory_map_
, struct H_uefi_Z_memory_descriptor *memory_map_end
, N descriptor_l
, N loader_start
, N loader_end
, B *has_memory_map_new_entry
, N *next_virtual_address
, B *loader_computed
){  memory_map_->virtual_start = *next_virtual_address;
    *next_virtual_address += memory_map_->pages * H_oux_E_mem_S_page_size;
    if( !*loader_computed
    && *next_virtual_address >= loader_start
    )
    {   if( *next_virtual_address != loader_start )
        {   *has_memory_map_new_entry = yes;
            struct H_uefi_Z_memory_descriptor *memory_map_new_entry = memory_map_end;
            memory_map_new_entry->type = memory_map_->type;
            memory_map_new_entry->pages = ( *next_virtual_address - loader_start ) / H_oux_E_mem_S_page_size;
            memory_map_->pages -= memory_map_new_entry->pages;
            memory_map_new_entry->physical_start = memory_map_->physical_start + memory_map_->pages * H_oux_E_mem_S_page_size;
            memory_map_new_entry->virtual_start = loader_end;
        }
        *next_virtual_address += loader_end - loader_start;
        *loader_computed = yes;
    }
}
void
E_main_Q_memory_map_I_set_virtual( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
, B reserved_from_end
, N loader_start
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
        {   if( memory_map_->type != H_uefi_Z_memory_Z_reserved
            && memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            && memory_map_->type != (N32)H_uefi_Z_memory_Z_kernel
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == (N32)H_uefi_Z_memory_Z_kernel )
            {   E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
                break;
            }
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
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        //DFN Początkowo stos jest w pamięci typu “H_uefi_Z_memory_Z_boot_services_data”, a “memory_map” 〃 “H_uefi_Z_memory_Z_loader_data”.
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_data
            || memory_map_->type == H_uefi_Z_memory_Z_boot_services_data
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_reserved )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_end( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }else
    {   next_virtual_address = H_oux_E_mem_S_page_size;
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            && memory_map_->type != (N32)H_uefi_Z_memory_Z_kernel
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == (N32)H_uefi_Z_memory_Z_kernel )
            {   E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
                break;
            }
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
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
        //DFN Początkowo stos jest w pamięci typu “H_uefi_Z_memory_Z_boot_services_data”, a “memory_map” 〃 “H_uefi_Z_memory_Z_loader_data”.
        memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_data
            || memory_map_->type == H_uefi_Z_memory_Z_boot_services_data
            )
                E_main_Q_memory_map_I_set_virtual_I_entry_from_start( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , descriptor_l
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }
}
//------------------------------------------------------------------------------
__attribute__ (( __warn_unused_result__ ))
S
E_main_I_allocate_page_table_I_next_page( struct H_uefi_Z_memory_descriptor **entry
, N descriptor_l
, P memory_map_start_end
, B reserved_from_end
, N *pages
){  if( reserved_from_end )
    {   if( !(*pages)-- )
        {   if( *entry == memory_map_start_end )
                return ~0;
            *entry = (P)( (Pc)*entry - descriptor_l );
            if( (*entry)->type == H_uefi_Z_memory_Z_loader_code )
            {   if( *entry == memory_map_start_end )
                    return ~0;
                *entry = (P)( (Pc)*entry - descriptor_l );
            }
            if( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
            && (*entry)->type != H_uefi_Z_memory_Z_conventional
            )
                return ~0;
            *pages = (*entry)->pages - 1;
        }
    }else
        if( ++*pages == (*entry)->pages )
        {   *entry = (P)( (Pc)*entry + descriptor_l );
            if( *entry == memory_map_start_end )
                return ~0;
            if( (*entry)->type == H_uefi_Z_memory_Z_loader_code )
            {   *entry = (P)( (Pc)*entry + descriptor_l );
                if( *entry == memory_map_start_end )
                    return ~0;
            }
            if( (*entry)->type != H_uefi_Z_memory_Z_boot_services_code
            && (*entry)->type != H_uefi_Z_memory_Z_conventional
            )
                return ~0;
            *pages = 0;
        }
    return 0;
}
__attribute__ (( __warn_unused_result__ ))
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
__attribute__ (( __warn_unused_result__ ))
S
E_main_I_allocate_page_table( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_l
, N max_memory
, B reserved_from_end
, N *pml4_
, N *start_end_address
){  P memory_map_end = (Pc)memory_map + memory_map_l;
    struct H_uefi_Z_memory_descriptor *memory_map_entry_conventional;
    P memory_map_start_end;
    N pages;
    //DFN Jest co najmniej jeden blok pamięci z typów: “H_uefi_Z_memory_Z_boot_services_code”, “H_uefi_Z_memory_Z_conventional”.
    if( reserved_from_end )
    {   memory_map_entry_conventional = (P)( (Pc)memory_map_end - descriptor_l );
        while( memory_map_entry_conventional->type != H_uefi_Z_memory_Z_boot_services_code
        && memory_map_entry_conventional->type != H_uefi_Z_memory_Z_conventional
        )
            memory_map_entry_conventional = (P)( (Pc)memory_map_entry_conventional - descriptor_l );
        memory_map_start_end = memory_map;
        pages = memory_map_entry_conventional->pages - 1;
    }else
    {   memory_map_entry_conventional = memory_map;
        while( memory_map_entry_conventional->type != H_uefi_Z_memory_Z_boot_services_code
        && memory_map_entry_conventional->type != H_uefi_Z_memory_Z_conventional
        )
            memory_map_entry_conventional = (P)( (Pc)memory_map_entry_conventional + descriptor_l );
        memory_map_start_end = memory_map_end;
        pages = 0;
    }
    Pn pml4 = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
    N physical_pages = -1;
    B end = no;
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
    {   if( !end )
        {   S status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_start_end, reserved_from_end, &pages );
            if( status < 0 )
                return status;
            Pn pdpt = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
            for_n( pdpt_i, table_n )
            {   if( !end )
                {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_start_end, reserved_from_end, &pages );
                    if( status < 0 )
                        return status;
                    Pn pd = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
                    for_n( pd_i, table_n )
                    {   if( !end )
                        {   status = E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, descriptor_l, memory_map_start_end, reserved_from_end, &pages );
                            if( status < 0 )
                                return status;
                            Pn pt = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i * ( 1ULL << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory - H_oux_E_mem_S_page_size )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   status = E_main_I_allocate_page_table_I_next_physical_address( &memory_map, descriptor_l, memory_map_end, &physical_pages );
                                        if( status < 0 )
                                            return status;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | memory_map->physical_start + physical_pages * H_oux_E_mem_S_page_size;
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
    *start_end_address = memory_map_entry_conventional->virtual_start + ( pages + ( reserved_from_end ? 0 : 1 )) * H_oux_E_mem_S_page_size;
    return 0;
}
//------------------------------------------------------------------------------
__attribute__ (( __warn_unused_result__ ))
S
E_main_Q_loader_I_relocate( N loader_start
, N delta
){  struct E_base_Z_image_dos_header *image_dos_header = (P)loader_start;
    if( image_dos_header->magic != 0x5a4d )
        return ~0;
    struct E_base_Z_image_nt_headers64 *image_nt_headers64 = (P)( (Pc)loader_start + image_dos_header->lfanew );
    if( image_nt_headers64->signature != 0x4550 )
        return ~0;
    struct E_base_Z_image_data_directory *reloc = &image_nt_headers64->optional_header.data_directory[5];
    if( !reloc->virtual_address
    || !reloc->size
    )
        return ~0;
    struct E_base_Z_image_relocation *image_relocation = (P)( loader_start + reloc->virtual_address );
    while( image_relocation->virtual_address
    && image_relocation->size_of_block
    )
    {   for_n( i, ( image_relocation->size_of_block - sizeof( *image_relocation )) / sizeof( image_relocation->entries[0] ))
        {   N offset = image_relocation->entries[i] & 0xfff;
            switch( image_relocation->entries[i] >> 12 )
            { case 0:
                    break;
              case 1:
                {   N16 *fixup = (P)( loader_start + image_relocation->virtual_address + offset );
                    *fixup += ( delta & 0xffff0000 ) >> 16;
                    break;
                }
              case 2:
                {   N16 *fixup = (P)( loader_start + image_relocation->virtual_address + offset );
                    *fixup += delta & 0xffff;
                    break;
                }
              case 3:
                {   N32 *fixup = (P)( loader_start + image_relocation->virtual_address + offset );
                    *fixup += delta & 0xffffffff;
                    break;
                }
              case 4:
                {   N16 *fixup = (P)( loader_start + image_relocation->virtual_address + offset );
                    fixup[0] += ( delta & 0xffff0000 ) >> 16;
                    fixup[1] = delta & 0xffff;
                    break;
                }
              case 10:
                {   N64 *fixup = (P)( loader_start + image_relocation->virtual_address + offset );
                    *fixup += delta;
                    break;
                }
              default:
                return ~0;
            }
        }
        image_relocation = (P)( (Pc)image_relocation + image_relocation->size_of_block );
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
    // Włączenie obsługi SSE.
    /*__asm__ volatile (
    "\n"    "mov    %%cr0,%%rax"
    "\n"    "and    $~( 1 << 2 ),%%rax"
    "\n"    "or     $( 1 << 1 ),%%rax"
    "\n"    "mov    %%rax,%%cr0"
    "\n"    "mov    %%cr4,%%rax"
    "\n"    "or     $(( 1 << 9 ) | ( 1 << 10 )),%%rax"
    "\n"    "mov    %%rax,%%cr4"
    :
    :
    : "rax"
    );*/
    /*{
        struct H_uefi_Z_guid guid_;
#define Z_guid_T_eq( variable, guid ) guid_ = ( struct H_uefi_Z_guid )guid; if( E_mem_Q_blk_T_eq( &variable, &guid_, sizeof(variable) ))
        for_n( i, system_table->configuration_table_n )
        {   Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_acpi_table )
            {   status = system_table->output->output( system_table->output, L"ACPI, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_acpi1_table )
            {   status = system_table->output->output( system_table->output, L"ACPI 1, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_sal_system_table )
            {   status = system_table->output->output( system_table->output, L"SAL, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_smbios_table )
            {   status = system_table->output->output( system_table->output, L"SMBIOS, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_smbios3_table )
            {   status = system_table->output->output( system_table->output, L"SMBIOS 3, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_mps_table )
            {   status = system_table->output->output( system_table->output, L"MPS, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_dtb_table )
            {   status = system_table->output->output( system_table->output, L"DTB, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_rt_properties_table )
            {   status = system_table->output->output( system_table->output, L"RT_PROPERTIES, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_memory_attributes_table )
            {   status = system_table->output->output( system_table->output, L"MEMORY_ATTRIBUTES, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_conformance_profiles_table )
            {   status = system_table->output->output( system_table->output, L"CONFORMANCE_PROFILES, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_memory_range_capsule )
            {   status = system_table->output->output( system_table->output, L"MEMORY_RANGE_CAPSULE, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_debug_image_info_table )
            {   status = system_table->output->output( system_table->output, L"DEBUG_IMAGE_INFO, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_system_resource_table )
            {   status = system_table->output->output( system_table->output, L"SYSTEM_RESOURCE, " );
                continue;
            }
            Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_image_security_database )
            {   status = system_table->output->output( system_table->output, L"IMAGE_SECURITY_DATABASE, " );
                continue;
            }
            status = system_table->output->output( system_table->output, L"unknown, " );
        }
#undef Z_guid_T_eq
        struct H_uefi_Z_input_key key;
        while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready ){}
        goto End;
    }*/
    status = E_main_I_acpi( system_table );
    struct H_uefi_Z_input_key key;
    while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready ){}
    if( status < 0 )
        return status;
    N disk_io_handles_n;
    P *disk_io_handles;
    struct H_uefi_Z_guid H_uefi_Z_guid_S_disk_io_S = H_uefi_Z_guid_S_disk_io;
    status = system_table->boot_services->locate_handle_buffer( H_uefi_Z_locate_search_Z_by_protocol, &H_uefi_Z_guid_S_disk_io_S, 0, &disk_io_handles_n, &disk_io_handles );
    if( status < 0 )
        return status;
    struct H_uefi_Z_guid H_uefi_Z_guid_S_block_io_S = H_uefi_Z_guid_S_block_io;
    N kernel_size;
    for_n( disk_io_handles_i, disk_io_handles_n )
    {   struct H_uefi_Z_protocol_Z_block_io *block_io;
        status = system_table->boot_services->open_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_block_io_S, ( P * )&block_io, image_handle, 0, H_uefi_Z_open_protocol_Z_attribute_S_by_handle_protocol );
        if( status < 0 )
            continue;
        N32 media_id = block_io->media->media_id;
        status = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_block_io_S, image_handle, 0 );
        if( status < 0 )
            break;
        struct H_uefi_Z_protocol_Z_disk_io *disk_io;
        status = system_table->boot_services->open_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, ( P * )&disk_io, image_handle, 0, H_uefi_Z_open_protocol_Z_attribute_S_by_handle_protocol );
        if( status < 0 )
            continue;
        status = H_oux_E_fs_Q_disk_M( system_table, disk_io, media_id );
        if( status < 0 )
        {   S status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            if( status_ < 0 )
            {   status = status_;
                break;
            }
            if( !~status // Brak lub błąd systemu plików.
            || status == H_uefi_Z_error_S_no_media
            || status == H_uefi_Z_error_S_media_changed
            )
                continue;
            break;
        }
        kernel_size = H_oux_E_fs_Q_kernel_R_size( system_table, disk_io, media_id );
        if( kernel_size < H_oux_E_mem_S_page_size )
        {   H_oux_E_fs_Q_disk_W( system_table );
            S status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            status = ~0;
            break;
        }
        status = system_table->boot_services->M_pages( H_uefi_Z_allocate_Z_any, H_uefi_Z_memory_Z_kernel, kernel_size / H_oux_E_mem_S_page_size + ( kernel_size % H_oux_E_mem_S_page_size ? 1 : 0 ), ( N64 * )&E_main_S_kernel_args.kernel );
        if( status < 0 )
        {   H_oux_E_fs_Q_disk_W( system_table );
            S status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = H_oux_E_fs_Q_kernel_I_read( system_table, disk_io, media_id, (P)E_main_S_kernel_args.kernel );
        if( status < 0 )
        {   S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, kernel_size / H_oux_E_mem_S_page_size + ( kernel_size % H_oux_E_mem_S_page_size ? 1 : 0 ));
            H_oux_E_fs_Q_disk_W( system_table );
            status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = H_oux_E_fs_Q_disk_W( system_table );
        if( status < 0 )
        {   S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, kernel_size / H_oux_E_mem_S_page_size + ( kernel_size % H_oux_E_mem_S_page_size ? 1 : 0 ));
            status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
        if( status < 0 )
            S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, kernel_size / H_oux_E_mem_S_page_size + ( kernel_size % H_oux_E_mem_S_page_size ? 1 : 0 ));
        break;
    }
    status = system_table->boot_services->W_pool( disk_io_handles );
    if( disk_io_handles_i == disk_io_handles_n )
        return ~0;
    Pn kernel_p = E_main_S_kernel_args.kernel;
    if( !E_mem_Q_blk_T_eq( kernel_p, "OUXEXE", 6 ))
        return ~0;
    kernel_p = (P)( (Pc)kernel_p + 6 );
    struct E_main_Z_kernel_data kernel_data;
    kernel_data.rela_plt = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.exports = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.dynstr = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.got = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.text = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.data = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.entry = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.rela = (P)kernel_p;
    if( (N)kernel_data.rela != (N)kernel_data.rela_plt
    || (N)kernel_data.rela_plt != (N)kernel_data.exports
    || (N)kernel_data.exports > (N)kernel_data.dynstr
    || (N)kernel_data.dynstr > (N)kernel_data.got
    || (N)kernel_data.got > (N)kernel_data.text
    || (N)kernel_data.text > (N)kernel_data.data
    || (N)kernel_data.data > (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size )
    || (N)kernel_data.entry < (N)kernel_data.text
    || (N)kernel_data.entry >= (N)kernel_data.data
    )
        return ~0;
    for_n( i, ( (N)kernel_data.rela_plt - (N)kernel_data.rela ) / sizeof( *kernel_data.rela ))
        if( kernel_data.rela[i].offset < (N)kernel_data.got - (N)E_main_S_kernel_args.kernel
        || kernel_data.rela[i].offset >= kernel_size //NDFN Być może ‘relokacje’ mogą być tylko w GOT.
        )
            return ~0;
    for_n_( i, ( (N)kernel_data.dynstr - (N)kernel_data.exports ) / sizeof( *kernel_data.exports ))
        if( kernel_data.exports[i].offset < (N)kernel_data.text - (N)E_main_S_kernel_args.kernel
        || kernel_data.exports[i].offset >= (N)kernel_data.data - (N)E_main_S_kernel_args.kernel
        )
            return ~0;
    P event;
    status = system_table->boot_services->M_event( 0x60000202, 8, E_main_I_virtual_address_change, 0, &event );
    if( status < 0 )
        return status;
    N memory_map_l = 0;
    N map_key;
    N32 descriptor_version;
    status = system_table->boot_services->R_memory_map( &memory_map_l, 0, &map_key, &E_main_S_descriptor_l, &descriptor_version );
    if( status < 0
    && status != H_uefi_Z_error_S_buffer_too_small
    )
        return status;
    memory_map_l += ( 2 + 1 ) * E_main_S_descriptor_l; // 2 na możliwość wstawienia w następującym “M_pool”, 1 na możliwość podziału wirtualnych adresów przez blok tego programu pozostający w mapowaniu identycznym do fizycznych adresów.
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, memory_map_l, ( P * )&E_main_S_memory_map );
    if( status < 0 )
        return status;
    N E_main_S_descriptor_l;
    status = system_table->boot_services->R_memory_map( &memory_map_l, E_main_S_memory_map, &map_key, &E_main_S_descriptor_l, &descriptor_version );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( E_main_S_memory_map );
        return status;
    }
    N memory_map_n = memory_map_l / E_main_S_descriptor_l;
    E_main_Q_memory_map_I_sort_physical( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N loader_start, loader_end;
    E_main_Q_memory_map_R_loader_location( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, &loader_start, &loader_end );
    N reserved_size = E_main_Q_memory_map_R_reserved_size( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    B reserved_from_end = loader_start < H_oux_E_mem_S_page_size + reserved_size;
    N reserved_size_from_start = E_main_Q_memory_map_R_reserved_size_from_start( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N memory_size = E_main_Q_memory_map_R_size( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    B has_memory_map_new_entry;
    E_main_Q_memory_map_I_set_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, reserved_from_end, loader_start, loader_end, memory_size, &has_memory_map_new_entry );
    if( has_memory_map_new_entry )
    {   memory_map_l += E_main_S_descriptor_l;
        memory_map_n++;
    }
    E_main_Q_memory_map_I_sort_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    /*{   struct H_uefi_Z_memory_descriptor *memory_map_ = E_main_S_memory_map;
        for_n( i, memory_map_n )
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
            memory_map_ = (P)( (Pc)memory_map_ + E_main_S_descriptor_l );
        }
        system_table->output->output( system_table->output, L"has_new_entry=" );
        H_uefi_I_print( system_table, has_memory_map_new_entry, sizeof( has_memory_map_new_entry ), 10 );
        system_table->output->output( system_table->output, L", from_end=" );
        H_uefi_I_print( system_table, reserved_from_end, sizeof( reserved_from_end ), 10 );
        N rsp;
        __asm__ volatile (
        "\n"    "mov    %%rsp,%0"
        : "=g" (rsp)
        );
        system_table->output->output( system_table->output, L", rsp=" );
        H_uefi_I_print( system_table, rsp, sizeof(rsp), 16 );
        memory_map_ = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( rsp >= memory_map_->physical_start
            && rsp < memory_map_->physical_start + memory_map_->pages * H_oux_E_mem_S_page_size
            )
            {   E_main_S_loader_stack = ( memory_map_->virtual_start + (( rsp & ~0xfff ) - memory_map_->physical_start )) | ( rsp & 0xfff );
                break;
            }
            memory_map_ = (P)( (Pc)memory_map_ + E_main_S_descriptor_l );
        }
        system_table->output->output( system_table->output, L", E_main_S_loader_stack=" );
        H_uefi_I_print( system_table, E_main_S_loader_stack, sizeof( E_main_S_loader_stack ), 16 );
        struct H_uefi_Z_input_key key;
        while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready ){}
        goto End;
    }*/
    status = system_table->boot_services->exit_boot_services( image_handle, map_key );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( E_main_S_memory_map );
        return status;
    }
    N pml4, start_end_address;
    status = E_main_I_allocate_page_table( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_l, memory_size, reserved_from_end, &pml4, &start_end_address );
    if( status < 0 )
        goto End;
    E_main_S_kernel_args.memory_map_n = memory_map_n;
    E_main_S_system_table = system_table;
    __asm__ volatile (
    "\n"    "mov    %%rsp,%0"
    : "=m" ( E_main_S_loader_stack )
    );
    status = system_table->runtime_services->P_virtual_address_map( memory_map_l, E_main_S_descriptor_l, descriptor_version, E_main_S_memory_map );
    if( status < 0 )
        goto End;
    __asm__ volatile (
    "\n"    "mov    %0,%%cr3"
    "\n"    "mov    %1,%%rsp"
    :
    : "r" (pml4), "m" ( E_main_S_loader_stack )
    );
    system_table = E_main_S_system_table;
    N page_table_size = reserved_from_end
    ? (N64)E_main_S_kernel_args.kernel - start_end_address
    : start_end_address - ( (N64)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ))
    ;
    E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );
#define E_main_J_code_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_code | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_Z_code_S_64bit | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_data_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_data_S_write | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_local_descriptor_1( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_ldt | E_cpu_Z_gdt_S_present | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
    gdt[1] = E_main_J_code_descriptor( 0, ~0ULL );
    gdt[2] = E_main_J_data_descriptor( 0, ~0ULL );
    gdt[3] = E_main_J_local_descriptor_1( (N)&ldt[0], sizeof(ldt) - 1 );
    gdt[4] = (N)&ldt[0] >> 32;
    ldt[0] = 0;
    ldt[1] = 0;
    idt[0] = 0;
    idt[1] = 0;
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
    "\n"    "mov    $3 << 3,%%ax"
    "\n"    "lldt   %%ax"
    "\n"    "lidt   %1"
    "\n"    "mov    $2 << 3,%%ax"
    "\n"    "mov    %%ax,%%ds"
    "\n"    "mov    %%ax,%%es"
    "\n"    "mov    %%ax,%%ss"
    "\n"    "mov    %%ax,%%fs"
    "\n"    "mov    %%ax,%%gs"
    "\n"    "movw   $1 << 3,-8(%%rsp)"
    "\n"    "movq   $0f,-16(%%rsp)"
    "\n"    ".byte  0x48"
    "\n"    "ljmp   *-16(%%rsp)"
    "\n0:"
    :
    : "p" ( &gd.limit ), "p" ( &id.limit )
    : "ax"
    );
    E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
    E_main_S_kernel_args.memory_map = (P)( reserved_from_end ? (Pc)E_main_S_kernel_args.page_table - memory_map_size : (Pc)E_main_S_kernel_args.page_table + page_table_size );
    N stack_size = H_oux_E_mem_S_page_size;
    E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
    ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
    : memory_size - stack_size
    );
    // Ewentualne przeniesienie programu ‘bootloadera’.
    N loader_start_new = 0;
    struct H_uefi_Z_memory_descriptor *memory_map_ = E_main_S_memory_map;
    if( reserved_from_end )
    {   B loader_inside_free_end = no;
        for_n( i, memory_map_n )
        {   if(( memory_map_->type == H_uefi_Z_memory_Z_conventional
              || memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
              || memory_map_->type == H_uefi_Z_memory_Z_loader_data
              || memory_map_->type == H_uefi_Z_memory_Z_boot_services_data
              || memory_map_->type == H_uefi_Z_memory_Z_reserved
              )
            && memory_map_->virtual_start == loader_end
            )
            {   loader_inside_free_end = yes;
                break;
            }
        }
        N loader_end_max = E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.kernel_stack - ( E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ) + E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated )), H_oux_E_mem_S_page_size );
        if(( loader_inside_free_end
          && loader_end > loader_end_max
          )
        || ( !loader_inside_free_end
          && loader_start < (N)E_main_S_kernel_args.page_table
        ))
        {   memory_map_ = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if(( memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
                  || memory_map_->type == H_uefi_Z_memory_Z_conventional
                  )
                && memory_map_->pages * H_oux_E_mem_S_page_size >= loader_end - loader_start
                )
                {   loader_start_new = memory_map_->virtual_start;
                    break;
                }
                memory_map_ = (P)( (Pc)memory_map_ + E_main_S_descriptor_l );
            }
            if( !loader_start_new
            || loader_start_new + 2 * ( loader_end - loader_start ) > loader_end_max
            )
                goto End;
        }
    }else
    {   B loader_inside_free_start = no;
        for_n( i, memory_map_n )
        {   if(( memory_map_->type == H_uefi_Z_memory_Z_conventional
              || memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
              )
            && memory_map_->virtual_start + memory_map_->pages * H_oux_E_mem_S_page_size == loader_start
            )
                loader_inside_free_start = yes;
            if( loader_inside_free_start )
                break;
        }
        N loader_start_min = E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.memory_map + memory_map_size + E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ) + E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated ), H_oux_E_mem_S_page_size );
        if(( loader_inside_free_start
          && loader_start < loader_start_min
          )
        || ( !loader_inside_free_start
          && loader_end > (N)E_main_S_kernel_args.page_table + page_table_size
        ))
        {   memory_map_ = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if(( memory_map_->type == H_uefi_Z_memory_Z_boot_services_code
                  || memory_map_->type == H_uefi_Z_memory_Z_conventional
                  )
                && memory_map_->pages * H_oux_E_mem_S_page_size >= loader_end - loader_start + stack_size
                )
                    loader_start_new = memory_map_->virtual_start + memory_map_->pages * H_oux_E_mem_S_page_size - ( loader_end - loader_start + stack_size );
                memory_map_ = (P)( (Pc)memory_map_ + E_main_S_descriptor_l );
            }
            if( !loader_start_new
            || loader_start_new < loader_start_min
            )
                goto End;
        }
    }
    if( loader_start_new )
    {   E_mem_Q_blk_I_copy( (P)loader_start_new, (P)loader_start, loader_end - loader_start );
        status = E_main_Q_loader_I_relocate( loader_start_new, loader_start_new - loader_start );
        if( status < 0 )
            goto End;
        loader_end = loader_start_new + ( loader_end - loader_start );
        loader_start = loader_start_new;
        __asm__ volatile (
        "\n"    "lea    0f,%%rax"
        "\n"    "sub    %0,%%rax"
        "\n"    "add    %1,%%rax"
        "\n"    "jmp    *%%rax"
        "\n0:"
        :
        : "g" ( loader_start ), "g" ( loader_start_new )
        : "rax"
        );
    }
    // Przeniesienie stosu.
    E_mem_Q_blk_I_copy( E_main_S_kernel_args.kernel_stack, (P)E_simple_Z_n_I_align_down_to_v2( E_main_S_loader_stack, H_oux_E_mem_S_page_size ), H_oux_E_mem_S_page_size );
    __asm__ volatile (
    "\n"    "mov    %%rsp,%%rax"
    "\n"    "and    $0xfff,%%rax"
    "\n"    "or     %0,%%rax"
    "\n"    "mov    %%rax,%%rsp"
    :
    : "g" ( E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size )
    : "rax"
    );
    // ‘Relokacja’ kernela.
    kernel_p = E_main_S_kernel_args.kernel;
    kernel_p = (P)( (Pc)kernel_p + 6 );
    kernel_data.rela_plt = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.exports = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.dynstr = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.got = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.text = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.data = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.entry = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.rela = (P)kernel_p;
    for_n_( i, ( (N)kernel_data.rela_plt - (N)kernel_data.rela ) / sizeof( *kernel_data.rela ))
        *(Pn)( (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].offset ) += (N)E_main_S_kernel_args.kernel;
    struct H_oux_E_mem_Z_memory_map *my_memory_map = E_main_S_kernel_args.memory_map;
    memory_map_ = E_main_S_memory_map;
    for_n_( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_reserved //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_runtime_services_code //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_runtime_services_data //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_acpi_reclaim //TODO Czy potrzebne?
        || memory_map_->type == H_uefi_Z_memory_Z_acpi_nvs
        || memory_map_->type == H_uefi_Z_memory_Z_memory_mapped_io
        || memory_map_->type == H_uefi_Z_memory_Z_memory_mapped_io_port_space
        || memory_map_->type == H_uefi_Z_memory_Z_pal_code //TODO Czy potrzebne?
        )
        {   my_memory_map->physical_start = memory_map_->physical_start;
            my_memory_map->virtual_start = memory_map_->virtual_start;
            my_memory_map->pages = memory_map_->pages;
        }
        memory_map_ = (P)( (Pc)memory_map_ + E_main_S_descriptor_l );
        my_memory_map++;
    }
    E_mem_M( reserved_from_end, reserved_size_from_start, loader_start, loader_end - loader_start, (N)E_main_S_kernel_args.kernel_stack, stack_size, (N)E_main_S_kernel_args.memory_map, memory_map_size, (N)E_main_S_kernel_args.page_table, page_table_size, (N)E_main_S_kernel_args.kernel, kernel_size, memory_size, reserved_size );
    E_main_S_kernel_args.bootloader = (P)loader_start;
    E_main_S_kernel_args.uefi_runtime_services.R_time = system_table->runtime_services->R_time;
    E_main_S_kernel_args.uefi_runtime_services.P_time = system_table->runtime_services->P_time;
    E_main_S_kernel_args.uefi_runtime_services.R_wakeup_time = system_table->runtime_services->R_wakeup_time;
    E_main_S_kernel_args.uefi_runtime_services.P_wakeup_time = system_table->runtime_services->P_wakeup_time;
    E_main_S_kernel_args.uefi_runtime_services.R_variable = system_table->runtime_services->R_variable;
    E_main_S_kernel_args.uefi_runtime_services.R_next_variable_name = system_table->runtime_services->R_next_variable_name;
    E_main_S_kernel_args.uefi_runtime_services.P_variable = system_table->runtime_services->P_variable;
    E_main_S_kernel_args.uefi_runtime_services.R_next_high_monotonic_count = system_table->runtime_services->R_next_high_monotonic_count;
    E_main_S_kernel_args.uefi_runtime_services.reset_system = system_table->runtime_services->reset_system;
    E_main_S_kernel_args.uefi_runtime_services.update_capsule = system_table->runtime_services->update_capsule;
    E_main_S_kernel_args.uefi_runtime_services.R_capsule_capabilities = system_table->runtime_services->R_capsule_capabilities;
    E_main_S_kernel_args.uefi_runtime_services.R_variable_info = system_table->runtime_services->R_variable_info;
    // Przed wyrzuceniem z pamięci programu ‘bootloadera’ ‘kernel’ potrzebuje przenieść dostarczone dane i GDT, ustawić LDT i IDT.
    __asm__ volatile (
    "\n"    "lea    %1,%%rdi"
    "\n"    "mov    %0,%%rsp"
    "\n"    "jmp    *%2"
    :
    : "g" ( E_main_S_kernel_args.kernel_stack + stack_size ), "p" ( &E_main_S_kernel_args ), "r" ( kernel_data.entry )
    : "rdi"
    );
    __builtin_unreachable();
End:__asm__ volatile (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
    __builtin_unreachable();
}
/******************************************************************************/
