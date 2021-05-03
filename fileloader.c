/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   file boot loader
*         after protected mode initialization
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
*******************************************************************************/
typedef char            C;
typedef unsigned long   N64;
typedef unsigned        N32;
typedef unsigned short  N16;
typedef unsigned char   N8;
typedef N64             N;
typedef _Bool               B;
typedef void            *P;
typedef C               *Pc;
typedef N               *Pn;
//------------------------------------------------------------------------------
#define false                               0
#define true                                1
#define no                                  false
#define yes                                 true
//------------------------------------------------------------------------------
#define _J_ab(a,b)                          a##b
#define J_ab(a,b)                           _J_ab(a,b)
#define J_a_b(a,b)                          J_ab(J_ab(a,_),b)
//------------------------------------------------------------------------------
#define J_swap(type,a,b)                    { type J_autogen(c) = a; a = b; b = J_autogen(c); }
//------------------------------------------------------------------------------
#define J_autogen_S                         _autogen
#define J_autogen(a)                        J_a_b( a, J_autogen_S )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define O                                   while(yes)
#define for_n_(i_var,n)                     for( i_var = 0; i_var != (n); i_var++ )
#define for_n(i_var,n)                      N i_var; for_n_(i_var,(n))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _internal                           static
#define _inline                             static __attribute__ ((__always_inline__,__unused__))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define E_main_Z_memory_table_end           0x80000
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define Z_page_entry_S_p                    ( 1 << 0 )
#define Z_page_entry_S_rw                   ( 1 << 1 )
//==============================================================================
#define E_simple_Z_p_I_align_down_to_v2(p,v2) (Pc)E_simple_Z_n_I_align_down_to_v2( (N)p, v2 )
_inline
N
E_simple_Z_n_I_align_down_to_v2( N n
, N v2
){  if( !v2 )
        return n;
    return n & ~( v2 - 1 );
}
//==============================================================================
struct __attribute__(( __packed__ )) E_main_Z_memory_table_entry
{ Pc address;
  N64 size;
  N32 type;
  N32 extended_attributes;
};
//==============================================================================
#define E_simple_Z_p_I_align_up_to_v2(p,v2)     (P)E_simple_Z_n_I_align_up_to_v2( (N)p, v2 )
_internal
N
E_simple_Z_n_I_align_up_to_v2( N n
, N v2
){  if( !v2 )
        return n;
    N a = v2 - 1;
    return ( n + a ) & ~a;
}
_internal
P
E_mem_Q_blk_I_copy( P dst
, P src
, N l
){  Pn dst_n = E_simple_Z_p_I_align_up_to_v2( dst, sizeof(N) );
    Pn src_n = E_simple_Z_p_I_align_up_to_v2( src, sizeof(N) );
    if( (Pc)src + l >= (Pc)src_n
    && (Pc)dst_n - (Pc)dst == (Pc)src_n - (Pc)src
    )
    {   N l_0 = (Pc)src_n - (Pc)src;
        N l_1 = ( l - l_0 ) / sizeof(N);
        N l_2 = ( l - l_0 ) % sizeof(N);
        Pc dst_c = dst, src_c = src;
        for_n( i, l_0 )
        {   *dst_c = *src_c;
            dst_c++;
            src_c++;
        }
        for_n_( i, l_1 )
        {   *dst_n = *src_n;
            dst_n++;
            src_n++;
        }
        dst_c = (Pc)dst_n;
        src_c = (Pc)src_n;
        for_n_( i, l_2 )
        {   *dst_c = *src_c;
            dst_c++;
            src_c++;
        }
        return dst_c;
    }
    Pc dst_c = dst, src_c = src;
    for_n( i, l )
    {   *dst_c = *src_c;
        dst_c++;
        src_c++;
    }
    return dst_c;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
        return no;
    }
    *address = 0xaa55aa55aa55aa55;
    __asm__ (
    "\n"    "wbinvd"
    );
    if( *address != 0xaa55aa55aa55aa55 )
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
N
E_main_I_allocate_page_table(  struct E_main_Z_memory_table_entry *memory_table
){  N max_free_memory = ~0;
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
            {   max_free_memory = address;
                goto End_loop_1;
            } 
        }
    }
