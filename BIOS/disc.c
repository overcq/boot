/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         disc manager
* (c)overcq              on WSL\Debian (Linux 6.6.87.2)              2026-1-27 a
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
struct __attribute__(( __packed__ )) E_disc_Z_mbr_Z_partition_record
{ N8 bootable           :1;
  N8 reserved           :7;
  N8 start_head;
  N8 start_sector       :6;
  N8 start_cylineder_1  :2;
  N8 start_cylinder_1;
  N8 type;
  N8 end_head;
  N8 end_sector         :6;
  N8 end_cylinder_1     :2;
  N8 end_cylinder_2;
  N32 lba;
  N32 count;
};
struct __attribute__(( __packed__ )) E_disc_Z_mbr
{ N8 boot_code_0[218];
  struct
  { N8 reserved[2];
    N8 drive;
    N8 seconds;
    N8 minutes;
    N8 hours;
  }timestamp;
  N8 boot_code_1[216];
  N32 disc_signature;
  N16 copy_protected;
  struct E_disc_Z_mbr_Z_partition_record partition[4];
  N16 mbr_signature;
};
struct __attribute__(( __packed__ )) E_disc_Z_gpt_header
{ N64 signature;
  N32 revision;
  N32 size;
  N32 crc32;
  N32 reserved;
  N64 my_lba;
  N64 alternate_lba;
  N64 first_usable_lba;
  N64 last_usable_lba;
  N8 disc_guid[16];
  N64 partition_entry_lba;
  N32 partition_entry_count;
  N32 partition_entry_size;
  N32 partition_entry_crc32;
};
struct __attribute__(( __packed__ )) E_disc_Z_gpt_partition_record
{ N8 type_guid[16];
  N8 unique_guid[16];
  N64 start_lba;
  N64 end_lba;
  N64 required      :1;
  N64 no_block_io   :1;
  N64 legacy_bios   :1;
  N64 reserved      :45;
  N64 guid_reserved :16;
  C name[72];
};
//==============================================================================
struct E_disc_Z E_disc_S;
//==============================================================================
N32
E_main_I_crc32_ieee( N8 *data
, N l
){  N32 crc = 0xFFFFFFFF;
    while( l-- )
    {   crc ^= *data++;
        for_n( i, 8 )
            if( crc & 1 )
                crc = ( crc >> 1 ) ^ 0xEDB88320;
            else
                crc >>= 1;
    }
    return crc ^ 0xFFFFFFFF;
}
N
E_disc_I_init( void
){  N r = ~0;
    switch( E_disc_S.type )
    { case E_disc_Z_type_S_sata_ahci:
        {   struct E_disc_Z_mbr *mbr = E_mem_Q_blk_Z_single_memory_M( E_disc_S.sata_ahci.logical_sector_size );
            Kp_( ~2, mbr );
            N32 slot_mask = 0;
            K_( ~2, E_sata_Q_sata_ahci_I_read( 0, 1, mbr, &slot_mask ));
            E_sata_Q_sata_ahci_I_wait_on_completion( slot_mask );
            if( mbr->mbr_signature != 0xaa55 )
            {   K_( ~1, W(mbr) );
                return 0;
            }
            for_n( i, 4 )
            {   switch( mbr->partition[i].type )
                { case 0xee:
                    {   slot_mask = 0;
                        struct E_disc_Z_gpt_header *gpt_header = E_mem_Q_blk_Z_single_memory_M( E_disc_S.sata_ahci.logical_sector_size );;
                        Kp_( ~2, gpt_header );
                        K_( ~2, E_sata_Q_sata_ahci_I_read( 1, 1, gpt_header, &slot_mask ));
                        E_sata_Q_sata_ahci_I_wait_on_completion( slot_mask );
                        if( gpt_header->signature != 0x5452415020494645
                        || gpt_header->size < 92
                        || gpt_header->size > E_disc_S.sata_ahci.logical_sector_size
                        || gpt_header->size % sizeof( N32 )
                        )
                            return ~2;
                        N32 crc32 = gpt_header->crc32;
                        gpt_header->crc32 = 0;
                        if( E_main_I_crc32_ieee( (P)gpt_header, gpt_header->size ) != crc32 )
                            return ~2;
                        if( gpt_header->my_lba != 1
                        || gpt_header->partition_entry_lba != 2
                        )
                            return ~2;
                        if( !E_simple_Z_n_T_power_2( gpt_header->partition_entry_size ))
                            return ~2;
                        if( gpt_header->partition_entry_size & 0x3f )
                            return ~2;
                        slot_mask = 0;
                        N first_usable_lba = gpt_header->partition_entry_lba
                        + E_simple_Z_n_I_align_up_to_v2( (N)gpt_header->partition_entry_count * gpt_header->partition_entry_size, E_disc_S.sata_ahci.logical_sector_size ) / E_disc_S.sata_ahci.logical_sector_size;
                        if( gpt_header->first_usable_lba != first_usable_lba )
                            return ~2;
                        first_usable_lba += E_disc_S.sata_ahci.logical_sector_shift;
                        N last_usable_lba = E_disc_S.sata_ahci.logical_sector_n - 1
                        - E_simple_Z_n_I_align_up_to_v2( (N)gpt_header->partition_entry_count * gpt_header->partition_entry_size, E_disc_S.sata_ahci.logical_sector_size ) / E_disc_S.sata_ahci.logical_sector_size
                        - 1;
                        if( gpt_header->last_usable_lba != last_usable_lba )
                            return ~2;
                        if( E_disc_S.sata_ahci.logical_sector_shift )
                            last_usable_lba -= E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size - E_disc_S.sata_ahci.logical_sector_shift;
                        struct E_disc_Z_gpt_partition_record *gpt_partition_record = E_mem_Q_blk_Z_single_memory_M( E_simple_Z_n_I_align_up_to_v2( (N)gpt_header->partition_entry_count * gpt_header->partition_entry_size, E_disc_S.sata_ahci.logical_sector_size ));
                        Kp_( ~2, gpt_partition_record );
                        K_( ~2, E_sata_Q_sata_ahci_I_read(
                          gpt_header->partition_entry_lba
                        , E_simple_Z_n_I_align_up_to_v2( (N)gpt_header->partition_entry_count * gpt_header->partition_entry_size, E_disc_S.sata_ahci.logical_sector_size ) / E_disc_S.sata_ahci.logical_sector_size
                        , gpt_partition_record
                        , &slot_mask
                        ));
                        E_sata_Q_sata_ahci_I_wait_on_completion( slot_mask );
                        crc32 = gpt_header->partition_entry_crc32;
                        gpt_header->partition_entry_crc32 = 0;
                        if( E_main_I_crc32_ieee( (P)gpt_partition_record, gpt_header->partition_entry_count * gpt_header->partition_entry_size ) != crc32 )
                            return ~2;
                        struct E_disc_Z_gpt_partition_record *gpt_partition_record_ = gpt_partition_record;
                        for_n( partition_record_i, gpt_header->partition_entry_count )
                        {   for_n( i, J_a_R_n( gpt_partition_record_->type_guid ))
                                if( gpt_partition_record_->type_guid[i] )
                                    break;
                            if( i != J_a_R_n( gpt_partition_record_->type_guid ))
                            {   if( gpt_partition_record_->start_lba < first_usable_lba
                                || gpt_partition_record_->end_lba > last_usable_lba
                                || gpt_partition_record_->start_lba % ( E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size ) != E_disc_S.sata_ahci.logical_sector_shift
                                || ( gpt_partition_record_->end_lba - gpt_partition_record_->start_lba + 1 ) % ( E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size )
                                )
                                    return ~2;
                                E_disc_S.partition.pba = gpt_partition_record_->start_lba / ( E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size );
                                E_disc_S.partition.count = ( gpt_partition_record_->end_lba - gpt_partition_record_->start_lba + 1 ) / ( E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size );
                                r = E_ouxfs_Q_disk_M( E_disc_S.sata_ahci.physical_sector_size );
                                K(r){}
                                if( ~r )
                                    break;
                            }
                            gpt_partition_record_ = (P)(( Pc )gpt_partition_record_ + gpt_header->partition_entry_size );
                        }
                        K_( ~1, W( gpt_partition_record ));
                        K_( ~1, W( gpt_header ));
                        break;
                    }
                }
            }
            K_( ~1, W(mbr) );
            break;
        }
      case E_disc_Z_type_S_satapi_ahci:
            break;
      default:
            return ~0;
    }
    return r;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_disc_R(
  N pba
, N count
, P data
){  pba += E_disc_S.partition.pba;
    if( pba + count > E_disc_S.partition.pba + E_disc_S.partition.count )
        return ~0;
    switch( E_disc_S.type )
    { case E_disc_Z_type_S_sata_ahci:
        {   N32 slot_mask = 0;
            K( E_sata_Q_sata_ahci_I_read( E_disc_S.sata_ahci.logical_sector_shift + pba * E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size
            , count * E_disc_S.sata_ahci.physical_sector_size / E_disc_S.sata_ahci.logical_sector_size
            , data
            , &slot_mask
            ))
                return ~0;
            E_sata_Q_sata_ahci_I_wait_on_completion( slot_mask );
            break;
        }
      default:
            return ~0;
    }
    return 0;
}
/******************************************************************************/
