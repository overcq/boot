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
#define Z_page_entry_S_p                    ( 1 << 0 )
#define Z_page_entry_S_rw                   ( 1 << 1 )
//==============================================================================
#define E_main_S_top                        0x80000
//==============================================================================
//#define C_text_mode
#ifdef C_text_mode
struct __attribute__(( __packed__ )) E_text_Z_c
{ N8 character;
  N8 color;
} *E_text_S_buffer = (P)0xb8000;
enum
{ E_text_Z_color_S_black
, E_text_Z_color_S_blue
, E_text_Z_color_S_green
, E_text_Z_color_S_cyan
, E_text_Z_color_S_red
, E_text_Z_color_S_magenta
, E_text_Z_color_S_brown
, E_text_Z_color_S_light_gray
, E_text_Z_color_S_dark_gray
, E_text_Z_color_S_light_blue
, E_text_Z_color_S_light_green
, E_text_Z_color_S_light_cyan
, E_text_Z_color_S_light_red
, E_text_Z_color_S_pink
, E_text_Z_color_S_yellow
, E_text_Z_color_S_white
};
N E_text_S_columns = 80;
N E_text_S_rows = 25;
N E_text_S_column = 0;
N E_text_S_row = 0;
N8 E_text_S_color = ( E_text_Z_color_S_black << 4 ) | E_text_Z_color_S_light_gray;
void
E_text_I_clear_row( N y
){  for_n( x, E_text_S_columns )
        E_text_S_buffer[ y * E_text_S_columns + x ] = ( struct E_text_Z_c ){ ' ', ( E_text_Z_color_S_black << 4 ) | E_text_Z_color_S_light_gray };
}
void
E_text_I_clear( void
){  for_n( y, E_text_S_rows )
        E_text_I_clear_row(y);
    E_text_S_column = 0;
    E_text_S_row = 0;
}
void
E_text_I_scroll( void
){  for_n( y, E_text_S_rows - 1 )
    {   for_n( x, E_text_S_columns )
           E_text_S_buffer[ y * E_text_S_columns + x ] = E_text_S_buffer[ ( y + 1 ) * E_text_S_columns + x ];
    }
    E_text_I_clear_row( E_text_S_rows - 1 );
}
void
E_text_I_print_c( C c
){  if( E_text_S_column == E_text_S_columns )
    {   E_text_S_column = 0;
        if( E_text_S_row + 1 == E_text_S_rows )
            E_text_I_scroll();
        else
            E_text_S_row++;
    }
    if( c == '\n' )
        E_text_S_column = E_text_S_columns;
    else
    {   E_text_S_buffer[ E_text_S_row * E_text_S_columns + E_text_S_column ] = ( struct E_text_Z_c ){ c, E_text_S_color };
        E_text_S_column++;
    }
}
void
E_text_I_print( Pc s
){  while( *s )
    {   E_text_I_print_c( *s );
        s++;
    }
}
void
E_text_I_print_hex( N n
){  E_text_I_print( "0x" );
    for_n_rev( i, sizeof(N) * 2 )
    {   C c = ( n >> ( i * 4 )) & 0xf;
        if( c < 10 )
            c += '0';
        else
            c += 'a' - 10;
        E_text_I_print_c(c);
    }
}
void
E_text_I_print_memory_ranges( struct E_main_Z_memory_table_entry *memory_table
){  E_text_I_print( "Memory ranges:\n" );
    for( struct E_main_Z_memory_table_entry *entry = memory_table; entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end; entry++ )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        )
            continue;
        E_text_I_print_hex( (N)entry->address );
        E_text_I_print( ", " );
        E_text_I_print_hex( entry->size );
        E_text_I_print( ", " );
        E_text_I_print_hex( entry->type );
        E_text_I_print_c( '\n' );
    }
    E_text_I_print( "End.\n" );
}
#endif
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Pc
E_text_Z_s_Z_utf8_R_u( Pc s
, U *u
){  if( (S8)*s >= 0 )
    {   *u = *s;
        return s + 1;
    }
    U v = ~(S8)*s;
    N n = E_asm_I_bsr(v);
    if( n == ~0
    || n == 6
    )
        return s;
    v = *s & ( _v( v, 1 ) << n ) - 1;
    for_n( i, 6 - n )
    {   if(( *++s & 0xc0 ) != 0x80 )
            return s;
        v <<= 6;
        v |= *s & 0x3f;
    }
    *u = v;
    return s + 1;
}
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
    {   entry->extended_attributes = 1;
        return entry;
    }
    E_mem_Q_blk_I_copy( *memory_table - 1, *memory_table, ( entry - *memory_table ) * sizeof( struct E_main_Z_memory_table_entry ));
    (*memory_table)--;
    ( entry - 1 )->extended_attributes = 1;
    return entry - 1;
}
//------------------------------------------------------------------------------
_internal
void
E_main_Q_memory_table_I_sort( struct E_main_Z_memory_table_entry *memory_table
){  N n = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - memory_table;
    while( n > 1 )
    {   struct E_main_Z_memory_table_entry *entry;
        for_n( i, n - 1 )
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
                        entry--;
                        next_entry--;
                        new_entry->address = next_entry->address + next_entry->size;
                        new_entry->size = entry->address + entry->size - new_entry->address;
                        new_entry->type = entry->type;
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
                        entry--;
                        next_entry--;
                        new_entry->address = next_entry->address + next_entry->size;
                        new_entry->size = entry->address + entry->size - new_entry->address;
                        new_entry->type = entry->type;
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
N
E_main_Q_memory_table_R_max_memory( struct E_main_Z_memory_table_entry *memory_table
){  N ret = 0;
    for( struct E_main_Z_memory_table_entry *entry = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - 1; entry >= memory_table; entry-- )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        )
            continue;
        if( entry->type == E_main_Z_memory_table_Z_memory_type_S_available )
        {   ret = (N)entry->address + entry->size;
            break;
        }
    }
    return ret;
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
        {   if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) == (Pc)address )
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    prev_address = entry->address;
            }else if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) < (Pc)address )
                break;
        }else
            if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) == E_simple_Z_p_I_align_up_to_v2( prev_address, E_memory_S_page_size ))
            {   if( entry->type != E_main_Z_memory_table_Z_memory_type_S_available )
                    prev_address = entry->address;
            }else if( E_simple_Z_p_I_align_up_to_v2( entry->address + entry->size, E_memory_S_page_size ) < (Pc)prev_address )
                break;
    }
    return prev_address ? E_simple_Z_p_I_align_down_to_v2( prev_address, E_memory_S_page_size ) : address;
}
_internal
void
E_main_Q_memory_table_I_reduce_by_page_tables( struct E_main_Z_memory_table_entry *memory_table
, Pc page_tables
){  struct E_main_Z_memory_table_entry *entry;
    for( entry = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - 1; entry >= memory_table; entry-- )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        || entry->type != E_main_Z_memory_table_Z_memory_type_S_available
        )
            continue;
        if( entry->address > page_tables )
            entry->extended_attributes = 0;
        else
            break;
    }
    entry->size = page_tables - entry->address;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