End_loop_1:
    if( ~max_free_memory )
        goto End;
//#define DEBUG
#define DEBUG_2
#ifdef DEBUG
    pml4 = (Pn)0x100000; // Start od pierwszego magabajta, rosnąco.
#else
    pml4 = (Pn)0x200000;
#endif
    pdpt = (Pn)( (Pc)pml4 + 0x1000 );
    pml4[0] = (N)pdpt | Z_page_entry_S_p | Z_page_entry_S_rw;
    pd = (Pn)( (Pc)pdpt + 0x1000 );
    B end = no;
    for_n( pdpt_i, 16 ) // 16 GiB pamięci. Tablice stron pamięci zajmują ok. 32 MiB.
    {   pt = (Pn)( (Pc)pd + 0x1000 );
        for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
#ifdef DEBUG
                if( address < 24 * 1024 * 1024
                || ( address >= 25 * 1024 * 1024
                  && address < 240 * 1024 * 1024
                )
                || address >= 241 * 1024 * 1024
                ) //DBG
#endif
                    pt[ pt_i ] = address | Z_page_entry_S_p | Z_page_entry_S_rw;
#ifdef DEBUG_2
                if( address == 505 * 1024 * 1024 )
                {   end = yes;
                    break;
                }
#endif
            }
            pd[ pd_i ] = (N)pt | Z_page_entry_S_p | Z_page_entry_S_rw;
            if(end)
                break;
            pt = (Pn)( (Pc)pt + 0x1000 );
        }
        pdpt[ pdpt_i ] = (N)pd | Z_page_entry_S_p | Z_page_entry_S_rw;
        if(end)
            break;
        pd = pt;
    }
#ifdef DEBUG
    return 0; //DBG
#endif
    E_main_Q_cr0_P(pml4);
    return 0; //DBG
    for_n_( pdpt_i, 16 )
    {   for_n( pd_i, 512 )
        {   for_n( pt_i, 512 )
            {   N address = ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                if( pd_i >= 32
                && !( address & 0xfffff )
                && !E_main_I_test_page(address)
                )
                {   max_free_memory = address;
                    goto End_loop_2;
                } 
            }
        }
    }
End_loop_2:
    max_free_memory = 256 * 1024 * 1024; //DBG
    if( ~max_free_memory )
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
        pdpt = pd;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pml4_i, 8 )
    {   for_n( pdpt_i, 512 )
        {   for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    if( pdpt_i >= 16
                    && !( address & 0xfffff )
                    && !E_main_I_test_page(address)
                    )
                    {   max_free_memory = address;
                        goto End_loop_3;
                    } 
                }
            }
        }
    }
End_loop_3:
    if( ~max_free_memory )
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
        pdpt = pd;
    }
    E_main_Q_cr0_P(pml4);
    for_n_( pml4_i, 512 )
    {   for_n( pdpt_i, 512 )
        {   for_n( pd_i, 512 )
            {   for_n( pt_i, 512 )
                {   N address = ( pml4_i * ( 1L << 39 )) | ( pdpt_i * ( 1 << 30 )) | ( pd_i * ( 1 << 21 )) | ( pt_i * 0x1000 );
                    if( pml4_i >= 8
                    && !( address & 0xfffff )
                    && !E_main_I_test_page(address)
                    )
                    {   max_free_memory = address;
                        goto End;
                    } 
                }
            }
        }
    }
End:
    
    return 0;
}
void
main( struct E_main_Z_memory_table_entry *memory_table
, P video_buffer
){  E_main_Q_memory_table_I_sort( memory_table );
    E_main_Q_memory_table_I_remove_overlapping( &memory_table );
    if( E_main_I_allocate_page_table( memory_table ))
        goto End;
    //N16 *pixel = video_buffer;
    //for_n( i, 1 )
        //pixel[i] = 0xffff;
    
End:__asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
