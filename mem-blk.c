/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   file boot loader
*         memory blocks manager
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒12 e
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
#define E_mem_Q_blk_S_allocated_S_free_id   ( E_base_S->E_mem_Q_blk_S_table_allocated_id - 1 )
//==============================================================================
struct E_base_Z
{ struct E_mem_Q_blk_Z_allocated *E_mem_Q_blk_S_allocated;
  N E_mem_Q_blk_S_table_allocated_id;
} *E_base_S = (P)E_memory_S_start;
struct E_mem_Q_blk_Z_free
{ Pc p;
  N l;
};
struct E_mem_Q_blk_Z_allocated
{ Pc p;
  N u;
  N n;
};
//==============================================================================
_internal N E_mem_Q_blk_Q_sys_table_R_new_id( N, N, N, P, N );
_internal P E_mem_Q_blk_Q_table_M_from_free( N, N, N, P, N, N );
_internal __attribute__ ((__malloc__)) P E_mem_Q_blk_R_new_0(void);
//==============================================================================
void
E_mem_M( struct E_main_Z_memory_table_entry *memory_table
){  E_base_S->E_mem_Q_blk_S_table_allocated_id = 1;
    E_base_S->E_mem_Q_blk_S_allocated = (P)( (Pc)E_base_S + sizeof( *E_base_S ));
    E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p = (P)( (Pc)E_base_S + sizeof( *E_base_S ) + 3 * sizeof( struct E_mem_Q_blk_Z_allocated ));
    E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].u = sizeof( struct E_mem_Q_blk_Z_free );
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].p = (P)E_base_S->E_mem_Q_blk_S_allocated;
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].u = sizeof( struct E_mem_Q_blk_Z_allocated );
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n = 3;
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id + 1 ].p = (P)E_base_S;
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id + 1 ].u = 1;
    E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id + 1 ].n = sizeof( *E_base_S );
    struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
    N free_i = 0;
    for( struct E_main_Z_memory_table_entry *entry = memory_table; entry != ( struct E_main_Z_memory_table_entry * )E_main_Z_memory_table_end; entry++ )
    {   if( !entry->size
        || !( entry->extended_attributes & 1 )
        || entry->type != E_main_Z_memory_table_Z_memory_type_S_available
        )
            continue;
        free_p[ free_i ].p = entry->address;
        free_p[ free_i ].l = entry->size;
        free_i++;
    }
    E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n = free_i;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
