/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         filesystem kernel reader
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”             2025‒3‒15 V
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
#define E_ouxfs_Q_device_S_ident "OUXFS"
//==============================================================================
extern struct E_disc_Z E_disc_S;
//==============================================================================
N64 E_ouxfs_S_file_uid;
//==============================================================================
#define E_ouxfs_Q_device_I_switch_item( type, item, end ) \
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
N
E_ouxfs_Q_disk_M( N block_size
){  Pc sector = M( block_size );
    Kp(sector)
        return ~0;
    E_disc_S.partition.oux.sector_size = block_size;
    N r = E_disc_R( 0, 1, sector );
    if( K_error(r) )
        goto Error_0;
    if( !E_mem_Q_blk_T_eq( sector, E_ouxfs_Q_device_S_ident, J_s0_R_l( E_ouxfs_Q_device_S_ident )))
        goto Error_0;
    N64 *block_table_n_ = E_simple_Z_p_I_align_up_to_v2( sector + J_s0_R_l( E_ouxfs_Q_device_S_ident ), sizeof( N64 ));
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
    Mt_( E_disc_S.partition.oux.block_table, block_table_n );
    if( K_error( E_disc_S.partition.oux.block_table )
    || !E_disc_S.partition.oux.block_table
    )
        goto Error_0;
    // Odczyt tablicy bloków do pamięci operacyjnej.
    N continue_from = ~0;
    N data_i = 0;
    N64 block_table_i = ~0UL;
    do // Czyta wpisy pliku tablicy bloków znajdujące się w pierwszym sektorze.
    {   switch( continue_from )
        { case ~0:
                block_table_i++;
                if( block_table_i == block_table_n )
                    goto End_loop_0;
                continue_from++;
          case 0:
                E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].sector
                , sector + E_disc_S.partition.oux.sector_size
                );
          default:
                if( continue_from == 1 )
                {   E_ouxfs_Q_device_I_switch_item( char, E_disc_S.partition.oux.block_table[ block_table_i ].location_type
                    , sector + E_disc_S.partition.oux.sector_size
                    );
                    if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_sectors
                    && E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_in_sector
                    )
                        goto Error_1;
                }
                if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
                    switch( continue_from )
                    { case 2:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 3:
                            E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre >= E_disc_S.partition.oux.sector_size )
                                goto Error_1;
                      case 4:
                            E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post >= E_disc_S.partition.oux.sector_size )
                                goto Error_1;
                            if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                            && ( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                || !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                            ))
                                goto Error_1;
                            continue_from = ~0;
                    }
                else
                    switch( continue_from )
                    { case 2:
                            E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 3:
                            E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size )
                                goto Error_1;
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                + E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                > E_disc_S.partition.oux.sector_size
                            )
                                goto Error_1;
                            continue_from = ~0;
                    }
        }
    }while( data != sector + E_disc_S.partition.oux.sector_size );
