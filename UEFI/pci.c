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
extern struct H_uefi_Z_memory_type_descriptor *E_main_S_memory_map;
extern N E_main_S_descriptor_l;
//------------------------------------------------------------------------------
extern N32 E_main_S_sata_ahci_addresses[8];
extern N8 E_main_S_sata_ahci_n;
extern N64 E_main_S_ethernet_address, E_main_S_ethernet_eeprom_address;
//==============================================================================
extern N32 E_main_I_in_32( N16 );
extern void E_main_I_out_32( N16, N32 );
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N E_pci_I_check_bus_0( N8, Pc );
N E_pci_I_check_bus( N *, N8, Pc );
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
E_pci_I_check_device_0( N8 bus_i
, N8 device_i
, N8 function_i
, N8 header_type
){  // Obliczenie liczby koniecznych do dodania bloków pamięci MMIO do mapy pamięci.
    N n = 0;
    N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    switch( header_type )
    { case E_pci_Z_header_Z_header_type_S_general:
        {   if( class != E_pci_Z_header_Z_class_S_bridge )
            {   N8 offset = 0x10;
                do
                {   N64 address = E_pci_I_read( bus_i, device_i, function_i, offset );
                    if( !address
                    || ( address & 1 ) // I/O space
                    )
                        continue;
                    if(( address & 6 ) == 4 )
                        offset += sizeof( N32 );
                    n++;
                }while(( offset += sizeof( N32 )) != 0x28 );
            }
            break;
        }
      case E_pci_Z_header_Z_header_type_S_pci2pci_bridge:
        {   N8 offset = 0x10;
            do
            {   N64 address = E_pci_I_read( bus_i, device_i, function_i, offset );
                if( !address
                || ( address & 1 ) // I/O space
                )
                    continue;
                if(( address & 6 ) == 4 )
                    offset += sizeof( N32 );
                n++;
            }while(( offset += sizeof( N32 )) != 0x18 );
            N64 base = E_pci_I_read( bus_i, device_i, function_i, 0x20 );
            N64 limit = ( base >> 16 ) & 0xfff0;
            base &= 0xfff0;
            if( base
            && limit
            && base <= limit
            )
                n++;
            base = E_pci_I_read( bus_i, device_i, function_i, 0x24 );
            limit = ( base >> 16 ) & 0xfff0;
            if(( base & 0xf ) == 1 )
            {   base |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, 0x28 ) << 16;
                limit |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, 0x2c ) << 16;
            }
            base &= 0xfff0;
            if( base
            && limit
            && base <= limit
            )
                n++;
            break;
        }
    }
    return n;
}
N
E_pci_I_check_function_0( N8 bus_i
, N8 device_i
, N8 function_i
, Pc bus_mask
){  N n = 0;
    N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    N8 subclass = ( rev_prog_sub_class >> 16 ) & 0xff;
    if( class == E_pci_Z_header_Z_class_S_bridge
    && subclass == E_pci_Z_header_Z_subclass_bridge_S_pci2pci
    )
    {   N32 buses_latency = E_pci_I_read( bus_i, device_i, function_i, 0x18 );
        N8 secondary_bus = ( buses_latency >> 8 ) & 0xff;
        n = E_pci_I_check_bus_0( secondary_bus, bus_mask );
        K(n)
            return ~0;
    }
    return n;
}
N
E_pci_I_check_bus_0( N8 bus_i
, Pc bus_mask
){  if( E_mem_Q_mask_R( bus_mask, bus_i ))
        return 0;
    E_mem_Q_mask_P_set( bus_mask, bus_i );
    N n = 0;
    for_n( device_i, 32 )
    {   N32 ids = E_pci_I_read( bus_i, device_i, 0, 0 );
        if( !~ids )
            continue;
        N8 header_type = E_pci_I_read( bus_i, device_i, 0, 0xc ) >> 16;
        N r = E_pci_I_check_device_0( bus_i, device_i, 0, header_type & 0x7f );
        K(r)
            return ~0;
        n += r;
        r = E_pci_I_check_function_0( bus_i, device_i, 0, bus_mask );
        K(r)
            return ~0;
        n += r;
        if( header_type & 0x80 )
        {   for_n( function_i, 7 )
            {   ids = E_pci_I_read( bus_i, device_i, 1 + function_i, 0 );
                if( ~ids )
                {   r = E_pci_I_check_device_0( bus_i, device_i, 1 + function_i, header_type & 0x7f );
                    K(r)
                        return ~0;
                    n += r;
                    r = E_pci_I_check_function_0( bus_i, device_i, 1 + function_i, bus_mask );
                    K(r)
                        return ~0;
                    n += r;
                }
            }
        }
    }
    return n;
}
N
E_pci_I_check_buses_0( void
){  C bus_mask[ 256 / 8 ];
    _0( &bus_mask[0], 256 / 8 );
    N n = 0;
    N8 header_type = E_pci_I_read( 0, 0, 0, 0xc ) >> 16;
    if( header_type & 0x80 )
    {   for_n( function_i, 8 )
        {   N r = E_pci_I_check_bus_0( function_i, bus_mask );
            K(r)
                return ~0;
            n += r;
        }
    }else
    {   N r = E_pci_I_check_bus_0( 0, bus_mask );
        K(r)
            return ~0;
        n += r;
    }
    return n;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_pci_I_check_device(
  N *memory_map_l
, N8 bus_i
, N8 device_i
, N8 function_i
, N8 header_type
, N32 ids
){  // Uruchomienie testu urządzenia.
    N32 bist_24 = E_pci_I_read( bus_i, device_i, function_i, 0xc );
    struct E_pci_Z_header_Z_bist *bist = ( struct E_pci_Z_header_Z_bist * )(( N8 * )&bist_24 + 3 );
    if( bist->capable )
    {   bist->start = yes;
        E_pci_I_write( bus_i, device_i, function_i, 0xc, bist_24 );
    }
    // Dodanie adresów bloków pamięci MMIO do mapy pamięci.
    N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    N8 subclass = ( rev_prog_sub_class >> 16 ) & 0xff;
    struct H_uefi_Z_memory_type_descriptor *memory_map = (P)(( Pc )E_main_S_memory_map + *memory_map_l );
    switch( header_type )
    { case E_pci_Z_header_Z_header_type_S_general:
        {   if( class != E_pci_Z_header_Z_class_S_bridge )
            {   N32 command_status = E_pci_I_read( bus_i, device_i, function_i, 4 );
                if( command_status & ( 1 << 1 ))
                    E_pci_I_write( bus_i, device_i, function_i, 4, command_status & ~( 1 << 1 )); // Wyłącza reakcję na dostęp do MMIO.
                N8 offset = 0x10;
                do
                {   N64 address = E_pci_I_read( bus_i, device_i, function_i, offset );
                    if( !address
                    || ( address & 1 ) // I/O space
                    )
                        continue;
                    E_pci_I_write( bus_i, device_i, function_i, offset, ~0 );
                    N64 size = E_pci_I_read( bus_i, device_i, function_i, offset );
                    E_pci_I_write( bus_i, device_i, function_i, offset, address );
                    if(( address & 6 ) == 4 )
                    {   offset += sizeof( N32 );
                        N32 bar = E_pci_I_read( bus_i, device_i, function_i, offset );
                        address |= ( N64 )bar << 32;
                        E_pci_I_write( bus_i, device_i, function_i, offset, ~0 );
                        size |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, offset ) << 32;
                        E_pci_I_write( bus_i, device_i, function_i, offset, bar );
                    }else
                        size |= 0xffffffff00000000ULL;
                    address = address & ~0xfULL;
                    size = ~( size & ~0xfULL ) + 1;
                    memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
                    memory_map->physical_start = address;
                    memory_map->pages = E_simple_Z_n_I_align_up_to_v2( address % H_oux_E_mem_S_page_size + size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size;
                    *memory_map_l += E_main_S_descriptor_l;
                    memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
                }while(( offset += sizeof( N32 )) != 0x28 );
                if( command_status & ( 1 << 1 ))
                    E_pci_I_write( bus_i, device_i, function_i, 4, command_status );
            }
            break;
        }
      case E_pci_Z_header_Z_header_type_S_pci2pci_bridge:
        {   N32 command_status = E_pci_I_read( bus_i, device_i, function_i, 4 );
            E_pci_I_write( bus_i, device_i, function_i, 4, command_status & ~( 1 << 1 )); // Wyłącza reakcję na dostęp do MMIO.
            N8 offset = 0x10;
            do
            {   N64 address = E_pci_I_read( bus_i, device_i, function_i, offset );
                if( !address
                || ( address & 1 ) // I/O space
                )
                    continue;
                E_pci_I_write( bus_i, device_i, function_i, offset, ~0 );
                N64 size = E_pci_I_read( bus_i, device_i, function_i, offset );
                E_pci_I_write( bus_i, device_i, function_i, offset, address );
                if(( address & 6 ) == 4 )
                {   offset += sizeof( N32 );
                    N32 bar = E_pci_I_read( bus_i, device_i, function_i, offset );
                    address |= ( N64 )bar << 32;
                    E_pci_I_write( bus_i, device_i, function_i, offset, ~0 );
                    size |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, offset ) << 32;
                    E_pci_I_write( bus_i, device_i, function_i, offset, bar );
                }else
                    size |= 0xffffffff00000000ULL;
                address = address & ~0xfULL;
                size = ~( size & ~0xfULL ) + 1;
                memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
                memory_map->physical_start = address;
                memory_map->pages = E_simple_Z_n_I_align_up_to_v2( address % H_oux_E_mem_S_page_size + size, H_oux_E_mem_S_page_size ) / H_oux_E_mem_S_page_size;
                *memory_map_l += E_main_S_descriptor_l;
                memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
            }while(( offset += sizeof( N32 )) != 0x18 );
            E_pci_I_write( bus_i, device_i, function_i, 4, command_status );
            N64 base = E_pci_I_read( bus_i, device_i, function_i, 0x20 );
            N64 limit = ( base >> 16 ) & 0xfff0;
            base &= 0xfff0;
            if( base
            && limit
            && base <= limit
            )
            {   base <<= 16;
                limit <<= 16;
                limit += 0x100000;
                memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
                memory_map->physical_start = base;
                memory_map->pages = ( limit - base ) / H_oux_E_mem_S_page_size;
                *memory_map_l += E_main_S_descriptor_l;
                memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
            }
            base = E_pci_I_read( bus_i, device_i, function_i, 0x24 );
            limit = ( base >> 16 ) & 0xfff0;
            if(( base & 0xf ) == 1 )
            {   base |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, 0x28 ) << 16;
                limit |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, 0x2c ) << 16;
            }
            base &= 0xfff0;
            if( base
            && limit
            && base <= limit
            )
            {   base <<= 16;
                limit <<= 16;
                limit += 0x100000;
                memory_map->type = H_uefi_Z_memory_type_S_memory_mapped_io;
                memory_map->physical_start = base;
                memory_map->pages = ( limit - base ) / H_oux_E_mem_S_page_size;
                *memory_map_l += E_main_S_descriptor_l;
                memory_map = (P)(( Pc )memory_map + E_main_S_descriptor_l );
            }
            break;
        }
    }
    // Przygotowanie adresów MMIO wybranych urządzeń.
    if( class == E_pci_Z_header_Z_class_S_mass_storage
    && subclass == E_pci_Z_header_Z_subclass_mass_storage_S_sata
    )
    {   if( E_main_S_sata_ahci_n == J_a_R_n( E_main_S_sata_ahci_addresses ))
            return ~0;
        E_main_S_sata_ahci_addresses[ E_main_S_sata_ahci_n++ ] = E_pci_I_read( bus_i, device_i, function_i, 0x24 ) & ~0xf;
    }
    switch(ids)
    { case 0x100f8086:
        {   N8 offset = 0x10;
            E_main_S_ethernet_address = E_pci_I_read( bus_i, device_i, function_i, offset );
            if(( E_main_S_ethernet_address & 6 ) == 4 ) //NDFN Czy jest sens sprawdzać dla tego konkretnego urządzenia?
            {   offset += sizeof( N32 );
                E_main_S_ethernet_address |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, offset ) << 32;
            }
            E_main_S_ethernet_address &= ~0xfULL;
            offset += sizeof( N32 );
            E_main_S_ethernet_eeprom_address = E_pci_I_read( bus_i, device_i, function_i, offset );
            if(( E_main_S_ethernet_eeprom_address & 6 ) == 4 ) //NDFN Czy jest sens sprawdzać dla tego konkretnego urządzenia?
            {   offset += sizeof( N32 );
                E_main_S_ethernet_eeprom_address |= ( N64 )E_pci_I_read( bus_i, device_i, function_i, offset ) << 32;
            }
            E_main_S_ethernet_eeprom_address &= ~0xfULL;
            break;
        }
    }
    return 0;
}
N
E_pci_I_check_function(
  N *memory_map_l
, N8 bus_i
, N8 device_i
, N8 function_i
, Pc bus_mask
){  N32 rev_prog_sub_class = E_pci_I_read( bus_i, device_i, function_i, 8 );
    N8 class = rev_prog_sub_class >> 24;
    N8 subclass = ( rev_prog_sub_class >> 16 ) & 0xff;
    if( class == E_pci_Z_header_Z_class_S_bridge
    && subclass == E_pci_Z_header_Z_subclass_bridge_S_pci2pci
    )
    {   N32 buses_latency = E_pci_I_read( bus_i, device_i, function_i, 0x18 );
        N8 secondary_bus = ( buses_latency >> 8 ) & 0xff;
        K( E_pci_I_check_bus( memory_map_l, secondary_bus, bus_mask ))
            return ~0;
    }
    return 0;
}
N
E_pci_I_check_bus(
  N *memory_map_l
, N8 bus_i
, Pc bus_mask
){  if( E_mem_Q_mask_R( bus_mask, bus_i ))
        return 0;
    E_mem_Q_mask_P_set( bus_mask, bus_i );
    for_n( device_i, 32 )
    {   N32 ids = E_pci_I_read( bus_i, device_i, 0, 0 );
        if( !~ids )
            continue;
        N8 header_type = E_pci_I_read( bus_i, device_i, 0, 0xc ) >> 16;
        K( E_pci_I_check_device( memory_map_l, bus_i, device_i, 0, header_type & 0x7f, ids ))
            return ~0;
        K( E_pci_I_check_function( memory_map_l, bus_i, device_i, 0, bus_mask ))
            return ~0;
        if( header_type & 0x80 )
        {   for_n( function_i, 7 )
            {   ids = E_pci_I_read( bus_i, device_i, 1 + function_i, 0 );
                if( ~ids )
                {   K( E_pci_I_check_device( memory_map_l, bus_i, device_i, 1 + function_i, header_type & 0x7f, ids ))
                        return ~0;
                    K( E_pci_I_check_function( memory_map_l, bus_i, device_i, 1 + function_i, bus_mask ))
                        return ~0;
                }
            }
        }
    }
    return 0;
}
N
E_pci_I_check_buses(
  N *memory_map_l
){  C bus_mask[ 256 / 8 ];
    _0( &bus_mask[0], 256 / 8 );
    N8 header_type = E_pci_I_read( 0, 0, 0, 0xc ) >> 16;
    if( header_type & 0x80 )
    {   for_n( function_i, 8 )
        {   K( E_pci_I_check_bus( memory_map_l, function_i, bus_mask ))
                return ~0;
        }
    }else
    {   K( E_pci_I_check_bus( memory_map_l, 0, bus_mask ))
            return ~0;
    }
    return 0;
}
/******************************************************************************/
