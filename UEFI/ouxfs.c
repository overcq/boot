/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         filesystem kernel reader
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”             2025‒3‒15 V
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
#define H_oux_E_fs_Q_device_S_ident "OUXFS"
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
enum H_oux_E_Fs_Z_block_Z_location
{ H_oux_E_fs_Z_block_Z_location_S_sectors
, H_oux_E_fs_Z_block_Z_location_S_in_sector
};
struct H_oux_E_fs_Z_block
{ N64 sector;
  union
  { struct
    { N64 n;
      N16 pre, post;
    }sectors;
    struct
    { N16 start;
      N16 size;
    }in_sector;
  }location;
  enum H_oux_E_Fs_Z_block_Z_location location_type;
}*H_oux_E_fs_S_block_table;
N H_oux_E_fs_S_sector_size;
N64 H_oux_E_fs_S_file_uid;
N64 H_oux_E_fs_S_file_block_table_start, H_oux_E_fs_S_file_block_table_n;
//==============================================================================
#define H_oux_E_fs_Q_device_I_switch_item( type, item, end ) \
    if( data_i ) \
    {   do \
        {   item |= (type)*data++ << data_i++ * 8; \
        }while( data_i != sizeof(type) \
        && data != (end) \
        ); \
        if( data_i == sizeof(type) ) \
            data_i = 0; \
        else \
            break; \
    }else \
    {   if( data == (end) ) \
            break; \
        if( data + sizeof(type) > (end) ) \
        {   item = 0; \
            do \
            {   item |= (type)*data++ << data_i++ * 8; \
            }while( data != (end) ); \
            break; \
        } \
        item = *( type * )data; \
        data += sizeof(type); \
    } \
    continue_from++
