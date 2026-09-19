/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         main
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 d
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
#define E_cpu_Z_cr0_S_mp                ( 1ULL << 1 )
#define E_cpu_Z_cr0_S_em                ( 1ULL << 2 )
#define E_cpu_Z_cr0_S_ts                ( 1ULL << 3 )
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
#define E_cpu_Z_cr4_S_umip              ( 1ULL << 11 )
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
#define E_cpu_Z_page_entry_S_pat        ( 1ULL << 7 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_code       ( 1ULL << ( 32 + 11 ))
#define E_cpu_Z_gdt_S_code_data         ( 1ULL << ( 32 + 12 ))
#define E_cpu_Z_gdt_S_present           ( 1ULL << ( 32 + 15 ))
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1ULL << ( 32 + 21 ))
#define E_cpu_Z_gdt_S_granularity       ( 1ULL << ( 32 + 23 ))
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1ULL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_tss        ( 9ULL << ( 32 + 8 ))
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
typedef void ( *E_main_Z_remap_jump )( N stack, N pml4, N delta );
//==============================================================================
int _fltused = 0;
struct E_main_Z_kernel_args E_main_S_kernel_args;
struct H_uefi_Z_system_table *E_main_S_system_table;
struct H_uefi_Z_memory_type_descriptor *E_main_S_memory_map;
N E_main_S_memory_map_n;
N E_main_S_descriptor_l;
N E_main_S_loader_stack;
N64 gdt[7], ldt[2], idt[2];
struct __attribute__ (( __packed__ )) E_main_I_tss
{ N32 reserved_0;
  N rsp[3];
  N ist[8];
  N reserved_1;
  N16 reserved_2;
  N16 io_map_base_address;
}E_main_S_tss;
B E_main_S_pic_mode = no;
P E_main_S_apic_content;
N E_main_S_apic_content_l;
N32 E_main_S_sata_ahci_addresses[8];
N64 E_main_S_ethernet_address, E_main_S_ethernet_eeprom_address;
N8 E_main_S_sata_ahci_n;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern P E_remap_jump_I, E_remap_jump_I_end;
extern P E_mp_init_I, E_mp_init_I_end, E_mp_init_I_reloc_1, E_mp_init_I_reloc_2, E_mp_init_I_reloc_3, E_mp_init_I_reloc_4, E_mp_init_I_reloc_5, E_mp_init_I_reloc_6, E_mp_init_I_reloc_7, E_mp_init_I_reloc_8, E_mp_init_S_gdt_32, E_mp_init_S_gdt, E_mp_init_S_gd_32, E_mp_init_S_gd;
//==============================================================================
void
E_main_I_out_8( N16 port
, N8 v
){  __asm__ volatile (
    "\n" "out   %0,%1"
    :
    : "a" (v), "d" (port)
    );
}
N32
E_main_I_in_32( N16 port
){  N32 v;
    __asm__ volatile (
    "\n" "in    %1,%0"
    : "=a" (v)
    : "d" (port)
    );
    return v;
}
void
E_main_I_out_32( N16 port
, N32 v
){  __asm__ volatile (
    "\n" "out   %0,%1"
    :
    : "a" (v), "d" (port)
    );
}
//------------------------------------------------------------------------------
N32
E_interrupt_Q_io_apic_R( N8 i
){  *( volatile N32 * )E_main_S_kernel_args.io_apic_address = i;
    return *( volatile N32 * )(( Pc )E_main_S_kernel_args.io_apic_address + 0x10 );
}
//------------------------------------------------------------------------------
N64
E_main_Q_msr_R( N32 i
){  N32 l, h;
    __asm__ volatile (
    "\n" "rdmsr"
    : "=a" (l), "=d" (h)
    : "c" (i)
    );
    return (( N64 )h << 32 ) | l;
}
void
E_main_Q_msr_P( N32 i
, N64 v
){  __asm__ volatile (
    "\n" "wrmsr"
    :
    : "c" (i), "d" ( v >> 32 ), "a" ( v & 0xffffffff )
    );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_interrupt_I_ipi_init( N32 processor
){  if( E_main_S_kernel_args.x2apic )
        E_main_Q_msr_P( 0x830, (( N64 )processor << 32 ) | ( 5 << 8 ));
    else
    {   *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0x310 ) = processor << 24;
        *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0x300 ) = 5 << 8;
    }
}
//==============================================================================
__attribute__ (( __warn_unused_result__ ))
S
H_uefi_I_print_n( struct H_uefi_Z_system_table *system_table
, N v
, N base
){  Pc16 s;
    N n = ( base == 2 || base == 16 ? 2 : base == 8 ? 1 : 0 ) + E_text_Z_n_N_s_G( v, base ) + 1;
    S status = system_table->boot_services->M_pool( H_uefi_Z_memory_type_S_loader_data, n * sizeof( C16 ), ( P * )&s );
    if( status < 0 )
        return status;
    s[ n - 1 ] = L'\0';
    E_text_Z_n_N_s( s + n - 1, v, base );
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
N8
E_main_I_acpi_I_checksum( P table
, N size
){  Pc table_ = table;
    C checksum = 0;
    for_n( i, size )
        checksum += table_[i];
    return checksum;
}
__attribute__ (( __warn_unused_result__ ))
S
E_main_I_acpi( struct H_uefi_Z_system_table *system_table
){  _0( &E_main_S_kernel_args.acpi, sizeof( E_main_S_kernel_args.acpi ));
    E_main_S_kernel_args.io_apic_address = 0;
    E_main_S_kernel_args.pcie_base_address = 0;
    struct H_uefi_Z_guid guid_;
#define Z_guid_T_eq( variable, guid ) guid_ = ( struct H_uefi_Z_guid )guid; if( E_mem_Q_blk_T_eq( &variable, &guid_, sizeof(variable) ))
    for_n( i, system_table->configuration_table_n )
    {   Z_guid_T_eq( system_table->configuration_table[i].vendor_guid, H_uefi_Z_guid_S_acpi_table )
        {   struct H_acpi_Z_rsdp *rsdp = system_table->configuration_table[i].vendor_table;
            if( !E_mem_Q_blk_T_eq( &rsdp->signature[0], "RSD PTR ", sizeof( rsdp->signature ))
            || E_main_I_acpi_I_checksum( rsdp, ( Pc )&rsdp->length - ( Pc )rsdp )
            || ( rsdp->revision == 2
              && ( rsdp->length != sizeof( *rsdp )
                || E_main_I_acpi_I_checksum( rsdp, rsdp->length )
            )))
                return ~0;
            if( rsdp->revision < 2 )
            {   struct H_acpi_Z_rsdt *rsdt = (P)(N)rsdp->RSDT_address;
                if( !E_mem_Q_blk_T_eq( &rsdt->header.signature[0], "XSDT", sizeof( rsdt->header.signature ))
                || rsdt->header.length < sizeof( rsdt->header ) + sizeof( rsdt->table_address[0] )
                || E_main_I_acpi_I_checksum( rsdt, rsdt->header.length )
                )
                    return ~0;
                N table_n = ( rsdt->header.length - sizeof( rsdt->header )) / sizeof( rsdt->table_address[0] );
                for_n( table_i, table_n )
                {   struct H_acpi_Z_table_header *header = (P)(N)rsdt->table_address[ table_i ];
                    if( header->length <= sizeof( *header )
                    || E_main_I_acpi_I_checksum( header, header->length )
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
                    status = H_uefi_I_print_n( system_table, header->revision, 10 );
                    if( status < 0 )
                        return status;
                    if( E_mem_Q_blk_T_eq( &header->signature[0], "APIC", sizeof( header->signature )))
                    {   struct H_acpi_Z_apic *apic = (P)header;
                        E_main_S_apic_content = ( Pc )apic + sizeof( *apic );
                        E_main_S_apic_content_l = apic->header.length - sizeof( *apic );
                        E_main_S_kernel_args.local_apic_address = (P)(N)apic->local_interrupt_controler;
                        E_main_S_pic_mode = apic->flags & 1;
                        E_main_S_kernel_args.processor_n = 0;
                        Pc table = E_main_S_apic_content;
                        N l = E_main_S_apic_content_l;
                        while(l)
                        {   if( l < ( N8 )table[1] )
                                return ~0;
                            switch(( N8 )table[0] )
                            { case 0: // local APIC
                                {   struct H_acpi_Z_madt_Z_local_apic *local_apic = (P)&table[0];
                                    if( local_apic->l != sizeof( *local_apic ))
                                        return ~0;
                                    if(( local_apic->flags & 3 ) == 1 )
                                        E_main_S_kernel_args.processor_n++;
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
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "DMAR", sizeof( header->signature )))
                    {   struct H_acpi_Z_dmar *dmar = (P)header;
                        E_main_S_kernel_args.acpi.dmar_content = ( Pc )dmar + sizeof( *dmar );
                        E_main_S_kernel_args.acpi.dmar_content_l = dmar->header.length - sizeof( *dmar );
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "FACP", sizeof( header->signature )))
                    {   struct H_acpi_Z_facs *facs;
                        if( header->revision == 1 )
                        {   struct H_acpi_Z_fadt_v1 *fadt = (P)header;
                            if( header->length != sizeof( *fadt )
                            || !fadt->dsdt
                            )
                                return ~0;
                            header = (P)(N)fadt->dsdt;
                            facs = (P)(N)fadt->facs;
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
                        status = H_uefi_I_print_n( system_table, header->revision, 10 );
                        if( status < 0 )
                            return status;
                        if(facs)
                        {   status = system_table->output->output( system_table->output, L"),FACS(" );
                            if( status < 0 )
                                return status;
                            status = H_uefi_I_print_n( system_table, facs->version, 10 );
                            if( status < 0 )
                                return status;
                        }
                        status = system_table->output->output( system_table->output, L")" );
                        if( status < 0 )
                            return status;
                        if( !E_mem_Q_blk_T_eq( &header->signature[0], "DSDT", sizeof( header->signature ))
                        || header->length <= sizeof( *header )
                        || E_main_I_acpi_I_checksum( header, header->length )
                        )
                            return ~0;
                        E_main_S_kernel_args.acpi.dsdt_content = ( Pc )header + sizeof( *header );
                        E_main_S_kernel_args.acpi.dsdt_content_l = header->length - sizeof( *header );
                        if(facs)
                        {   if( !E_mem_Q_blk_T_eq( &facs->signature[0], "FACS", sizeof( facs->signature ))
                            || facs->length != sizeof( *facs )
                            )
                                return ~0;
                        }
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "HPET", sizeof( header->signature )))
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
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "MCFG", sizeof( header->signature )))
                    {   struct H_acpi_Z_mcfg_entry *mcfg_entry = (P)(( Pc )header + sizeof( *header ));
                        N n = ( header->length - sizeof( *header )) / sizeof( struct H_acpi_Z_mcfg_entry );
                        if( n != 1 )
                            return ~0;
                        E_main_S_kernel_args.pcie_base_address = (P)mcfg_entry->base_address;
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "SSDT", sizeof( header->signature )))
                    {   if( E_main_S_kernel_args.acpi.ssdt_content_n == J_a_R_n( E_main_S_kernel_args.acpi.ssdt_content ))
                            return ~0;
                        E_main_S_kernel_args.acpi.ssdt_content[ E_main_S_kernel_args.acpi.ssdt_content_n ].address = ( Pc )header + sizeof( *header );
                        E_main_S_kernel_args.acpi.ssdt_content[ E_main_S_kernel_args.acpi.ssdt_content_n ].l = header->length - sizeof( *header );
                        E_main_S_kernel_args.acpi.ssdt_content_n++;
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WAET", sizeof( header->signature )))
                    {   struct H_acpi_Z_waet *waet = (P)header;
                        if( header->length != sizeof( *waet ))
                            return ~0;
                        E_main_S_kernel_args.acpi.virt_guest_rtc_good = waet->flags & ( 1 << 0 );
                        E_main_S_kernel_args.acpi.virt_guest_pm_good = waet->flags & ( 1 << 1 );
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WSMT", sizeof( header->signature )))
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
            }else if( rsdp->revision == 2 )
            {   struct H_acpi_Z_xsdt *xsdt = (P)rsdp->XSDT_address;
                if( !E_mem_Q_blk_T_eq( &xsdt->header.signature[0], "XSDT", sizeof( xsdt->header.signature ))
                || xsdt->header.length < sizeof( xsdt->header ) + sizeof( xsdt->table_address[0] )
                || E_main_I_acpi_I_checksum( xsdt, xsdt->header.length )
                )
                    return ~0;
                N table_n = ( xsdt->header.length - sizeof( xsdt->header )) / sizeof( xsdt->table_address[0] );
                for_n( table_i, table_n )
                {   struct H_acpi_Z_table_header *header = (P)xsdt->table_address[ table_i ];
                    if( header->length <= sizeof( *header )
                    || E_main_I_acpi_I_checksum( header, header->length )
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
                    status = H_uefi_I_print_n( system_table, header->revision, 10 );
                    if( status < 0 )
                        return status;
                    if( E_mem_Q_blk_T_eq( &header->signature[0], "APIC", sizeof( header->signature )))
                    {   struct H_acpi_Z_apic *apic = (P)header;
                        E_main_S_apic_content = ( Pc )apic + sizeof( *apic );
                        E_main_S_apic_content_l = apic->header.length - sizeof( *apic );
                        E_main_S_kernel_args.local_apic_address = (P)(N)apic->local_interrupt_controler;
                        E_main_S_pic_mode = apic->flags & 1;
                        E_main_S_kernel_args.processor_n = 0;
                        Pc table = E_main_S_apic_content;
                        N l = E_main_S_apic_content_l;
                        while(l)
                        {   if( l < ( N8 )table[1] )
                                return ~0;
                            switch(( N8 )table[0] )
                            { case 0: // local APIC
                                {   struct H_acpi_Z_madt_Z_local_apic *local_apic = (P)&table[0];
                                    if( local_apic->l != sizeof( *local_apic ))
                                        return ~0;
                                    if(( local_apic->flags & 3 ) == 1 )
                                        E_main_S_kernel_args.processor_n++;
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
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "DMAR", sizeof( header->signature )))
                    {   struct H_acpi_Z_dmar *dmar = (P)header;
                        E_main_S_kernel_args.acpi.dmar_content = ( Pc )dmar + sizeof( *dmar );
                        E_main_S_kernel_args.acpi.dmar_content_l = dmar->header.length - sizeof( *dmar );
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "FACP", sizeof( header->signature )))
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
                        status = H_uefi_I_print_n( system_table, header->revision, 10 );
                        if( status < 0 )
                            return status;
                        if(facs)
                        {   status = system_table->output->output( system_table->output, L"),FACS(" );
                            if( status < 0 )
                                return status;
                            status = H_uefi_I_print_n( system_table, facs->version, 10 );
                            if( status < 0 )
                                return status;
                        }
                        status = system_table->output->output( system_table->output, L")" );
                        if( status < 0 )
                            return status;
                        if( header->length <= sizeof( *header )
                        || E_main_I_acpi_I_checksum( header, header->length )
                        || !E_mem_Q_blk_T_eq( &header->signature[0], "DSDT", sizeof( header->signature ))
                        )
                            return ~0;
                        E_main_S_kernel_args.acpi.dsdt_content = ( Pc )header + sizeof( *header );
                        E_main_S_kernel_args.acpi.dsdt_content_l = header->length - sizeof( *header );
                        if(facs)
                        {   if( !E_mem_Q_blk_T_eq( &facs->signature[0], "FACS", sizeof( facs->signature ))
                            || facs->length != sizeof( *facs )
                            )
                                return ~0;
                        }
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "HPET", sizeof( header->signature )))
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
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "MCFG", sizeof( header->signature )))
                    {   struct H_acpi_Z_mcfg_entry *mcfg_entry = (P)(( Pc )header + sizeof( *header ));
                        N n = ( header->length - sizeof( *header )) / sizeof( struct H_acpi_Z_mcfg_entry );
                        if( n != 1 )
                            return ~0;
                        E_main_S_kernel_args.pcie_base_address = (P)mcfg_entry->base_address;
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "SSDT", sizeof( header->signature )))
                    {   if( E_main_S_kernel_args.acpi.ssdt_content_n == J_a_R_n( E_main_S_kernel_args.acpi.ssdt_content ))
                            return ~0;
                        E_main_S_kernel_args.acpi.ssdt_content[ E_main_S_kernel_args.acpi.ssdt_content_n ].address = ( Pc )header + sizeof( *header );
                        E_main_S_kernel_args.acpi.ssdt_content[ E_main_S_kernel_args.acpi.ssdt_content_n ].l = header->length - sizeof( *header );
                        E_main_S_kernel_args.acpi.ssdt_content_n++;
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WAET", sizeof( header->signature )))
                    {   struct H_acpi_Z_waet *waet = (P)header;
                        if( header->length != sizeof( *waet ))
                            return ~0;
                        E_main_S_kernel_args.acpi.virt_guest_rtc_good = waet->flags & ( 1 << 0 );
                        E_main_S_kernel_args.acpi.virt_guest_pm_good = waet->flags & ( 1 << 1 );
                    }else if( E_mem_Q_blk_T_eq( &header->signature[0], "WSMT", sizeof( header->signature )))
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
            }else
                return ~0;
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
E_main_I_convert_pointer( N memory_map_n
, P *p
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   if( (N)*p >= memory_map->physical_start
        && (N)*p < memory_map->physical_start + memory_map->pages * H_oux_E_mem_S_page_size
        )
        {   *p = (P)( memory_map->virtual_start + ( (N)*p - memory_map->physical_start ));
            break;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
}
void
E_main_I_virtual_address_change_I_convert_pointer( struct H_uefi_Z_runtime_services *runtime_services
, P *p
){  S status = runtime_services->convert_pointer( 0, p );
    if( status < 0 )
        E_main_I_convert_pointer( E_main_S_memory_map_n, p );
}
void
H_uefi_Z_api
E_main_I_virtual_address_change( P event
, P context
){  struct H_uefi_Z_runtime_services *runtime_services = E_main_S_system_table->runtime_services;
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , ( P * )&E_main_S_kernel_args.framebuffer.p
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_apic_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_kernel_args.acpi.dmar_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_kernel_args.acpi.dsdt_content
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_kernel_args.pcie_base_address
    );
    for_n( i, E_main_S_kernel_args.acpi.ssdt_content_n )
        E_main_I_virtual_address_change_I_convert_pointer( runtime_services
        , &E_main_S_kernel_args.acpi.ssdt_content[i].address
        );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_kernel_args.io_apic_address
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , &E_main_S_kernel_args.local_apic_address
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , ( P * )&E_main_S_system_table
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , ( P * )&E_main_S_loader_stack
    );
    E_main_I_virtual_address_change_I_convert_pointer( runtime_services
    , ( P * )&E_main_S_memory_map
    );
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_sort_physical( N memory_map_n
){  while( memory_map_n > 1 )
    {   struct H_uefi_Z_memory_type_descriptor *entry_prev = E_main_S_memory_map;
        N new_memory_map_n = 0;
        for( N i = 1; i != memory_map_n; i++ )
        {   struct H_uefi_Z_memory_type_descriptor *entry = ( struct H_uefi_Z_memory_type_descriptor * )(( Pc )E_main_S_memory_map + i * E_main_S_descriptor_l );
            if( entry_prev->physical_start > entry->physical_start )
            {   J_swap( struct H_uefi_Z_memory_type_descriptor, *entry_prev, *entry );
                new_memory_map_n = i;
            }
            entry_prev = entry;
        }
        memory_map_n = new_memory_map_n;
    }
}
void
E_main_Q_memory_map_I_sort_virtual( N memory_map_n
){  while( memory_map_n > 1 )
    {   struct H_uefi_Z_memory_type_descriptor *entry_prev = E_main_S_memory_map;
        N new_memory_map_n = 0;
        for( N i = 1; i != memory_map_n; i++ )
        {   struct H_uefi_Z_memory_type_descriptor *entry = ( struct H_uefi_Z_memory_type_descriptor * )(( Pc )E_main_S_memory_map + i * E_main_S_descriptor_l );
            if( entry_prev->virtual_start > entry->virtual_start )
            {   J_swap( struct H_uefi_Z_memory_type_descriptor, *entry_prev, *entry );
                new_memory_map_n = i;
            }
            entry_prev = entry;
        }
        memory_map_n = new_memory_map_n;
    }
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_align( N memory_map_n
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   memory_map->physical_start = E_simple_Z_n_I_align_down_to_v2( memory_map->physical_start, H_oux_E_mem_S_page_size );
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
}
//DFN Nie usuwa wszystkich przecinających się bloków. Liczy na to, że niektóre bezsensowne przecięcia nie wystąpią.
__attribute__ (( __warn_unused_result__ ))
S
E_main_Q_memory_map_I_remove_overlapped( N *memory_map_n
){  struct H_uefi_Z_memory_type_descriptor *entry = E_main_S_memory_map;
    while( entry != (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l )
    && !entry->pages
    )
        entry = (P)(( Pc )entry + E_main_S_descriptor_l );
    if( entry == (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l ))
        return 0;
    struct H_uefi_Z_memory_type_descriptor *next_entry = entry;
    O{  do
            next_entry = (P)(( Pc )next_entry + E_main_S_descriptor_l );
        while( next_entry != (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l )
        && !next_entry->pages
        );
        if( next_entry == (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l ))
            break;
        N64 entry_physical_start = entry->physical_start;
        N64 entry_size = entry->pages * H_oux_E_mem_S_page_size;
        N32 entry_type = entry->type;
        N64 next_entry_physical_start = next_entry->physical_start;
        N64 next_entry_size = next_entry->pages * H_oux_E_mem_S_page_size;
        N32 next_entry_type = next_entry->type;
        if( entry_physical_start + entry_size >= next_entry_physical_start
        && entry_type == next_entry_type
        && next_entry_physical_start != (N)E_main_S_kernel_args.kernel
        ) // Bloki są scalane w jeden.
        {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                entry->pages = ( next_entry_physical_start + next_entry_size - entry_physical_start ) / H_oux_E_mem_S_page_size;
            E_mem_Q_blk_I_copy( next_entry, ( Pc )next_entry + E_main_S_descriptor_l, ( *memory_map_n - (( (N)next_entry - (N)E_main_S_memory_map ) / E_main_S_descriptor_l + 1 )) * E_main_S_descriptor_l );
            ( *memory_map_n )--;
            next_entry = (P)(( Pc )next_entry - E_main_S_descriptor_l );
        }else if( entry_physical_start + entry_size > next_entry_physical_start )
            if( entry_type == H_uefi_Z_memory_type_S_conventional
            || next_entry_type == H_uefi_Z_memory_type_S_unusable
            || next_entry_type == H_uefi_Z_memory_type_S_unaccepted
            || ( entry_type == H_uefi_Z_memory_type_S_reserved
              && ( next_entry_type == H_uefi_Z_memory_type_S_acpi_reclaim
                || next_entry_type == H_uefi_Z_memory_type_S_acpi_nvs
                || next_entry_type == H_uefi_Z_memory_type_S_memory_mapped_io
                || next_entry_type == H_uefi_Z_memory_type_S_memory_mapped_io_port_space
            ))
            || (( entry_type == H_uefi_Z_memory_type_S_boot_services_code
                || entry_type == H_uefi_Z_memory_type_S_boot_services_data
                || entry_type == H_uefi_Z_memory_type_S_runtime_services_code
                || entry_type == H_uefi_Z_memory_type_S_runtime_services_data
                || entry_type == H_uefi_Z_memory_type_S_acpi_nvs
              )
              && ( next_entry_type == H_uefi_Z_memory_type_S_acpi_reclaim
                || next_entry_type == H_uefi_Z_memory_type_S_acpi_nvs
                || next_entry_type == H_uefi_Z_memory_type_S_memory_mapped_io
            ))
            || ( entry_type == H_uefi_Z_memory_type_S_boot_services_data
              && next_entry_type == H_uefi_Z_memory_type_S_reserved
            )
            || next_entry_physical_start == (N)E_main_S_kernel_args.kernel
            ) // Pierwszy blok ustępuje drugiemu.
            {   if( entry_physical_start + entry_size > next_entry_physical_start + next_entry_size )
                {   struct H_uefi_Z_memory_type_descriptor *new_entry = (P)(( Pc )next_entry + E_main_S_descriptor_l );
                    while( new_entry != (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l )
                    && new_entry->physical_start < next_entry_physical_start + next_entry_size
                    )
                        new_entry = (P)(( Pc )new_entry + E_main_S_descriptor_l );
                    E_mem_Q_blk_I_copy( ( Pc )new_entry + E_main_S_descriptor_l, new_entry, ( *memory_map_n - ( (N)new_entry - (N)E_main_S_memory_map ) / E_main_S_descriptor_l ) * E_main_S_descriptor_l );
                    ( *memory_map_n )++;
                    new_entry->physical_start = next_entry_physical_start + next_entry_size;
                    new_entry->pages = ( entry_physical_start + entry_size - new_entry->physical_start ) / H_oux_E_mem_S_page_size;
                    new_entry->type = entry_type;
                }
                if( entry_physical_start != next_entry_physical_start )
                    entry->pages = ( next_entry_physical_start - entry_physical_start ) / H_oux_E_mem_S_page_size;
                else
                {   E_mem_Q_blk_I_copy( entry, ( Pc )entry + E_main_S_descriptor_l, ( *memory_map_n - (( (N)entry - (N)E_main_S_memory_map ) / E_main_S_descriptor_l + 1 )) * E_main_S_descriptor_l );
                    ( *memory_map_n )--;
                    next_entry = (P)(( Pc )next_entry - E_main_S_descriptor_l );
                    if( entry != E_main_S_memory_map )
                        entry = (P)(( Pc )entry - E_main_S_descriptor_l );
                    struct H_uefi_Z_memory_type_descriptor *entry_ = entry;
                    while( entry_ != E_main_S_memory_map
                    && !entry_->pages
                    )
                        entry_ = (P)(( Pc )entry_ - E_main_S_descriptor_l );
                    if( entry_ != entry
                    && entry_->pages
                    )
                        entry = entry_;
                    else
                        goto Entry_next;
                }
            }else if( next_entry_physical_start != (N)E_main_S_kernel_args.kernel
            && ( next_entry_type == H_uefi_Z_memory_type_S_conventional
              || entry_type == H_uefi_Z_memory_type_S_unusable
              || entry_type == H_uefi_Z_memory_type_S_unaccepted
              || (( entry_type == H_uefi_Z_memory_type_S_acpi_reclaim
                  || entry_type == H_uefi_Z_memory_type_S_acpi_nvs
                  || entry_type == H_uefi_Z_memory_type_S_memory_mapped_io
                  || entry_type == H_uefi_Z_memory_type_S_memory_mapped_io_port_space
                )
                && ( next_entry_type == H_uefi_Z_memory_type_S_reserved
              ))
              || (( entry_type == H_uefi_Z_memory_type_S_acpi_reclaim
                  || entry_type == H_uefi_Z_memory_type_S_acpi_nvs
                  || entry_type == H_uefi_Z_memory_type_S_memory_mapped_io
                )
                && ( next_entry_type == H_uefi_Z_memory_type_S_boot_services_code
                  || next_entry_type == H_uefi_Z_memory_type_S_boot_services_data
                  || next_entry_type == H_uefi_Z_memory_type_S_runtime_services_code
                  || next_entry_type == H_uefi_Z_memory_type_S_runtime_services_data
                  || next_entry_type == H_uefi_Z_memory_type_S_acpi_nvs
              ))
              || ( entry_type == H_uefi_Z_memory_type_S_reserved
                && next_entry_type == H_uefi_Z_memory_type_S_boot_services_data
            ))) // Drugi blok ustępuje pierwszemu.
            {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                {   struct H_uefi_Z_memory_type_descriptor *new_entry = (P)(( Pc )next_entry + E_main_S_descriptor_l );
                    while( new_entry != (P)(( Pc )E_main_S_memory_map + *memory_map_n * E_main_S_descriptor_l )
                    && new_entry->physical_start < entry_physical_start + entry_size
                    )
                        new_entry = (P)(( Pc )new_entry + E_main_S_descriptor_l );
                    new_entry = (P)(( Pc )new_entry - E_main_S_descriptor_l );
                    E_mem_Q_blk_I_copy( next_entry, ( Pc )next_entry + E_main_S_descriptor_l, (N)new_entry - (N)next_entry );
                    new_entry->physical_start = entry_physical_start + entry_size;
                    new_entry->pages = ( next_entry_physical_start + next_entry_size - new_entry->physical_start ) / H_oux_E_mem_S_page_size;
                    new_entry->type = next_entry_type;
                    next_entry = (P)(( Pc )next_entry - E_main_S_descriptor_l );
                }else
                {   E_mem_Q_blk_I_copy( next_entry, ( Pc )next_entry + E_main_S_descriptor_l, ( *memory_map_n - (( (N)next_entry - (N)E_main_S_memory_map ) / E_main_S_descriptor_l + 1 )) * E_main_S_descriptor_l );
                    ( *memory_map_n )--;
                    next_entry = (P)(( Pc )next_entry - E_main_S_descriptor_l );
                }
            }else
                return ~0;
        else
Entry_next:
        {   do
                entry = (P)(( Pc )entry + E_main_S_descriptor_l );
            while( !entry->pages );
            next_entry = entry;
        }
    }
    return 0;
}
void
E_main_Q_memory_map_I_remove_bad( N *memory_map_n
){  N n = *memory_map_n;
    while( n > 1 )
    {   struct H_uefi_Z_memory_type_descriptor *entry_prev = E_main_S_memory_map;
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   struct H_uefi_Z_memory_type_descriptor *entry = ( struct H_uefi_Z_memory_type_descriptor * )(( Pc )E_main_S_memory_map + i * E_main_S_descriptor_l );
            if(( !entry_prev->pages
              || entry_prev->type == H_uefi_Z_memory_type_S_unusable
              || entry_prev->type == H_uefi_Z_memory_type_S_unaccepted
              || entry_prev->physical_start > entry->physical_start
            )
            && entry->pages
            && entry->type != H_uefi_Z_memory_type_S_unusable
            && entry->type != H_uefi_Z_memory_type_S_unaccepted
            )
            {   J_swap( struct H_uefi_Z_memory_type_descriptor, *entry_prev, *entry );
                new_n = i;
            }
            entry_prev = entry;
        }
        n = new_n;
    }
    for_n_rev( i, *memory_map_n )
    {   struct H_uefi_Z_memory_type_descriptor *entry = ( struct H_uefi_Z_memory_type_descriptor * )(( Pc )E_main_S_memory_map + i * E_main_S_descriptor_l );
        if( entry->pages
        && entry->type != H_uefi_Z_memory_type_S_unusable
        && entry->type != H_uefi_Z_memory_type_S_unaccepted
        )
            break;
    }
    *memory_map_n = i + 1;
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_R_loader_location( N memory_map_n
, N *loader_start
, N *loader_end
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_type_S_loader_code )
        {   *loader_start = memory_map->physical_start;
            *loader_end = memory_map->physical_start + memory_map->pages * H_oux_E_mem_S_page_size;
            break;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
}
N
E_main_Q_memory_map_R_reserved_size( N memory_map_n
){  N size = 0;
    struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map->type != H_uefi_Z_memory_type_S_loader_code
        && memory_map->type != H_uefi_Z_memory_type_S_loader_data
        && memory_map->type != H_uefi_Z_memory_type_S_boot_services_code
        && memory_map->type != H_uefi_Z_memory_type_S_boot_services_data
        && memory_map->type != H_uefi_Z_memory_type_S_conventional
        )
            size += memory_map->pages * H_oux_E_mem_S_page_size;
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_reserved_size_from_start( N memory_map_n
){  N size = 0;
    struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_type_S_reserved )
            size += memory_map->pages * H_oux_E_mem_S_page_size;
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_size( N memory_map_n
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    N size = H_oux_E_mem_S_page_size;
    for_n( i, memory_map_n )
    {   size += memory_map->pages * H_oux_E_mem_S_page_size;
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    return size;
}
N
E_main_Q_memory_map_R_saved_n( N memory_map_n
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    N n = 0;
    for_n( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_type_S_reserved //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_loader_code
        || memory_map->type == H_uefi_Z_memory_type_S_loader_data
        || memory_map->type == H_uefi_Z_memory_type_S_boot_services_code
        || memory_map->type == H_uefi_Z_memory_type_S_boot_services_data
        || memory_map->type == H_uefi_Z_memory_type_S_runtime_services_code //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_runtime_services_data //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_conventional
        || memory_map->type == H_uefi_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_acpi_nvs
        || memory_map->type == H_uefi_Z_memory_type_S_memory_mapped_io
        || memory_map->type == H_uefi_Z_memory_type_S_memory_mapped_io_port_space
        || memory_map->type == H_uefi_Z_memory_type_S_pal_code //TODO Czy potrzebne?
        )
            n++;
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    return n;
}
void
E_main_Q_memory_map_I_set_virtual_I_entry( struct H_uefi_Z_memory_type_descriptor *memory_map_
, struct H_uefi_Z_memory_type_descriptor **memory_map_end
, N *memory_map_new_entries
, N *next_virtual_address
, B *processor_start_page_computed
){  memory_map_->virtual_start = *next_virtual_address;
    *next_virtual_address += memory_map_->pages * H_oux_E_mem_S_page_size;
    if( !*processor_start_page_computed
    && *next_virtual_address >= E_main_S_kernel_args.processor_start_page
    )
    {   if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
        {   struct H_uefi_Z_memory_type_descriptor *memory_map_new_entry = *memory_map_end;
            memory_map_new_entry->type = memory_map_->type;
            memory_map_new_entry->pages = ( *next_virtual_address - E_main_S_kernel_args.processor_start_page ) / H_oux_E_mem_S_page_size;
            memory_map_->pages -= memory_map_new_entry->pages;
            memory_map_new_entry->physical_start = memory_map_->physical_start + memory_map_->pages * H_oux_E_mem_S_page_size;
            memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size;
            ( *memory_map_new_entries )++;
            *memory_map_end = (P)(( Pc )*memory_map_end + E_main_S_descriptor_l );
        }
        *next_virtual_address += H_oux_E_mem_S_page_size;
        *processor_start_page_computed = yes;
    }
}
__attribute__ (( __warn_unused_result__ ))
S
E_main_Q_memory_map_I_set_virtual( N memory_map_n
, B reserved_from_end
, N *memory_map_new_entries
){  *memory_map_new_entries = 0;
    struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    for_n( i, memory_map_n )
    {   if( memory_map->physical_start == E_main_S_kernel_args.processor_start_page )
        {   memory_map->virtual_start = memory_map->physical_start;
            break;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    struct H_uefi_Z_memory_type_descriptor *memory_map_end = (P)(( Pc )E_main_S_memory_map + memory_map_n * E_main_S_descriptor_l );
    B processor_start_page_computed = no;
    N next_virtual_address = H_oux_E_mem_S_page_size;
    if( reserved_from_end )
    {   memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_type_S_reserved )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        if( next_virtual_address < E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size )
            return ~0;
        //DFN Początkowo stos jest w pamięci typu “H_uefi_Z_memory_type_S_boot_services_data”, a “E_main_S_memory_map” 〃 “H_uefi_Z_memory_type_S_loader_data”.
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_type_S_loader_code 
            || ( memory_map->type == H_uefi_Z_memory_type_S_loader_data
              && memory_map->physical_start != E_main_S_kernel_args.processor_start_page
              && memory_map->physical_start != (N)E_main_S_kernel_args.kernel
            )
            || memory_map->type == H_uefi_Z_memory_type_S_boot_services_data
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_type_S_boot_services_code
            || memory_map->type == H_uefi_Z_memory_type_S_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->physical_start == (N)E_main_S_kernel_args.kernel )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
                break;
            }
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type != H_uefi_Z_memory_type_S_reserved
            && memory_map->type != H_uefi_Z_memory_type_S_loader_code
            && memory_map->type != H_uefi_Z_memory_type_S_loader_data
            && memory_map->type != H_uefi_Z_memory_type_S_boot_services_code
            && memory_map->type != H_uefi_Z_memory_type_S_boot_services_data
            && memory_map->type != H_uefi_Z_memory_type_S_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
    }else
    {   memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type != H_uefi_Z_memory_type_S_loader_code
            && memory_map->type != H_uefi_Z_memory_type_S_loader_data
            && memory_map->type != H_uefi_Z_memory_type_S_boot_services_code
            && memory_map->type != H_uefi_Z_memory_type_S_boot_services_data
            && memory_map->type != H_uefi_Z_memory_type_S_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        if( next_virtual_address < E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size )
            return ~0;
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->physical_start == (N)E_main_S_kernel_args.kernel )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
                break;
            }
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_type_S_boot_services_code
            || memory_map->type == H_uefi_Z_memory_type_S_conventional
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
        //DFN Początkowo stos jest w pamięci typu “H_uefi_Z_memory_type_S_boot_services_data”, a “E_main_S_memory_map” 〃 “H_uefi_Z_memory_type_S_loader_data”.
        memory_map = E_main_S_memory_map;
        for_n_( i, memory_map_n )
        {   if( memory_map->type == H_uefi_Z_memory_type_S_loader_code
            || ( memory_map->type == H_uefi_Z_memory_type_S_loader_data
              && memory_map->physical_start != E_main_S_kernel_args.processor_start_page
              && memory_map->physical_start != (N)E_main_S_kernel_args.kernel
            )
            || memory_map->type == H_uefi_Z_memory_type_S_boot_services_data
            )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &memory_map_end
                , memory_map_new_entries
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
        }
    }
    return 0;
}
N
E_main_Q_memory_map_I_remove_empty( N memory_map_n
){  N n = 0;
    struct H_uefi_Z_memory_type_descriptor *memory_map = (P)(( Pc )E_main_S_memory_map + ( memory_map_n - 1 ) * E_main_S_descriptor_l );
    for_n_rev( i, memory_map_n )
    {   if( !memory_map->pages )
        {   n++;
            E_mem_Q_blk_I_copy( memory_map, ( Pc )memory_map + E_main_S_descriptor_l, ( memory_map_n-- - i ) * E_main_S_descriptor_l );
        }
        memory_map = (P)(( Pc )memory_map - E_main_S_descriptor_l );
    }
    return n;
}
N
E_main_Q_memory_map_I_join( N memory_map_n
){  N n = 0;
    struct H_uefi_Z_memory_type_descriptor *memory_map_prev = (P)(( Pc )E_main_S_memory_map + ( memory_map_n - 1 ) * E_main_S_descriptor_l );
    struct H_uefi_Z_memory_type_descriptor *memory_map = (P)(( Pc )memory_map_prev - E_main_S_descriptor_l );
    for_n_rev( i, memory_map_n - 1 )
    {   if( memory_map->type == memory_map_prev->type
        && memory_map->virtual_start + memory_map->pages * H_oux_E_mem_S_page_size == memory_map_prev->virtual_start
        )
        {   n++;
            memory_map->pages += memory_map_prev->pages;
            E_mem_Q_blk_I_copy( memory_map_prev, ( Pc )memory_map_prev + E_main_S_descriptor_l, ( memory_map_n-- - ( 1 + i ) - 1 ) * E_main_S_descriptor_l );
        }
        memory_map_prev = memory_map;
        memory_map = (P)(( Pc )memory_map - E_main_S_descriptor_l );
    }
    return n;
}
//------------------------------------------------------------------------------
void
E_main_I_allocate_page_table_I_next_page( struct H_uefi_Z_memory_type_descriptor **entry
, B reserved_from_end
, N *pages
){  if( reserved_from_end )
    {   if( !(*pages)-- )
        {   *entry = (P)(( Pc )*entry - E_main_S_descriptor_l );
            while(( *entry )->physical_start == E_main_S_kernel_args.processor_start_page )
                *entry = (P)(( Pc )*entry - E_main_S_descriptor_l );
            *pages = ( *entry )->pages - 1;
        }
    }else
        if( ++*pages == ( *entry )->pages )
        {   *entry = (P)(( Pc )*entry + E_main_S_descriptor_l );
            while(( *entry )->physical_start == E_main_S_kernel_args.processor_start_page )
                *entry = (P)(( Pc )*entry + E_main_S_descriptor_l );
            *pages = 0;
        }
}
void
E_main_I_allocate_page_table_I_next_physical_address( struct H_uefi_Z_memory_type_descriptor **entry
, N *pages
){  if( ++*pages == ( *entry )->pages )
    {   *entry = (P)(( Pc )*entry + E_main_S_descriptor_l );
        *pages = 0;
    }
}
__attribute__ (( __warn_unused_result__ ))
S
E_main_I_allocate_page_table( N memory_map_l
, N max_memory
, B reserved_from_end
, N *pml4_
, N *start_end_address
, N *additional_pages
){  struct H_uefi_Z_memory_type_descriptor *memory_map = E_main_S_memory_map;
    P memory_map_end = ( Pc )memory_map + memory_map_l;
    struct H_uefi_Z_memory_type_descriptor *memory_map_entry_conventional;
    N pages;
    //DFN Jest co najmniej jeden blok pamięci z typów: “H_uefi_Z_memory_type_S_boot_services_code”, “H_uefi_Z_memory_type_S_conventional”.
    if( reserved_from_end )
    {   memory_map_entry_conventional = (P)(( Pc )memory_map_end - E_main_S_descriptor_l );
        while( memory_map_entry_conventional->type != H_uefi_Z_memory_type_S_boot_services_code
        && memory_map_entry_conventional->type != H_uefi_Z_memory_type_S_conventional
        )
            memory_map_entry_conventional = (P)(( Pc )memory_map_entry_conventional - E_main_S_descriptor_l );
        pages = memory_map_entry_conventional->pages - 1;
    }else
    {   memory_map_entry_conventional = memory_map;
        while( memory_map_entry_conventional->type != H_uefi_Z_memory_type_S_boot_services_code
        && memory_map_entry_conventional->type != H_uefi_Z_memory_type_S_conventional
        )
            memory_map_entry_conventional = (P)(( Pc )memory_map_entry_conventional + E_main_S_descriptor_l );
        pages = 0;
    }
    volatile Pn pml4 = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
    N physical_pages = -1;
    B end = no;
    *additional_pages = 0;
    N additional_pages_ = 1ULL << 15; //CONF Maksymalna liczba (‘guard pages’ stosów) ‹zadań› w systemie.
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
    {   if( !end
        || additional_pages_
        )
        {   E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, reserved_from_end, &pages );
            volatile Pn pdpt = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
            for_n( pdpt_i, table_n )
            {   if( !end
                || additional_pages_
                )
                {   E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, reserved_from_end, &pages );
                    volatile Pn pd = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
                    for_n( pd_i, table_n )
                    {   if( !end
                        || additional_pages_
                        )
                        {   E_main_I_allocate_page_table_I_next_page( &memory_map_entry_conventional, reserved_from_end, &pages );
                            volatile Pn pt = (P)( memory_map_entry_conventional->physical_start + pages * H_oux_E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory - H_oux_E_mem_S_page_size )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   E_main_I_allocate_page_table_I_next_physical_address( &memory_map, &physical_pages );
                                        N physical_address = memory_map->physical_start + physical_pages * H_oux_E_mem_S_page_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                        B sata_ahci = no;
                                        for_n( i, E_main_S_sata_ahci_n )
                                            if( physical_address == E_main_S_sata_ahci_addresses[i]
                                            || physical_address == E_main_S_sata_ahci_addresses[i] + H_oux_E_mem_S_page_size
                                            )
                                            {   sata_ahci = yes;
                                                break;
                                            }
                                        if( sata_ahci
                                        || physical_address == (N)E_main_S_kernel_args.local_apic_address
                                        || physical_address == (N)E_main_S_kernel_args.io_apic_address
                                        || ( E_main_S_kernel_args.pcie_base_address
                                          && physical_address >= (N)E_main_S_kernel_args.pcie_base_address
                                          && physical_address < (N)E_main_S_kernel_args.pcie_base_address + 256 * 32 * 8 * H_oux_E_mem_S_page_size
                                        )
                                        || ( physical_address >= E_main_S_ethernet_address
                                          && physical_address < E_main_S_ethernet_address + 0x20000
                                        )
                                        || physical_address == E_main_S_ethernet_eeprom_address
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pwt | E_cpu_Z_page_entry_S_pcd;
                                        else if( physical_address >= (N)E_main_S_kernel_args.framebuffer.p
                                        && physical_address < E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.framebuffer.p + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, H_oux_E_mem_S_page_size )
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pat;
                                    }
                                }else
                                {   pt[ pt_i ] = 0;
                                    if( additional_pages_ )
                                        additional_pages_--;
                                    ( *additional_pages )++;
                                }
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
    struct E_base_Z_image_nt_headers64 *image_nt_headers64 = (P)(( Pc )loader_start + image_dos_header->lfanew );
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
        image_relocation = (P)(( Pc )image_relocation + image_relocation->size_of_block );
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
N
E_main_M_madt( Pc table
, N l
){  N apic_source_override_n = 0;
    struct E_main_Z_apic_source_override *apic_source_override;
    Mt_( apic_source_override, apic_source_override_n );
    Kp( apic_source_override )
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
    Kp( E_main_S_kernel_args.gsi )
    {   K_( ~1, W( apic_source_override ));
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
            if( i != E_main_S_kernel_args.gsi[i].source )
                E_main_S_kernel_args.gsi[ E_main_S_kernel_args.gsi[i].source ].source = ~0;
            apic_source_override_i++;
        }
    K_( ~1, W( apic_source_override ));
    return 0;
}
//------------------------------------------------------------------------------
S
H_uefi_Z_api
H_uefi_I_main( P image_handle
, struct H_uefi_Z_system_table *system_table
){  S status = system_table->output->output( system_table->output, L"OUX/C+ OS boot loader ©overcq <overcq@int.pl> http://github.com/overcq\r\n" );
    if( status < 0 )
        return status;
    N32 eax = 0, ebx, ecx, edx;
    __asm__ volatile (
    "\n" "cpuid"
    : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    );
    if( !(( ebx == 0x756e6547 // Intel
      && edx == 0x49656e69
      && ecx == 0x6c65746e
    )
    || ( ebx == 0x68747541 // qemu
      && edx == 0x69746e65
      && ecx == 0x444d4163
    )))
        return ~0;
    eax = 1;
    __asm__ volatile (
    "\n" "cpuid"
    : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    );
    if( !( edx & (( 1 << 16 ) | ( 1 << 15 ) | ( 1 << 9 ) | ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 0 )))) // PAT, CMOV, APIC, MSR, TSC, FPU
        return ~0;
    E_main_S_kernel_args.sse = !!( edx & ( 1 << 25 ));
    E_main_S_kernel_args.x2apic = !!( ecx & ( 1 << 21 ));
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
        if( selected_info.horizontal_resolution <= info->horizontal_resolution )
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
    E_main_S_kernel_args.framebuffer.bits_per_pixel = 32;
    E_main_S_kernel_args.framebuffer.red_size = E_main_S_kernel_args.framebuffer.green_size = E_main_S_kernel_args.framebuffer.blue_size = 8;
    switch( selected_info.pixel_format )
    { case H_uefi_Z_pixel_format_S_rgb8:
            E_main_S_kernel_args.framebuffer.red_shift = 0;
            E_main_S_kernel_args.framebuffer.green_shift = 8;
            E_main_S_kernel_args.framebuffer.blue_shift = 16;
            break;
      case H_uefi_Z_pixel_format_S_bgr8:
            E_main_S_kernel_args.framebuffer.red_shift = 16;
            E_main_S_kernel_args.framebuffer.green_shift = 8;
            E_main_S_kernel_args.framebuffer.blue_shift = 0;
            break;
      case H_uefi_Z_pixel_format_S_bitmask:
            E_main_S_kernel_args.framebuffer.red_shift = E_asm_I_bsf( selected_info.pixel_bitmask.red );
            if( selected_info.pixel_bitmask.red ^ ( 0xff << E_main_S_kernel_args.framebuffer.red_shift ) != ( 0xff << E_main_S_kernel_args.framebuffer.red_shift ))
                return ~0;
            E_main_S_kernel_args.framebuffer.green_shift = E_asm_I_bsf( selected_info.pixel_bitmask.green );
            if( selected_info.pixel_bitmask.green ^ ( 0xff << E_main_S_kernel_args.framebuffer.green_shift ) != ( 0xff << E_main_S_kernel_args.framebuffer.green_shift ))
                return ~0;
            E_main_S_kernel_args.framebuffer.blue_shift = E_asm_I_bsf( selected_info.pixel_bitmask.blue );
            if( selected_info.pixel_bitmask.blue ^ ( 0xff << E_main_S_kernel_args.framebuffer.blue_shift ) != ( 0xff << E_main_S_kernel_args.framebuffer.blue_shift ))
                return ~0;
            break;
      default:
            return ~0;
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
    N block_size;
    for_n( disk_io_handles_i, disk_io_handles_n )
    {   struct H_uefi_Z_protocol_Z_block_io *block_io;
        status = system_table->boot_services->open_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_block_io_S, ( P * )&block_io, image_handle, 0, H_uefi_Z_open_protocol_Z_attribute_S_by_handle_protocol );
        if( status < 0 )
            continue;
        N32 media_id = block_io->media->media_id;
        B logical_partition = block_io->media->logical_partition;
        if( !logical_partition ) //NDFN Liczenie na to, że partycje są wyliczane po całym dysku.
            block_size = block_io->media->logical_blocks_per_physical_block * block_io->media->block_size;
        status = system_table->boot_services->close_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_block_io_S, image_handle, 0 );
        if( status < 0 )
            break;
        if( !logical_partition )
            continue;
        struct H_uefi_Z_protocol_Z_disk_io *disk_io;
        status = system_table->boot_services->open_protocol( disk_io_handles[ disk_io_handles_i ], &H_uefi_Z_guid_S_disk_io_S, ( P * )&disk_io, image_handle, 0, H_uefi_Z_open_protocol_Z_attribute_S_by_handle_protocol );
        if( status < 0 )
            continue;
        status = H_oux_E_fs_Q_disk_M( system_table, disk_io, media_id, block_size );
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
        status = system_table->boot_services->M_pages( H_uefi_Z_allocate_Z_any, H_uefi_Z_memory_type_S_loader_data
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
    kernel_p = (P)(( Pc )kernel_p + 6 );
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
        || kernel_data.exports[i].offset >= (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size )
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
    memory_map_l += 2 * E_main_S_descriptor_l; // Na możliwość wstawienia w następującym “M_pool”.
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_type_S_loader_data, memory_map_l, ( P * )&E_main_S_memory_map );
    if( status < 0 )
        return status;
    status = system_table->boot_services->R_memory_map( &memory_map_l, E_main_S_memory_map, &map_key, &E_main_S_descriptor_l, &descriptor_version );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( E_main_S_memory_map );
        return status;
    }
    N loader_start, loader_end;
    E_main_Q_memory_map_R_loader_location( memory_map_l / E_main_S_descriptor_l, &loader_start, &loader_end );
    status = system_table->boot_services->W_pool( E_main_S_memory_map );
    N loader_start_new_physical;
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_type_S_loader_data, loader_end - loader_start, ( P * )&loader_start_new_physical );
    if( status < 0 )
        return status;
    N n = E_pci_I_check_buses_0();
    if( K_error(n) )
    {   S status_ = system_table->boot_services->W_pool( (P)loader_start_new_physical );
        return ~0;
    }
    memory_map_l += ( 2 + 2 + n + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 + 1 ) * 2 * E_main_S_descriptor_l;
    /* 2 na możliwość wstawienia w poprzednim “M_pool”
     * 2 na możliwość wstawienia w następnym “M_pool”
     * n na zakresy odczytane podczas wyliczania PCI
     * 1 na 
     * 1 na zakres SMM
     * 1 na 
     * 1 na dopisanie bloku ‘kernela’
     * 1 na dopisanie bloku ‘framebuffera’
     * 1 na dopisanie bloku “local_apic_address”
     * 1 na dopisanie bloku “io_apic_address”
     * 1 na dopisanie bloku PCIe
     * 1 na stronę pamięci poniżej 1 MiB na program startowy procesorów
     * 1 na wykonanie procedury “E_main_Q_memory_map_I_set_virtual”
     * pomnożone przez 2 na możliwość dodania bloków z powodu przecinania się zakresów
     */
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_type_S_loader_data, memory_map_l, ( P * )&E_main_S_memory_map );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( (P)loader_start_new_physical );
        return status;
    }
    status = system_table->boot_services->R_memory_map( &memory_map_l, E_main_S_memory_map, &map_key, &E_main_S_descriptor_l, &descriptor_version );
    if( status < 0 )
    {   S status_ = system_table->boot_services->W_pool( E_main_S_memory_map );
        status_ = system_table->boot_services->W_pool( (P)loader_start_new_physical );
        return status;
    }
    status = system_table->boot_services->exit_boot_services( image_handle, map_key );
    if( status < 0 )
        return status;
    __asm__ volatile (
    "\n" "cli"
    );
    // Wyłączenie PIC.
    if( E_main_S_pic_mode )
    {   E_main_I_out_8( 0x21, 0xff );
        E_main_I_out_8( 0xa1, 0xff );
    }
    // Przygotowanie ‘write‐combining’ (dla ‘frame buffera’).
    N64 v = E_main_Q_msr_R( 0x277 );
    v &= ~0x70000ULL;
    v |= 0x10000;
    E_main_Q_msr_P( 0x277, v );
    // Włączenie x2APIC.
    if( E_main_S_kernel_args.x2apic )
    {   v = E_main_Q_msr_R( 0x1b );
        v |= ( 1 << 11 ) | ( 1 << 10 );
        E_main_Q_msr_P( 0x1b, v );
    }
    E_main_S_sata_ahci_n = 0;
    N r = E_pci_I_check_buses( &memory_map_l );
    if( K_error(r) )
        goto End;
    struct H_uefi_Z_memory_type_descriptor *memory_map = (P)(( Pc )E_main_S_memory_map + memory_map_l );
    memory_map->type = H_uefi_Z_memory_type_S_reserved;
    memory_map->physical_start = 0;
    memory_map->pages = 1;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_reserved;
    memory_map->physical_start = 0x30000;
    memory_map->pages = 0x20;
    memory_map_l += E_main_S_descriptor_l;
    //memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    //memory_map->type = H_uefi_Z_memory_type_S_reserved;
    //memory_map->physical_start = 0x80000;
    //memory_map->pages = 0x80;
    //memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_loader_data;
    memory_map->physical_start = (N)E_main_S_kernel_args.kernel;
    memory_map->pages = E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.framebuffer.p;
    memory_map->pages = E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.framebuffer.p % H_oux_E_mem_S_page_size + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.local_apic_address;
    memory_map->pages = 1;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.io_apic_address;
    memory_map->pages = 1;
    memory_map_l += E_main_S_descriptor_l;
    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
    memory_map->physical_start = (N)E_main_S_kernel_args.pcie_base_address;
    memory_map->pages = 256 * 32 * 8;
    memory_map_l += E_main_S_descriptor_l;
    N memory_map_n = memory_map_l / E_main_S_descriptor_l;
    E_main_Q_memory_map_I_align( memory_map_n );
    E_main_Q_memory_map_I_sort_physical( memory_map_n );
    memory_map = E_main_S_memory_map;
    for_n_( i, memory_map_n ) //NDFN
    {   if( memory_map->type == H_uefi_Z_memory_type_S_memory_mapped_io
        && !memory_map->physical_start
        && memory_map->pages == 0x10000
        )
        {   memory_map->pages = 0;
            break;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    status = E_main_Q_memory_map_I_remove_overlapped( &memory_map_n );
    if( status < 0 )
        goto End;
    E_main_Q_memory_map_I_remove_bad( &memory_map_n );
    memory_map_l = memory_map_n * E_main_S_descriptor_l;
    E_main_S_kernel_args.processor_start_page = ~0;
    memory_map = E_main_S_memory_map;
    for_n_( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_type_S_conventional )
        {   if( memory_map->physical_start >= 1024 * 1024 )
                goto End;
            E_main_S_kernel_args.processor_start_page = memory_map->physical_start;
            memory_map->physical_start += H_oux_E_mem_S_page_size;
            memory_map->pages--;
            break;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    if( !~E_main_S_kernel_args.processor_start_page )
        goto End;
    if( memory_map->pages )
    {   memory_map = (P)(( Pc )E_main_S_memory_map + memory_map_n * E_main_S_descriptor_l );
        memory_map_l += E_main_S_descriptor_l;
        memory_map_n++;
        memory_map->type = H_uefi_Z_memory_type_S_loader_data;
        memory_map->physical_start = E_main_S_kernel_args.processor_start_page;
        memory_map->pages = 1;
        E_main_Q_memory_map_I_sort_physical( memory_map_n );
    }else
    {   memory_map->type = H_uefi_Z_memory_type_S_loader_data;
        memory_map->physical_start = E_main_S_kernel_args.processor_start_page;
        memory_map->pages = 1;
    }
    N reserved_size = E_main_Q_memory_map_R_reserved_size( memory_map_n );
    B reserved_from_end = yes; //CONF
    N memory_size = E_main_Q_memory_map_R_size( memory_map_n );
    N reserved_size_from_start;
    if( reserved_from_end )
    {   reserved_size_from_start = E_main_Q_memory_map_R_reserved_size_from_start( memory_map_n );
        if( memory_size - ( reserved_size - reserved_size_from_start ) - E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ) - H_oux_E_mem_S_page_size > 0x100000000ULL - H_oux_E_mem_S_page_size )
            reserved_from_end = no;
    }
    N memory_map_new_entries;
    status = E_main_Q_memory_map_I_set_virtual( memory_map_n, reserved_from_end, &memory_map_new_entries );
    if( status < 0 )
        goto End;
    memory_map_n += memory_map_new_entries;
    memory_map_l = memory_map_n * E_main_S_descriptor_l;
    E_main_Q_memory_map_I_sort_virtual( memory_map_n );
    N pml4, start_end_address;
    status = E_main_I_allocate_page_table( memory_map_l, memory_size, reserved_from_end, &pml4, &start_end_address, &E_main_S_kernel_args.additional_pages );
    if( status < 0 )
        goto End;
    E_main_I_convert_pointer( memory_map_n, &E_main_S_kernel_args.kernel );
    N page_table_size = reserved_from_end
    ? (N)E_main_S_kernel_args.kernel - start_end_address
    : start_end_address - ( (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, H_oux_E_mem_S_page_size ));
    E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );
    E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n( memory_map_n );
    N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
    E_main_S_kernel_args.memory_map = (P)( reserved_from_end
    ? ( Pc )E_main_S_kernel_args.page_table - memory_map_size
    : ( Pc )E_main_S_kernel_args.page_table + page_table_size
    );
    E_main_S_kernel_args.stack_pages = 1 + 1; //CONF Co najmniej 2 strony pamięci, ponieważ jedna będzie zmieniona na ‘guard page’.
    N stack_size = E_main_S_kernel_args.stack_pages * H_oux_E_mem_S_page_size;
    E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
    ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
    : memory_size - stack_size
    );
    E_mem_Q_blk_I_copy( (P)loader_start_new_physical, (P)loader_start, loader_end - loader_start );
    N loader_start_old = loader_start;
    E_main_I_convert_pointer( memory_map_n, ( P * )&loader_start );
    loader_end += loader_start - loader_start_old;
    status = E_main_Q_loader_I_relocate( loader_start_new_physical, loader_start - loader_start_old );
    if( status < 0 )
        goto End;
    E_main_Z_remap_jump remap_jump = (P)(N)E_main_S_kernel_args.processor_start_page;
    E_mem_Q_blk_I_copy( remap_jump, &E_remap_jump_I, ( Pc )&E_remap_jump_I_end - ( Pc )&E_remap_jump_I );
    E_main_S_memory_map_n = memory_map_n;
    E_main_S_system_table = system_table;
    __asm__ volatile (
    "\n" "mov   %%rsp,%0"
    : "=g" ( E_main_S_loader_stack )
    );
    status = system_table->runtime_services->P_virtual_address_map( memory_map_l, E_main_S_descriptor_l, descriptor_version, E_main_S_memory_map );
    if( status < 0 )
        goto End;
    remap_jump( E_main_S_loader_stack, pml4, loader_start - loader_start_old );
    __asm__ volatile ( "" ::: "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rsp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "memory" );
    system_table = E_main_S_system_table;
    _0_( &E_main_S_tss );
#define E_main_J_code_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_code | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_Z_code_S_64bit | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_data_descriptor( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_data_S_write | E_cpu_Z_gdt_S_code_data | E_cpu_Z_gdt_S_present | E_cpu_Z_gdt_S_granularity | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_local_descriptor_low( base, limit ) (( (N)(limit) & (( 1 << 16 ) - 1 )) | (( (N)(base) & (( 1 << 24 ) - 1 )) << 16 ) | E_cpu_Z_gdt_Z_type_S_ldt | E_cpu_Z_gdt_S_present | ((( (N)(limit) >> 16 ) & (( 1 << 4 ) - 1 )) << ( 32 + 16 )) | (( (N)(base) >> 24 ) << ( 32 + 24 )))
#define E_main_J_task_descriptor_low( segment_selector, offset ) (( (N)(offset) & (( 1 << 16 ) - 1 )) | ( (N)( segment_selector ) << 16 ) | E_cpu_Z_gdt_Z_type_S_tss | E_cpu_Z_gdt_S_present | ((( (N)(offset) >> 16 ) & (( 1 << 16 ) - 1 )) << ( 32 + 16 )))
    gdt[1] = E_main_J_code_descriptor( 0, ~0ULL );
    gdt[2] = E_main_J_data_descriptor( 0, ~0ULL );
    gdt[3] = E_main_J_local_descriptor_low( (N)&ldt[0], sizeof(ldt) - 1 );
    gdt[4] = (N)&ldt[0] >> 32;
    gdt[5] = E_main_J_task_descriptor_low( 2 << 8, (N)&E_main_S_tss );
    gdt[6] = (N)&E_main_S_tss >> 32;
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
    "\n" "lgdt  %0"
    "\n" "mov   $3 << 3,%%ax"
    "\n" "lldt  %%ax"
    "\n" "lidt  %1"
    "\n" "mov   $2 << 3,%%ax"
    "\n" "mov   %%ax,%%ds"
    "\n" "mov   %%ax,%%es"
    "\n" "mov   %%ax,%%ss"
    "\n" "mov   %%ax,%%fs"
    "\n" "mov   %%ax,%%gs"
    "\n" "movw  $1 << 3,-8(%%rsp)"
    "\n" "lea   0f(%%rip),%%rax"
    "\n" "movq  %%rax,-16(%%rsp)"
    "\n" ".byte 0x48"
    "\n" "ljmp  *-16(%%rsp)"
    "\n0: mov   $5 << 3,%%ax"
    "\n" "ltr   %%ax"
    :
    : "p" ( &gd.limit ), "p" ( &id.limit )
    : "rax", "memory"
    );
    // Przeniesienie stosu.
    __asm__ volatile (
    "\n" "mov   %%rsp,%0"
    : "=g" ( E_main_S_loader_stack )
    );
    if((( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff )) != E_main_S_loader_stack )
    {   E_mem_Q_blk_I_copy( (P)(( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff ))
        , (P)E_main_S_loader_stack
        , H_oux_E_mem_S_page_size - ( E_main_S_loader_stack & 0xfff )
        );
        __asm__ volatile (
        "\n" "lea   %0,%%rsp"
        :
        : "p" (( (N)E_main_S_kernel_args.kernel_stack + stack_size - H_oux_E_mem_S_page_size ) | ( E_main_S_loader_stack & 0xfff ))
        : "memory"
        );
    }
    // ‘Relokacja’ kernela.
    kernel_p = (P)(( Pc )E_main_S_kernel_args.kernel + 6 );
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
    E_main_Q_memory_map_I_sort_physical( memory_map_n );
    struct H_oux_E_mem_Z_memory_map *my_memory_map = E_main_S_kernel_args.memory_map;
    memory_map = E_main_S_memory_map;
    for_n_( i, memory_map_n )
    {   if( memory_map->type == H_uefi_Z_memory_type_S_reserved //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_loader_code
        || memory_map->type == H_uefi_Z_memory_type_S_loader_data
        || memory_map->type == H_uefi_Z_memory_type_S_boot_services_code
        || memory_map->type == H_uefi_Z_memory_type_S_boot_services_data
        || memory_map->type == H_uefi_Z_memory_type_S_runtime_services_code //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_runtime_services_data //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_conventional
        || memory_map->type == H_uefi_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == H_uefi_Z_memory_type_S_acpi_nvs
        || memory_map->type == H_uefi_Z_memory_type_S_memory_mapped_io
        || memory_map->type == H_uefi_Z_memory_type_S_memory_mapped_io_port_space
        || memory_map->type == H_uefi_Z_memory_type_S_pal_code //TODO Czy potrzebne?
        )
        {   my_memory_map->physical_start = memory_map->physical_start;
            my_memory_map->virtual_start = memory_map->virtual_start;
            my_memory_map->pages = memory_map->pages;
            my_memory_map++;
        }
        memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
    }
    r = E_mem_M( reserved_from_end, reserved_size_from_start, loader_start, loader_end - loader_start, (N)E_main_S_kernel_args.kernel_stack, stack_size, (N)E_main_S_kernel_args.memory_map, memory_map_size, (N)E_main_S_kernel_args.page_table, page_table_size, (N)E_main_S_kernel_args.kernel, kernel_size, memory_size, reserved_size );
    if( K_error(r) )
        goto End;
    E_vga_I_fill_rect( 0, 0, E_main_S_kernel_args.framebuffer.width, E_main_S_kernel_args.framebuffer.height, E_vga_R_video_color( E_vga_S_background_color ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 38, E_main_S_kernel_args.framebuffer.height / 2 - 37, 38 + 34, 37 + 36, E_vga_R_video_color( 0x4e864f ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    r = E_font_M();
    if( K_error(r) )
        goto End;
    E_font_I_print( "OUX/C+ OS boot loader ©overcq <overcq@int.pl> https://github.com/overcq\n" );
    r = E_font_W();
    if( K_error(r) )
        goto End;
    r = E_main_M_madt( E_main_S_apic_content, E_main_S_apic_content_l );
    if( K_error(r) )
        goto End;
    Mt_( E_main_S_kernel_args.processor_proc, E_main_S_kernel_args.processor_n - 1 );
    if( K_error( E_main_S_kernel_args.processor_proc )
    || !E_main_S_kernel_args.processor_proc
    )
        goto End;
    E_mem_Q_blk_I_copy( (P)(N)E_main_S_kernel_args.processor_start_page, &E_mp_init_I, ( Pc )&E_mp_init_I_end - ( Pc )&E_mp_init_I );
    Pc p = (P)(N)E_main_S_kernel_args.processor_start_page;
    i = ( Pc )&E_mp_init_I_reloc_1 - ( Pc )&E_mp_init_I + 1;
    *( N16 * )( p + i ) = E_main_S_kernel_args.processor_start_page >> 4;
    i = ( Pc )&E_mp_init_I_reloc_2 - ( Pc )&E_mp_init_I + 2;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + i + 6;
    i = ( Pc )&E_mp_init_I_reloc_3 - ( Pc )&E_mp_init_I + 1;
    *( N32 * )( p + i ) = pml4;
    i = ( Pc )&E_mp_init_I_reloc_4 - ( Pc )&E_mp_init_I + 3;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gd - ( Pc )&E_mp_init_I;
    i = ( Pc )&E_mp_init_I_reloc_5 - ( Pc )&E_mp_init_I + 1;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + i + 6;
    i = ( Pc )&E_mp_init_I_reloc_6 - ( Pc )&E_mp_init_I + 1;
    *( N8 * )( p + i ) = E_main_S_kernel_args.x2apic;
    i = ( Pc )&E_mp_init_I_reloc_7 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = (N)E_main_S_kernel_args.local_apic_address;
    i = ( Pc )&E_mp_init_I_reloc_8 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = (N)E_main_S_kernel_args.processor_proc;
    i = ( Pc )&E_mp_init_S_gd_32 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gdt_32 - ( Pc )&E_mp_init_I;
    i = ( Pc )&E_mp_init_S_gd - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gdt - ( Pc )&E_mp_init_I;
    for_n_( i, E_main_S_kernel_args.processor_n - 1 )
    {   E_main_S_kernel_args.processor_proc[i] = (P)~0ULL;
        E_interrupt_I_ipi_init( 1 + i );
    }
    E_main_S_kernel_args.boot_loader = (P)loader_start;
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
    "\n" "mov   %%cr0,%%rax"
    "\n" "and   %0,%%rax"
    "\n" "or    %1,%%rax"
    "\n" "mov   %%rax,%%cr0"
    "\n" "mov   %%cr4,%%rax"
    "\n" "and   %2,%%rax"
    "\n" "or    %3,%%rax"
    "\n" "mov   %%rax,%%cr4"
    "\n" "mov   %%cr8,%%rax"
    "\n" "and   $~0xf,%%rax"
    "\n" "mov   %%rax,%%cr8"
    :
    : "i" ( ~( E_cpu_Z_cr0_S_em | E_cpu_Z_cr0_S_ts | E_cpu_Z_cr0_S_nw | E_cpu_Z_cr0_S_cd ))
    , "i" ( E_cpu_Z_cr0_S_mp | E_cpu_Z_cr0_S_ne | E_cpu_Z_cr0_S_wp )
    , "i" ( ~( E_cpu_Z_cr4_S_pvi | E_cpu_Z_cr4_S_tsd | E_cpu_Z_cr4_S_pge | E_cpu_Z_cr4_S_pcide | E_cpu_Z_cr4_S_smep | E_cpu_Z_cr4_S_smap | E_cpu_Z_cr4_S_pke | E_cpu_Z_cr4_S_pks | E_cpu_Z_cr4_S_uintr | E_cpu_Z_cr4_S_lam_sup ))
    , "i" ( E_cpu_Z_cr4_S_de | E_cpu_Z_cr4_S_mce | E_cpu_Z_cr4_S_pce | E_cpu_Z_cr4_S_osfxsr | E_cpu_Z_cr4_S_osxmmexcpt | E_cpu_Z_cr4_S_fsgsbase | E_cpu_Z_cr4_S_osxsave )
    : "rax"
    );
    // Przed wyrzuceniem z pamięci programu ‘boot loadera’ ‘kernel’ potrzebuje przenieść dostarczone dane, ustawić LDT, IDT, TSS.
    __asm__ volatile (
    "\n" "mov   %0,%%rsp"
    "\n" "jmp   *%1"
    :
    : "g" ( (N)E_main_S_kernel_args.kernel_stack + stack_size ), "r" ( kernel_data.entry ), "D" ( &E_main_S_kernel_args )
    );
    __builtin_unreachable();
End:
    O{  __asm__ volatile (
        "\n" "hlt"
        );
    }
    __builtin_unreachable();
}
/******************************************************************************/
