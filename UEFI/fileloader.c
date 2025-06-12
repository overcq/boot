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
#define E_cpu_Z_cr0_S_mp                ( 1ULL << 1 )
#define E_cpu_Z_cr0_S_em                ( 1ULL << 2 )
#define E_cpu_Z_cr0_S_ne                ( 1ULL << 5 )
#define E_cpu_Z_cr0_S_wp                ( 1ULL << 16 )
#define E_cpu_Z_cr0_S_nw                ( 1ULL << 29 )
#define E_cpu_Z_cr0_S_cd                ( 1ULL << 30 )
#define E_cpu_Z_cr3_S_pwt               ( 1ULL << 3 )
#define E_cpu_Z_cr3_S_pcd               ( 1ULL << 4 )
#define E_cpu_Z_cr4_S_vme               ( 1ULL << 0 )
#define E_cpu_Z_cr4_S_pvi               ( 1ULL << 1 )
#define E_cpu_Z_cr4_S_tsd               ( 1ULL << 2 )
#define E_cpu_Z_cr4_S_de                ( 1ULL << 3 )
#define E_cpu_Z_cr4_S_mce               ( 1ULL << 6 )
#define E_cpu_Z_cr4_S_pge               ( 1ULL << 7 )
#define E_cpu_Z_cr4_S_pce               ( 1ULL << 8 )
#define E_cpu_Z_cr4_S_osfxsr            ( 1ULL << 9 )
#define E_cpu_Z_cr4_S_osxmmexcpt        ( 1ULL << 10 )
#define E_cpu_Z_cr4_S_fsgsbase          ( 1ULL << 16 )
#define E_cpu_Z_cr4_S_pcide             ( 1ULL << 17 )
#define E_cpu_Z_cr4_S_osxsave           ( 1ULL << 18 )
#define E_cpu_Z_cr4_S_smep              ( 1ULL << 20 )
#define E_cpu_Z_cr4_S_smap              ( 1ULL << 21 )
#define E_cpu_Z_cr4_S_pke               ( 1ULL << 22 )
#define E_cpu_Z_cr4_S_pks               ( 1ULL << 24 )
#define E_cpu_Z_cr4_S_uintr             ( 1ULL << 25 )
#define E_cpu_Z_cr4_S_lam_sup           ( 1ULL << 28 )
#define E_cpu_Z_page_entry_S_present    ( 1ULL << 0 )
#define E_cpu_Z_page_entry_S_write      ( 1ULL << 1 )
#define E_cpu_Z_page_entry_S_pwt        ( 1ULL << 3 )
#define E_cpu_Z_page_entry_S_pcd        ( 1ULL << 4 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_code       ( 1ULL << ( 32 + 11 ))
#define E_cpu_Z_gdt_S_code_data         ( 1ULL << ( 32 + 12 ))
#define E_cpu_Z_gdt_S_present           ( 1ULL << ( 32 + 15 ))
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1ULL << ( 32 + 21 ))
#define E_cpu_Z_gdt_S_granularity       ( 1ULL << ( 32 + 23 ))
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1ULL << ( 32 + 9 ))
//==============================================================================
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_local_apic
{ N8 type;
  N8 l;
  N8 processor;
  N8 id;
  N32 flags;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_io_apic
{ N8 type;
  N8 l;
  N8 id;
  N8 reserved;
  N32 address;
  N32 gsi_base;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_source_override
{ N8 type;
  N8 l;
  N8 bus;
  N8 source;
  N32 gsi;
  N16 flags;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_local_apic_nmi
{ N8 type;
  N8 l;
  N8 processor;
  N16 flags;
  N8 int_;
};
struct E_main_Z_apic_source_override
{ N8 source;
  N8 gsi;
  N8 flags;
};
//------------------------------------------------------------------------------
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
int _fltused = 0;
struct E_main_Z_kernel_args E_main_S_kernel_args;
struct H_uefi_Z_system_table *E_main_S_system_table;
struct H_uefi_Z_memory_descriptor *E_main_S_memory_map;
N E_main_S_memory_map_n;
N E_main_S_descriptor_l;
N E_main_S_loader_stack;
N64 gdt[5], ldt[2], idt[2];
B E_main_S_pic_mode = no;
P E_main_S_apic_content;
N E_main_S_apic_content_l;
//==============================================================================
void
E_main_I_outb( N16 port
, N8 v
){  __asm__ volatile(
    "\n"    "out    %0,%1"
    :
    : "a" (v), "d" (port)
    );
}
//==============================================================================
__attribute__ (( __warn_unused_result__ ))
S
H_uefi_I_print_n( struct H_uefi_Z_system_table *system_table
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
    E_main_S_kernel_args.io_apic_address = 0;
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
                C16 s[5];
                for_n( i, 4 )
                    s[i] = header->signature[i];
                s[4] = '\0';
                S status = system_table->output->output( system_table->output, &s[0] );
                if( status < 0 )
                    return status;
                status = system_table->output->output( system_table->output, L"(" );
                if( status < 0 )
                    return status;
                status = H_uefi_I_print_n( system_table, header->revision, sizeof( header->revision ), 10 );
                if( status < 0 )
                    return status;
                if( E_mem_Q_blk_T_eq( &header->signature[0], "APIC", sizeof( xsdt->header.signature )))
                {   struct H_acpi_Z_apic *apic = (P)header;
                    E_main_S_apic_content = (Pc)apic + sizeof( *apic );
                    E_main_S_apic_content_l = apic->header.length - sizeof( *apic );
                    E_main_S_kernel_args.local_apic_address = (P)(N)apic->local_interrupt_controler;
                    E_main_S_pic_mode = apic->flags & 1;
                    Pc table = E_main_S_apic_content;
                    N l = E_main_S_apic_content_l;
                    while(l)
                    {   if( l < (N8)table[1] )
                            return ~0;
                        switch( (N8)table[0] )
                        { case 0: // local APIC
                            {   struct H_acpi_Z_madt_Z_local_apic *local_apic = (P)&table[0];
                                if( local_apic->l != sizeof( *local_apic ))
                                    return ~0;
                                if( !(( local_apic->flags & 3 ) == 1
                                  || ( local_apic->flags & 3 ) == 2
                                )
                                || ( local_apic->flags & ~3 )
                                )
                                    return ~0;
                                break;
                            }
                          case 1: // I/O APIC
                            {   if( E_main_S_kernel_args.io_apic_address ) // Obsługiwany tylko jeden kontroler I/O APIC.
                                    return ~0;
                                struct H_acpi_Z_madt_Z_io_apic *io_apic = (P)&table[0];
                                if( io_apic->l != sizeof( *io_apic ))
                                    return ~0;
                                if( io_apic->gsi_base )
                                    return ~0;
                                E_main_S_kernel_args.io_apic_address = (P)(N)io_apic->address;
                                break;
                            }
                          case 2: // source override
                            {   struct H_acpi_Z_madt_Z_source_override *source_override = (P)&table[0];
                                if( source_override->l != sizeof( *source_override ))
                                    return ~0;
                                if( source_override->source > 254 - 32
                                || source_override->gsi > 254 - 32
                                || ( source_override->flags & 3 ) == 2
                                || ( source_override->flags & ( 3 << 2 )) == ( 2 << 2 )
                                )
                                    return ~0;
                                break;
                            }
                          case 4: // local APIC NMI
                            {   struct H_acpi_Z_madt_Z_local_apic_nmi *local_apic_nmi = (P)&table[0];
                                if( local_apic_nmi->l != sizeof( *local_apic_nmi ))
                                    return ~0;
                                break;
                            }
                          default:
                                return ~0;
                        }
                        l -= (N8)table[1];
                        table += (N8)table[1];
                    }
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
                    status = system_table->output->output( system_table->output, L"," );
                    if( status < 0 )
                        return status;
                    for_n( i, 4 )
                        s[i] = header->signature[i];
                    s[4] = '\0';
                    S status = system_table->output->output( system_table->output, &s[0] );
                    if( status < 0 )
                        return status;
                    status = system_table->output->output( system_table->output, L"(" );
                    if( status < 0 )
                        return status;
                    status = H_uefi_I_print_n( system_table, header->revision, sizeof( header->revision ), 10 );
                    if( status < 0 )
                        return status;
                    status = system_table->output->output( system_table->output, L"),FACS(" );
                    if( status < 0 )
                        return status;
                    status = H_uefi_I_print_n( system_table, facs->version, sizeof( facs->version ), 10 );
                    if( status < 0 )
                        return status;
                    status = system_table->output->output( system_table->output, L")" );
                    if( status < 0 )
                        return status;
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
                status = system_table->output->output( system_table->output, L"), " );
                if( status < 0 )
                    return status;
            }
            break;
        }
    }
#undef Z_guid_T_eq
    if( !E_main_S_kernel_args.acpi.dsdt_content_l
    || !E_main_S_kernel_args.io_apic_address
    )
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
        {   *p = (P)( memory_map->virtual_start + ( (N)*p - memory_map->physical_start ));
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
){  struct H_uefi_Z_runtime_services *runtime_services = E_main_S_system_table->runtime_services;
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , ( P * )&E_main_S_kernel_args.framebuffer.p
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_apic_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_kernel_args.acpi.dmar_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_kernel_args.acpi.dsdt_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_kernel_args.acpi.facs
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , ( P * )&E_main_S_kernel_args.acpi.mcfg_content
    );
    for_n( i, E_main_S_kernel_args.acpi.ssdt_contents_n )
        E_main_I_virtual_address_change_I_convert_pointer( runtime_services
        , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
        , &E_main_S_kernel_args.acpi.ssdt_contents[i].address
        );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_kernel_args.io_apic_address
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , &E_main_S_kernel_args.local_apic_address
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , ( P * )&E_main_S_system_table
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
    , ( P * )&E_main_S_loader_stack
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , E_main_S_memory_map, E_main_S_descriptor_l, E_main_S_memory_map_n
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
E_main_Q_memory_map_I_set_virtual_I_entry( struct H_uefi_Z_memory_descriptor *memory_map_
, struct H_uefi_Z_memory_descriptor *memory_map_end
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
, B *has_memory_map_new_entry
){  *has_memory_map_new_entry = no;
    struct H_uefi_Z_memory_descriptor *memory_map_ = memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map_->type == H_uefi_Z_memory_Z_loader_code )
        {   memory_map_->virtual_start = memory_map_->physical_start;
            break;
        }
        memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
    }
    B loader_computed = no;
    N next_virtual_address = H_oux_E_mem_S_page_size;
    if( reserved_from_end )
    {   memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type == H_uefi_Z_memory_Z_reserved
            || memory_map_->type == H_uefi_Z_memory_Z_reserved
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , loader_start, loader_end
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
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
        {   if( memory_map_->type != H_uefi_Z_memory_Z_reserved
            && memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            && memory_map_->type != (N32)H_uefi_Z_memory_Z_kernel
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }else
    {   memory_map_ = memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map_->type != H_uefi_Z_memory_Z_loader_code
            && memory_map_->type != H_uefi_Z_memory_Z_loader_data
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_code
            && memory_map_->type != H_uefi_Z_memory_Z_boot_services_data
            && memory_map_->type != H_uefi_Z_memory_Z_conventional
            && memory_map_->type != (N32)H_uefi_Z_memory_Z_kernel
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
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
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map_
                , (P)( (Pc)memory_map + memory_map_n * descriptor_l )
                , loader_start, loader_end
                , has_memory_map_new_entry
                , &next_virtual_address
                , &loader_computed
                );
            memory_map_ = (P)( (Pc)memory_map_ + descriptor_l );
        }
    }
}
N
E_main_Q_memory_map_I_join( struct H_uefi_Z_memory_descriptor *memory_map
, N descriptor_l
, N memory_map_n
){  N n = 0;
    struct H_uefi_Z_memory_descriptor *memory_map_prev = (P)( (Pc)memory_map + ( memory_map_n - 1 ) * descriptor_l );
    struct H_uefi_Z_memory_descriptor *memory_map_ = (P)( (Pc)memory_map_prev - descriptor_l );
    for_n_rev( i, memory_map_n - 1 )
    {   if( memory_map_->type == memory_map_prev->type
        && memory_map_->virtual_start + memory_map_->pages * H_oux_E_mem_S_page_size == memory_map_prev->virtual_start
        )
        {   n++;
            memory_map_->pages += memory_map_prev->pages;
            E_mem_Q_blk_I_copy( memory_map_prev, (Pc)memory_map_prev + descriptor_l, ( memory_map_n-- - ( 1 + i ) - 1 ) * descriptor_l );
        }
        memory_map_prev = memory_map_;
        memory_map_ = (P)( (Pc)memory_map_ - descriptor_l );
    }
    return n;
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
                                        N physical_address = memory_map->physical_start + physical_pages * H_oux_E_mem_S_page_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                        if(( physical_address >= (N)E_main_S_kernel_args.framebuffer.p
                                          && physical_address < (N)E_main_S_kernel_args.framebuffer.p + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * sizeof( *E_main_S_kernel_args.framebuffer.p )
                                        )
                                        || physical_address == (N)E_main_S_kernel_args.local_apic_address
                                        || physical_address == (N)E_main_S_kernel_args.io_apic_address
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pcd;
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
        return 0;
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
void
E_main_M_madt_I_source_override_sort_gsi( struct E_main_Z_apic_source_override *apic_source_override
, N apic_source_override_n
){  N n = apic_source_override_n;
    while( n > 1 )
    {   struct E_main_Z_apic_source_override *entry_prev = &apic_source_override[0];
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if( entry_prev->gsi > apic_source_override[i].gsi )
            {   J_swap( struct E_main_Z_apic_source_override, *entry_prev, apic_source_override[i] );
                new_n = i;
            }
            entry_prev = &apic_source_override[i];
        }
        n = new_n;
    }
}
void
E_main_M_madt_I_source_override_sort_source( struct E_main_Z_apic_source_override *apic_source_override
, N apic_source_override_n
){  N n = apic_source_override_n;
    while( n > 1 )
    {   struct E_main_Z_apic_source_override *entry_prev = &apic_source_override[0];
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if( entry_prev->source > apic_source_override[i].source )
            {   J_swap( struct E_main_Z_apic_source_override, *entry_prev, apic_source_override[i] );
                new_n = i;
            }
            entry_prev = &apic_source_override[i];
        }
        n = new_n;
    }
}
N32
E_interrupt_Q_io_apic_R( N8 i
){  *( volatile N32 * )E_main_S_kernel_args.io_apic_address = i;
    return *( volatile N32 * )( (Pc)E_main_S_kernel_args.io_apic_address + 0x10 );
}
N
E_main_M_madt( Pc table
, N l
){  N apic_source_override_n = 0;
    struct E_main_Z_apic_source_override *apic_source_override;
    Mt_( apic_source_override, apic_source_override_n );
    if( !apic_source_override )
        return ~0;
    while(l)
    {   switch( (N8)table[0] )
        { case 2: // source override
            {   struct H_acpi_Z_madt_Z_source_override *source_override = (P)&table[0];
                N n_prepended;
                if( !E_mem_Q_blk_I_add( &apic_source_override, 1, &n_prepended, 0 ))
                {   W( apic_source_override );
                    return ~0;
                }
                N i = n_prepended ? 0 : apic_source_override_n;
                apic_source_override[i].source = source_override->source;
                apic_source_override[i].gsi = source_override->gsi;
                apic_source_override[i].flags = source_override->flags & 0xff;
                apic_source_override_n++;
                break;
            }
        }
        l -= (N8)table[1];
        table += (N8)table[1];
    }
    // Sprawdzenie, czy nie ma duplikatów ‘source override’.
    if( apic_source_override_n > 1 )
    {   E_main_M_madt_I_source_override_sort_source( apic_source_override, apic_source_override_n );
        N source = apic_source_override[0].source;
        for_n( i, apic_source_override_n - 1 )
        {   if( apic_source_override[ 1 + i ].source == source )
            {   W( apic_source_override );
                return ~0;
            }
            source = apic_source_override[ 1 + i ].source;
        }
        E_main_M_madt_I_source_override_sort_gsi( apic_source_override, apic_source_override_n );
        N gsi = apic_source_override[0].gsi;
        for_n_( i, apic_source_override_n - 1 )
        {   if( apic_source_override[ 1 + i ].gsi == gsi )
            {   W( apic_source_override );
                return ~0;
            }
            gsi = apic_source_override[ 1 + i ].gsi;
        }
    }
    E_main_S_kernel_args.gsi_n = (( E_interrupt_Q_io_apic_R(1) >> 16 ) & 0xff ) + 1;
    if( E_main_S_kernel_args.gsi_n < 17 )
        return ~0;
    Mt_( E_main_S_kernel_args.gsi, E_main_S_kernel_args.gsi_n );
    if( !E_main_S_kernel_args.gsi )
    {   W( apic_source_override );
        return ~0;
    }
    N apic_source_override_i = 0;
    for_n( i, E_main_S_kernel_args.gsi_n )
        if( apic_source_override_i == apic_source_override_n
        || i != apic_source_override[ apic_source_override_i ].gsi
        )
        {   E_main_S_kernel_args.gsi[i].source = i;
            E_main_S_kernel_args.gsi[i].flags = 0;
        }else
        {   E_main_S_kernel_args.gsi[i].source = apic_source_override[ apic_source_override_i ].source;
            E_main_S_kernel_args.gsi[i].flags = apic_source_override[ apic_source_override_i ].flags;
            E_main_S_kernel_args.gsi[ E_main_S_kernel_args.gsi[i].source ].source = ~0;
            apic_source_override_i++;
        }
    W( apic_source_override );
    return 0;
}
//------------------------------------------------------------------------------
S
H_uefi_Z_api
H_uefi_I_main(
  P image_handle
, struct H_uefi_Z_system_table *system_table
){  S status = system_table->output->output( system_table->output, L"OUX/C+ OS boot loader. ©overcq <overcq@int.pl>. http://github.com/overcq\r\n" );
    if( status < 0 )
        return status;
    struct H_uefi_Z_guid H_uefi_Z_guid_S_graphics_S = H_uefi_Z_guid_S_graphics;
    struct H_uefi_Z_protocol_Z_graphics *graphics;
    status = system_table->boot_services->locate_protocol( &H_uefi_Z_guid_S_graphics_S, 0, ( P * )&graphics );
    if( status < 0 )
        return status;
    N selected_mode;
    struct H_uefi_Z_protocol_Z_graphics_Z_mode_information selected_info;
    selected_info.horizontal_resolution = 0;
    for_n( mode, graphics->mode->max_mode )
    {   N l;
        struct H_uefi_Z_protocol_Z_graphics_Z_mode_information *info;
        status = graphics->query( graphics, mode, &l, &info );
        if( status < 0
        || l < sizeof( *info )
        )
            return status;
        if( info->pixel_format != H_uefi_Z_pixel_format_S_rgb8
        && info->pixel_format != H_uefi_Z_pixel_format_S_bgr8
        && info->pixel_format != H_uefi_Z_pixel_format_S_bitmask
        )
            continue;
        if( info->horizontal_resolution <= 1024 //TEST Tymczasowo, ponieważ w oknie maszyny wirtualnej włączają się paski przewijania.
        && selected_info.horizontal_resolution <= info->horizontal_resolution
        )
        {   selected_info = *info;
            selected_mode = mode;
        }
    }
    if( !selected_info.horizontal_resolution )
        return ~0;
    status = graphics->P_mode( graphics, selected_mode );
    if( status < 0 )
        return status;
    E_main_S_kernel_args.framebuffer.p = graphics->mode->framebuffer;
    E_main_S_kernel_args.framebuffer.width = selected_info.horizontal_resolution;
    E_main_S_kernel_args.framebuffer.height = selected_info.vertical_resolution;
    E_main_S_kernel_args.framebuffer.pixels_per_scan_line = selected_info.pixels_per_scan_line;
    E_main_S_kernel_args.framebuffer.pixel_format = selected_info.pixel_format;
    switch( selected_info.pixel_format )
    { case H_uefi_Z_pixel_format_S_rgb8:
            E_main_S_kernel_args.framebuffer.pixel_shifts.red = 0;
            E_main_S_kernel_args.framebuffer.pixel_shifts.green = 8;
            E_main_S_kernel_args.framebuffer.pixel_shifts.blue = 16;
            break;
      case H_uefi_Z_pixel_format_S_bgr8:
            E_main_S_kernel_args.framebuffer.pixel_shifts.red = 16;
            E_main_S_kernel_args.framebuffer.pixel_shifts.green = 8;
            E_main_S_kernel_args.framebuffer.pixel_shifts.blue = 0;
            break;
      case H_uefi_Z_pixel_format_S_bitmask:
            E_main_S_kernel_args.framebuffer.pixel_shifts.red = E_asm_I_bsf( selected_info.pixel_bitmask.red );
            E_main_S_kernel_args.framebuffer.pixel_shifts.green = E_asm_I_bsf( selected_info.pixel_bitmask.green );
            E_main_S_kernel_args.framebuffer.pixel_shifts.blue = E_asm_I_bsf( selected_info.pixel_bitmask.blue );
            break;
    }
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
        kernel_size = H_oux_E_fs_Q_kernel_R_size();
        if( kernel_size < H_oux_E_mem_S_page_size )
        {   H_oux_E_fs_Q_disk_W( system_table );
            S status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            status = ~0;
            break;
        }
        status = system_table->boot_services->M_pages( H_uefi_Z_allocate_Z_any, H_uefi_Z_memory_Z_kernel
        , E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size
        , ( N64 * )&E_main_S_kernel_args.kernel
        );
        if( status < 0 )
        {   H_oux_E_fs_Q_disk_W( system_table );
            S status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = H_oux_E_fs_Q_kernel_I_read( disk_io, media_id, (P)E_main_S_kernel_args.kernel );
        if( status < 0 )
        {   S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size );
            H_oux_E_fs_Q_disk_W( system_table );
            status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = H_oux_E_fs_Q_disk_W( system_table );
        if( status < 0 )
        {   S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size );
            status_ = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
            break;
        }
        status = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, image_handle, 0 );
        if( status < 0 )
            S status_ = system_table->boot_services->W_pages( (N64)E_main_S_kernel_args.kernel, E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size );
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
    if( (N)kernel_data.rela > (N)kernel_data.rela_plt
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
    memory_map_l += ( 2 + 1 + 1 + 1 + 1 + 3 ) * E_main_S_descriptor_l;
    /* 2 na możliwość wstawienia w następującym “M_pool”
     * 1 na dopisanie bloku ‘framebuffer’
     * 1 na dopisanie bloku “local_apic_address”
     * 1 na dopisanie bloku “io_apic_address”
     * 1 na możliwość podziału wirtualnych adresów przez blok tego programu pozostający w mapowaniu identycznym do fizycznych adresów
     * 3 na możliwość przenoszenia ‘bootloadera’
     */
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, memory_map_l, ( P * )&E_main_S_memory_map );
    if( status < 0 )
        return status;
    status = system_table->boot_services->R_memory_map( &memory_map_l, E_main_S_memory_map, &map_key, &E_main_S_descriptor_l, &descriptor_version );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( E_main_S_memory_map );
        return status;
    }
    struct H_uefi_Z_memory_descriptor *memory_map = (P)( (Pc)E_main_S_memory_map + memory_map_l );
    memory_map->type = H_uefi_Z_memory_Z_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.framebuffer.p;
    memory_map->pages = graphics->mode->framebuffer_l / H_oux_E_mem_S_page_size + ( graphics->mode->framebuffer_l % H_oux_E_mem_S_page_size ? 1 : 0 );
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_Z_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.local_apic_address;
    memory_map->pages = 1;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_Z_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.io_apic_address;
    memory_map->pages = 1;
    memory_map_l += E_main_S_descriptor_l;
    N memory_map_n = memory_map_l / E_main_S_descriptor_l;
    E_main_Q_memory_map_I_sort_physical( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N loader_start, loader_end;
    E_main_Q_memory_map_R_loader_location( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, &loader_start, &loader_end );
    N reserved_size = E_main_Q_memory_map_R_reserved_size( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    B reserved_from_end = loader_start < H_oux_E_mem_S_page_size + reserved_size;
//reserved_from_end = yes; //TEST
    N reserved_size_from_start;
    if( reserved_from_end )
        reserved_size_from_start = E_main_Q_memory_map_R_reserved_size_from_start( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N memory_size = E_main_Q_memory_map_R_size( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    B has_memory_map_new_entry;
    E_main_Q_memory_map_I_set_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, reserved_from_end, loader_start, loader_end, &has_memory_map_new_entry );
    if( has_memory_map_new_entry )
    {   memory_map_l += E_main_S_descriptor_l;
        memory_map_n++;
    }
    E_main_Q_memory_map_I_sort_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    /*{   status = system_table->output->output( system_table->output, L"has_new_entry=" );
        status = H_uefi_I_print_n( system_table, has_memory_map_new_entry, sizeof( has_memory_map_new_entry ), 10 );
        status = system_table->output->output( system_table->output, L", from_end=" );
        status = H_uefi_I_print_n( system_table, reserved_from_end, sizeof( reserved_from_end ), 10 );
        N pml4, start_end_address;
        status = E_main_I_allocate_page_table( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_l, memory_size, reserved_from_end, &pml4, &start_end_address );
        if( status < 0 )
            goto End;
        E_main_I_convert_pointer( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, &E_main_S_kernel_args.kernel );
        N page_table_size = reserved_from_end
        ? (N)E_main_S_kernel_args.kernel - start_end_address
        : start_end_address - ( (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ))
        ;
        E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );
        E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
        N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
        E_main_S_kernel_args.memory_map = (P)( reserved_from_end
        ? (Pc)E_main_S_kernel_args.page_table - memory_map_size
        : (Pc)E_main_S_kernel_args.page_table + page_table_size
        );
        N stack_size = H_oux_E_mem_S_page_size; //CONF
        E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
        ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
        : memory_size - stack_size
        );
        N loader_start_min = E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.memory_map + memory_map_size + E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ) + E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated ), H_oux_E_mem_S_page_size );
        status = system_table->output->output( system_table->output, L", kernel=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.kernel, sizeof( (N)E_main_S_kernel_args.kernel ), 16 );
        status = system_table->output->output( system_table->output, L", page_table=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.page_table, sizeof( (N)E_main_S_kernel_args.page_table ), 16 );
        status = system_table->output->output( system_table->output, L", start_end_address=" );
        status = H_uefi_I_print_n( system_table, start_end_address, sizeof( start_end_address ), 16 );
        status = system_table->output->output( system_table->output, L", page_table_size=" );
        status = H_uefi_I_print_n( system_table, page_table_size, sizeof( page_table_size ), 16 );
        status = system_table->output->output( system_table->output, L", memory_map=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.memory_map, sizeof( (N)E_main_S_kernel_args.memory_map ), 16 );
        status = system_table->output->output( system_table->output, L", framebuffer=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.framebuffer.p, sizeof( (N)E_main_S_kernel_args.framebuffer.p ), 16 );
        status = system_table->output->output( system_table->output, L", local_apic=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.local_apic_address, sizeof( (N)E_main_S_kernel_args.local_apic_address ), 16 );
        status = system_table->output->output( system_table->output, L", io_apic=" );
        status = H_uefi_I_print_n( system_table, (N)E_main_S_kernel_args.io_apic_address, sizeof( (N)E_main_S_kernel_args.io_apic_address ), 16 );
        struct H_uefi_Z_memory_descriptor *memory_map = E_main_S_memory_map;
        for_n( i, memory_map_n )
        {   if( i % 40 == 0 )
            {   struct H_uefi_Z_input_key key;
                while( system_table->input->read_key_stroke( system_table->input, &key ) == H_uefi_Z_error_S_not_ready ){}
            }
            status = system_table->output->output( system_table->output, ( i % 2 == 0 ? L"\r\n" : L";   " ));
            status = H_uefi_I_print_n( system_table, i, sizeof(i), 10 );
            status = system_table->output->output( system_table->output, L". " );
            status = H_uefi_I_print_n( system_table, memory_map->type, sizeof( memory_map->type ), 10 );
            status = system_table->output->output( system_table->output, L", " );
            status = H_uefi_I_print_n( system_table, memory_map->virtual_start, sizeof( memory_map->virtual_start ), 16 );
            status = system_table->output->output( system_table->output, L", " );
            status = H_uefi_I_print_n( system_table, memory_map->physical_start, sizeof( memory_map->physical_start ), 16 );
            status = system_table->output->output( system_table->output, L", " );
            status = H_uefi_I_print_n( system_table, memory_map->pages, sizeof( memory_map->pages ), 10 );
            memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
        }
        goto End;
    }*/
    status = system_table->boot_services->exit_boot_services( image_handle, map_key ); //TODO To może być wykonane wcześniej.
    if( status < 0 )
        goto End;
    __asm__ volatile (
    "\n"    "cli"
    );
    // Wyłączenie PIC.
    if( E_main_S_pic_mode )
    {   E_main_I_outb( 0x21, 0xff );
        E_main_I_outb( 0xa1, 0xff );
    }
    N pml4, start_end_address;
    status = E_main_I_allocate_page_table( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_l, memory_size, reserved_from_end, &pml4, &start_end_address );
    if( status < 0 )
        goto End;
    E_main_I_convert_pointer( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, &E_main_S_kernel_args.kernel );
    N page_table_size = reserved_from_end
    ? (N)E_main_S_kernel_args.kernel - start_end_address
    : start_end_address - ( (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ))
    ;
    E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );
    E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
    E_main_S_kernel_args.memory_map = (P)( reserved_from_end
    ? (Pc)E_main_S_kernel_args.page_table - memory_map_size
    : (Pc)E_main_S_kernel_args.page_table + page_table_size
    );
    N stack_size = H_oux_E_mem_S_page_size; //CONF
    E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
    ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
    : memory_size - stack_size
    );
    // Ewentualne przeniesienie programu ‘bootloadera’.
    N loader_start_new;
    B loader_move = no;
    if( reserved_from_end )
    {   N loader_end_max = (N)E_main_S_kernel_args.kernel_stack - ( E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ) + E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated ));
        // Sprawdzenie, czy ‘bootloader’ przecina pamięć niedozwoloną.
        if( loader_start < H_oux_E_mem_S_page_size + reserved_size_from_start ) // ‘Bootloader’ był na dole w nieodpowiednim miejscu.
        {   memory_map = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                || memory_map->type == H_uefi_Z_memory_Z_conventional
                )
                {   N start = memory_map->virtual_start < loader_end + loader_end - loader_start
                    ? memory_map->virtual_start - ( loader_end + loader_end - loader_start )
                    : 0;
                    if( memory_map->pages * H_oux_E_mem_S_page_size >= start + loader_end - loader_start )
                    {   loader_start_new = memory_map->virtual_start + start;
                        if( memory_map->pages * H_oux_E_mem_S_page_size != start + loader_end - loader_start )
                            if(start)
                            {   memory_map->pages = start / H_oux_E_mem_S_page_size;
                                struct H_uefi_Z_memory_descriptor *memory_map_ = (P)( (Pc)memory_map + memory_map_n * E_main_S_descriptor_l );
                                memory_map_->type = memory_map->type;
                                memory_map_->physical_start = memory_map->physical_start + start + loader_end - loader_start;
                                memory_map_->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( start + loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                                memory_map_l += E_main_S_descriptor_l;
                                memory_map_n++;
                            }else
                            {   memory_map->physical_start += loader_end - loader_start;
                                memory_map->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                            }
                        else
                        {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                            memory_map_l -= E_main_S_descriptor_l;
                            memory_map_n--;
                        }
                        loader_move = yes;
                        break;
                    }
                }
                memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
            }
            if( !loader_move
            || loader_start_new + loader_end - loader_start > loader_end_max
            )
                goto End;
        }else if( loader_end > loader_end_max
        && !( loader_start >= (N)E_main_S_kernel_args.page_table
          && loader_end <= (N)E_main_S_kernel_args.page_table + page_table_size
        )) // ‘Bootloader’ był na górze w nieodpowiednim miejscu.
        {   if( loader_end_max > loader_start )
                loader_end_max = loader_start;
            for_n_( i, memory_map_n )
            {   N end = memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size > loader_end_max
                  ? memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size - loader_end_max
                  : 0
                  ;
                if(( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                  || memory_map->type == H_uefi_Z_memory_Z_conventional
                  )
                && memory_map->pages * H_oux_E_mem_S_page_size >= loader_end - loader_start + end
                )
                {   loader_start_new = memory_map->virtual_start;
                    if( memory_map->pages * H_oux_E_mem_S_page_size != loader_end - loader_start + end )
                    {   memory_map->physical_start += loader_end - loader_start;
                        memory_map->pages -= ( loader_end - loader_start ) / H_oux_E_mem_S_page_size;
                    }else
                    {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                        memory_map_l -= E_main_S_descriptor_l;
                        memory_map_n--;
                    }
                    loader_move = yes;
                    break;
                }
                memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
            }
            if( !loader_move )
                goto End;
        }else //TEST
        {   memory_map = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if(( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                  || memory_map->type == H_uefi_Z_memory_Z_conventional
                  )
                && ( loader_end <= memory_map->virtual_start
                  || loader_start >= memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size
                )
                && memory_map->pages >= ( loader_end - loader_start ) / H_oux_E_mem_S_page_size
                )
                {   loader_start_new = memory_map->virtual_start;
                    if( memory_map->pages -= ( loader_end - loader_start ) / H_oux_E_mem_S_page_size )
                        memory_map->physical_start += loader_end - loader_start;
                    else
                    {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                        memory_map_l -= E_main_S_descriptor_l;
                        memory_map_n--;
                    }
                    loader_move = yes;
                    break;
                }
                memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
            }
            if( !loader_move
            || loader_start_new + loader_end - loader_start > loader_end_max
            )
                goto End;
        }
    }else
    {   N loader_start_min = E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.memory_map + memory_map_size + E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ) + E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated ), H_oux_E_mem_S_page_size );
        // Sprawdzenie, czy ‘bootloader’ przecina pamięć niedozwoloną.
        if( loader_start < loader_start_min )
        {   if( !( loader_start >= (N)E_main_S_kernel_args.page_table
              && loader_end <= (N)E_main_S_kernel_args.page_table + page_table_size
            )) // ‘Bootloader’ był na dole w nieodpowiednim miejscu.
            {   if( loader_start_min < loader_end )
                    loader_start_min = loader_end;
                memory_map = E_main_S_memory_map;
                for_n_( i, memory_map_n )
                {   if( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                    || memory_map->type == H_uefi_Z_memory_Z_conventional
                    )
                    {   N start = loader_start_min + loader_end - loader_start > memory_map->virtual_start
                        ? loader_start_min + loader_end - loader_start - memory_map->virtual_start
                        : 0;
                        if( memory_map->pages * H_oux_E_mem_S_page_size >= start + loader_end - loader_start )
                        {   loader_start_new = memory_map->virtual_start + start;
                            if( memory_map->pages * H_oux_E_mem_S_page_size != start + loader_end - loader_start )
                                if(start)
                                {   memory_map->pages = start / H_oux_E_mem_S_page_size;
                                    struct H_uefi_Z_memory_descriptor *memory_map_ = (P)( (Pc)memory_map + memory_map_n * E_main_S_descriptor_l );
                                    memory_map_->type = memory_map->type;
                                    memory_map_->physical_start = memory_map->physical_start + start + loader_end - loader_start;
                                    memory_map_->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( start + loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                                    memory_map_l += E_main_S_descriptor_l;
                                    memory_map_n++;
                                }else
                                {   memory_map->physical_start += loader_end - loader_start;
                                    memory_map->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                                }
                            else
                            {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                                memory_map_l -= E_main_S_descriptor_l;
                                memory_map_n--;
                            }
                            loader_move = yes;
                            break;
                        }
                    }
                    memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
                }
                if( !loader_move
                || loader_start_new + loader_end - loader_start > (N)E_main_S_kernel_args.kernel_stack
                )
                    goto End;
            }else //TEST
                goto Test;
        }else if( loader_end > (N)E_main_S_kernel_args.kernel_stack ) // ‘Bootloader’ był na górze w nieodpowiednim miejscu.
        {   memory_map = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                || memory_map->type == H_uefi_Z_memory_Z_conventional
                )
                {   N start = loader_start_min > memory_map->virtual_start ? loader_start_min - memory_map->virtual_start : 0;
                    N end = loader_start < (N)E_main_S_kernel_args.kernel_stack
                    ? ( memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size > loader_start
                      ? memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size - loader_start
                      : 0
                      )
                    : ( memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size > (N)E_main_S_kernel_args.kernel_stack
                      ? memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size - (N)E_main_S_kernel_args.kernel_stack
                      : 0
                      );
                    if( memory_map->pages * H_oux_E_mem_S_page_size >= start + loader_end - loader_start + end )
                    {   loader_start_new = memory_map->virtual_start + start;
                        if(start)
                        {   if( memory_map->pages * H_oux_E_mem_S_page_size != start + loader_end - loader_start )
                            {   struct H_uefi_Z_memory_descriptor *memory_map_ = (P)( (Pc)memory_map + memory_map_n * E_main_S_descriptor_l );
                                memory_map_->type = memory_map->type;
                                memory_map_->physical_start = memory_map->physical_start + start + loader_end - loader_start;
                                memory_map_->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( start + loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                                memory_map_l += E_main_S_descriptor_l;
                                memory_map_n++;
                            }
                            memory_map->pages = start / H_oux_E_mem_S_page_size;
                        }else
                            if( memory_map->pages * H_oux_E_mem_S_page_size != start + loader_end - loader_start )
                            {   memory_map->physical_start += start + loader_end - loader_start;
                                memory_map->pages = ( memory_map->pages * H_oux_E_mem_S_page_size - ( start + loader_end - loader_start )) / H_oux_E_mem_S_page_size;
                            }else
                            {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                                memory_map_l -= E_main_S_descriptor_l;
                                memory_map_n--;
                            }
                        loader_move = yes;
                        break;
                    }
                }
                memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
            }
            if( !loader_move )
                goto End;
        }else //TEST
