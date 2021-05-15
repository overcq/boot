/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   file boot loader
*         after protected mode initialization
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
#define E_main_Z_memory_table_end           0x80000
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Z_page_entry_S_p                    ( 1 << 0 )
#define Z_page_entry_S_rw                   ( 1 << 1 )
//==============================================================================
struct __attribute__(( __packed__ )) E_main_Z_memory_table_entry
{ Pc address;
  N64 size;
  N32 type;
  N32 extended_attributes;
};
//==============================================================================
_internal
struct E_main_Z_memory_table_entry *
E_main_Q_memory_table_I_insert( struct E_main_Z_memory_table_entry **memory_table
, struct E_main_Z_memory_table_entry *entry
){  entry++;
    if((N)entry != E_main_Z_memory_table_end
    && ( !entry->size
      || !( entry->extended_attributes & 1 )
    ))
    {   entry->extended_attributes |= 1;
        return entry;
    }
    E_mem_Q_blk_I_copy( *memory_table - 1, *memory_table, ( entry - *memory_table ) * sizeof( struct E_main_Z_memory_table_entry ));
    --*memory_table;
    return entry - 1;
}
//------------------------------------------------------------------------------
_internal
void
E_main_Q_memory_table_I_sort( struct E_main_Z_memory_table_entry *memory_table
){  N n = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - memory_table;
    while( n > 1 )
    {   struct E_main_Z_memory_table_entry *entry;
        N i;
        for_n_( i, n - 1 )
            if( memory_table[i].size
            && ( memory_table[i].extended_attributes & 1 )
            )
            {   entry = &memory_table[i];
                break;
            }
        if( i == n - 1 )
            break;
        N new_n = 0;
        for( i++; i != n; i++ )
        {   if( !memory_table[i].size
            || !( memory_table[i].extended_attributes & 1 )
            )
                continue;
            if( entry->address > memory_table[i].address )
            {   J_swap( struct E_main_Z_memory_table_entry, *entry, memory_table[i] );
                new_n = i;
            }
            entry = &memory_table[i];
        }
        n = new_n;
    }
}
_internal
void
E_main_Q_memory_table_I_remove_overlapping( struct E_main_Z_memory_table_entry **memory_table
){  struct E_main_Z_memory_table_entry *entry = *memory_table;
    while( entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end
    && ( !entry->size
      || !( entry->extended_attributes & 1 )
    ))
        entry++;
    if( entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end )
    {   struct E_main_Z_memory_table_entry *next_entry = entry;
        O{  do
            {   next_entry++;
            }while( next_entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end
            && ( !next_entry->size
              || !( next_entry->extended_attributes & 1 )
            ));
            if( next_entry == ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end )
                break;
            if( entry->address + entry->size > next_entry->address )
            {   if((( entry->type == 1
                    || entry->type == 3
                  )
                  && ( next_entry->type == 2
                    || next_entry->type == 4
                    || next_entry->type == 5
                ))
                || ( entry->type == 1
                  && next_entry->type == 3
                ))
                {   if( entry->address + entry->size > next_entry->address + next_entry->size )
                    {   struct E_main_Z_memory_table_entry *new_entry = E_main_Q_memory_table_I_insert( memory_table, next_entry );
                        new_entry->address = next_entry->address + next_entry->size;
                        new_entry->size = entry->address + entry->size - new_entry->address;
                    }
                    entry->size = next_entry->address - entry->address;
                }else if((( entry->type == 2
                    || entry->type == 4
                    || entry->type == 5
                  )
                  && ( next_entry->type == 1
                    || next_entry->type == 3
                ))
                || ( entry->type == 3
                  && next_entry->type == 1
                ))
                {   if( entry->address + entry->size > next_entry->address + next_entry->size )
                    {   struct E_main_Z_memory_table_entry *new_entry = E_main_Q_memory_table_I_insert( memory_table, next_entry );
                        new_entry->address = next_entry->address + next_entry->size;
                        new_entry->size = entry->address + entry->size - new_entry->address;
                    }
                    next_entry->size -= entry->address + entry->size - next_entry->address;
                    next_entry->address = entry->address + entry->size;
                }
            }
            entry = next_entry;
        }
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
_internal
B
E_main_I_test_page( N address_
){  volatile Pn address = (Pn)address_;
    N original_data = *address;
    *address = 0x55aa55aa55aa55aa;
    __asm__ (
    "\n"    "wbinvd"
    );
    if( *address != 0x55aa55aa55aa55aa )
    {   *address = original_data;
        __asm__ (
        "\n"    "wbinvd"
        );
        return no;
    }
    *address = 0xaa55aa55aa55aa55;
    __asm__ (
    "\n"    "wbinvd"
    );
    if( *address != 0xaa55aa55aa55aa55 )
    {   *address = original_data;
        __asm__ (
        "\n"    "wbinvd"
        );
        return no;
    }
    *address = original_data;
    return yes;
}
_inline
void
E_main_Q_cr0_P( P pml4
){  __asm__(
    "\n"    "mov    %0, %%cr3"
    :
    : "r" (pml4)
    );
}
_internal
Pc
E_main_I_allocate_page_table(  struct E_main_Z_memory_table_entry *memory_table
, struct E_main_Z_video *video
, N *max_memory
){  *max_memory = ~0;
    Pn pml4 = (Pn)( E_simple_Z_p_I_align_down_to_v2( memory_table, 0xfff ) - 0x1000 ); // Start poniżej tablicy pamięci, malejąco.
    Pn pdpt = (Pn)( (Pc)pml4 - 0x1000 );
    pml4[0] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
    Pn pd = (Pn)( (Pc)pdpt - 0x1000 );
    pdpt[0] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
    Pn pt = (Pn)( (Pc)pd - 0x1000 );
    for_n( pd_i, 32 ) // 64 MiB pamięci. Tablice stron pamięci zajmują ok. 140 KiB.
    {   for_n( pt_i, 512 )
        {   N address = pd_i * ( 1 << 21 ) + pt_i * 0x1000;
            pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
        }
        pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
        pt = (Pn)( (Pc)pt - 0x1000 );
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pd_i, 32 )
    {   for_n( pt_i, 512 )
        {   N address = pd_i * ( 1 << 21 ) + pt_i * 0x1000;
            if( pt_i
            && !( address & 0xfffff )
            && !E_main_I_test_page(address)
            )
            {   *max_memory = address;
                goto End_loop_1;
            } 
        }
    }
End_loop_1:
    if( ~*max_memory )
        goto End;
    pml4 = (Pn)E_memory_S_start; // Start od czwartego mibibajta, rosnąco.
    pdpt = (Pn)( (Pc)pml4 + 0x1000 );
    pml4[0] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
    pd = (Pn)( (Pc)pdpt + 0x1000 );
    for_n( pdpt_i, 16 ) // 16 GiB pamięci. Tablice stron pamięci zajmują ok. 32 MiB.
    {   pt = (Pn)( (Pc)pd + 0x1000 );
        for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
            }
            pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
            pt = (Pn)( (Pc)pt + 0x1000 );
        }
        pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
        pd = pt;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pdpt_i, 16 )
    {   for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                if( pd_i >= 32
                && !( address & 0xfffff ) // Sprawdzanie co 1 MiB.
                && !E_main_I_test_page(address)
                )
                {   *max_memory = address;
                    goto End_loop_2;
                } 
            }
        }
    }