//------------------------------------------------------------------------------
S
H_uefi_I_print( struct H_uefi_Z_system_table *system_table
, N v
, N v_l
, N base
);
S
H_oux_E_fs_Q_disk_M( struct H_uefi_Z_system_table *system_table
, struct H_uefi_Z_protocol_Z_disk_io *disk_io
, N32 media_id
){  Pc sector;
    S status = system_table->boot_services->M_pages( H_uefi_Z_allocate_Z_any, H_uefi_Z_memory_Z_loader_data, 1, ( N64 * )&sector );
    if( status < 0 )
        return status;
    status = disk_io->read( disk_io, media_id, 0, H_oux_E_mem_S_page_size, sector );
    if( status < 0 )
        return status;
    if( !E_text_Z_sl_T_eq( sector, H_oux_E_fs_Q_device_S_ident, sizeof( H_oux_E_fs_Q_device_S_ident )))
        goto Error_0;
    N sector_size_bit = sector[ sizeof( H_oux_E_fs_Q_device_S_ident ) ];
    if( sector_size_bit != 9
    && sector_size_bit != 12
    )
        goto Error_0;
    H_oux_E_fs_S_sector_size = 1 << sector_size_bit;
    N64 *block_table_n_ = (P)H_oux_J_align_up_p( sector + sizeof( H_oux_E_fs_Q_device_S_ident ) + 1, sizeof(N64) );
    N64 block_table_n = block_table_n_[0];
    N64 block_table_block_table_n = block_table_n_[1];
    N64 block_table_directory_table_start = block_table_n_[2];
    N64 block_table_directory_table_n = block_table_n_[3];
    N64 block_table_file_table_start = block_table_n_[4];
    N64 block_table_file_table_n = block_table_n_[5];
    if( block_table_n < block_table_block_table_n
    || ( block_table_directory_table_n
      && ( block_table_n < block_table_directory_table_start + block_table_directory_table_n
        || block_table_block_table_n > block_table_directory_table_start
    ))
    || ( block_table_file_table_n
      && ( block_table_n < block_table_file_table_start + block_table_file_table_n
        || block_table_block_table_n > block_table_file_table_start
    ))
    || ( block_table_directory_table_n
      && block_table_file_table_n
      && (( block_table_directory_table_start >= block_table_file_table_start
          && block_table_directory_table_start < block_table_file_table_start + block_table_file_table_n
        )
        || ( block_table_directory_table_start < block_table_file_table_start
          && block_table_directory_table_start + block_table_directory_table_n > block_table_file_table_start
        )
        || ( block_table_file_table_start >= block_table_directory_table_start
          && block_table_file_table_start < block_table_directory_table_start + block_table_directory_table_n
        )
        || ( block_table_file_table_start < block_table_directory_table_start
          && block_table_file_table_start + block_table_file_table_n > block_table_directory_table_start
    ))))
        goto Error_0;
    Pc data = (P)&block_table_n_[6];
    status = system_table->boot_services->M_pool( H_uefi_Z_memory_Z_loader_data, block_table_n * sizeof( *H_oux_E_fs_S_block_table ), ( P * )&H_oux_E_fs_S_block_table );
    if( status < 0 )
        goto Error_0;
    // Odczyt tablicy bloków do pamięci operacyjnej.
    N continue_from = ~0;
    N data_i = 0;
    N64 block_table_i = ~0ULL;
    do // Czyta wpisy pliku tablicy bloków znajdujące się w pierwszym sektorze.
    {   switch( continue_from )
        { case ~0:
          case 0:
                if( !~continue_from )
                {   block_table_i++;
                    if( block_table_i == block_table_n )
                        goto End_loop_0;
                    continue_from = 0;
                }
                H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].sector
                , sector + H_oux_E_fs_S_sector_size
                );
          default:
                if( continue_from == 1 )
                {   H_oux_E_fs_Q_device_I_switch_item( char, H_oux_E_fs_S_block_table[ block_table_i ].location_type
                    , sector + H_oux_E_fs_S_sector_size
                    );
                    if( H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_sectors
                    && H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_in_sector
                    )
                        goto Error_1;
                }
                if( H_oux_E_fs_S_block_table[ block_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
                    switch( continue_from )
                    { case 2:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 3:
                            H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 4 )
                                if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre >= H_oux_E_fs_S_sector_size )
                                    goto Error_1;
                      case 4:
                            H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 5 )
                            {   if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post >= H_oux_E_fs_S_sector_size )
                                    goto Error_1;
                                if( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                && ( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                  || !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                ))
                                    goto Error_1;
                                continue_from = ~0;
                            }
                    }
                else
                    switch( continue_from )
                    { case 2:
                            H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 3:
                            H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 4 )
                            {   if( !H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size )
                                    goto Error_1;
                                if( H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                  + H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                  > H_oux_E_fs_S_sector_size
                                )
                                    goto Error_1;
                                continue_from = ~0;
                            }
                    }
        }
    }while( data != sector + H_oux_E_fs_S_sector_size );
