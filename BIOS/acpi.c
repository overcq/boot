/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         ACPI search
* (c)overcq              on WSL\Debian (Linux 6.18.33.2)             2026-8-26 V
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
P E_main_Z_p_I_to_virtual_pre( struct E_main_Z_memory_map_entry *, P );
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
//==============================================================================
B E_acpi_S_pic_mode;
P E_acpi_S_apic_content;
N E_acpi_S_apic_content_l;
//==============================================================================
__attribute__ (( __warn_unused_result__ ))
N8
E_acpi_I_checksum( P table
, N size
){  Pc table_ = table;
    C checksum = 0;
    for_n( i, size )
        checksum += table_[i];
    return checksum;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
__attribute__ (( __warn_unused_result__ ))
N
E_acpi_I_rsdp( struct E_main_Z_memory_map_entry *memory_map
, struct H_acpi_Z_rsdp *rsdp
){  struct H_acpi_Z_xsdt *xsdt = E_main_Z_p_I_to_virtual_pre( memory_map, (P)rsdp->XSDT_address );
    if( !xsdt )
        return ~0;
    if( xsdt->header.length < sizeof( xsdt->header ) + sizeof( xsdt->table_address[0] )
    || E_acpi_I_checksum( xsdt, xsdt->header.length )
    || !E_mem_Q_blk_T_eq( &xsdt->header.signature[0], "XSDT", sizeof( xsdt->header.signature ))
    )
        return ~0;
    _0( &E_main_S_kernel_args.acpi, sizeof( E_main_S_kernel_args.acpi ));
    E_main_S_kernel_args.io_apic_address = 0;
    E_main_S_kernel_args.pcie_base_address = 0;
    N table_n = ( xsdt->header.length - sizeof( xsdt->header )) / sizeof( xsdt->table_address[0] );
    for_n( table_i, table_n )
    {   struct H_acpi_Z_table_header *header = E_main_Z_p_I_to_virtual_pre( memory_map, (P)xsdt->table_address[ table_i ] );
        if( !header
        || header->length <= sizeof(header)
        || E_acpi_I_checksum( header, header->length )
        )
            return ~0;
        if( E_mem_Q_blk_T_eq( &header->signature[0], "APIC", sizeof( xsdt->header.signature )))
        {   struct H_acpi_Z_apic *apic = (P)header;
            E_acpi_S_apic_content = ( Pc )apic + sizeof( *apic );
            E_acpi_S_apic_content_l = apic->header.length - sizeof( *apic );
            E_main_S_kernel_args.local_apic_address = (P)(N)apic->local_interrupt_controler;
            E_acpi_S_pic_mode = apic->flags & 1;
            E_main_S_kernel_args.processor_n = 0;
            Pc table = E_acpi_S_apic_content;
            N l = E_acpi_S_apic_content_l;
            while(l)
            {   if( l < ( N8 )table[1] )
                    return ~0;
                switch(( N8 )table[0] )
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
        }else if( E_mem_Q_blk_T_eq( &header->signature[0], "DMAR", sizeof( xsdt->header.signature )))
        {   struct H_acpi_Z_dmar *dmar = (P)header;
            E_main_S_kernel_args.acpi.dmar_content = ( Pc )dmar + sizeof( *dmar );
            E_main_S_kernel_args.acpi.dmar_content_l = dmar->header.length - sizeof( *dmar );
        }else if( E_mem_Q_blk_T_eq( &header->signature[0], "FACP", sizeof( xsdt->header.signature )))
        {   P facs_physical;
            if( header->revision == 6 )
            {   struct H_acpi_Z_fadt *fadt = (P)header;
                if( header->length != sizeof( *fadt )
                || fadt->table_minor_version != 5
                || !fadt->ex_dsdt
                )
                    return ~0;
                header = E_main_Z_p_I_to_virtual_pre( memory_map, (P)fadt->ex_dsdt );
                facs_physical = (P)fadt->ex_facs;
            }else if( header->revision == 3
            || header->revision == 4
            )
            {   struct H_acpi_Z_fadt_v3 *fadt = (P)header;
                if( header->length != sizeof( *fadt )
                || !fadt->ex_dsdt
                )
                    return ~0;
                header = E_main_Z_p_I_to_virtual_pre( memory_map, (P)fadt->ex_dsdt );
                facs_physical = (P)fadt->ex_facs;
            }else
                return ~0;
            struct H_acpi_Z_facs *facs = E_main_Z_p_I_to_virtual_pre( memory_map, (P)facs_physical );
            if( !header
            || !facs
            )
                return ~0;
            if( header->length <= sizeof( *header )
            || E_acpi_I_checksum( header, header->length )
            || !E_mem_Q_blk_T_eq( &header->signature[0], "DSDT", sizeof( xsdt->header.signature ))
            )
                return ~0;
            E_main_S_kernel_args.acpi.dsdt_content = ( Pc )header + sizeof( *header );
            E_main_S_kernel_args.acpi.dsdt_content_l = header->length - sizeof( *header );
            if( facs
            && ( facs->length != sizeof( *facs )
              || facs->version > 3
            ))
                return ~0;
            E_main_S_kernel_args.acpi.facs = facs_physical;
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
        {   struct H_acpi_Z_mcfg_entry *mcfg_entry = (P)(( Pc )header + sizeof( *header ));
            N n = ( header->length - sizeof( *header )) / sizeof( struct H_acpi_Z_mcfg_entry );
            if( n != 1 )
                return ~0;
            E_main_S_kernel_args.pcie_base_address = (P)mcfg_entry->base_address;
        }else if( E_mem_Q_blk_T_eq( &header->signature[0], "SSDT", sizeof( xsdt->header.signature )))
        {   if( E_main_S_kernel_args.acpi.ssdt_contents_n == J_a_R_n( E_main_S_kernel_args.acpi.ssdt_contents ))
                return ~0;
            E_main_S_kernel_args.acpi.ssdt_contents[ E_main_S_kernel_args.acpi.ssdt_contents_n ].address = ( Pc )header + sizeof( *header );
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
    if( !E_main_S_kernel_args.acpi.dsdt_content_l
    || !E_main_S_kernel_args.io_apic_address
    )
        return ~0;
    return 0;
}
__attribute__ (( __warn_unused_result__ ))
N
E_acpi_I_search( struct E_main_Z_memory_map_entry *memory_map
){  struct H_acpi_Z_rsdp *rsdp = (P)(N)*( N16 * )E_simple_Z_p_I_align_up_to_v2( E_main_Z_p_I_to_virtual_pre( memory_map, (P)0x40e ), 16 );
    P rsdp_end = (P)( (N)rsdp + 1024 - sizeof( *rsdp ));
    while( (N)rsdp <= (N)rsdp_end )
    {   if( E_mem_Q_blk_T_eq( &rsdp->signature[0], "RSD PTR ", 8 )
        && rsdp->revision >= 2
        && !E_acpi_I_checksum( rsdp, ( Pc )&rsdp->length - ( Pc )rsdp )
        && rsdp->length >= sizeof( *rsdp )
        && !E_acpi_I_checksum( rsdp, sizeof( *rsdp ))
        )
            return E_acpi_I_rsdp( memory_map, rsdp );
        rsdp = (P)( (N)rsdp + 16 );
    }
    rsdp = E_main_Z_p_I_to_virtual_pre( memory_map, (P)0xe0000 );
    rsdp_end = (P)( (N)rsdp + 0x100000 - 0xe0000 - sizeof( *rsdp ));
    while( (N)rsdp <= (N)rsdp_end )
    {   if( E_mem_Q_blk_T_eq( &rsdp->signature[0], "RSD PTR ", 8 )
        && rsdp->revision >= 2
        && !E_acpi_I_checksum( rsdp, ( Pc )&rsdp->length - ( Pc )rsdp )
        && rsdp->length >= sizeof( *rsdp )
        && !E_acpi_I_checksum( rsdp, sizeof( *rsdp ))
        )
            return E_acpi_I_rsdp( memory_map, rsdp );
        rsdp = (P)( (N)rsdp + 16 );
    }
    return ~0;
}
/******************************************************************************/