End_loop_2:
    if( ~*max_memory )
        goto End;
    for_n( pml4_i, 8 ) // 4 TiB pamięci. Tablice stron pamięci zajmują ok. 8 GiB.
    {   pd = (Pn)( (Pc)pdpt + 0x1000 );
        for_n( pdpt_i, 512 )
        {   pt = (Pn)( (Pc)pd + 0x1000 );
            for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                }
                pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pt = (Pn)( (Pc)pt + 0x1000 );
            }
            pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
            pd = pt;
        }
        pml4[ pml4_i ] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
        pdpt = pt;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pml4_i, 8 )
    {   for_n( pdpt_i, 512 )
        {   for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    if( pdpt_i >= 16
                    && !( address & 0x3fffffff ) // Sprawdzanie co 1 GiB.
                    && !E_main_I_test_page(address)
                    )
                    {   *max_memory = address;
                        goto End_loop_3;
                    } 
                }
            }
        }
    }
End_loop_3:
    if( ~*max_memory )
        goto End;
    pdpt = (Pn)( (Pc)pml4 + 0x1000 );
    for_n_( pml4_i, 512 ) // 256 TiB pamięci. Tablice stron pamięci zajmują ok. 513 GiB.
    {   pd = (Pn)( (Pc)pdpt + 0x1000 );
        for_n( pdpt_i, 512 )
        {   pt = (Pn)( (Pc)pd + 0x1000 );
            for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                }
                pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pt = (Pn)( (Pc)pt + 0x1000 );
            }
            pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
            pd = pt;
        }
        pml4[ pml4_i ] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
        pdpt = pt;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pml4_i, 512 )
    {   for_n( pdpt_i, 512 )
        {   for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    if( pml4_i >= 8
                    && !( address & 0xffffffffff ) // Sprawdzanie co 1 TiB.
                    && !E_main_I_test_page(address)
                    )
                    {   *max_memory = address;
                        goto End;
                    } 
                }
            }
        }
    }