End_loop_0:
    for( N64 block_table_i_read = 0; block_table_i_read != block_table_block_table_n; block_table_i_read++ ) // Czyta wszystkie pozostałe wpisy pliku tablicy bloków.
    {   if( block_table_i_read > block_table_i ) //NDFN Przemyśleć i zagwarantować, by zawsze starczało.
            goto Error_1;
    if( H_oux_E_fs_S_block_table[ block_table_i_read ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
        {   if( H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.pre )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_i_read ].sector - 1 ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector + ( H_oux_E_fs_S_sector_size - H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                            {   block_table_i++;
                                if( block_table_i == block_table_n )
                                    goto Error_1;
                                continue_from = 0;
                            }
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].sector
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      default:
                            if( continue_from == 1 )
                            {   H_oux_E_fs_Q_device_I_switch_item( char, H_oux_E_fs_S_block_table[ block_table_i ].location_type
                                , sector + H_oux_E_fs_S_sector_size
                                );
                                if( H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_sectors
                                && H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( H_oux_E_fs_S_block_table[ block_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 4 )
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                  case 4:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 5 )
                                        {   if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                            if( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                            && ( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                              || !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                            ))
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 4 )
                                        {   if( !H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size )
                                                goto Error_1;
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                              + H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                              > H_oux_E_fs_S_sector_size
                                            )
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            for( N64 sector_i = 0; sector_i != H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.n; sector_i++ ) // Czyta kolejne sektory z szeregu ciągłych.
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_i_read ].sector + sector_i ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                            {   block_table_i++;
                                if( block_table_i == block_table_n )
                                    goto Error_1;
                                continue_from = 0;
                            }
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].sector
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      default:
                            if( continue_from == 1 )
                            {   H_oux_E_fs_Q_device_I_switch_item( char, H_oux_E_fs_S_block_table[ block_table_i ].location_type
                                , sector + H_oux_E_fs_S_sector_size
                                );
                                if( H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_sectors
                                && H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( H_oux_E_fs_S_block_table[ block_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 4 )
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                  case 4:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 5 )
                                        {   if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                            if( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                            && ( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                              || !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                            ))
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                        , sector + H_oux_E_fs_S_sector_size
                                        );
                                        if( continue_from == 4 )
                                        {   if( !H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size )
                                                goto Error_1;
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                              + H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                              > H_oux_E_fs_S_sector_size
                                            )
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            if( H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_i_read ].sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.n ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                            {   block_table_i++;
                                if( block_table_i == block_table_n )
                                    goto Error_1;
                                continue_from = 0;
                            }
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].sector
                            , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                            );
                      default:
                            if( continue_from == 1 )
                            {   H_oux_E_fs_Q_device_I_switch_item( char, H_oux_E_fs_S_block_table[ block_table_i ].location_type
                                , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                );
                                if( H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_sectors
                                && H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( H_oux_E_fs_S_block_table[ block_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                        , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                        , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( continue_from == 4 )
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                  case 4:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                        , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( continue_from == 5 )
                                        {   if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post >= H_oux_E_fs_S_sector_size )
                                                goto Error_1;
                                            if( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                            && ( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                              || !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                            ))
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                        , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                        );
                                  case 3:
                                        H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                        , sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( continue_from == 4 )
                                        {   if( !H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size )
                                                goto Error_1;
                                            if( H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                              + H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                              > H_oux_E_fs_S_sector_size
                                            )
                                                goto Error_1;
                                            continue_from = ~0;
                                        }
                                }
                    }
                }while( data != sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.sectors.post );
            }
        }else
        {   N64 offset = H_oux_E_fs_S_block_table[ block_table_i_read ].sector * H_oux_E_fs_S_sector_size;
            status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
            if( status < 0 )
                goto Error_1;
            Pc data = sector + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start;
            do // Czyta wpisy pliku tablicy bloków znajdujące się we fragmencie sektora.
            {   switch( continue_from )
                { case ~0:
                  case 0:
                        if( !~continue_from )
                        {   block_table_i++;
                            if( block_table_i == block_table_n )
                                goto Error_1;
                            continue_from = 0;
                        }
                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].sector
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                        );
                  default:
                        if( continue_from == 1 )
                        {   H_oux_E_fs_Q_device_I_switch_item( char, H_oux_E_fs_S_block_table[ block_table_i ].location_type
                            , sector
                              + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                              + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                            );
                            if( H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_sectors
                            && H_oux_E_fs_S_block_table[ block_table_i ].location_type != H_oux_E_fs_Z_block_Z_location_S_in_sector
                            )
                                goto Error_1;
                        }
                        if( H_oux_E_fs_S_block_table[ block_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
                            switch( continue_from )
                            { case 2:
                                    H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                    , sector
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                                    );
                              case 3:
                                    H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                    , sector
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( continue_from == 4 )
                                        if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre >= H_oux_E_fs_S_sector_size )
                                            goto Error_1;
                              case 4:
                                    H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                    , sector
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( continue_from == 5 )
                                    {   if( H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post >= H_oux_E_fs_S_sector_size )
                                            goto Error_1;
                                        if( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.n
                                        && ( !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.pre
                                          || !H_oux_E_fs_S_block_table[ block_table_i ].location.sectors.post
                                        ))
                                            goto Error_1;
                                        continue_from = ~0;
                                    }
                            }
                        else
                            switch( continue_from )
                            { case 2:
                                    H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                    , sector
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                                    );
                              case 3:
                                    H_oux_E_fs_Q_device_I_switch_item( N16, H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                    , sector
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
                                      + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( continue_from == 4 )
                                    {   if( !H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size )
                                            goto Error_1;
                                        if( H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.start
                                          + H_oux_E_fs_S_block_table[ block_table_i ].location.in_sector.size
                                          > H_oux_E_fs_S_sector_size
                                        )
                                            goto Error_1;
                                        continue_from = ~0;
                                    }
                            }
                }
            }while( data != sector
              + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.start
              + H_oux_E_fs_S_block_table[ block_table_i_read ].location.in_sector.size
            );
        }
    }
    if( ~continue_from )
        goto Error_1;
    // Odczyt tablicy katalogów i wyszukanie katalogu “/system”.
    const Pc directory_name = "system";
    N64 directory_uid;
    N64 directory_parent;
    N64 uid_last = ~0ULL;
    data_i = 0;
    N char_i;
    N64 directory_table_i;
    for( directory_table_i = 0; directory_table_i != block_table_directory_table_n; directory_table_i++ )
    {   if( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
        {   if( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.pre )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].sector - 1 ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector + ( H_oux_E_fs_S_sector_size - H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_uid
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 1 )
                            {   if( !~directory_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= directory_uid
                                )
                                    goto Error_1;
                                uid_last = directory_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_parent
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 2 )
                                char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + H_oux_E_fs_S_sector_size )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            for( N64 sector_i = 0; sector_i != H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.n; sector_i++ )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].sector + sector_i ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_uid
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 1 )
                            {   if( !~directory_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= directory_uid
                                )
                                    goto Error_1;
                                uid_last = directory_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_parent
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 2 )
                                char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + H_oux_E_fs_S_sector_size )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            if( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].sector
                  + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.n
                ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_uid
                            , sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post
                            );
                            if( continue_from == 1 )
                            {   if( !~directory_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= directory_uid
                                )
                                    goto Error_1;
                                uid_last = directory_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, directory_parent
                            , sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post
                            );
                            if( continue_from == 2 )
                                char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post );
            }
        }else
        {   N64 offset = H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].sector * H_oux_E_fs_S_sector_size;
            status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
            if( status < 0 )
                goto Error_1;
            Pc data = sector + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start;
            do
            {   switch( continue_from )
                { case ~0:
                  case 0:
                        if( !~continue_from )
                            continue_from = 0;
                        H_oux_E_fs_Q_device_I_switch_item( N64, directory_uid
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        );
                        if( continue_from == 1 )
                        {   if( !~directory_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= directory_uid
                            )
                                goto Error_1;
                            uid_last = directory_uid;
                        }
                  case 1:
                        H_oux_E_fs_Q_device_I_switch_item( N64, directory_parent
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        );
                        if( continue_from == 2 )
                            char_i = 0;
                  case 2:
                        if( !~directory_parent )
                            while( data != sector
                              + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                              + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                            )
                            {   if( *data != directory_name[ char_i++ ] )
                                    break;
                                if( !*data )
                                    goto End_loop_1;
                                data++;
                            }
                        while( data != sector
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        )
                        {   if( !*data )
                                break;
                            data++;
                        }
                        if( data != sector
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        )
                        {   data++;
                            continue_from = ~0;
                        }
                }
            }while( data != sector
              + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
              + H_oux_E_fs_S_block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
            );
        }
    }
