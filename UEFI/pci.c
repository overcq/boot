//-*-C-*-
/*******************************************************************************
*   ___   public
*  ¦OUX¦  C+
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         PCI driver
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”             2025‒6‒25 T
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
enum E_pci_Z_header_Z_header_type
{ E_pci_Z_header_Z_header_type_S_general
, E_pci_Z_header_Z_header_type_S_pci2pci_bridge
, E_pci_Z_header_Z_header_type_S_pci2cardbus_bridge
};
enum E_pci_Z_header_Z_class
{ E_pci_Z_header_Z_class_S_unclassified
, E_pci_Z_header_Z_class_S_mass_storage
, E_pci_Z_header_Z_class_S_network
, E_pci_Z_header_Z_class_S_display
, E_pci_Z_header_Z_class_S_multimedia
, E_pci_Z_header_Z_class_S_memory
, E_pci_Z_header_Z_class_S_bridge
, E_pci_Z_header_Z_class_S_simple_comm
, E_pci_Z_header_Z_class_S_base_system_peripheral
, E_pci_Z_header_Z_class_S_input_device
, E_pci_Z_header_Z_class_S_docking_station
, E_pci_Z_header_Z_class_S_processor
, E_pci_Z_header_Z_class_S_serial_bus
, E_pci_Z_header_Z_class_S_wireless
, E_pci_Z_header_Z_class_S_intelligent
, E_pci_Z_header_Z_class_S_satellite_comm
, E_pci_Z_header_Z_class_S_encryption
, E_pci_Z_header_Z_class_S_signal_processing
, E_pci_Z_header_Z_class_S_processing_accelerator
, E_pci_Z_header_Z_class_S_non_essential_intrumentation
, E_pci_Z_header_Z_class_S_coprocessor = 0x40
, E_pci_Z_header_Z_class_S_vendor = 0xff
};
enum E_pci_Z_header_Z_subclass_unclassified
{ E_pci_Z_header_Z_subclass_unclassified_S_non_vga_compatible
, E_pci_Z_header_Z_subclass_unclassified_S_vga_compatible
};
enum E_pci_Z_header_Z_subclass_mass_storage
{ E_pci_Z_header_Z_subclass_mass_storage_S_scsi_bus
, E_pci_Z_header_Z_subclass_mass_storage_S_ide
, E_pci_Z_header_Z_subclass_mass_storage_S_floppy_disk
, E_pci_Z_header_Z_subclass_mass_storage_S_ipi_bus
, E_pci_Z_header_Z_subclass_mass_storage_S_raid
, E_pci_Z_header_Z_subclass_mass_storage_S_ata
, E_pci_Z_header_Z_subclass_mass_storage_S_sata
, E_pci_Z_header_Z_subclass_mass_storage_S_serial_attached_scsi
, E_pci_Z_header_Z_subclass_mass_storage_S_non_volatile_memory
, E_pci_Z_header_Z_subclass_mass_storage_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_ide
{ E_pci_Z_header_Z_prog_if_ide_S_isa_compatibility_mode_only
, E_pci_Z_header_Z_prog_if_ide_S_pci_native_mode_only = 5
, E_pci_Z_header_Z_prog_if_ide_S_isa_compatibility_mode_both_channels = 0xa
, E_pci_Z_header_Z_prog_if_ide_S_pci_native_mode_both_channels = 0xf
, E_pci_Z_header_Z_prog_if_ide_S_isa_compatibility_mode_bus_mastering = 0x80
, E_pci_Z_header_Z_prog_if_ide_S_pci_native_mode_bus_mastering = 0x85
, E_pci_Z_header_Z_prog_if_ide_S_isa_compatibility_mode_both_channels_bus_mastering = 0x8a
, E_pci_Z_header_Z_prog_if_ide_S_pci_native_mode_both_channels_bus_mastering = 0x8f
};
enum E_pci_Z_header_Z_prog_if_ata
{ E_pci_Z_header_Z_prog_if_ata_S_single_dma = 0x20
, E_pci_Z_header_Z_prog_if_ata_S_chained_dma = 0x30
};
enum E_pci_Z_header_Z_prog_if_sata
{ E_pci_Z_header_Z_prog_if_sata_S_vendor_specific
, E_pci_Z_header_Z_prog_if_sata_S_ahci_1
, E_pci_Z_header_Z_prog_if_sata_S_ssb
};
enum E_pci_Z_header_Z_prog_if_serial_attached_scsi
{ E_pci_Z_header_Z_prog_if_serial_attached_scsi_S_sas
, E_pci_Z_header_Z_prog_if_serial_attached_scsi_S_ssb
};
enum E_pci_Z_header_Z_prog_if_non_volatile_memory
{ E_pci_Z_header_Z_prog_if_non_volatile_memory_S_nvmhci
, E_pci_Z_header_Z_prog_if_non_volatile_memory_S_nvm_express
};
enum E_pci_Z_header_Z_subclass_network
{ E_pci_Z_header_Z_subclass_network_S_ethernet
, E_pci_Z_header_Z_subclass_network_S_token_ring
, E_pci_Z_header_Z_subclass_network_S_fddi
, E_pci_Z_header_Z_subclass_network_S_atm
, E_pci_Z_header_Z_subclass_network_S_isdn
, E_pci_Z_header_Z_subclass_network_S_worldfip
, E_pci_Z_header_Z_subclass_network_S_picmg
, E_pci_Z_header_Z_subclass_network_S_infiniband
, E_pci_Z_header_Z_subclass_network_S_fabric
, E_pci_Z_header_Z_subclass_network_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_display
{ E_pci_Z_header_Z_subclass_display_S_vga_compatible
, E_pci_Z_header_Z_subclass_display_S_xga
, E_pci_Z_header_Z_subclass_display_S_3d
, E_pci_Z_header_Z_subclass_display_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_vga_compatible
{ E_pci_Z_header_Z_prog_if_vga_compatible_S_vga
, E_pci_Z_header_Z_prog_if_vga_compatible_S_8514_compatible
};
enum E_pci_Z_header_Z_subclass_multimedia
{ E_pci_Z_header_Z_subclass_multimedia_S_video_controller
, E_pci_Z_header_Z_subclass_multimedia_S_audio_controller
, E_pci_Z_header_Z_subclass_multimedia_S_telephony_device
, E_pci_Z_header_Z_subclass_multimedia_S_audio_device
, E_pci_Z_header_Z_subclass_multimedia_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_memory
{ E_pci_Z_header_Z_subclass_memory_S_ram
, E_pci_Z_header_Z_subclass_memory_S_flash
, E_pci_Z_header_Z_subclass_memory_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_bridge
{ E_pci_Z_header_Z_subclass_bridge_S_host
, E_pci_Z_header_Z_subclass_bridge_S_isa
, E_pci_Z_header_Z_subclass_bridge_S_eisa
, E_pci_Z_header_Z_subclass_bridge_S_mca
, E_pci_Z_header_Z_subclass_bridge_S_pci2pci
, E_pci_Z_header_Z_subclass_bridge_S_pcmcia
, E_pci_Z_header_Z_subclass_bridge_S_nubus
, E_pci_Z_header_Z_subclass_bridge_S_cardbus
, E_pci_Z_header_Z_subclass_bridge_S_raceway
, E_pci_Z_header_Z_subclass_bridge_S_pci2pci_2
, E_pci_Z_header_Z_subclass_bridge_S_infiniband2pci
, E_pci_Z_header_Z_subclass_bridge_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_pci2pci
{ E_pci_Z_header_Z_prog_if_pci2pci_S_normal_decode
, E_pci_Z_header_Z_prog_if_pci2pci_S_subtractive_decode
};
enum E_pci_Z_header_Z_prog_if_raceway
{ E_pci_Z_header_Z_prog_if_raceway_S_transparent_mode
, E_pci_Z_header_Z_prog_if_raceway_S_endpoint_mode
};
enum E_pci_Z_header_Z_prog_if_pci2pci_2
{ E_pci_Z_header_Z_prog_if_pci2pci_2_S_semi_transparent_primary = 0x40
, E_pci_Z_header_Z_prog_if_pci2pci_2_S_semi_transparent_secondary = 0x80
};
enum E_pci_Z_header_Z_subclass_simple_comm
{ E_pci_Z_header_Z_subclass_simple_comm_S_serial
, E_pci_Z_header_Z_subclass_simple_comm_S_parallel
, E_pci_Z_header_Z_subclass_simple_comm_S_multiport_serial
, E_pci_Z_header_Z_subclass_simple_comm_S_modem
, E_pci_Z_header_Z_subclass_simple_comm_S_ieee488
, E_pci_Z_header_Z_subclass_simple_comm_S_smart_card
, E_pci_Z_header_Z_subclass_simple_comm_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_serial
{ E_pci_Z_header_Z_prog_if_serial_S_8250
, E_pci_Z_header_Z_prog_if_serial_S_16450
, E_pci_Z_header_Z_prog_if_serial_S_16550
, E_pci_Z_header_Z_prog_if_serial_S_16650
, E_pci_Z_header_Z_prog_if_serial_S_16750
, E_pci_Z_header_Z_prog_if_serial_S_16850
, E_pci_Z_header_Z_prog_if_serial_S_16950
};
enum E_pci_Z_header_Z_prog_if_parallel
{ E_pci_Z_header_Z_prog_if_parallel_S_standard
, E_pci_Z_header_Z_prog_if_parallel_S_bidirectional
, E_pci_Z_header_Z_prog_if_parallel_S_ecp
, E_pci_Z_header_Z_prog_if_parallel_S_ieee1284_controller
, E_pci_Z_header_Z_prog_if_parallel_S_ieee1284_device = 0xfe
};
enum E_pci_Z_header_Z_prog_if_modem
{ E_pci_Z_header_Z_prog_if_modem_S_generic
, E_pci_Z_header_Z_prog_if_modem_S_hayes16450
, E_pci_Z_header_Z_prog_if_modem_S_hayes16550
, E_pci_Z_header_Z_prog_if_modem_S_hayes16650
, E_pci_Z_header_Z_prog_if_modem_S_hayes16750
};
enum E_pci_Z_header_Z_subclass_base_system_peripheral
{ E_pci_Z_header_Z_subclass_base_system_peripheral_S_pic
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_dma
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_timer
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_rtc
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_pci_hotplug
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_sd_host
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_iommu
, E_pci_Z_header_Z_subclass_base_system_peripheral_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_pic
{ E_pci_Z_header_Z_prog_if_pic_S_generic_8259
, E_pci_Z_header_Z_prog_if_pic_S_isa
, E_pci_Z_header_Z_prog_if_pic_S_eisa
, E_pci_Z_header_Z_prog_if_pic_S_io_apic = 0x10
, E_pci_Z_header_Z_prog_if_pic_S_iox_apic = 0x20
};
enum E_pci_Z_hedaer_Z_prog_if_dma
{ E_pci_Z_hedaer_Z_prog_if_dma_S_generic_8237
, E_pci_Z_hedaer_Z_prog_if_dma_S_isa
, E_pci_Z_hedaer_Z_prog_if_dma_S_eisa
};
enum E_pci_Z_header_Z_prog_if_timer
{ E_pci_Z_header_Z_prog_if_timer_S_generic_8254
, E_pci_Z_header_Z_prog_if_timer_S_isa
, E_pci_Z_header_Z_prog_if_timer_S_eisa
, E_pci_Z_header_Z_prog_if_timer_S_hpet
};
enum E_pci_Z_header_Z_prog_if_rtc
{ E_pci_Z_header_Z_prog_if_rtc_S_generic
, E_pci_Z_header_Z_prog_if_rtc_S_isa
};
enum E_pci_Z_header_Z_subclass_input_device
{ E_pci_Z_header_Z_input_device_S_keyboard
, E_pci_Z_header_Z_input_device_S_digitized_pen
, E_pci_Z_header_Z_input_device_S_mouse
, E_pci_Z_header_Z_input_device_S_scanner
, E_pci_Z_header_Z_input_device_S_gameport
, E_pci_Z_header_Z_input_device_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_gameport
{ E_pci_Z_header_Z_prog_if_gameport_S_generic
, E_pci_Z_header_Z_prog_if_gameport_S_extended = 0x10
};
enum E_pci_Z_header_Z_subclass_docking_station
{ E_pci_Z_header_Z_subclass_docking_station_S_generic
, E_pci_Z_header_Z_subclass_docking_station_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_processor
{ E_pci_Z_header_Z_subclass_processor_S_386
, E_pci_Z_header_Z_subclass_processor_S_486
, E_pci_Z_header_Z_subclass_processor_S_pentium
, E_pci_Z_header_Z_subclass_processor_S_pentium_pro
, E_pci_Z_header_Z_subclass_processor_S_alpha = 0x10
, E_pci_Z_header_Z_subclass_processor_S_powerpc = 0x20
, E_pci_Z_header_Z_subclass_processor_S_mips = 0x30
, E_pci_Z_header_Z_subclass_processor_S_coprocessor = 0x40
, E_pci_Z_header_Z_subclass_processor_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_serial_bus
{ E_pci_Z_header_Z_subclass_serial_bus_S_firewire
, E_pci_Z_header_Z_subclass_serial_bus_S_access_bus
, E_pci_Z_header_Z_subclass_serial_bus_S_ssa
, E_pci_Z_header_Z_subclass_serial_bus_S_usb
, E_pci_Z_header_Z_subclass_serial_bus_S_fibre_channel
, E_pci_Z_header_Z_subclass_serial_bus_S_smbus
, E_pci_Z_header_Z_subclass_serial_bus_S_infiniband
, E_pci_Z_header_Z_subclass_serial_bus_S_ipmi
, E_pci_Z_header_Z_subclass_serial_bus_S_sercos
, E_pci_Z_header_Z_subclass_serial_bus_S_canbus
, E_pci_Z_header_Z_subclass_serial_bus_S_other = 0x80
};
enum E_pci_Z_header_Z_prog_if_firewire
{ E_pci_Z_header_Z_prog_if_firewire_S_generic
, E_pci_Z_header_Z_prog_if_firewire_S_ohci = 0x10
};
enum E_pci_Z_header_Z_prog_if_usb
{ E_pci_Z_header_Z_prog_if_usb_S_uhci
, E_pci_Z_header_Z_prog_if_usb_S_ohci
, E_pci_Z_header_Z_prog_if_usb_S_ehci
, E_pci_Z_header_Z_prog_if_usb_S_xhci
, E_pci_Z_header_Z_prog_if_usb_S_unspecified = 0x80
, E_pci_Z_header_Z_prog_if_usb_S_device = 0xf3
};
enum E_pci_Z_header_Z_prog_if_ipmi
{ E_pci_Z_header_Z_prog_if_ipmi_S_smic
, E_pci_Z_header_Z_prog_if_ipmi_S_keyboard_style
, E_pci_Z_header_Z_prog_if_ipmi_S_block_transfer
};
enum E_pci_Z_header_Z_subclass_wireless
{ E_pci_Z_header_Z_subclass_wireless_S_irda
, E_pci_Z_header_Z_subclass_wireless_S_consumer_ir
, E_pci_Z_header_Z_subclass_wireless_S_rf = 0x10
, E_pci_Z_header_Z_subclass_wireless_S_bluetooth
, E_pci_Z_header_Z_subclass_wireless_S_broadband
, E_pci_Z_header_Z_subclass_wireless_S_ethernet_802_1a = 0x20
, E_pci_Z_header_Z_subclass_wireless_S_ethernet_802_1b
, E_pci_Z_header_Z_subclass_wireless_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_intelligent
{ E_pci_Z_header_Z_subclass_intelligent_S_i2o
};
enum E_pci_Z_header_Z_subclass_satellite_comm
{ E_pci_Z_header_Z_subclass_satellite_comm_S_tv
, E_pci_Z_header_Z_subclass_satellite_comm_S_audio
, E_pci_Z_header_Z_subclass_satellite_comm_S_voice
, E_pci_Z_header_Z_subclass_satellite_comm_S_data
};
enum E_pci_Z_header_Z_subclass_encryption
{ E_pci_Z_header_Z_subclass_encryption_S_network_computing
, E_pci_Z_header_Z_subclass_encryption_S_entertainment
, E_pci_Z_header_Z_subclass_encryption_S_other = 0x80
};
enum E_pci_Z_header_Z_subclass_signal_processing
{ E_pci_Z_header_Z_subclass_signal_processing_S_dpio_modules
, E_pci_Z_header_Z_subclass_signal_processing_S_performance_counters
, E_pci_Z_header_Z_subclass_signal_processing_S_comm_synchronizer = 0x10
, E_pci_Z_header_Z_subclass_signal_processing_S_management = 0x20
, E_pci_Z_header_Z_subclass_signal_processing_S_other = 0x80
};
union __attribute__ (( __packed__ )) E_pci_Z_bar
{ struct __attribute__ (( __packed__ ))
  { N32 bar_type    :1; // 0
    N32 type        :2;
    N32 prefechable :1;
    N32 base        :28;
  }memory;
  struct __attribute__ (( __packed__ ))
  { N32 bar_type    :1; // 1
    N32 reserved    :1;
    N32 base        :30;
  }io;
};
struct __attribute__ (( __packed__ )) E_pci_Z_header_Z_command
{ N16 io_space          :1;
  N16 memory_space      :1;
  N16 bus_master        :1;
  N16 special_cycles    :1;
  N16 memory_w_inv      :1;
  N16 vga_snoop         :1;
  N16 pairty_err_resp   :1;
  N16 reserved          :1;
  N16 serr              :1;
  N16 fast_back2back    :1;
  N16 interrupt_disable :1;
};
struct __attribute__ (( __packed__ )) E_pci_Z_header_Z_status
{ N16 reserved_1            :3;
  N16 interrupt             :1;
  N16 cap_list              :1;
  N16 _66_cap               :1;
  N16 reserved_2            :1;
  N16 fast_back2_back_cap   :1;
  N16 master_data_parity_err:1;
  N16 devsel_timing         :2;
  N16 signaled_target_abort :1;
  N16 received_target_abort :1;
  N16 received_master_abort :1;
  N16 signaled_system_err   :1;
  N16 detected_parity_err   :1;
};
struct __attribute__ (( __packed__ )) E_pci_Z_header_Z_bist
{ N8 completion_code    :4;
  N8 reserved           :2;
  N8 start              :1;
  N8 capable            :1;
};
//==============================================================================
extern N32 E_main_S_sata_ahci_addresses[8];
extern N8 E_main_S_sata_ahci_n;
//==============================================================================
extern N32 E_main_I_in_32( N16 );
extern void E_main_I_out_32( N16, N32 );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N E_pci_I_check_bus( N8 );
//==============================================================================
N32
E_pci_I_read( N8 bus
, N8 device
, N8 function
, N8 offset
){  E_main_I_out_32( 0xcf8, 0x80000000 | ( bus << 16 ) | ( device << 11 ) | ( function << 8 ) | offset );
    return E_main_I_in_32( 0xcfc );
}
void
E_pci_I_write( N8 bus
, N8 device
, N8 function
, N8 offset
, N32 value
){  E_main_I_out_32( 0xcf8, 0x80000000 | ( bus << 16 ) | ( device << 11 ) | ( function << 8 ) | offset );
    E_main_I_out_32( 0xcfc, value );
}
N
E_pci_I_check_device( N8 bus_i
, N8 device_i
, N8 function_i
, N32 ids
){  N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    N8 subclass = ( rev_prog_sub_class >> 16 ) & 0xff;
    if( class == E_pci_Z_header_Z_class_S_mass_storage
    && subclass == E_pci_Z_header_Z_subclass_mass_storage_S_sata
    )
    {   if( E_main_S_sata_ahci_n == J_a_R_n( E_main_S_sata_ahci_addresses ))
            return ~0;
        E_main_S_sata_ahci_addresses[ E_main_S_sata_ahci_n++ ] = E_pci_I_read( bus_i, device_i, function_i, 0x24 );
    }
    switch(ids)
    { case 0ULL: // Padding to avoid empty switch if needed, or keep existing cases.
        break;
    }
    N32 bist_24 = E_pci_I_read( bus_i, device_i, function_i, 0xc );
    struct E_pci_Z_header_Z_bist *bist = ( struct E_pci_Z_header_Z_bist * )(( N8 * )&bist_24 + 3 );
    if( bist->capable )
    {   bist->start = yes;
        E_pci_I_write( bus_i, device_i, function_i, 0xc, bist_24 );
    }
    return 0;
}
N
E_pci_I_check_function( N8 bus_i
, N8 device_i
, N8 function_i
){  N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    N8 subclass = ( rev_prog_sub_class >> 16 ) & 0xff;
    if( class == E_pci_Z_header_Z_class_S_bridge
    && subclass == E_pci_Z_header_Z_subclass_bridge_S_pci2pci
    )
    {   N32 buses_latency = E_pci_I_read( bus_i, device_i, function_i, 0x18 );
        N8 secondary_bus = ( buses_latency >> 8 ) & 0xff;
        N r = E_pci_I_check_bus( secondary_bus );
        if(r)
            return r;
    }
    return 0;
}
N
E_pci_I_check_bus( N8 bus_i
){  static N8 bus_mask[ 256 / 8 ];
    if( bus_mask[ bus_i / 8 ] & ( 1 << ( bus_i % 8 )))
        return 0;
    bus_mask[ bus_i / 8 ] |= ( 1 << ( bus_i % 8 ));
    for_n( device_i, 32 )
    {   N32 ids = E_pci_I_read( bus_i, device_i, 0, 0 );
        if( !~ids )
            continue;
        N r = E_pci_I_check_device( bus_i, device_i, 0, ids );
        if(r)
            return r;
        r = E_pci_I_check_function( bus_i, device_i, 0 );
        if(r)
            return r;
        N8 header_type = E_pci_I_read( bus_i, device_i, 0, 0xe );
        if( header_type & 0x80 )
        {   for_n( function_i, 7 )
            {   ids = E_pci_I_read( bus_i, device_i, 1 + function_i, 0 );
                if( ~ids )
                {   r = E_pci_I_check_device( bus_i, device_i, 1 + function_i, ids );
                    if(r)
                        return r;
                    r = E_pci_I_check_function( bus_i, device_i, 1 + function_i );
                    if(r)
                        return r;
                }
            }
        }
    }
    return 0;
}
N
E_pci_I_check_buses( void
){  N8 header_type = E_pci_I_read( 0, 0, 0, 0xe );
    N r;
    if( header_type & 0x80 )
    {   for_n( function_i, 8 )
        {   r = E_pci_I_check_bus( function_i );
            if(r)
                goto End;
        }
    }else
        r = E_pci_I_check_bus(0);
End:return r;
}
/******************************************************************************/
