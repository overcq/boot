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
enum
{ E_main_Z_memory_table_Z_memory_type_S_available = 1
, E_main_Z_memory_table_Z_memory_type_S_reserved
, E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
, E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
, E_main_Z_memory_table_Z_memory_type_S_bad
};
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
            {   if((( entry->type == E_main_Z_memory_table_Z_memory_type_S_available
                    || entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                  )
                  && ( next_entry->type == E_main_Z_memory_table_Z_memory_type_S_reserved
                    || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                    || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_bad
                    || next_entry->type > E_main_Z_memory_table_Z_memory_type_S_bad
                ))
                || ( entry->type == E_main_Z_memory_table_Z_memory_type_S_available
                  && next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                ))
                {   if( entry->address + entry->size > next_entry->address + next_entry->size )
                    {   struct E_main_Z_memory_table_entry *new_entry = E_main_Q_memory_table_I_insert( memory_table, next_entry );
                        new_entry->address = next_entry->address + next_entry->size;
                        new_entry->size = entry->address + entry->size - new_entry->address;
                    }
                    entry->size = next_entry->address - entry->address;
                }else if((( entry->type == E_main_Z_memory_table_Z_memory_type_S_reserved
                    || entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                    || entry->type == E_main_Z_memory_table_Z_memory_type_S_bad
                    || entry->type > E_main_Z_memory_table_Z_memory_type_S_bad
                  )
                  && ( next_entry->type == E_main_Z_memory_table_Z_memory_type_S_available
                    || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                ))
                || ( entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                  && next_entry->type == E_main_Z_memory_table_Z_memory_type_S_available
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
_internal
P
E_main_Q_memory_table_I_after_hole( struct E_main_Z_memory_table_entry *memory_table
, P address
){  P next_address = 0;
    for( struct E_main_Z_memory_table_entry *entry = memory_table; entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end; entry++ )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        )
            continue;
        if( !next_address )
        {   if( E_simple_Z_p_I_align_down_to_v2( entry->address, E_memory_S_page_size ) == (P)address )
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    next_address = entry->address + entry->size;
            }else
                break;
        }else
            if( E_simple_Z_p_I_align_down_to_v2( entry->address, E_memory_S_page_size ) == E_simple_Z_p_I_align_down_to_v2( next_address, E_memory_S_page_size ))
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    next_address = entry->address + entry->size;
            }else
                break;
    }
    //N video_start = E_simple_Z_n_I_align_down_to_v2( video->framebuffer, E_memory_S_page_size );
    //N video_end = E_simple_Z_n_I_align_up_to_v2( (N)video->framebuffer + video->line_width * video->height, E_memory_S_page_size );
    //if( address == (P)video_start )
        //return (P)video_end;
    return next_address ? E_simple_Z_p_I_align_up_to_v2( next_address, E_memory_S_page_size ) : address;
}
_internal
Pc
E_main_Q_memory_table_I_before_hole( struct E_main_Z_memory_table_entry *memory_table
, P address
){  P prev_address = 0;
    for( struct E_main_Z_memory_table_entry *entry = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - 1; entry >= memory_table; entry-- )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        )
            continue;
        if( !prev_address )
        {   if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) == (P)address )
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    prev_address = entry->address;
            }else
                break;
        }else
            if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) == E_simple_Z_p_I_align_up_to_v2( prev_address, E_memory_S_page_size ))
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    prev_address = entry->address;
            }else
                break;
    }
    //N video_start = E_simple_Z_n_I_align_down_to_v2( video->framebuffer, E_memory_S_page_size );
    //N video_end = E_simple_Z_n_I_align_up_to_v2( (N)video->framebuffer + video->line_width * video->height, E_memory_S_page_size );
    //if( address == (P)video_end )
        //return (P)video_start;
    return prev_address ? E_simple_Z_p_I_align_down_to_v2( prev_address, E_memory_S_page_size ) : address;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