End_loop_1:
    if( directory_table_i == block_table_directory_table_n )
        goto Error_1;
    // Odczyt tablicy plików i wyszukanie pliku “/system/kernel”.
    const Pc file_name = "kernel";
    N64 file_parent;
    uid_last = ~0ULL;
    data_i = 0;
    N64 file_table_i;
    for( file_table_i = 0; file_table_i != block_table_file_table_n; file_table_i++ )
    {   if( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
        {   if( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.pre )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].sector - 1 ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector + ( H_oux_E_fs_S_sector_size - H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_uid
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 1 )
                            {   if( !~H_oux_E_fs_S_file_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= H_oux_E_fs_S_file_uid
                                )
                                    goto Error_1;
                                uid_last = H_oux_E_fs_S_file_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, file_parent
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 2:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_start
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 3:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_n
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 4 )
                            {   if( H_oux_E_fs_S_file_block_table_n
                                && !~H_oux_E_fs_S_file_block_table_start
                                )
                                    goto Error_1;
                                if( !~H_oux_E_fs_S_file_block_table_n )
                                    goto Error_1;
                                char_i = 0;
                            }
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + H_oux_E_fs_S_sector_size )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            for( N64 sector_i = 0; sector_i != H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.n; sector_i++ )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].sector + sector_i ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_uid
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 1 )
                            {   if( !~H_oux_E_fs_S_file_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= H_oux_E_fs_S_file_uid
                                )
                                    goto Error_1;
                                uid_last = H_oux_E_fs_S_file_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, file_parent
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 2:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_start
                            , sector + H_oux_E_fs_S_sector_size
                            );
                      case 3:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_n
                            , sector + H_oux_E_fs_S_sector_size
                            );
                            if( continue_from == 4 )
                            {   if( H_oux_E_fs_S_file_block_table_n
                                && !~H_oux_E_fs_S_file_block_table_start
                                )
                                    goto Error_1;
                                if( !~H_oux_E_fs_S_file_block_table_n )
                                    goto Error_1;
                                char_i = 0;
                            }
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + H_oux_E_fs_S_sector_size )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_sector_size );
            }
            if( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
            {   N64 offset = ( H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].sector
                  + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.n
                ) * H_oux_E_fs_S_sector_size;
                status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
                if( status < 0 )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                      case 0:
                            if( !~continue_from )
                                continue_from = 0;
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_uid
                            , sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                            if( continue_from == 1 )
                            {   if( !~H_oux_E_fs_S_file_uid )
                                    goto Error_1;
                                if( ~uid_last
                                && uid_last >= H_oux_E_fs_S_file_uid
                                )
                                    goto Error_1;
                                uid_last = H_oux_E_fs_S_file_uid;
                            }
                      case 1:
                            H_oux_E_fs_Q_device_I_switch_item( N64, file_parent
                            , sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                      case 2:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_start
                            , sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                      case 3:
                            H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_n
                            , sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                            if( continue_from == 4 )
                            {   if( H_oux_E_fs_S_file_block_table_n
                                && !~H_oux_E_fs_S_file_block_table_start
                                )
                                    goto Error_1;
                                if( !~H_oux_E_fs_S_file_block_table_n )
                                    goto Error_1;
                                char_i = 0;
                            }
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.sectors.post );
            }
        }else
        {   N64 offset = H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].sector * H_oux_E_fs_S_sector_size;
            status = disk_io->read( disk_io, media_id, offset, H_oux_E_fs_S_sector_size, sector );
            if( status < 0 )
                goto Error_1;
            Pc data = sector + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start;
            do
            {   switch( continue_from )
                { case ~0:
                  case 0:
                        if( !~continue_from )
                            continue_from = 0;
                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_uid
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                        if( continue_from == 1 )
                        {   if( !~H_oux_E_fs_S_file_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= H_oux_E_fs_S_file_uid
                            )
                                goto Error_1;
                            uid_last = H_oux_E_fs_S_file_uid;
                        }
                  case 1:
                        H_oux_E_fs_Q_device_I_switch_item( N64, file_parent
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                  case 2:
                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_start
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                  case 3:
                        H_oux_E_fs_Q_device_I_switch_item( N64, H_oux_E_fs_S_file_block_table_n
                        , sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                        if( continue_from == 4 )
                        {   if( H_oux_E_fs_S_file_block_table_n
                            && !~H_oux_E_fs_S_file_block_table_start
                            )
                                goto Error_1;
                            char_i = 0;
                        }
                  case 4:
                        if( file_parent == directory_uid )
                            while( data != sector
                              + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                              + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                            )
                            {   if( *data != file_name[ char_i++ ] )
                                    break;
                                if( !*data )
                                    goto End_loop_2;
                                data++;
                            }
                        while( data != sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        )
                        {   if( !*data )
                                break;
                            data++;
                        }
                        if( data != sector
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        )
                        {   data++;
                            continue_from = ~0;
                        }
                }
            }while( data != sector
              + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
              + H_oux_E_fs_S_block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
            );
        }
    }
