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
typedef N64             N;
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define E_main_Z_memory_table_end           0x80000UL
//==============================================================================
struct __attribute__((__packed__)) E_main_Z_memory_table_entry
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
){  return;
    N n = ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end - memory_table;
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
void
main( struct E_main_Z_memory_table_entry *memory_table
){  E_main_Q_memory_table_I_sort( memory_table );
    //E_main_Q_memory_table_I_remove_overlapping( &memory_table );
    __asm__ (
    "\n0:"  "hlt"
    "\n"    "jmp    0b"
    );
}
/******************************************************************************/