, N max_memory
){  if( max_memory < E_memory_S_start + 0x100000 ) // NDFN
        return 0;
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
    if( max_memory <= 64 * 1024 * 1024 )
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
    if( max_memory <= 16UL * 1024 * 1024 * 1024 )
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
    if( max_memory <= 4UL * 1024 * 1024 * 1024 * 1024 )
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
End:pml4 = (P)( E_main_Q_memory_table_I_before_hole( memory_table, (P)max_memory ) - E_memory_S_page_size ); // Start od końca pamięci rzeczywistej, malejąco.
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
                                {   if( address == max_memory - E_memory_S_page_size )
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
    N video_end = E_simple_Z_n_I_align_up_to_v2( (N)E_vga_S_video->framebuffer + E_vga_S_video->line_width * E_vga_S_video->height, E_memory_S_page_size );
    if( max_memory < video_end )
    {   N video_start = E_simple_Z_n_I_align_down_to_v2( E_vga_S_video->framebuffer, E_memory_S_page_size );
        pdpt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pml4 ) - E_memory_S_page_size );
        end = no;
        B started = no;
        B ending_max_memory = no;
        N address = 0;
        B pd_inited, pt_inited;
        for_n_( pml4_i, 512 )
        {   if( !end )
            {   if( address < max_memory )
                    pd_inited = yes;
                else
                    pd_inited = no;
                if( address < max_memory
                || started
                )
                    pd = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pdpt ) - E_memory_S_page_size );
                for_n( pdpt_i, 512 )
                {   if( !end )
                    {   if( address < max_memory )
                            pt_inited = yes;
                        else
                            pt_inited = no;
                        if( address < max_memory
                        || started
                        )
                            pt = (P)( E_main_Q_memory_table_I_before_hole( memory_table, pd ) - E_memory_S_page_size );
                        for_n( pd_i, 512 )
                        {   if( !end )
                            {   for_n( pt_i, 512 )
                                {   address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * E_memory_S_page_size );
                                    if( !end )
                                    {   if( address == max_memory )
                                            ending_max_memory = yes;
                                        if( address >= max_memory
                                        && address >= video_start
                                        ){  if( !started
                                            && ( address == max_memory
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
                                if( address < max_memory
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
__attribute__ ((__noreturn__))
void
main( struct E_main_Z_memory_table_entry *memory_table
, struct E_main_Z_video *video_
){  E_vga_S_video = video_;
    memory_table--;
    *memory_table = ( struct E_main_Z_memory_table_entry )
    { (Pc)(N)E_vga_S_video->framebuffer
    , E_vga_S_video->line_width * E_vga_S_video->height
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    , 1
    };
    memory_table--;
    *memory_table = ( struct E_main_Z_memory_table_entry ) // NDFN
    { (Pc)0x200000
    , 0x100000
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    , 1
    };
    memory_table--;
    *memory_table = ( struct E_main_Z_memory_table_entry )
    { (Pc)0
    , E_main_S_top
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    , 1
    };
    E_main_Q_memory_table_I_sort( memory_table );
    E_main_Q_memory_table_I_remove_overlapping( &memory_table );
#ifdef C_text_mode
    E_text_I_clear();
    E_text_I_print_hex( E_vga_S_video->framebuffer );
    E_text_I_print_c( ',' );
    E_text_I_print_hex( E_vga_S_video->framebuffer + E_vga_S_video->line_width * E_vga_S_video->height );
    E_text_I_print_c( '\n' );
    E_text_I_print_memory_ranges( memory_table );
#endif
    N max_memory = E_main_Q_memory_table_R_max_memory( memory_table );
    Pc page_tables = E_main_I_allocate_page_table( memory_table, max_memory );
    if( !page_tables )
        goto End;
    E_main_Q_memory_table_I_reduce_by_page_tables( memory_table, page_tables );
    E_mem_M( memory_table );
    if( !~E_font_M() )
        goto End;
#ifndef C_text_mode
    E_vga_I_fill_rect( 0, 0, E_vga_S_video->width, E_vga_S_video->height, E_vga_R_video_color( E_vga_S_background_color ));
    E_vga_I_draw_rect( E_vga_S_video->width / 4, E_vga_S_video->height / 4, E_vga_S_video->width / 2, E_vga_S_video->height / 2, E_vga_R_video_color(0) );
    E_font_I_print( "test string" );
#endif
    
End:__asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