Test:   {   memory_map = E_main_S_memory_map;
            for_n_( i, memory_map_n )
            {   if(( memory_map->type == H_uefi_Z_memory_Z_boot_services_code
                  || memory_map->type == H_uefi_Z_memory_Z_conventional
                  )
                && memory_map->virtual_start >= loader_start_min + loader_end - loader_start
                && ( loader_end + loader_end - loader_start <= memory_map->virtual_start
                  || loader_start >= memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size
                )
                && memory_map->pages >= ( loader_end - loader_start ) / H_oux_E_mem_S_page_size
                )
                {   loader_start_new = memory_map->virtual_start;
                    if( memory_map->pages -= ( loader_end - loader_start ) / H_oux_E_mem_S_page_size )
                        memory_map->physical_start += loader_end - loader_start;
                    else
                    {   E_mem_Q_blk_I_copy( memory_map, (Pc)memory_map + E_main_S_descriptor_l, ( memory_map_n - ( i + 1 )) * E_main_S_descriptor_l );
                        memory_map_l -= E_main_S_descriptor_l;
                        memory_map_n--;
                    }
                    loader_move = yes;
                    break;
                }
                memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
            }
            if( !loader_move )
                goto End;
        }
    }
    N loader_start_old;
    if( loader_move )
    {   loader_start_old = loader_start;
        loader_end = loader_start_new + ( loader_end - loader_start );
        loader_start = loader_start_new;
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_Z_loader_code )
            {   memory_map->type = H_uefi_Z_memory_Z_conventional;
                break;
            }
            memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
        }
        memory_map = (P)( (Pc)E_main_S_memory_map + memory_map_n * E_main_S_descriptor_l );
        memory_map->type = H_uefi_Z_memory_Z_loader_code;
        memory_map->physical_start = loader_start;
        memory_map->pages = ( loader_end - loader_start ) / H_oux_E_mem_S_page_size;
        memory_map_l += E_main_S_descriptor_l;
        memory_map_n++;
        E_main_Q_memory_map_I_sort_physical( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
        E_main_Q_memory_map_I_set_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n, reserved_from_end, loader_start, loader_end, &has_memory_map_new_entry );
        if( has_memory_map_new_entry )
        {   memory_map_l += E_main_S_descriptor_l;
            memory_map_n++;
        }
        E_main_Q_memory_map_I_sort_virtual( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
        status = E_main_I_allocate_page_table( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_l, memory_size, reserved_from_end, &pml4, &start_end_address );
        if( status < 0 )
            goto End;
    }
    E_main_S_memory_map_n = memory_map_n;
    E_main_S_system_table = system_table;
    __asm__ volatile (
    "\n"    "mov    %%rsp,%0"
    : "=g" ( E_main_S_loader_stack )
    );
    status = system_table->runtime_services->P_virtual_address_map( memory_map_l, E_main_S_descriptor_l, descriptor_version, E_main_S_memory_map );
    if( status < 0 )
        goto End;
    if( loader_move )
    {   memory_map_n -= E_main_Q_memory_map_I_join( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
        memory_map_l = memory_map_n * E_main_S_descriptor_l;
        E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );
        E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
        memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
        E_main_S_kernel_args.memory_map = (P)( reserved_from_end
        ? (Pc)E_main_S_kernel_args.page_table - memory_map_size
        : (Pc)E_main_S_kernel_args.page_table + page_table_size
        );
        E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
        ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
        : memory_size - stack_size
        );
        E_mem_Q_blk_I_copy( (P)loader_start, (P)loader_start_old, loader_end - loader_start );
        status = E_main_Q_loader_I_relocate( loader_start, loader_start - loader_start_old );
        if( status < 0 )
            goto End;
        __asm__ volatile (
        "\n"    "lea    0f(%0),%%rax"
        "\n"    "jmp    *%%rax"
        "\n0:"
        :
        : "r" ( loader_start - loader_start_old )
        : "rax"
        );
    }
    __asm__ volatile (
    "\n"    "mov    %0,%%rsp"
    "\n"    "mov    %1,%%cr3"
    :
    : "g" ( E_main_S_loader_stack ), "r" (pml4)
    );
    system_table = E_main_S_system_table;
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
    // Przeniesienie stosu.
    __asm__ volatile (
    "\n"    "mov    %%rsp,%0"
    : "=g" ( E_main_S_loader_stack )
    );
    if((( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff )) != E_main_S_loader_stack )
    {   E_mem_Q_blk_I_copy( (P)(( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff ))
        , (P)E_main_S_loader_stack
        , H_oux_E_mem_S_page_size - ( E_main_S_loader_stack & 0xfff )
        );
        __asm__ volatile (
        "\n"    "lea    %0,%%rsp"
        :
        : "p" (( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff ))
        );
    }
    // ‘Relokacja’ kernela.
    kernel_p = (P)( (Pc)E_main_S_kernel_args.kernel + 6 );
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
        switch( kernel_data.rela[i].type )
        { case 6:
                *(Pn)( (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].offset ) += (N)E_main_S_kernel_args.kernel;
                break;
          case 8:
                *(Pn)( (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].offset ) = (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].addend;
                break;
        }
    E_main_Q_memory_map_I_sort_physical( E_main_S_memory_map, E_main_S_descriptor_l, memory_map_n );
    struct H_oux_E_mem_Z_memory_map *my_memory_map = E_main_S_kernel_args.memory_map;
    memory_map = E_main_S_memory_map;
    for_n_( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_Z_reserved //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_Z_runtime_services_code //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_Z_runtime_services_data //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_Z_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_Z_acpi_nvs
        || memory_map->type == H_uefi_Z_memory_Z_memory_mapped_io
        || memory_map->type == H_uefi_Z_memory_Z_memory_mapped_io_port_space
        || memory_map->type == H_uefi_Z_memory_Z_pal_code //TODO Czy potrzebne?
        )
        {   my_memory_map->physical_start = memory_map->physical_start;
            my_memory_map->virtual_start = memory_map->virtual_start;
            my_memory_map->pages = memory_map->pages;
        }
        memory_map = (P)( (Pc)memory_map + E_main_S_descriptor_l );
        my_memory_map++;
    }
    if( !~E_mem_M( reserved_from_end, reserved_size_from_start, loader_start, loader_end - loader_start, (N)E_main_S_kernel_args.kernel_stack, stack_size, (N)E_main_S_kernel_args.memory_map, memory_map_size, (N)E_main_S_kernel_args.page_table, page_table_size, (N)E_main_S_kernel_args.kernel, kernel_size, memory_size, reserved_size ))
        goto End;
    E_vga_I_fill_rect( 0, 0, E_main_S_kernel_args.framebuffer.width, E_main_S_kernel_args.framebuffer.height, E_vga_R_video_color( E_vga_S_background_color ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 38, E_main_S_kernel_args.framebuffer.height / 2 - 37, 38 + 34, 37 + 36, E_vga_R_video_color( 0x43864f ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    if( !~E_font_M() )
        goto End;
    E_font_I_print( "OUX/C+ OS bootloader. ©overcq <overcq@int.pl>. https://github.com/overcq\n" );
    if( !~E_main_M_madt( E_main_S_apic_content, E_main_S_apic_content_l ))
        goto End;
    E_font_W();
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
    __asm__ volatile (
    "\n"    "mov    %%cr0,%%rax"
    "\n"    "and    %0,%%rax"
    "\n"    "or     %1,%%rax"
    "\n"    "mov    %%rax,%%cr0"
    "\n"    "mov    %%cr3,%%rax"
    "\n"    "and    %2,%%rax"
    "\n"    "mov    %%rax,%%cr3"
    "\n"    "mov    %%cr4,%%rax"
    "\n"    "and    %3,%%rax"
    "\n"    "or     %4,%%rax"
    "\n"    "mov    %%rax,%%cr4"
    "\n"    "mov    %%cr8,%%rax"
    "\n"    "and    $~0xf,%%rax"
    "\n"    "mov    %%rax,%%cr8"
    :
    : "i" ( ~( E_cpu_Z_cr0_S_em | E_cpu_Z_cr0_S_nw | E_cpu_Z_cr0_S_cd ))
    , "i" ( E_cpu_Z_cr0_S_mp | E_cpu_Z_cr0_S_ne | E_cpu_Z_cr0_S_wp )
    , "i" ( ~( E_cpu_Z_cr3_S_pwt | E_cpu_Z_cr3_S_pcd ))
    , "i" ( ~( E_cpu_Z_cr4_S_tsd | E_cpu_Z_cr4_S_pcide | E_cpu_Z_cr4_S_smep | E_cpu_Z_cr4_S_smap | E_cpu_Z_cr4_S_pke | E_cpu_Z_cr4_S_pks | E_cpu_Z_cr4_S_uintr | E_cpu_Z_cr4_S_lam_sup ))
    , "i" ( E_cpu_Z_cr4_S_vme | E_cpu_Z_cr4_S_pvi | E_cpu_Z_cr4_S_de | E_cpu_Z_cr4_S_mce | E_cpu_Z_cr4_S_pge | E_cpu_Z_cr4_S_pce | E_cpu_Z_cr4_S_osfxsr | E_cpu_Z_cr4_S_osxmmexcpt | E_cpu_Z_cr4_S_fsgsbase | E_cpu_Z_cr4_S_osxsave )
    : "rax"
    );
    // Przed wyrzuceniem z pamięci programu ‘bootloadera’ ‘kernel’ potrzebuje przenieść dostarczone dane i GDT, ustawić LDT i IDT.
    __asm__ volatile (
    "\n"    "mov    %0,%%rsp"
    "\n"    "jmp    *%1"
    :
    : "g" ( (N)E_main_S_kernel_args.kernel_stack + stack_size ), "r" ( kernel_data.entry ), "D" ( &E_main_S_kernel_args )
    );
    __builtin_unreachable();
End:__asm__ volatile (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
    __builtin_unreachable();
}
/******************************************************************************/