_internal
B
E_main_I_test_page( N address_
){  volatile Pn address = (Pn)address_;
    N original_data = *address;
    N data = 0x55aa55aa55aa55aa;
    *address = data;
    __asm__ (
    "\n"    "wbinvd"
    );
    if( *address != data )
    {   *address = original_data;
        return no;
    }
    data = 0xaa55aa55aa55aa55;
    *address = data;
    __asm__ (
    "\n"    "wbinvd"
    );
    if( *address != data )
    {   *address = original_data;
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
, N *max_memory
){  *max_memory = ~0;
    Pn pml4 = (Pn)( E_simple_Z_p_I_align_down_to_v2( memory_table, E_memory_S_page_size ) - E_memory_S_page_size ); // Start poniżej tablicy pamięci, malejąco.
    Pn pdpt = (Pn)( (Pc)pml4 - E_memory_S_page_size );
    pml4[0] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
    Pn pd = (Pn)( (Pc)pdpt - E_memory_S_page_size );
    pdpt[0] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
    Pn pt = (Pn)( (Pc)pd - E_memory_S_page_size );
    for_n( pd_i, 32 ) // 64 MiB pamięci. Tablice stron pamięci zajmują ok. 140 KiB.
    {   for_n( pt_i, 512 )
        {   N address = pd_i * ( 1 << 21 ) + pt_i * E_memory_S_page_size;
            pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
        }
        pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
        pt = (Pn)( (Pc)pt - E_memory_S_page_size );
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pd_i, 32 )
    {   for_n( pt_i, 512 )
        {   N address = pd_i * ( 1 << 21 ) + pt_i * E_memory_S_page_size;
            if( pd_i > 1
            && !( address & 0xfffff ) // Sprawdzanie co 1 MiB.
            && !E_main_I_test_page(address)
            )
            {   *max_memory = address;
                goto End_loop_1;
            } 
        }
    }
End_loop_1:;
    if( ~*max_memory )
        goto End;
    pml4 = E_main_Q_memory_table_I_after_hole( memory_table, (P)E_memory_S_start ); // Start od czwartego mibibajta, rosnąco.
    pdpt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pml4 + E_memory_S_page_size );
    pml4[0] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
    pd = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pdpt + E_memory_S_page_size );
    for_n( pdpt_i, 16 ) // 16 GiB pamięci. Tablice stron pamięci zajmują ok. 32 MiB.
    {   pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pd + E_memory_S_page_size );
        for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
            }
            pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
            pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pt + E_memory_S_page_size );
        }
        pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
        pd = pt;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pdpt_i, 16 )
    {   for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
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
    {   pd = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pdpt + E_memory_S_page_size );
        for_n( pdpt_i, 512 )
        {   pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pd + E_memory_S_page_size );
            for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                }
                pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pt + E_memory_S_page_size );
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
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
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
    pdpt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pml4 + E_memory_S_page_size );
    for_n_( pml4_i, 512 ) // 256 TiB pamięci. Tablice stron pamięci zajmują ok. 513 GiB.
    {   pd = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pdpt + E_memory_S_page_size );
        for_n( pdpt_i, 512 )
        {   pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pd + E_memory_S_page_size );
            for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                }
                pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pt = E_main_Q_memory_table_I_after_hole( memory_table, (Pc)pt + E_memory_S_page_size );
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
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
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
End:;
    if( *max_memory < E_memory_S_start + 0x100000 ) // NDFN
        return 0;
    pml4 = (P)( E_main_Q_memory_table_I_before_hole( memory_table, (P)*max_memory ) - E_memory_S_page_size ); // Start od końca pamięci rzeczywistej, malejąco.
    pdpt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pml4 ) - E_memory_S_page_size );
    B end = no;
    for_n_( pml4_i, 512 )
    {   if( !end )
        {   pd = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pdpt ) - E_memory_S_page_size );
            for_n( pdpt_i, 512 )
            {   if( !end )
                {   pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pd ) - E_memory_S_page_size );
                    for_n( pd_i, 512 )
                    {   if( !end )
                        {   for_n( pt_i, 512 )
                            {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                                if( !end )
                                {   if( address == *max_memory - E_memory_S_page_size )
                                        end = yes;
                                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                                }else
                                    pt[ pt_i ] = 0;
                            }
                            pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                            pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pt ) - E_memory_S_page_size );
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
    N video_end = E_simple_Z_n_I_align_up_to_v2( (N)video->framebuffer + video->line_width * video->height, E_memory_S_page_size );
    if( *max_memory < video_end )
    {   N video_start = E_simple_Z_n_I_align_down_to_v2( video->framebuffer, E_memory_S_page_size );
        pdpt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pml4 ) - E_memory_S_page_size );
        end = no;
        B started = no;
        B ending_max_memory = no;
        N address = 0;
        B pd_inited, pt_inited;
        for_n_( pml4_i, 512 )
        {   if( !end )
            {   if( address < *max_memory )
                    pd_inited = yes;
                else
                    pd_inited = no;
                if( address < *max_memory
                || started
                )
                    pd = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pdpt ) - E_memory_S_page_size );
                for_n( pdpt_i, 512 )
                {   if( !end )
                    {   if( address < *max_memory )
                            pt_inited = yes;
                        else
                            pt_inited = no;
                        if( address < *max_memory
                        || started
                        )
                            pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pd ) - E_memory_S_page_size );
                        for_n( pd_i, 512 )
                        {   if( !end )
                            {   for_n( pt_i, 512 )
                                {   address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                                    if( !end )
                                    {   if( address == *max_memory )
                                            ending_max_memory = yes;
                                        if( address >= *max_memory
                                        && address >= video_start
                                        ){  if( !started
                                            && ( address == *max_memory
                                              || address == video_start
                                            ))
                                            {   started = yes;
                                                if( !pd_inited )
                                                {   for_n( i, pdpt_i )
                                                        pdpt[i] = 0;
                                                    pd = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pdpt ) - E_memory_S_page_size );
                                                    for_n_( i, pd_i )
                                                        pd[i] = 0;
                                                }
                                                if( !pt_inited )
                                                {   pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pd ) - E_memory_S_page_size );
                                                    for_n( i, pt_i )
                                                        pt[i] = 0;
                                                }
                                            }
                                            if( address == video_end - E_memory_S_page_size )
                                                end = yes;
                                            pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
                                        }
                                    }else
                                        pt[ pt_i ] = 0;
                                }
                                if( started )
                                    pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
                                if( address < *max_memory
                                || ending_max_memory
                                || started
                                )
                                {   ending_max_memory = no;
                                    pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pt ) - E_memory_S_page_size );
                                }
                            }else
                                pd[ pd_i ] = 0;
                        }
                        if( started )
                            pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
                        pd = pt;
                    }else
                        pdpt[ pdpt_i ] = 0;
                }
                if( started )
                    pml4[ pml4_i ] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
                pdpt = pt;
            }else
                pml4[ pml4_i ] = 0;
        }
    }
    E_main_Q_cr0_P(pml4);
    return (Pc)pt + E_memory_S_page_size;
}
//__attribute__ ((__noreturn__))
void
main( struct E_main_Z_memory_table_entry *memory_table
, struct E_main_Z_video *video_
){  video = video_;
    E_main_Q_memory_table_I_sort( memory_table );
    E_main_Q_memory_table_I_remove_overlapping( &memory_table );
    N max_memory;
    Pc page_tables = E_main_I_allocate_page_table( memory_table, &max_memory );
    if( !page_tables )
        goto End;
    //E_mem_M( page_tables );
    //if( !~E_font_M() )
        //goto End;
    E_vga_I_fill_rect( 0, 0, video->width, video->height, E_vga_R_video_color( E_vga_S_background_color ));
    E_vga_I_draw_rect( video->width / 4, video->height / 4, video->width / 2, video->height / 2, E_vga_R_video_color(0) );
    //E_font_I_draw( 100, 100, E_vga_Z_color_M( 0xff, 0, 0 ), 'A' );
    
End:__asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