End_loop_0:
    for( N64 block_table_i_read = 0; block_table_i_read != block_table_block_table_n; block_table_i_read++ ) // Czyta wszystkie pozostałe wpisy pliku tablicy bloków.
    {   if( block_table_i_read > block_table_i ) //NDFN Przemyśleć i zagwarantować, by zawsze starczało.
            goto Error_1;
    if( E_disc_S.partition.oux.block_table[ block_table_i_read ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
        {   if( E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.pre )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_i_read ].sector - 1, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector + ( E_disc_S.partition.oux.sector_size - E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                            block_table_i++;
                            if( block_table_i == block_table_n )
                                goto Error_1;
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].sector
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      default:
                            if( continue_from == 1 )
                            {   E_ouxfs_Q_device_I_switch_item( char, E_disc_S.partition.oux.block_table[ block_table_i ].location_type
                                , sector + E_disc_S.partition.oux.sector_size
                                );
                                if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_sectors
                                && E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                  case 4:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        && ( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                            || !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        ))
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size )
                                            goto Error_1;
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                            + E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                            > E_disc_S.partition.oux.sector_size
                                        )
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            for( N64 sector_i = 0; sector_i != E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.n; sector_i++ ) // Czyta kolejne sektory z szeregu ciągłych.
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_i_read ].sector + sector_i, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            block_table_i++;
                            if( block_table_i == block_table_n )
                                goto Error_1;
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].sector
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      default:
                            if( continue_from == 1 )
                            {   E_ouxfs_Q_device_I_switch_item( char, E_disc_S.partition.oux.block_table[ block_table_i ].location_type
                                , sector + E_disc_S.partition.oux.sector_size
                                );
                                if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_sectors
                                && E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                  case 4:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        && ( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                            || !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        ))
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                        , sector + E_disc_S.partition.oux.sector_size
                                        );
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size )
                                            goto Error_1;
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                            + E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                            > E_disc_S.partition.oux.sector_size
                                        )
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            if( E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_i_read ].sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.n, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            block_table_i++;
                            if( block_table_i == block_table_n )
                                goto Error_1;
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].sector
                            , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                            );
                      default:
                            if( continue_from == 1 )
                            {   E_ouxfs_Q_device_I_switch_item( char, E_disc_S.partition.oux.block_table[ block_table_i ].location_type
                                , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                );
                                if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_sectors
                                && E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_in_sector
                                )
                                    goto Error_1;
                            }
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                        , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                  case 4:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post >= E_disc_S.partition.oux.sector_size )
                                            goto Error_1;
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                        && ( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                            || !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                        ))
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                            else
                                switch( continue_from )
                                { case 2:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                        , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                        );
                                  case 3:
                                        E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                        , sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post
                                        );
                                        if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size )
                                            goto Error_1;
                                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                            + E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                            > E_disc_S.partition.oux.sector_size
                                        )
                                            goto Error_1;
                                        continue_from = ~0;
                                }
                    }
                }while( data != sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.sectors.post );
            }
        }else
        {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_i_read ].sector, 1, sector );
            if( K_error(r) )
                goto Error_1;
            Pc data = sector + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start;
            do // Czyta wpisy pliku tablicy bloków znajdujące się we fragmencie sektora.
            {   switch( continue_from )
                { case ~0:
                        block_table_i++;
                        if( block_table_i == block_table_n )
                            goto Error_1;
                        continue_from++;
                  case 0:
                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].sector
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                        );
                  default:
                        if( continue_from == 1 )
                        {   E_ouxfs_Q_device_I_switch_item( char, E_disc_S.partition.oux.block_table[ block_table_i ].location_type
                            , sector
                              + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                              + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                            );
                            if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_sectors
                            && E_disc_S.partition.oux.block_table[ block_table_i ].location_type != E_ouxfs_Z_block_Z_location_S_in_sector
                            )
                                goto Error_1;
                        }
                        if( E_disc_S.partition.oux.block_table[ block_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
                            switch( continue_from )
                            { case 2:
                                    E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                    , sector
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                                    );
                              case 3:
                                    E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                    , sector
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre >= E_disc_S.partition.oux.sector_size )
                                        goto Error_1;
                              case 4:
                                    E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                    , sector
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post >= E_disc_S.partition.oux.sector_size )
                                        goto Error_1;
                                    if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.n
                                    && ( !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.pre
                                        || !E_disc_S.partition.oux.block_table[ block_table_i ].location.sectors.post
                                    ))
                                        goto Error_1;
                                    continue_from = ~0;
                            }
                        else
                            switch( continue_from )
                            { case 2:
                                    E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                    , sector
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                                    );
                              case 3:
                                    E_ouxfs_Q_device_I_switch_item( N16, E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                    , sector
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
                                      + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
                                    );
                                    if( !E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size )
                                        goto Error_1;
                                    if( E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.start
                                        + E_disc_S.partition.oux.block_table[ block_table_i ].location.in_sector.size
                                        > E_disc_S.partition.oux.sector_size
                                    )
                                        goto Error_1;
                                    continue_from = ~0;
                            }
                }
            }while( data != sector
              + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.start
              + E_disc_S.partition.oux.block_table[ block_table_i_read ].location.in_sector.size
            );
        }
    }
    if( ~continue_from )
        goto Error_1;
    // Odczyt tablicy katalogów i wyszukanie katalogu “/system”.
    Pc directory_name = "system";
    N64 directory_uid;
    N64 directory_parent;
    N64 uid_last = ~0UL;
    data_i = 0;
    N char_i;
    N64 directory_table_i;
    for( directory_table_i = 0; directory_table_i != block_table_directory_table_n; directory_table_i++ )
    {   if( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
        {   if( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.pre )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].sector - 1, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector + ( E_disc_S.partition.oux.sector_size - E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_uid
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( !~directory_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= directory_uid
                            )
                                goto Error_1;
                            uid_last = directory_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_parent
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + E_disc_S.partition.oux.sector_size )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            for( N64 sector_i = 0; sector_i != E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.n; sector_i++ )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].sector + sector_i, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_uid
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( !~directory_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= directory_uid
                            )
                                goto Error_1;
                            uid_last = directory_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_parent
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + E_disc_S.partition.oux.sector_size )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            if( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
            {   r = E_disc_R(
                  E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].sector
                  + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.n
                , 1
                , sector
                );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_uid
                            , sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post
                            );
                            if( !~directory_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= directory_uid
                            )
                                goto Error_1;
                            uid_last = directory_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, directory_parent
                            , sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post
                            );
                            char_i = 0;
                      case 2:
                            if( !~directory_parent )
                                while( data != sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                                {   if( *data != directory_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_1;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.sectors.post );
            }
        }else
        {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].sector, 1, sector );
            if( K_error(r) )
                goto Error_1;
            Pc data = sector + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start;
            do
            {   switch( continue_from )
                { case ~0:
                        continue_from++;
                  case 0:
                        E_ouxfs_Q_device_I_switch_item( N64, directory_uid
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        );
                        if( !~directory_uid )
                            goto Error_1;
                        if( ~uid_last
                        && uid_last >= directory_uid
                        )
                            goto Error_1;
                        uid_last = directory_uid;
                  case 1:
                        E_ouxfs_Q_device_I_switch_item( N64, directory_parent
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        );
                        char_i = 0;
                  case 2:
                        if( !~directory_parent )
                            while( data != sector
                              + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                              + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                            )
                            {   if( *data != directory_name[ char_i++ ] )
                                    break;
                                if( !*data )
                                    goto End_loop_1;
                                data++;
                            }
                        while( data != sector
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        )
                        {   if( !*data )
                                break;
                            data++;
                        }
                        if( data != sector
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
                        )
                        {   data++;
                            continue_from = ~0;
                        }
                }
            }while( data != sector
              + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.start
              + E_disc_S.partition.oux.block_table[ block_table_directory_table_start + directory_table_i ].location.in_sector.size
            );
        }
    }