B
E_mem_Q_blk_T_eq( P p_1
, P p_2
, N l
){  for_n( i, l )
        if( *( (Pc)p_1 + l ) != *( (Pc)p_2 + l ))
            return no;
    return yes;
}
P
E_mem_Q_blk_I_copy( P dst
, P src
, N l
){  Pn dst_n = (Pn)E_simple_Z_p_I_align_up_to_v2( dst, sizeof(N) );
    Pn src_n = (Pn)E_simple_Z_p_I_align_up_to_v2( src, sizeof(N) );
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
P
E_mem_Q_blk_I_copy_rev( P dst
, P src
, N l
){  Pn dst_n = (Pn)E_simple_Z_p_I_align_down_to_v2( (Pc)dst + l, sizeof(N) );
    Pn src_n = (Pn)E_simple_Z_p_I_align_down_to_v2( (Pc)src + l, sizeof(N) );
    if( (Pc)src <= (Pc)src_n
    && (Pc)dst + l - (Pc)dst_n == (Pc)src + l - (Pc)src_n
    )
    {   N l_0 = (Pc)src + l - (Pc)src_n;
        N l_1 = ( l - l_0 ) / sizeof(N);
        Pc dst_c = (Pc)dst + l, src_c = (Pc)src + l;
        for_n( i, l_0 )
        {   dst_c--;
            src_c--;
            *dst_c = *src_c;
        }
        for_n_( i, l_1 )
        {   dst_n--;
            src_n--;
            *dst_n = *src_n;
        }
        dst_c = (Pc)dst_n;
        src_c = (Pc)src_n;
        while( src_c != src )
        {   dst_c--;
            src_c--;
            *dst_c = *src_c;
        }
        return dst_c;
    }
    Pc dst_c = (Pc)dst + l, src_c = (Pc)src + l;
    while( src_c != src )
    {   dst_c--;
        src_c--;
        *dst_c = *src_c;
    }
    return dst_c;
}
P
E_mem_Q_blk_I_copy_auto( P dst
, P src
, N l
){  if( (Pc)dst < (Pc)src
    || (Pc)dst >= (Pc)src + l
    )
        return E_mem_Q_blk_I_copy( dst, src, l );
    return E_mem_Q_blk_I_copy_rev( dst, src, l );
}
P
memmove( P dst
, P src
, N l
){  E_mem_Q_blk_I_copy_auto( dst, src, l );
    return dst;
}
__attribute__ ((__alias__( "memmove" )))
P memcpy( P, P, N );
P
E_mem_Q_blk_P_fill_c( P p
, N l
, C c
){  Pc s = p;
    for_n( i, l )
        *s++ = c;
    return s;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
_internal
B
E_mem_Q_blk_Q_sys_table_mf_I_unite( N table_i
, N rel_addr_p
, N rel_addr_l
, P p
, N l
){  N i_found = ~0;
    for_n( i, E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ) /// Szukanie bloku przyległego od dołu.
    {   Pc *p_ = (P)( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p );
        N *l_ = (P)( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l );
        if( *p_ + *l_ == p )
        {   p = *p_;
            l = *l_ += l;
            i_found = i;
            break;
        }
    }
    for_n_( i, E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ) /// Szukanie bloku przyległego od góry.
    {   Pc *p_ = (P)( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p );
        N *l_ = (P)( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l );
        if( (Pc)p + l == *p_ )
        {   if( ~i_found ) /// Był znaleziony blok przyległy od dołu.
            {   *( Pc * )( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + i_found * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) += *l_;
                *p_ = 0;
                *l_ = 0;
            }else
            {   *p_ = p;
                *l_ += l;
                i_found = i;
            }
            break;
        }
    }
    return !!~i_found;
}
_internal
N
E_mem_Q_blk_Q_sys_table_mf_P_put( N table_i
, N rel_addr_p
, N rel_addr_l
, P p
, N l
){  if( !E_mem_Q_blk_Q_sys_table_mf_I_unite( table_i, rel_addr_p, rel_addr_l, p, l ))
    {   if( table_i == E_mem_Q_blk_S_allocated_S_free_id
        && l >= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u
        )
        {   if( (Pc)p + l == E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p ) /// Nowy blok jest przyległy od dołu do tablicy bloków.
            {   E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p -= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p;
                free_p[0].l = l - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                free_p[0].p = free_p[0].l ? p : 0;
                return 0;
            }
            if( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p + E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u == p ) /// Nowy blok jest przyległy od góry do tablicy bloków.
            {   struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p;
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ].l = l - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ].p = free_p[ E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ].l
                  ? (Pc)p + E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u
                  : 0;
                E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                return 0;
            }
        }
        N i = E_mem_Q_blk_Q_sys_table_R_new_id( table_i, rel_addr_p, rel_addr_l, p, l );
        if( !~i )
            return ~0;
    }
    return 0;
}
_internal
N
E_mem_Q_blk_Q_sys_table_R_new_id( N table_i
, N rel_addr_p
, N rel_addr_l
, P p /// Adres do nowego wpisu.
, N l /// I rozmiar.
){  Pc p_0 = E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p;
    N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
    if( table_i == E_mem_Q_blk_S_allocated_S_free_id )
    {   for_n( i, E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ) /// Szukanie wolnego wpisu w tablicy.
            if( !*( P * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ))
            {   *( N * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
                *( P * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
                return i;
            }
        struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
        N l_ = E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
        for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
            if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
            {   if( free_p[ free_i ].l >= l_ )
                {   free_p[ free_i ].l -= l_;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
                        *( N * )( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
                    *( P * )( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
                    E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p -= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                    E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                    if( table_i == E_base_S->E_mem_Q_blk_S_table_allocated_id )
                    {   E_base_S->E_mem_Q_blk_S_allocated = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].p;
                        E_base_S->E_mem_Q_blk_S_table_allocated_id++;
                    }
                    return 0;
                }
                break;
            }
        for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
            if( p_0 + l_0 == free_p[ free_j ].p )
            {   if( free_p[ free_j ].l >= l_ )
                {   free_p[ free_j ].l -= l_;
                    if( free_p[ free_j ].l )
                        free_p[ free_j ].p += l_;
                    else
                        free_p[ free_j ].p = 0;
                    if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
                        *( N * )( p_0 + l_0 + rel_addr_l ) = l;
                    *( P * )( p_0 + l_0 + rel_addr_p ) = p;
                    return E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                }
                break;
            }
    }else
    {   for_n( i, E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ) /// Szukanie wolnego wpisu w tablicy.
            if( !*( P * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ))
            {   if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
                    *( N * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
                *( P * )( p_0 + i * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
                return i;
            }
        struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
        for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
            if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
            {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u )
                {   free_p[ free_i ].l -= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
                        *( N * )( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
                    *( P * )( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p - E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
                    E_base_S->E_mem_Q_blk_S_allocated[ table_i ].p -= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                    E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                    if( table_i == E_base_S->E_mem_Q_blk_S_table_allocated_id )
                        E_base_S->E_mem_Q_blk_S_allocated = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id++ ].p;
                    return 0;
                }
                break;
            }
        for_n_( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
            if( p_0 + l_0 == free_p[ free_i ].p )
            {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u )
                {   free_p[ free_i ].l -= E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                    if( free_p[ free_i ].l )
                        free_p[ free_i ].p += E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u;
                    else
                        free_p[ free_i ].p = 0;
                    if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
                        *( N * )( p_0 + l_0 + rel_addr_l ) = l;
                    *( P * )( p_0 + l_0 + rel_addr_p ) = p;
                    return E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n++;
                }
                break;
            }
    }
    Pc p_1 = E_mem_Q_blk_Q_table_M_from_free( table_i
    , E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u
    , E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n + 1
    , E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n ? p_0 : 0
    , l_0
    , 0
    );
    if( !p_1 )
        return ~0;
    if( table_i == E_mem_Q_blk_S_allocated_S_free_id )
    {   *( P * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = 0;
        *( N * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = 0;
        if( !E_mem_Q_blk_Q_sys_table_mf_I_unite( table_i, rel_addr_p, rel_addr_l, p, l ))
        {   *( P * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
            *( N * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
        }
    }else
    {   *( P * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_p ) = p;
        if( table_i != E_base_S->E_mem_Q_blk_S_table_allocated_id )
            *( N * )( p_1 + ( E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1 ) * E_base_S->E_mem_Q_blk_S_allocated[ table_i ].u + rel_addr_l ) = l;
    }
    return E_base_S->E_mem_Q_blk_S_allocated[ table_i ].n - 1;
}
/// Dla tablicy systemowej “free” ‘alokuje’ tyle “n”, ile żądane, lub więcej.
_internal
P
E_mem_Q_blk_Q_table_M_from_free( N allocated_or_table_i
, N u
, N n
, P p /// Adres uprzedniej zawartości lub 0, gdy brak.
, N l /// I rozmiar. Jeśli “p == 0”, to parametr ignorowany.
, N l_rel
){  N l_1 = n * u;
    Pc p_1;
    if(n)
    {   if( allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id
        && p /// Uprzedni obszar tablicy staje się wolnym blokiem.
        )
        {   n++;
            l_1 += u;
        }
        N l_align;
        if( allocated_or_table_i == E_base_S->E_mem_Q_blk_S_table_allocated_id
        || allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id
        )
            l_align = sizeof(N);
        else if( u > sizeof(N) && u % sizeof(N) == 0 )
            l_align = sizeof(N);
        else if( u <= sizeof(N) && E_simple_Z_n_T_power_2(u) )
            l_align = u;
        else
            l_align = 1;
        if( allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id ) /// Obszar przed wyrównanym adresem staje się wolnym blokiem.
        {   n++;
            l_1 += u;
        }
        N l_ = ~0;
        N i_found;
        struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
        for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku na całą tablicę.
        {   p_1 = E_simple_Z_p_I_align_up_to_v2( free_p[ free_i ].p, l_align );
            if( free_p[ free_i ].l >= ( p_1 - free_p[ free_i ].p ) + l_1
            && free_p[ free_i ].l < l_
            )
            {   l_ = free_p[ free_i ].l;
                i_found = free_i;
                if( l_ == ( p_1 - free_p[ free_i ].p ) + l_1 )
                    break;
            }
        }
        if( !~l_ )
            return 0;
        Pc old_free_p = free_p[ i_found ].p;
        p_1 = E_simple_Z_p_I_align_up_to_v2( old_free_p, l_align );
        free_p[ i_found ].l -= ( p_1 - old_free_p ) + l_1;
        if( free_p[ i_found ].l )
            free_p[ i_found ].p += ( p_1 - old_free_p ) + l_1;
        else
            free_p[ i_found ].p = 0;
        if(p)
        {   E_mem_Q_blk_I_copy( p_1 + l_rel, p, l );
            if( allocated_or_table_i == E_base_S->E_mem_Q_blk_S_table_allocated_id )
                E_base_S->E_mem_Q_blk_S_allocated = (P)p_1;
        }
        if( p_1 - old_free_p )
        {   if( allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id )
            {   free_p = (P)( p_1 + l_rel );
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n + 1 ].p = old_free_p;
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n + 1 ].l = p_1 - old_free_p;
            }else
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, old_free_p, p_1 - old_free_p ))
                    return 0;
            }
        }else if( allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id )
        {   free_p = (P)( p_1 + l_rel );
            free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n + 1 ].p = 0;
            free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n + 1 ].l = 0;
        }
    }else if( !( p_1 = E_mem_Q_blk_R_new_0() ))
        return 0;
    if( !p ) //NDFN Rozpoznanie niebezpośrednie, mimo że jednoznaczne w obecnej implementacji.
        E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].u = u;
    E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].p = p_1;
    if(p)
    {   struct E_mem_Q_blk_Z_free free_p_;
        if( allocated_or_table_i == E_mem_Q_blk_S_allocated_S_free_id )
        {   struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].p;
            if( E_mem_Q_blk_Q_sys_table_mf_I_unite( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, l ))
            {   free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n ].p = 0;
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n ].l = 0;
            }else
            {   free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n ].p = p;
                free_p[ E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n ].l = l;
            }
        }else
        {   if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, l ))
                return 0;
        }
    }
    E_base_S->E_mem_Q_blk_S_allocated[ allocated_or_table_i ].n = n;
    return p_1;
}
_internal
__attribute__ ((__malloc__))
P
E_mem_Q_blk_R_new_0( void
){  Pc p_end = (Pc)E_memory_S_start;
        Pc p = (P)1;
    B exists;
    O{  exists = no;
        N allocated_i = E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n;
        while( allocated_i-- )
        {   if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == p )
            {   exists = yes;
                break;
            }
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p < p_end
            && E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p > p
            )
            {   p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                exists = yes;
                break;
            }
        }
        if( !exists )
            return p;
        if( ++p == p_end )
            break;
    }
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
P
E_mem_Q_blk_M( N l
){  return E_mem_Q_blk_M_tab( 1, l );
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_tab(
  N u
, N n
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_i = E_mem_Q_blk_Q_sys_table_R_new_id( E_base_S->E_mem_Q_blk_S_table_allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p, (Pc)&allocated_p.n - (Pc)&allocated_p, 0, 0 );
    if( !~allocated_i )
        return 0;
    struct E_mem_Q_blk_Z_free free_p_;
    if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 )
    || !E_mem_Q_blk_Q_table_M_from_free( allocated_i, u, n, 0, 0, 0 )
    )
        return 0;
    return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_replace_tab( P *p
, N u
, N n
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, *( P * )p, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u ))
                    return 0;
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 )
            || !E_mem_Q_blk_Q_table_M_from_free( allocated_i, u, n, 0, 0, 0 )
            )
            {   *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p = 0;
                return 0;
            }
            P p_ = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
            *( P * )p = p_;
            return p_;
        }
    return 0;
}
P
E_mem_Q_blk_M_replace( P p
, N l
){  return E_mem_Q_blk_M_replace_tab( p, 1, l );
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_split( P p
, N i
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_i = E_mem_Q_blk_Q_sys_table_R_new_id( E_base_S->E_mem_Q_blk_S_table_allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p, (Pc)&allocated_p.n - (Pc)&allocated_p, 0, 0 );
    if( !~allocated_i )
        return 0;
    for_n( allocated_j, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_j ].p == p )
        {   E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n = E_base_S->E_mem_Q_blk_S_allocated[ allocated_j ].n - i;
            E_base_S->E_mem_Q_blk_S_allocated[ allocated_j ].n = i;
            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u = E_base_S->E_mem_Q_blk_S_allocated[ allocated_j ].u;
            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p = (Pc)p + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_j ].u;
            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
        }
    return 0;
}
N
E_mem_Q_blk_W( P p
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == p )
        {   E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p = 0;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u ))
                    return ~0;
            }
            return 0;
        }
    return ~0;
}
//------------------------------------------------------------------------------
P
E_mem_Q_blk_I_add( P p
, N n
, N *n_prepended
, N *n_appended
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            N l = n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
                for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
                    if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                    {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u )
                        {   l_1 = free_p[ free_i ].l;
                            if( l_1 > l )
                                l_1 = l;
                            else if( l_1 < l )
                                l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u );
                            l -= l_1;
                        }
                        break;
                    }
                if( !l )
                {   free_p[ free_i ].l -= l_1;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                    *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                    if( n_prepended )
                        *n_prepended = n;
                    if( n_appended )
                        *n_appended = 0;
                    return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1;
                }
                for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
                    if( p_0 + l_0 == free_p[ free_j ].p )
                    {   if( free_p[ free_j ].l >= l )
                        {   if( free_p[ free_j ].l != l )
                                free_p[ free_j ].p += l;
                            else
                                free_p[ free_j ].p = 0;
                            free_p[ free_j ].l -= l;
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( !free_p[ free_i ].l )
                                    free_p[ free_i ].p = 0;
                                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                            }
                            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                            if( n_prepended )
                                *n_prepended = l_1 / E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                            if( n_appended )
                                *n_appended = l / E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1 + l_0;
                        }
                        break;
                    }
                l += l_1; /// Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, zabrakło do pełnej liczby od góry.
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 ))
                return 0;
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( allocated_i
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            if( n_prepended )
                *n_prepended = 0;
            if( n_appended )
                *n_appended = n;
            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_0;
        }
    return 0;
}
P
E_mem_Q_blk_I_prepend_append( P p
, N n_prepend
, N n_append
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   N l = ( n_prepend + n_append ) * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                N l_1 = 0;
                p_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
                for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
                    if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                    {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u )
                        {   l_1 = free_p[ free_i ].l;
                            if( l_1 > l )
                                l_1 = l;
                            else if( l_1 < l )
                                l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u );
                            l -= l_1;
                        }
                        break;
                    }
                if( !n_append
                && !l
                )
                {   free_p[ free_i ].l -= l_1;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                    E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n_prepend;
                    return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1;
                }
                // Kontynuacja od tej linii: trzeba wybrać optymalne przesunięcie pomiędzy blokiem poprzedzającym a następującym, by ewentualnie nie kopiować.
                for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
                    if( p_0 + l_0 == free_p[ free_j ].p )
                    {   if( free_p[ free_j ].l >= l )
                        {   if( free_p[ free_j ].l != l )
                                free_p[ free_j ].p += l;
                            else
                                free_p[ free_j ].p = 0;
                            free_p[ free_j ].l -= l;
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( !free_p[ free_i ].l )
                                    free_p[ free_i ].p = 0;
                                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                            }
                            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n_prepend + n_append;
                            
                            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1 + l_0;
                        }
                        break;
                    }
                l += l_1; /// Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, zabrakło do pełnej liczby od góry.
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 ))
                return 0;
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( allocated_i
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n + n_prepend + n_append
            , p_0
            , l_0
            , n_prepend * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
        }
    return 0;
}
P
E_mem_Q_blk_I_append( P p
, N n
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            Pc p_0 = 0;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
                for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
                    if( p_0 + l_0 == free_p[ free_i ].p )
                    {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u )
                        {   l_1 = free_p[ free_i ].l;
                            if( l_1 > l )
                                l_1 = l;
                            else if( l_1 < l )
                                l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u );
                            l -= l_1;
                        }
                        break;
                    }
                if( !l )
                {   free_p[ free_i ].l -= l_1;
                    if( free_p[ free_i ].l )
                        free_p[ free_i ].p += l_1;
                    else
                        free_p[ free_i ].p = 0;
                    E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                    return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_0;
                }
                for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
                    if( free_p[ free_j ].p + free_p[ free_j ].l == p_0 )
                    {   if( free_p[ free_j ].l >= l )
                        {   if( free_p[ free_j ].l == l )
                                free_p[ free_j ].p = 0;
                            free_p[ free_j ].l -= l;
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( free_p[ free_i ].l )
                                    free_p[ free_i ].p += l_1;
                                else
                                    free_p[ free_i ].p = 0;
                            }
                            Pc p_1 = p_0 - l;
                            E_mem_Q_blk_I_copy( p_1, p_0, l_0 );
                            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                            *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p = p_1;
                            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_0;
                        }
                        break;
                    }
                l += l_1; /// Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, a zabrakło do pełnej liczby od góry.
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 ))
                return 0;
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( allocated_i
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return (Pc)p_1 + l_0;
        }
    return 0;
}
P
E_mem_Q_blk_I_prepend( P p
, N n
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            Pc p_0 = 0;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
                for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
                    if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                    {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u )
                        {   l_1 = free_p[ free_i ].l;
                            if( l_1 > l )
                                l_1 = l;
                            else if( l_1 < l )
                                l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u );
                            l -= l_1;
                        }
                        break;
                    }
                if( !l )
                {   free_p[ free_i ].l -= l_1;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                    *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                    return p_0;
                }
                for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
                    if( p_0 + l_0 == free_p[ free_j ].p )
                    {   if( free_p[ free_j ].l >= l )
                        {   if( free_p[ free_j ].l != l )
                                free_p[ free_j ].p += l;
                            else
                                free_p[ free_j ].p = 0;
                            free_p[ free_j ].l -= l;
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( !free_p[ free_i ].l )
                                    free_p[ free_i ].p = 0;
                            }
                            E_mem_Q_blk_I_copy_rev( p_0 + l, p_0, l_0 );
                            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                            if( l_1 )
                                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                            return p_0 + l;
                        }
                        break;
                    }
                l += l_1; /// Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, a zabrakło do pełnej liczby od góry.
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 ))
                return 0;
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( allocated_i
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , l
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l;
        }
    return 0;
}
P
E_mem_Q_blk_I_insert( P p
, N i
, N n
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
            {   N l = n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                N l_1 = 0;
                p_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].p;
                for_n( free_i, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od dołu.
                    if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                    {   if( free_p[ free_i ].l >= E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u )
                        {   l_1 = free_p[ free_i ].l;
                            if( l_1 > l )
                                l_1 = l;
                            else if( l_1 < l )
                                l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u );
                            l -= l_1;
                        }
                        break;
                    }
                if( !l )
                {   free_p[ free_i ].l -= l_1;
                    if( !free_p[ free_i ].l )
                        free_p[ free_i ].p = 0;
                    E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                    *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                    E_mem_Q_blk_I_copy( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p
                    , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1
                    , i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                    );
                    return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                }
                for_n( free_j, E_base_S->E_mem_Q_blk_S_allocated[ E_mem_Q_blk_S_allocated_S_free_id ].n ) /// Szukanie wolnego bloku przyległego od góry.
                    if( p_0 + l_0 == free_p[ free_j ].p )
                    {   if( free_p[ free_j ].l >= l )
                        {   if( free_p[ free_j ].l != l )
                                free_p[ free_j ].p += l;
                            else
                                free_p[ free_j ].p = 0;
                            free_p[ free_j ].l -= l;
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( !free_p[ free_i ].l )
                                    free_p[ free_i ].p = 0;
                                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p -= l_1;
                            }
                            E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n += n;
                            if( l_1 )
                                E_mem_Q_blk_I_copy( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p
                                , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1
                                , i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                                );
                            E_mem_Q_blk_I_copy_rev( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1 + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u + l
                            ,  E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + l_1 + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                            , l_0 - i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                            );
                            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                        }
                        break;
                    }
            }
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_R_new_id( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, 0, 0 ))
                return 0;
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( allocated_i
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
            , E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            );
            if( !p_1 )
                return 0;
            //TODO Zrobić w “E_mem_Q_blk_Q_table_M_from_free_or_map” parametr przesuniecia dla ‘split’ i kopiowania tam odrazu?
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n )
                E_mem_Q_blk_I_copy_rev( (Pc)p_1 + ( i + n ) * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                , (Pc)p_1 + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                , l_0 - i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u
                );
            *( P * )p = p_1;
            return (Pc)p_1 + i * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
        }
    return 0;
}
P
E_mem_Q_blk_I_remove( P p
, N i
, N n
){  for_n( allocated_i, E_base_S->E_mem_Q_blk_S_allocated[ E_base_S->E_mem_Q_blk_S_table_allocated_id ].n )
        if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            N l_0 = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
            if( E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n == n ) /// Usuwany cały blok.
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( P * )p
                , l
                ))
                    return 0;
                E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n = 0;
                E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p = E_mem_Q_blk_R_new_0();
                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
            }else if( i + n == E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n ) /// Usuwane na końcu bloku.
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( Pc * )p + l_0 - l
                , l
                ))
                    return 0;
                E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n -= n;
            }else if( !i ) /// Usuwane na początku bloku.
            {   P p_ = *( P * )p;
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , p_
                , l
                ))
                    return 0;
                E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n -= n;
                *( P * )p = E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p += l;
            }else /// Usuwane w środku bloku.
            {   Pc p_0 = *( Pc * )p + ( i + n ) * E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].u;
                E_mem_Q_blk_I_copy( p_0 - l
                , p_0
                , *( Pc * )p + l_0 - p_0
                );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_mf_P_put( E_mem_Q_blk_S_allocated_S_free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( Pc * )p + l_0 - l
                , l
                ))
                    return 0;
                E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].n -= n;
            }
            return E_base_S->E_mem_Q_blk_S_allocated[ allocated_i ].p;
        }
    return 0;
}
/******************************************************************************/