End:
    if( *max_memory < 0x400000 )
        return 0;
    pml4 = (Pn)( (Pc)*max_memory - 0x1000 ); // Start od końca pamięci rzeczywistej, malejąco.
    pdpt = (Pn)( (Pc)pml4 - 0x1000 );
    B end = no;
    for_n_( pml4_i, 512 )
    {   if( !end )
        {   pd = (Pn)( (Pc)pdpt - 0x1000 );
            for_n( pdpt_i, 512 )
            {   if( !end )
                {   pt = (Pn)( (Pc)pd - 0x1000 );
                    for_n( pd_i, 512 )
                    {   if( !end )
                        {   for_n( pt_i, 512 )
                            {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                                if( !end )
                                {   if( address == *max_memory - 0x1000 )
                                        end = yes;
                                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                                }else
                                    pt[ pt_i ] = 0;
                            }
                            pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                            pt = (Pn)( (Pc)pt - 0x1000 );
                        }else
                            pd[ pd_i ] = 0;
                    }
                    pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
                    pd = pt;
                }else
                    pdpt[ pdpt_i ] = 0;
            }
            pml4[ pml4_i ] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
            pdpt = pt;
        }else
            pml4[ pml4_i ] = 0;
    }
    if( *max_memory <= video->framebuffer )
    {   pml4 = (Pn)( (Pc)*max_memory - 0x1000 );
        pdpt = (Pn)( (Pc)pml4 - 0x1000 );
        end = no;
        N address = 0;
        for_n_( pml4_i, 512 )
        {   if( !end )
            {   if( address < *max_memory
                || address > video->framebuffer
                )
                    pd = (Pn)( (Pc)pdpt - 0x1000 );
                for_n( pdpt_i, 512 )
                {   if( !end )
                    {   if( address < *max_memory
                        || address > video->framebuffer
                        )
                            pt = (Pn)( (Pc)pd - 0x1000 );
                        for_n( pd_i, 512 )
                        {   if( !end )
                            {   for_n( pt_i, 512 )
                                {   address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                                    if( !end )
                                    {   if( address == video->framebuffer + video->line_width * video->height - 0x1000 )
                                            end = yes;
                                        if( address >= video->framebuffer )
                                        {   if( address == video->framebuffer )
                                            {   if( !pdpt_i )
                                                    pd = (Pn)( (Pc)pdpt - 0x1000 );
                                                pt = (Pn)( (Pc)pd - 0x1000 );
                                            }
                                            pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                                        }
                                    }else
                                        pt[ pt_i ] = 0;
                                }
                                if( address > video->framebuffer )
                                    pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                                if( address < *max_memory
                                || address > video->framebuffer
                                )
                                    pt = (Pn)( (Pc)pt - 0x1000 );
                            }else
                                pd[ pd_i ] = 0;
                        }
                        if( address > video->framebuffer )
                            pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
                        pd = pt;
                    }else
                        pdpt[ pdpt_i ] = 0;
                }
                if( address > video->framebuffer )
                    pml4[ pml4_i ] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pdpt = pt;
            }else
                pml4[ pml4_i ] = 0;
        }
    }
    E_main_Q_cr0_P(pml4);
    return (Pc)pt + 0x1000;
}
_internal
N
E_vga_R_video_color( struct E_main_Z_video *video
, N color
){  N video_color;
    if( video->bpp == 16
    || video->bpp == 15
    )
        video_color = ((( color >> 16 ) * (( 1 << video->red_size ) - 1 ) / 255 ) << video->red_start )
        | (((( color >> 8 ) & 0xff ) * (( 1 << video->green_size ) - 1 ) / 255 ) << video->green_start )
        | ((( color & 0xff ) * (( 1 << video->blue_size ) - 1 ) / 255 ) << video->blue_start );
    else
        video_color = (( color >> 16 ) << video->red_start )
        | ((( color >> 8 ) & 0xff ) << video->green_start )
        | (( color & 0xff ) << video->blue_start );
    return video_color;
}
_internal
void
E_vga_I_set_pixel( struct E_main_Z_video *video
, N x
, N y
, N video_color
){  Pc video_address = (Pc)(N)video->framebuffer + video->line_width * y;
    if( video->bpp == 16
    || video->bpp == 15
    )
    {   video_address += x * 2;
        *( N16 * )video_address = video_color;
    }else
    {   video_address += x * 4;
        *( N32 * )video_address = video_color;
    }
}
void
main( struct E_main_Z_memory_table_entry *memory_table
, struct E_main_Z_video *video
){  E_main_Q_memory_table_I_sort( memory_table );
    E_main_Q_memory_table_I_remove_overlapping( &memory_table );
    N max_memory;
    Pc page_tables = E_main_I_allocate_page_table( memory_table, video, &max_memory );
    if( !page_tables )
        goto End;
    E_memory_M( page_tables, video );
    for_n( y, video->height )
    {   for_n( x, video->width )
            E_vga_I_set_pixel( video, x, y, E_vga_R_video_color( video, 0xcacaca ));
    }
    
End:__asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