End_loop_1:
    if( directory_table_i == block_table_directory_table_n )
        goto Error_1;
    continue_from = ~0;
    // Odczyt tablicy plików i wyszukanie pliku “/system/kernel”.
    Pc file_name = "kernel";
    N64 file_parent;
    uid_last = ~0UL;
    data_i = 0;
    N64 file_table_i;
    for( file_table_i = 0; file_table_i != block_table_file_table_n; file_table_i++ )
    {   if( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location_type == E_ouxfs_Z_block_Z_location_S_sectors )
        {   if( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.pre )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].sector - 1, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector + ( E_disc_S.partition.oux.sector_size - E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.pre );
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_ouxfs_S_file_uid
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( !~E_ouxfs_S_file_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= E_ouxfs_S_file_uid
                            )
                                goto Error_1;
                            uid_last = E_ouxfs_S_file_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, file_parent
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 2:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.start
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 3:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.n
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( E_disc_S.partition.oux.file.block_table.n
                            && !~E_disc_S.partition.oux.file.block_table.start
                            )
                                goto Error_1;
                            if( !~E_disc_S.partition.oux.file.block_table.n )
                                goto Error_1;
                            char_i = 0;
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + E_disc_S.partition.oux.sector_size )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            for( N64 sector_i = 0; sector_i != E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.n; sector_i++ )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].sector + sector_i, 1, sector );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_ouxfs_S_file_uid
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( !~E_ouxfs_S_file_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= E_ouxfs_S_file_uid
                            )
                                goto Error_1;
                            uid_last = E_ouxfs_S_file_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, file_parent
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 2:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.start
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                      case 3:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.n
                            , sector + E_disc_S.partition.oux.sector_size
                            );
                            if( E_disc_S.partition.oux.file.block_table.n
                            && !~E_disc_S.partition.oux.file.block_table.start
                            )
                                goto Error_1;
                            if( !~E_disc_S.partition.oux.file.block_table.n )
                                goto Error_1;
                            char_i = 0;
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + E_disc_S.partition.oux.sector_size )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.sector_size )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.sector_size )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.sector_size );
            }
            if( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
            {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].sector
                  + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.n
                , 1
                , sector
                );
                if( K_error(r) )
                    goto Error_1;
                Pc data = sector;
                do
                {   switch( continue_from )
                    { case ~0:
                            continue_from++;
                      case 0:
                            E_ouxfs_Q_device_I_switch_item( N64, E_ouxfs_S_file_uid
                            , sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                            if( !~E_ouxfs_S_file_uid )
                                goto Error_1;
                            if( ~uid_last
                            && uid_last >= E_ouxfs_S_file_uid
                            )
                                goto Error_1;
                            uid_last = E_ouxfs_S_file_uid;
                      case 1:
                            E_ouxfs_Q_device_I_switch_item( N64, file_parent
                            , sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                      case 2:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.start
                            , sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                      case 3:
                            E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.n
                            , sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post
                            );
                            if( E_disc_S.partition.oux.file.block_table.n
                            && !~E_disc_S.partition.oux.file.block_table.start
                            )
                                goto Error_1;
                            if( !~E_disc_S.partition.oux.file.block_table.n )
                                goto Error_1;
                            char_i = 0;
                      case 4:
                            if( file_parent == directory_uid )
                                while( data != sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                                {   if( *data != file_name[ char_i++ ] )
                                        break;
                                    if( !*data )
                                        goto End_loop_2;
                                    data++;
                                }
                            while( data != sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                            {   if( !*data )
                                    break;
                                data++;
                            }
                            if( data != sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post )
                            {   data++;
                                continue_from = ~0;
                            }
                    }
                }while( data != sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.sectors.post );
            }
        }else
        {   r = E_disc_R( E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].sector, 1, sector );
            if( K_error(r) )
                goto Error_1;
            Pc data = sector + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start;
            do
            {   switch( continue_from )
                { case ~0:
                        continue_from++;
                  case 0:
                        E_ouxfs_Q_device_I_switch_item( N64, E_ouxfs_S_file_uid
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                        if( !~E_ouxfs_S_file_uid )
                            goto Error_1;
                        if( ~uid_last
                        && uid_last >= E_ouxfs_S_file_uid
                        )
                            goto Error_1;
                        uid_last = E_ouxfs_S_file_uid;
                  case 1:
                        E_ouxfs_Q_device_I_switch_item( N64, file_parent
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                  case 2:
                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.start
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                  case 3:
                        E_ouxfs_Q_device_I_switch_item( N64, E_disc_S.partition.oux.file.block_table.n
                        , sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        );
                        if( E_disc_S.partition.oux.file.block_table.n
                        && !~E_disc_S.partition.oux.file.block_table.start
                        )
                            goto Error_1;
                        char_i = 0;
                  case 4:
                        if( file_parent == directory_uid )
                            while( data != sector
                              + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                              + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                            )
                            {   if( *data != file_name[ char_i++ ] )
                                    break;
                                if( !*data )
                                    goto End_loop_2;
                                data++;
                            }
                        while( data != sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        )
                        {   if( !*data )
                                break;
                            data++;
                        }
                        if( data != sector
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
                          + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
                        )
                        {   data++;
                            continue_from = ~0;
                        }
                }
            }while( data != sector
              + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.start
              + E_disc_S.partition.oux.block_table[ block_table_file_table_start + file_table_i ].location.in_sector.size
            );
        }
    }
End_loop_2:
    if( file_table_i == block_table_file_table_n )
        goto Error_1;
    K_( ~1, W(sector) );
    return 0;
Error_1:
    K_( ~1, W( E_disc_S.partition.oux.block_table ));
Error_0:
    K_( ~1, W(sector) );
    return ~0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N64
E_ouxfs_Q_kernel_R_size( void
){  N64 size = 0;
    for( N64 block_table_i = 0; block_table_i != E_disc_S.partition.oux.file.block_table.n; block_table_i++ )
    {   struct E_ouxfs_Z_block *block = E_disc_S.partition.oux.block_table + E_disc_S.partition.oux.file.block_table.start + block_table_i;
        if( block->location_type == E_ouxfs_Z_block_Z_location_S_sectors )
            size += block->location.sectors.pre + block->location.sectors.n * E_disc_S.partition.oux.sector_size + block->location.sectors.post;
        else
            size += block->location.in_sector.size;
    }
    return size;
}
N
E_ouxfs_Q_kernel_I_read( Pc data
){  Pc sector = E_mem_Q_blk_Z_single_memory_M( E_disc_S.partition.oux.sector_size );
    Kp(sector)
        return ~0;
    for( N64 block_table_i = 0; block_table_i != E_disc_S.partition.oux.file.block_table.n; block_table_i++ )
    {   struct E_ouxfs_Z_block *block = E_disc_S.partition.oux.block_table + E_disc_S.partition.oux.file.block_table.start + block_table_i;
        if( block->location_type == E_ouxfs_Z_block_Z_location_S_sectors )
        {   if( block->location.sectors.pre )
            {   K( E_disc_R( block->sector - 1, 1, sector ))
                    return ~0;
                E_mem_Q_blk_I_copy( data, sector + E_disc_S.partition.oux.sector_size - block->location.sectors.pre, block->location.sectors.pre );
                data += block->location.sectors.pre;
            }
            for_n( i, block->location.sectors.n )
            {   K( E_disc_R( block->sector + i, 1, sector ))
                    return ~0;
                E_mem_Q_blk_I_copy( data, sector, E_disc_S.partition.oux.sector_size );
                data += E_disc_S.partition.oux.sector_size;
            }
            if( block->location.sectors.post )
            {   K( E_disc_R( block->sector + block->location.sectors.n, 1, sector ))
                    return ~0;
                E_mem_Q_blk_I_copy( data, sector, block->location.sectors.post );
                data += block->location.sectors.post;
            }
        }else
        {   K( E_disc_R( block->sector, 1, sector ))
                return ~0;
            E_mem_Q_blk_I_copy( data, sector + block->location.in_sector.start, block->location.in_sector.size );
            data += block->location.in_sector.size;
        }
    }
    K_( ~1, W(sector) );
    return 0;
}
/******************************************************************************/