End_loop_2:
    if( file_table_i == block_table_file_table_n )
        goto Error_1;
    status = system_table->boot_services->W_pages(( N64 )sector, 1 );;
    return status;
Error_1:
    S status_1 = system_table->boot_services->W_pool( H_oux_E_fs_S_block_table );
Error_0:
    S status_0 = system_table->boot_services->W_pages(( N64 )sector, 1 );
    if( !( status < 0 ))
        status = ~0;
    return status;
}
S
H_oux_E_fs_Q_disk_W( struct H_uefi_Z_system_table *system_table
){  return system_table->boot_services->W_pool( H_oux_E_fs_S_block_table );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N64
H_oux_E_fs_Q_kernel_R_size( struct H_uefi_Z_system_table *system_table
, struct H_uefi_Z_protocol_Z_disk_io *disk_io
, N32 media_id
){  N64 size = 0;
    for( N64 block_table_i = 0; block_table_i != H_oux_E_fs_S_file_block_table_n; block_table_i++ )
    {   struct H_oux_E_fs_Z_block *block = H_oux_E_fs_S_block_table + H_oux_E_fs_S_file_block_table_start + block_table_i;
        if( block->location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
            size += block->location.sectors.pre + block->location.sectors.n * H_oux_E_fs_S_sector_size + block->location.sectors.post;
        else
            size += block->location.in_sector.size;
    }
    return size;
}
S
H_oux_E_fs_Q_kernel_I_read( struct H_uefi_Z_system_table *system_table
, struct H_uefi_Z_protocol_Z_disk_io *disk_io
, N32 media_id
, Pc data
){  for( N64 block_table_i = 0; block_table_i != H_oux_E_fs_S_file_block_table_n; block_table_i++ )
    {   struct H_oux_E_fs_Z_block *block = H_oux_E_fs_S_block_table + H_oux_E_fs_S_file_block_table_start + block_table_i;
        if( block->location_type == H_oux_E_fs_Z_block_Z_location_S_sectors )
        {   if( block->location.sectors.pre )
            {   S status = disk_io->read( disk_io, media_id
                , block->sector * H_oux_E_fs_S_sector_size - block->location.sectors.pre
                , block->location.sectors.pre
                , data
                );
                if( status < 0 )
                    return status;
                data += block->location.sectors.pre;
            }
            if( block->location.sectors.n )
            {   S status = disk_io->read( disk_io, media_id, block->sector * H_oux_E_fs_S_sector_size, block->location.sectors.n * H_oux_E_fs_S_sector_size, data );
                if( status < 0 )
                    return status;
                data += block->location.sectors.n * H_oux_E_fs_S_sector_size;
            }
            if( block->location.sectors.post )
            {   S status = disk_io->read( disk_io, media_id
                , ( block->sector + block->location.sectors.n ) * H_oux_E_fs_S_sector_size
                , block->location.sectors.post
                , data
                );
                if( status < 0 )
                    return status;
                data += block->location.sectors.post;
            }
        }else
        {   S status = disk_io->read( disk_io, media_id
            , block->sector * H_oux_E_fs_S_sector_size + block->location.in_sector.start
            , block->location.in_sector.size
            , data
            );
            if( status < 0 )
                return status;
            data += block->location.in_sector.size;
        }
    }
    return 0;
}
/******************************************************************************/
