/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         SATA AHCI driver
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”             2025‒11‒7 d
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
#define E_sata_S_interrupt_status_clear_mask    0xfd8000af
//==============================================================================
enum E_sata_Z_ata_command
{ E_sata_Z_ata_command_S_data_set_management = 6
, E_sata_Z_ata_command_S_read_fpdma_queued = 0x60
, E_sata_Z_ata_command_S_write_fpdma_queued = 0x61
, E_sata_Z_ata_command_S_identify_packet_device = 0xa1
, E_sata_Z_ata_command_S_flush_cache_ext = 0xea
, E_sata_Z_ata_command_S_identify_device = 0xec
};
enum E_sata_Z_fis_type
{ E_sata_Z_fis_type_S_register_h2d = 0x27
, E_sata_Z_fis_type_S_register_d2h = 0x34
, E_sata_Z_fis_type_S_dma_activate = 0x39
, E_sata_Z_fis_type_S_dma_setup = 0x41
, E_sata_Z_fis_type_S_data = 0x46
, E_sata_Z_fis_type_S_bist = 0x58
, E_sata_Z_fis_type_S_pio_setup = 0x5f
, E_sata_Z_fis_type_S_device_bits = 0xa1
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_register_h2d
{ N8 type;
  N8 multiplier     :4;
  N8 reserved_1     :3;
  N8 control_command:1;
  N8 command;
  N8 feature_low;
  N8 lba_0, lba_1, lba_2;
  N8 device;
  N8 lba_3, lba_4, lba_5;
  N8 feature_high;
  N16 count;
  N8 isochronous_completion;
  N8 control;
  N16 auxiliary;
  N16 reserved_2;
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_register_d2h
{ N8 type;
  N8 multiplier :4;
  N8 reserved_1 :2;
  N8 interrupt  :1;
  N8 reserved_2 :1;
  N8 status;
  N8 error;
  N8 lba_0, lba_1, lba_2;
  N8 device;
  N8 lba_3, lba_4, lba_5;
  N8 reserved_3;
  N16 count;
  N8 reserved_4[6];
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_data
{ N8 type;
  N8 multiplier :4;
  N8 reserved_1 :4;
  N8 reserved_2[2];
  N32 data[];
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_pio_setup
{ N8 type;
  N8 multiplier :4;
  N8 reserved_1 :1;
  N8 direction  :1;
  N8 interrupt  :1;
  N8 reserved_2 :1;
  N8 status;
  N8 error;
  N8 lba_0, lba_1, lba_2;
  N8 device;
  N8 lba_3, lba_4, lba_5;
  N8 reserved_3;
  N16 count;
  N8 reserved_4;
  N8 status_new;
  N16 transfer_count;
  N16 reserved_5;
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_dma_setup
{ N8 type;
  N8 multiplier     :4;
  N8 reserved_1     :1;
  N8 direction      :1;
  N8 interrupt      :1;
  N8 auto_activate  :1;
  N16 reserved_2;
  N64 buffer_id;
  N32 reserved_3;
  N32 buffer_offset;
  N32 transfer_count;
  N32 reserved_4;
};
struct __attribute__ (( __packed__ )) E_sata_Z_fis_device_bits //NDFN
{ N8 type;
  N8 multiplier     :4;
  N8 reserved_1     :2;
  N8 interrupt      :1;
  N8 notification   :1;
  N8 status_low     :3;
  N8 reserved_2     :1;
  N8 status_high    :3;
  N8 reserved_3     :1;
  N8 error;
  N32 protocol_specific;
};
struct __attribute__ (( __packed__ )) E_sata_Z_recv_fis
{ struct E_sata_Z_fis_dma_setup dma_setup;
  N8 pad_1[4];
  struct E_sata_Z_fis_pio_setup pio_setup;
  N8 pad_2[12];
  struct E_sata_Z_fis_register_d2h register_d2h;
  N8 pad_3[4];
  struct E_sata_Z_fis_device_bits device_bits;
  N8 unknown_fis[64];
  N8 reserved[ 0x100 - 0xa0 ];
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) E_sata_Z_command_header
{ N8 command_fis_length :5;
  N8 atapi              :1;
  N8 write              :1;
  N8 prefechable        :1;
  N8 reset              :1;
  N8 bist               :1;
  N8 clear_busy         :1;
  N8 reserved_1         :1;
  N8 multiplier_port    :4;
  N16 region_table_length;
  volatile N32 region_table_transferred;
  N64 command_table_base;
  N32 reserved_2[4];
};
struct __attribute__ (( __packed__ )) E_sata_Z_region_table
{ N64 data_base_address;
  N32 reserved_1;
  N32 data_count                :22;
  N32 reserved_2                :9;
  N32 interrupt_on_completion   :1;
};
struct __attribute__ (( __packed__ )) E_sata_Z_command_table
{ struct E_sata_Z_fis_register_h2d command_fis;
  N8 reserved_1[ 0x40 - sizeof( struct E_sata_Z_fis_register_h2d ) ];
  N8 atapi_command[ 0x50 - 0x40 ];
  N8 reserved_2[ 0x80 - 0x50 ];
  struct E_sata_Z_region_table region[1];
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) E_sata_Z_port
{ N64 command_list_base;
  N64 recv_fis_base;
  N32 interrupt_status, interrupt_enable;
  N32 command_status;
  N32 reserved_1;
  N32 task_file_data;
  N32 signature;
  N32 sata_status;
  N32 sata_control;
  N32 sata_error;
  N32 sata_active;
  N32 command_issue;
  N32 sata_notification;
  N32 fis_based_switch_control;
  N32 reserved_2[11];
  N32 vendor[4];
};
struct __attribute__ (( __packed__ )) E_sata_Z_memory
{ N32 host_cap;
  N32 global_host_control;
  N32 interrupt_status;
  N32 port_implemented;
  N32 version;
  N32 command_completion_control;
  N32 command_completion_ports;
  N32 enclosure_management_location;
  N32 enclosure_management_control;
  N32 host_cap_2;
  N32 bios_os_handoff_control_status;
  N8 reserved[ 0xa0 - 0x2c ];
  N8 vendor[ 0x100 - 0xa0 ];
  struct E_sata_Z_port port[];
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct __attribute__ (( __packed__ )) E_sata_Z_dsm_range
{ N64 lba       :48;
  N64 count     :16;
};
//==============================================================================
extern struct E_disc_Z E_disc_S;
//==============================================================================
void
E_sata_I_interrupt( void
){  if( E_disc_S.type == E_disc_Z_type_S_satapi_ahci )
    {   if( !( E_disc_S.satapi_ahci.memory->interrupt_status & ( 1 << E_disc_S.satapi_ahci.port )))
            return;
        E_disc_S.satapi_ahci.memory->port[ E_disc_S.satapi_ahci.port ].interrupt_status = E_sata_S_interrupt_status_clear_mask;
        E_disc_S.satapi_ahci.memory->interrupt_status = 1 << E_disc_S.satapi_ahci.port;
    }else
    {   if( !( E_disc_S.sata_ahci.memory->interrupt_status & ( 1 << E_disc_S.sata_ahci.port )))
            return;
        E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].interrupt_status = E_sata_S_interrupt_status_clear_mask;
        E_disc_S.sata_ahci.memory->interrupt_status = 1 << E_disc_S.sata_ahci.port;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_sata_I_init( volatile struct E_sata_Z_memory *sata_memory
){  if( !( sata_memory->host_cap & ( 1 << 31 )) // 64‐bit addressing
    || !( sata_memory->host_cap & ( 1 << 30 )) // NCQ
    )
        return ~0;
    if( !( sata_memory->global_host_control & ( 1 << 31 ))) // AHCI enable
    {   sata_memory->global_host_control |= 1 << 31;
        if( !( sata_memory->global_host_control & ( 1 << 31 )))
            return ~0;
    }else if( sata_memory->host_cap_2 & 1 ) // BIOS/OS handoff
    {   sata_memory->bios_os_handoff_control_status |= 1 << 1; // OS owned semaphore
        while(( sata_memory->bios_os_handoff_control_status & 1 ) // BIOS owned semaphore
        && !( sata_memory->bios_os_handoff_control_status & ( 1 << 4 )) // BIOS busy
        )
            __asm__ volatile (
            "\n" "pause"
            );
        if( sata_memory->bios_os_handoff_control_status & ( 1 << 4 )) // BIOS busy
            while( sata_memory->bios_os_handoff_control_status & 1 ) // BIOS owned semaphore
                __asm__ volatile (
                "\n" "pause"
                );
    }
    sata_memory->global_host_control |= 1; // reset
    while( sata_memory->global_host_control & 1 )
        __asm__ volatile (
        "\n" "pause"
        );
    if( !( sata_memory->global_host_control & ( 1 << 31 )))
    {   sata_memory->global_host_control |= 1 << 31;
        if( !( sata_memory->global_host_control & ( 1 << 31 )))
            return ~0;
    }
    N32 port_implemented = sata_memory->port_implemented;
    N8 port_n = ( sata_memory->host_cap & 0x1f ) + 1;
    for_n( port, port_n )
    {   if( !( port_implemented & 1 )
        || !( sata_memory->port[port].sata_status & 0xf )
        || ( ( sata_memory->host_cap & ( 1 << 28 )) // mechanical presence switch
          && ( sata_memory->port[port].command_status & ( 1 << 19 )) // mechanical presence switch attached to port
          && ( sata_memory->port[port].command_status & ( 1 << 13 )) // mechanical presence switch state
        ))
            goto Next_port_0;
        if( sata_memory->host_cap & ( 1 << 27 )) // staggered spin‐up
        {   sata_memory->port[port].command_status |= ( 1 << 1 ); // spin‐up device
            if( sata_memory->port[port].sata_error & ~( 1 << 16 ))
                goto Next_port_0;
        }
        while(( sata_memory->port[port].sata_status & 0xf ) != 3 )
            __asm__ volatile (
            "\n" "pause"
            );
        sata_memory->port[port].sata_error = ~0;
        N command_list_n = (( sata_memory->host_cap >> 8 ) & 0x1f ) + 1;
        struct E_sata_Z_command_header *command_list = E_mem_Q_blk_Z_single_memory_M_align( command_list_n * sizeof( struct E_sata_Z_command_header ), 1024 );
        Kp_( ~2, command_list );
        _0( command_list, command_list_n * sizeof( struct E_sata_Z_command_header ));
        struct E_sata_Z_command_table *command_table_0;
        for_n( i, command_list_n )
        {   command_list[i].command_fis_length = sizeof( struct E_sata_Z_fis_register_h2d ) / sizeof( N32 );
            command_list[i].region_table_length = 1;
            struct E_sata_Z_command_table *command_table = E_mem_Q_blk_Z_single_memory_M_align( sizeof( struct E_sata_Z_command_table ), 128 );
            Kp_( ~2, command_table );
            _0_( command_table );
            if( !i )
                command_table_0 = command_table;
            command_table->command_fis.type = E_sata_Z_fis_type_S_register_h2d;
            command_table->command_fis.control_command = yes;
            command_list[i].command_table_base = (N)E_main_Z_p_I_to_physical( command_table );
        }
        command_table_0->region[0].interrupt_on_completion = no;
        struct E_sata_Z_recv_fis *recv_fis = E_mem_Q_blk_Z_single_memory_M_align( sizeof( struct E_sata_Z_recv_fis ), 256 );
        Kp_( ~2, recv_fis );
        sata_memory->port[port].command_list_base = (N)E_main_Z_p_I_to_physical( command_list );
        sata_memory->port[port].recv_fis_base = (N)E_main_Z_p_I_to_physical( recv_fis );
        sata_memory->port[port].command_status |= 1 << 4; // fis receive enable
        if( sata_memory->port[port].sata_error )
            goto Next_port_0;
        E_disc_S.sata_ahci.memory = sata_memory;
        E_disc_S.sata_ahci.port = port;
        E_disc_S.type = E_disc_Z_type_S_unknown;
        sata_memory->port[port].interrupt_enable = 0x7dc0007f;
        sata_memory->global_host_control |= 1 << 1; // interrupt enable
        while( sata_memory->port[port].task_file_data & (( 1 << 7 ) | ( 1 << 3 )))
            __asm__ volatile (
            "\n" "pause"
            );
        sata_memory->port[port].command_status |= 1; // start
        if( sata_memory->port[port].sata_error )
            goto Next_port_1;
        // IDENTIFY DEVICE
        command_table_0->command_fis.command = E_sata_Z_ata_command_S_identify_device;
        N16 *identify = E_mem_Q_blk_Z_single_memory_M_align( 256 * sizeof( N16 ), sizeof( N16 ));
        Kp_( ~2, identify );
        command_table_0->region[0].data_base_address = (N)E_main_Z_p_I_to_physical(identify);
        command_table_0->region[0].data_count = 256 * sizeof( N16 ) - 1;
        while( sata_memory->port[port].task_file_data & (( 1 << 7 ) | ( 1 << 3 )))
            __asm__ volatile (
            "\n" "pause"
            );
        sata_memory->port[port].command_issue = 1;
        do
            __asm__ volatile (
            "\n" "pause"
            );
        while( sata_memory->port[port].command_issue & 1 );
        switch( sata_memory->port[port].signature )
        { case 0x101:
                E_disc_S.type = E_disc_Z_type_S_sata_ahci;
                if( !( identify[83] & ( 1 << 10 ))) // 48‐bit addressable
                    goto Next_port_2;
                E_disc_S.sata_ahci.logical_sector_n = *( N64 * )&identify[100];
                if( E_disc_S.sata_ahci.logical_sector_n == 0xffffffffffffUL )
                {   //TODO Odczytać ze SMART.
                    return ~0;
                }
                if( identify[106] & ( 1 << 12 )) // Logiczny sektor większy niż 512.
                {   E_disc_S.sata_ahci.logical_sector_size = *( N32 * )&identify[117] / sizeof( N16 );
                    if( E_disc_S.sata_ahci.logical_sector_size < 512
                    || !E_simple_Z_n_T_power_2( E_disc_S.sata_ahci.logical_sector_size )
                    )
                        goto Next_port_2;
                }else
                    E_disc_S.sata_ahci.logical_sector_size = 512;
                if( identify[106] & ( 1 << 13 )) // Wiele logicznych sektorów w fizycznym sektorze.
                {   E_disc_S.sata_ahci.physical_sector_size = ( 1 << ( identify[106] & 0xf )) * E_disc_S.sata_ahci.logical_sector_size;
                    E_disc_S.sata_ahci.logical_sector_shift = identify[209] & 0x3fff;
                    if( E_disc_S.sata_ahci.logical_sector_shift >= E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size )
                        goto Next_port_2;
                }else
                {   E_disc_S.sata_ahci.physical_sector_size = E_disc_S.sata_ahci.logical_sector_size;
                    E_disc_S.sata_ahci.logical_sector_shift = 0;
                }
                E_disc_S.sata_ahci.queue_n = ( *( N8 * )&identify[75] & 0x1f ) + 1;
                E_disc_S.sata_ahci.max_sectors = ( 1 << 22 ) / E_disc_S.sata_ahci.logical_sector_size;
                break;
         case 0xeb140101:
                E_disc_S.satapi_ahci.memory = sata_memory;
                E_disc_S.satapi_ahci.port = port;
                E_disc_S.type = E_disc_Z_type_S_satapi_ahci;
                sata_memory->port[port].command_status |= 1 << 24; // ATAPI
                // IDENTIFY PACKET DEVICE
                command_table_0->command_fis.command = E_sata_Z_ata_command_S_identify_packet_device;
                command_table_0->region[0].data_base_address = (N)E_main_Z_p_I_to_physical(identify);
                command_table_0->region[0].data_count = 256 * sizeof( N16 ) - 1;
                while( sata_memory->port[port].task_file_data & (( 1 << 7 ) | ( 1 << 3 )))
                    __asm__ volatile (
                    "\n" "pause"
                    );
                sata_memory->port[port].command_issue = 1;
                do
                    __asm__ volatile (
                    "\n" "pause"
                    );
                while( sata_memory->port[port].command_issue & 1 );
                break;
         case 0xc33c0101:
                goto Next_port_2;
         case 0x96690101:
                goto Next_port_2;
         default:
                goto Next_port_2;
        }
        K( E_disc_I_init() )
        {   K_( ~1, W(identify) );
            return ~0;
        }
Next_port_2:
        K_( ~1, W(identify) );
Next_port_1:
        sata_memory->global_host_control &= ~( 1 << 1 ); // interrupt disable
Next_port_0:
        port_implemented >>= 1;
    }
    sata_memory->global_host_control |= 1 << 1; // interrupt enable
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_sata_Q_sata_ahci_I_wait_on_completion( N32 slot_mask
){  while( E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].sata_active & slot_mask )
        __asm__ volatile (
        "\n" "pause"
        );
}
//------------------------------------------------------------------------------
N8
E_sata_Q_disc_device_R_free_command_slot( void
){  for_n( i, E_disc_S.sata_ahci.queue_n )
        if( !( E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].sata_active & ( 1 << i )))
            return i;
    return ~0;
}
N
E_sata_Q_sata_ahci_I_read(
  N lba
, N logical_sector_count
, P buffer
, N32 *slot_mask
){  O{  N8 command_slot_i = E_sata_Q_disc_device_R_free_command_slot();
        while( !~command_slot_i )
        {   __asm__ volatile (
            "\n" "pause"
            );
            command_slot_i = E_sata_Q_disc_device_R_free_command_slot();
        }
        while( E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].task_file_data & (( 1 << 7 ) | ( 1 << 3 ))) // BSY lub DRQ
            __asm__ volatile (
            "\n" "pause"
            );
        struct E_sata_Z_command_header *command_list = E_main_Z_p_I_to_virtual( (P)E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].command_list_base );
        command_list[ command_slot_i ].write = 0;
        struct E_sata_Z_command_table *command_table = E_main_Z_p_I_to_virtual( (P)command_list[ command_slot_i ].command_table_base );
        command_table->command_fis.command = E_sata_Z_ata_command_S_read_fpdma_queued;
        command_table->command_fis.lba_0 = ( N8 )lba;
        command_table->command_fis.lba_1 = ( N8 )( lba >> 8 );
        command_table->command_fis.lba_2 = ( N8 )( lba >> 16 );
        command_table->command_fis.device = 1 << 6; // Zarezerwowany.
        command_table->command_fis.lba_3 = ( N8 )( lba >> 24 );
        command_table->command_fis.lba_4 = ( N8 )( lba >> 32 );
        command_table->command_fis.lba_5 = ( N8 )( lba >> 40 );
        N16 count = logical_sector_count < E_disc_S.sata_ahci.max_sectors ? logical_sector_count : E_disc_S.sata_ahci.max_sectors;
        command_table->command_fis.feature_low = count & 0xff;
        command_table->command_fis.feature_high = count >> 8;
        command_table->command_fis.count = command_slot_i << 3;
        command_table->region[0].data_base_address = (N)E_main_Z_p_I_to_physical(buffer);
        command_table->region[0].data_count = count * E_disc_S.sata_ahci.logical_sector_size - 1;
        *slot_mask &= E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].sata_active;
        E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].sata_active = 1 << command_slot_i;
        E_disc_S.sata_ahci.memory->port[ E_disc_S.sata_ahci.port ].command_issue = 1 << command_slot_i;
        *slot_mask |= 1 << command_slot_i;
        if( logical_sector_count <= E_disc_S.sata_ahci.max_sectors )
            break;
        logical_sector_count -= E_disc_S.sata_ahci.max_sectors;
        if( !logical_sector_count )
            break;
        buffer += E_disc_S.sata_ahci.max_sectors * E_disc_S.sata_ahci.logical_sector_size;
        lba += E_disc_S.sata_ahci.max_sectors;
    }
    return 0;
}
/******************************************************************************/
