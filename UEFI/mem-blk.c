/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         memory manager
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒4‒6 W
*******************************************************************************/
#include <stddef.h>
#include "fileloader.h"
//==============================================================================
#define E_mem_Q_blk_S_align_to_all  alignof(max_align_t)
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
//==============================================================================
_internal void E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(N);
_internal N E_mem_Q_blk_Q_sys_table_R_last( N, N );
_internal N E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( N, N );
_internal N E_mem_Q_blk_Q_sys_table_M_new_id( N, N, N, P, N );
_internal P E_mem_Q_blk_Q_table_M_from_free( N *, N, N, P, N, N, N );
_internal P E_mem_Q_blk_M_new_0( N * );
//==============================================================================
/* Jeśli pamięć zarezerwowana jest umieszczona od góry (“reserved_from_end”), to początkowo bloki pamięci są ułożne następująco od największego adresu wirtualnego:
 * • przestrzeń ‘niezmapowana’
 * • pamięć zarezerwowana (wewnątrz program ‘bootloadera”)
 * • ewentualny blok nie przydzielonej pamięci “mem-blk”
 * • ‘kernel’; wyrównany adres
 * • tablica stron pamięci wirtualnej; wyrównany adres i rozmiar (wewnątrz program ‘bootloadera”)
 * • tablica “memory_map”
 * • ewentualny blok nie przydzielonej pamięci “mem-blk”
 * • stos; wyrównany adres i rozmiar
 * • początkowe dane “mem-blk”: “allocated”, “free”
 * • ewentualna pozostała przestrzeń przydzialania pamięci przez “mem-blk”
 * • (program ‘bootloadera”)
 * • ewentualna pozostała przestrzeń przydzialania pamięci przez “mem-blk”
 * • nieprzenaszalna pamięć zarezerwowana (wewnątrz program ‘bootloadera”)
 * W przeciwnym przypadku (“!reserved_from_end”):
 * • przestrzeń ‘niezmapowana’
 * • stos; wyrównany adres i rozmiar
 * • ewentualna pozostała przestrzeń przydzialania pamięci przez “mem-blk”
 * • (program ‘bootloadera”)
 * • ewentualna pozostała przestrzeń przydzialania pamięci przez “mem-blk”
 * • początkowe dane “mem-blk”: “free”, “allocated”
 * • tablica “memory_map”
 * • tablica stron pamięci wirtualnej; wyrównany adres i rozmiar (wewnątrz program ‘bootloadera”)
 * • ewentualny blok nie przydzielonej pamięci “mem-blk”
 * • ‘kernel’; wyrównany adres
 * • pamięć zarezerwowana (wewnątrz program ‘bootloadera”)
 */
void
E_mem_M(
  B reserved_from_end
, N reserved_size_from_start
, N loader_start
, N loader_size
, N stack_address
, N stack_size
, N memory_map_address
, N memory_map_size
, N page_table_address
, N page_table_size
, N kernel_address
, N kernel_size
, N memory_size
, N reserved_size
){  E_main_S_kernel_args.mem_blk.memory_size = memory_size;
    E_main_S_kernel_args.mem_blk.reserved_size = reserved_size;
    E_main_S_kernel_args.mem_blk.reserved_from_end = reserved_from_end;
    E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n = 0;
    if( reserved_from_end )
    {   E_main_S_kernel_args.mem_blk.allocated = (P)( stack_address - ( E_mem_Q_blk_S_allocated_n_init * sizeof( struct E_mem_Q_blk_Z_allocated ) + E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free )));
        E_main_S_kernel_args.mem_blk.allocated_id = 0;
        E_main_S_kernel_args.mem_blk.free_id = 1;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p = (P)E_main_S_kernel_args.mem_blk.allocated;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u = sizeof( struct E_mem_Q_blk_Z_allocated );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n = E_mem_Q_blk_S_allocated_n_init;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p = (P)( stack_address - E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ));
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u = sizeof( struct E_mem_Q_blk_Z_free );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n = E_mem_Q_blk_S_free_n_init;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 1 ].p = (P)stack_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 1 ].u = H_oux_E_mem_S_page_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 1 ].n = stack_size / H_oux_E_mem_S_page_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 2 ].p = (P)memory_map_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 2 ].u = sizeof( struct H_oux_E_mem_Z_memory_map );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 2 ].n = memory_map_size / sizeof( struct H_oux_E_mem_Z_memory_map );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 3 ].p = (P)page_table_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 3 ].u = 1;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 3 ].n = page_table_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 4 ].p = (P)kernel_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 4 ].u = 1;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id + 4 ].n = kernel_size;
    }else
    {   E_main_S_kernel_args.mem_blk.allocated = (P)( memory_map_address + memory_map_size + E_mem_Q_blk_S_free_n_init * sizeof( struct E_mem_Q_blk_Z_free ));
        E_main_S_kernel_args.mem_blk.free_id = 3;
        E_main_S_kernel_args.mem_blk.allocated_id = 4;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 3 ].p = (P)kernel_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 3 ].u = 1;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 3 ].n = kernel_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 2 ].p = (P)page_table_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 2 ].u = 1;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 2 ].n = page_table_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 1 ].p = (P)memory_map_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 1 ].u = sizeof( struct H_oux_E_mem_Z_memory_map );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id - 1 ].n = memory_map_size / sizeof( struct H_oux_E_mem_Z_memory_map );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u = sizeof( struct E_mem_Q_blk_Z_free );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n = E_mem_Q_blk_S_free_n_init;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p = (P)( memory_map_address + memory_map_size );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p = (P)E_main_S_kernel_args.mem_blk.allocated;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u = sizeof( struct E_mem_Q_blk_Z_allocated );
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n = E_mem_Q_blk_S_allocated_n_init;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id + 1 ].p = (P)stack_address;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id + 1 ].u = H_oux_E_mem_S_page_size;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id + 1 ].n = stack_size / H_oux_E_mem_S_page_size;
    }
    struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
    _0( free_p + 1, ( E_mem_Q_blk_S_free_n_init - 1 ) * sizeof( *free_p ));
    if( reserved_from_end )
    {   if( loader_start >= H_oux_E_mem_S_page_size + reserved_size_from_start
        && loader_start + loader_size <= stack_address
        )
        {   if( free_p[0].l = loader_start - ( H_oux_E_mem_S_page_size + reserved_size_from_start ))
                free_p[0].p = (P)( H_oux_E_mem_S_page_size + reserved_size_from_start );
            if( free_p[1].l = stack_address - ( loader_start + loader_size ))
                free_p[1].p = (P)( loader_start + loader_size );
        }else
            if( free_p[0].l = stack_address - ( H_oux_E_mem_S_page_size + reserved_size_from_start ))
                free_p[0].p = (P)( H_oux_E_mem_S_page_size + reserved_size_from_start );
        free_p[2].l = memory_map_address - ( stack_address + stack_size );
        free_p[2].p = free_p[2].l ? (Pc)stack_address + stack_size : 0;
        free_p[3].l = H_oux_E_mem_S_page_size - kernel_size % H_oux_E_mem_S_page_size;
        free_p[3].p = free_p[3].l ? (Pc)kernel_address + kernel_size : 0;
        if( !free_p[2].p )
            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(2);
        if( !free_p[1].p )
            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(1);
        if( !free_p[0].p )
            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(0);
    }else
    {   free_p[0].l = H_oux_E_mem_S_page_size - kernel_size % H_oux_E_mem_S_page_size;
        free_p[0].p = free_p[3].l ? (Pc)kernel_address + kernel_size : 0;
        if( loader_start >= (N)( E_main_S_kernel_args.mem_blk.allocated + E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n )
        && loader_start + loader_size <= stack_address
        )
        {   if( free_p[1].l = loader_start - (N)( E_main_S_kernel_args.mem_blk.allocated + E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n ))
                free_p[1].p = (P)( E_main_S_kernel_args.mem_blk.allocated + E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n );
            if( free_p[2].l = stack_address - ( loader_start + loader_size ))
                free_p[2].p = (P)( loader_start + loader_size );
            if( !free_p[1].p )
                E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(1);
        }else
            if( free_p[1].l = stack_address - (N)( E_main_S_kernel_args.mem_blk.allocated + E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n ))
                free_p[1].p = (P)( E_main_S_kernel_args.mem_blk.allocated + E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n );
        if( !free_p[0].p )
            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(0);
    }
    struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_i = E_mem_Q_blk_Q_sys_table_M_new_id( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p, (Pc)&allocated_p.n - (Pc)&allocated_p, 0, 0 );
    if( ~allocated_i )
    {   E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p = (P)loader_start;
        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u = 1;
        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n = loader_size;
    }
    E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( allocated_i, ~0 );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
B
E_mem_Q_blk_T_eq( P p_1
, P p_2
, N l
){  for_n( i, l )
        if( *( (Pc)p_1 + i ) != *( (Pc)p_2 + i ))
            return no;
    return yes;
}
_internal
void
E_mem_Q_blk_I_copy_fwd( P dst
, P src
, N l
){
        #ifdef __SSE__
    N128 *dst_x = (P)E_simple_Z_p_I_align_up_to_v2( dst, sizeof(N128) );
    N128 *src_x = (P)E_simple_Z_p_I_align_up_to_v2( src, sizeof(N128) );
    N l_0 = (Pc)src_x - (Pc)src;
    if( l > l_0
    && l - l_0 >= sizeof(N128)
    && dst_x != src_x
    && (Pc)dst_x - (Pc)dst == (Pc)src_x - (Pc)src
    )
    {   N l_1 = ( l - l_0 ) / sizeof(N128);
        N l_2 = ( l - l_0 ) % sizeof(N128);
        __asm__ volatile (
        "\n"    "rep movsb"
        : "+D" (dst), "+S" (src), "+c" ( l_0 )
        :
        : "memory"
        );
        for_n( i, l_1 )
            __asm__ volatile (
            "\n"    "movaps %1,%%xmm0"
            "\n"    "movaps %%xmm0,%0"
            :
            : "p" ( dst_x++ ), "p" ( src_x++ )
            : "xmm0", "memory"
            );
        dst = dst_x;
        src = src_x;
        l = l_2;
    }
        #endif
    __asm__ volatile (
    "\n"    "rep movsb"
    : "+D" (dst), "+S" (src), "+c" (l)
    :
    : "memory"
    );
}
_internal
void
E_mem_Q_blk_I_copy_rev( P dst
, P src
, N l
){
        #ifdef __SSE__
    N128 *dst_x = (P)E_simple_Z_p_I_align_down_to_v2( dst + l, sizeof(N128) );
    N128 *src_x = (P)E_simple_Z_p_I_align_down_to_v2( src + l, sizeof(N128) );
    N l_0 = (Pc)src + l - (Pc)src_x;
    if( l > l_0
    && l - l_0 >= sizeof(N128)
    && dst_x != src_x
    && (Pc)dst + l - (Pc)dst_x == (Pc)src + l - (Pc)src_x
    )
    {   dst = (Pc)dst + l - 1;
        src = (Pc)src + l - 1;
        N l_1 = ( l - l_0 ) / sizeof(N128);
        N l_2 = ( l - l_0 ) % sizeof(N128);
        __asm__ volatile (
        "\n"    "std"
        "\n"    "rep movsb"
        : "+D" (dst), "+S" (src), "+c" ( l_0 )
        :
        : "memory"
        );
        for_n( i, l_1 )
            __asm__ volatile (
            "\n"    "movaps %1,%%xmm0"
            "\n"    "movaps %%xmm0,%0"
            :
            : "p" ( --dst_x ), "p" ( --src_x )
            : "xmm0", "memory"
            );
        dst = (P)( (Pc)dst_x - 1 );
        src = (P)( (Pc)src_x - 1 );
        l = l_2;
    }else
        #endif
    {   dst = (Pc)dst + l - 1;
        src = (Pc)src + l - 1;
    }
    __asm__ volatile (
    "\n"    "std"
    "\n"    "rep movsb"
    "\n"    "cld"
    : "+D" (dst), "+S" (src), "+c" (l)
    :
    : "cc", "memory"
    );
}
void
E_mem_Q_blk_I_copy( P dst
, P src
, N l
){  if( !l )
        return;
    if( (Pc)dst < (Pc)src
    || (Pc)dst >= (Pc)src + l
    )
        E_mem_Q_blk_I_copy_fwd( dst, src, l );
    else
        E_mem_Q_blk_I_copy_rev( dst, src, l );
}
P
memmove( P dst
, const P src
, size_t l
){  E_mem_Q_blk_I_copy( dst, src, l );
    return dst;
}
__attribute__ (( __alias__( "memmove" ) ))
P memcpy( P, const P, size_t );
void
E_mem_Q_blk_P_fill_c( P p
, N l
, C c
){
        #ifdef __SSE__
    N128 *p_x = (P)E_simple_Z_p_I_align_up_to_v2( p, sizeof(N128) );
    N l_0 = (Pc)p_x - (Pc)p;
    if( l > l_0
    && l - l_0 >= sizeof(N128)
    )
    {   N l_1 = ( l - l_0 ) / sizeof(N128);
        N l_2 = ( l - l_0 ) % sizeof(N128);
        __asm__ volatile (
        "\n"    "rep stosb"
        : "+D" (p), "+c" ( l_0 )
        : "a" (c)
        : "memory"
        );
        N128 __attribute__ (( __aligned__(16) )) x;
        p = &x;
        N cx = sizeof(N128);
        __asm__ volatile (
        "\n"    "rep stosb"
        "\n"    "movaps %2,%%xmm0"
        : "+D" (p), "+c" (cx)
        : "m" (x), "a" (c)
        : "xmm0", "memory"
        );
        for_n( i, l_1 )
            __asm__ volatile (
            "\n"    "movaps %%xmm0,%0"
            :
            : "p" ( p_x++ )
            : "memory"
            );
        p = p_x;
        l = l_2;
    }
        #endif
    __asm__ volatile (
    "\n"    "rep stosb"
    : "+D" (p), "+c" (l)
    : "a" (c)
    : "memory"
    );
}
P
memset( P p
, int c
, size_t l
){  E_mem_Q_blk_P_fill_c( p, c, l );
    return p;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
_internal
void
E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( N free_i
){  struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
    N free_end = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p->p - (Pc)free_p ) + 1;
    if( free_end - ( free_i + 1 ))
        E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p + free_i * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u
        , E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p + ( free_i + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u
        , ( free_end - ( free_i + 1 )) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u
        );
    free_p[ free_end - 1 ].p = 0;
    free_p[ free_end - 1 ].l = 0;
}
_internal
void
E_mem_Q_blk_Q_sys_table_a_I_move_empty_entry( N allocated_i
){  struct E_mem_Q_blk_Z_allocated *allocated_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p;
    N allocated_end = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p->p - (Pc)allocated_p ) + 1;
    if( allocated_end - ( allocated_i + 1 ))
    {   E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p + allocated_i * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
        , E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p + ( allocated_i + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
        , ( allocated_end - ( allocated_i + 1 )) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
        );
        if( allocated_i <= E_main_S_kernel_args.mem_blk.free_id )
            E_main_S_kernel_args.mem_blk.free_id--;
        if( allocated_i <= E_main_S_kernel_args.mem_blk.allocated_id )
            E_main_S_kernel_args.mem_blk.allocated_id--;
    }
    allocated_p[ allocated_end - 1 ].p = 0;
}
_internal
N
E_mem_Q_blk_Q_sys_table_R_last( N table_i
, N rel_addr_p
){  for_n_rev( i, E_main_S_kernel_args.mem_blk.allocated[ table_i ].n )
    {   Pc *p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
        if( *p_ )
            break;
    }
    return i;
}
_internal
N
E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( N table_i
, N rel_addr_p
, N rel_addr_l
, N inserted_i
){  N n = E_mem_Q_blk_Q_sys_table_R_last( table_i, rel_addr_p );
    if( !n ) // Tablica zawiera tylko jeden niepusty element czyli element wstawiony.
        return n;
    Pc tmp_p = *( Pc * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + inserted_i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
    N min = 0;
    N max = n;
    N middle = max / 2;
    O{  if( middle == inserted_i )
            if( middle > min )
                middle--;
            else if( middle < max )
                middle++;
            else
                break;
        Pc *p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
        if( *p_ > tmp_p )
        {   if( middle == min )
                break;
            max = middle - 1;
            middle = max - ( middle - min ) / 2;
            if( middle == max
            && middle == min
            )
            {   p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
                if( *p_ < tmp_p )
                    middle++;
                break;
            }
        }else // if( *p_ < tmp_p )
        {   if( middle == max )
            {   middle++;
                break;
            }
            min = middle + 1;
            middle = min + ( max - middle ) / 2;
            if( middle == min
            && middle == max
            )
            {   p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
                if( *p_ < tmp_p )
                    middle++;
                break;
            }
        }
    }
    N tmp_l = *( N * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + inserted_i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l );
    if( middle != inserted_i )
        if( middle < inserted_i )
            E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + ( middle + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
            , ( inserted_i - middle ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
            );
        else // if( middle > inserted_i )
        {   middle--;
            if( middle - inserted_i )
                E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + inserted_i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                , E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + ( inserted_i + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                , ( middle - inserted_i ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                );
        }
    *( Pc * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = tmp_p;
    *( N * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + middle * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = tmp_l;
    return middle;
}
_internal
N
E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( N inserted_i
, N middle
){  Pc tmp_p = E_main_S_kernel_args.mem_blk.allocated[ inserted_i ].p;
    if( !~middle )
    {   struct E_mem_Q_blk_Z_allocated allocated_p;
        N n = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
        if( !n ) // Tablica zawiera tylko jeden niepusty element czyli element wstawiony.
            return n;
        N min = 0;
        N max = n;
        middle = max / 2;
        O{  if( middle == inserted_i )
                if( middle > min )
                    middle--;
                else
                    middle++;
            Pc p_ = E_main_S_kernel_args.mem_blk.allocated[middle].p;
            if( p_ > tmp_p )
            {   if( middle == min )
                    break;
                max = middle - 1;
                middle = max - ( middle - min ) / 2;
                if( middle == max
                && middle == min
                )
                {   p_ = E_main_S_kernel_args.mem_blk.allocated[middle].p;
                    if( p_ < tmp_p )
                        middle++;
                    break;
                }
            }else // if( *p_ < tmp_p )
            {   if( middle == max )
                {   middle++;
                    break;
                }
                min = middle + 1;
                middle = min + ( max - middle ) / 2;
                if( middle == min
                && middle == max
                )
                {   p_ = E_main_S_kernel_args.mem_blk.allocated[middle].p;
                    if( p_ < tmp_p )
                        middle++;
                    break;
                }
            }
        }
    }
    N tmp_n = E_main_S_kernel_args.mem_blk.allocated[ inserted_i ].n;
    N tmp_u = E_main_S_kernel_args.mem_blk.allocated[ inserted_i ].u;
    if( middle != inserted_i )
    {   if( middle < inserted_i )
            E_mem_Q_blk_I_copy( &E_main_S_kernel_args.mem_blk.allocated[ middle + 1 ]
            , &E_main_S_kernel_args.mem_blk.allocated[middle]
            , ( inserted_i - middle ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
            );
        else // if( middle > inserted_i )
        {   middle--;
            if( middle - inserted_i )
                E_mem_Q_blk_I_copy( &E_main_S_kernel_args.mem_blk.allocated[ inserted_i ]
                , &E_main_S_kernel_args.mem_blk.allocated[ inserted_i + 1 ]
                , ( middle - inserted_i ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
                );
        }
        if( inserted_i == E_main_S_kernel_args.mem_blk.free_id )
            E_main_S_kernel_args.mem_blk.free_id = middle;
        else
        {   if( inserted_i < E_main_S_kernel_args.mem_blk.free_id )
                E_main_S_kernel_args.mem_blk.free_id--;
            if( middle <= E_main_S_kernel_args.mem_blk.free_id )
                E_main_S_kernel_args.mem_blk.free_id++;
        }
        if( inserted_i == E_main_S_kernel_args.mem_blk.allocated_id )
            E_main_S_kernel_args.mem_blk.allocated_id = middle;
        else
        {   if( inserted_i < E_main_S_kernel_args.mem_blk.allocated_id )
                E_main_S_kernel_args.mem_blk.allocated_id--;
            if( middle <= E_main_S_kernel_args.mem_blk.allocated_id )
                E_main_S_kernel_args.mem_blk.allocated_id++;
        }
        E_main_S_kernel_args.mem_blk.allocated[middle].p = tmp_p;
        E_main_S_kernel_args.mem_blk.allocated[middle].n = tmp_n;
        E_main_S_kernel_args.mem_blk.allocated[middle].u = tmp_u;
    }
    return middle;
}
_internal
B
E_mem_Q_blk_Q_sys_table_f_I_unite( N table_i
, N rel_addr_p
, N rel_addr_l
, P p
, N l
){  N i_found = ~0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( table_i, rel_addr_p );
    if( ~max )
    {   // Szukanie wolnego bloku przyległego od dołu.
        N min = 0;
        N max_0 = max;
        N i = max / 2;
        O{  Pc *p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
            N *l_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l );
            if( *p_ + *l_ == p )
            {   p = *p_;
                l = *l_ += l;
                i_found = i;
                break;
            }
            if( *p_ + *l_ > (Pc)p )
            {   if( i == min )
                    break;
                max = i - 1;
                i = max - ( i - min ) / 2;
            }else
            {   if( i == max )
                    break;
                min = i + 1;
                i = min + ( max - i ) / 2;
            }
        }
        // Szukanie wolnego bloku przyległego od góry.
        min = 0;
        max = max_0;
        i = max / 2;
        O{  Pc *p_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p );
            N *l_ = (P)( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l );
            if( (Pc)p + l == *p_ )
            {   if( ~i_found ) // Był znaleziony blok przyległy od dołu.
                {   *( Pc * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i_found * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) += *l_;
                    E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(i);
                }else
                {   *p_ = p;
                    *l_ += l;
                    i_found = i;
                }
                break;
            }
            if( *p_ > (Pc)p + l )
            {   if( i == min )
                    break;
                max = i - 1;
                i = max - ( i - min ) / 2;
            }else
            {   if( i == max )
                    break;
                min = i + 1;
                i = min + ( max - i ) / 2;
            }
        }
    }
    return !!~i_found;
}
_internal
N
E_mem_Q_blk_Q_sys_table_f_P_put( N table_i
, N rel_addr_p
, N rel_addr_l
, P p
, N l
){  if( !E_mem_Q_blk_Q_sys_table_f_I_unite( table_i, rel_addr_p, rel_addr_l, p, l ))
    {   if( table_i == E_main_S_kernel_args.mem_blk.free_id
        && l >= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
        )
        {   if( (Pc)p + l == E_main_S_kernel_args.mem_blk.allocated[ table_i ].p ) // Nowy blok jest przyległy od dołu do tablicy bloków.
            {   E_main_S_kernel_args.mem_blk.allocated[ table_i ].p -= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ table_i ].p;
                if( l - E_main_S_kernel_args.mem_blk.allocated[ table_i ].u )
                {   free_p[0].l = l - E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                    free_p[0].p = p;
                    E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, 0 );
                }else
                    E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry(0);
                return 0;
            }
            if( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + E_main_S_kernel_args.mem_blk.allocated[ table_i ].n * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u == p ) // Nowy blok jest przyległy od góry do tablicy bloków.
            {   N i = E_mem_Q_blk_Q_sys_table_R_last( table_i, rel_addr_p ) + 1;
                struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ table_i ].p;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ].p = 0;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ].l = 0;
                free_p[i].l = l - E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                free_p[i].p = free_p[i].l
                  ? (Pc)p + E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                  : 0;
                E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                if( free_p[i].l )
                    E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, i );
                return 0;
            }
        }
        N i = E_mem_Q_blk_Q_sys_table_M_new_id( table_i, rel_addr_p, rel_addr_l, p, l );
        if( !~i )
            //TODO Dopisać ewentualne przeniesienie tablicy wolnych bloków do tego nowego bloku.
            return ~0;
    }
    return 0;
}
_internal
N
E_mem_Q_blk_Q_sys_table_M_new_id( N table_i
, N rel_addr_p
, N rel_addr_l
, P p // Adres do nowego wpisu.
, N l // I rozmiar.
){  Pc p_0 = E_main_S_kernel_args.mem_blk.allocated[ table_i ].p;
    N l_0 = E_main_S_kernel_args.mem_blk.allocated[ table_i ].n * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
    N i = E_mem_Q_blk_Q_sys_table_R_last( table_i, rel_addr_p ) + 1;
    if( table_i == E_main_S_kernel_args.mem_blk.free_id )
    {   if( i != E_main_S_kernel_args.mem_blk.allocated[ table_i ].n )
        {   if(p)
            {   *( P * )( p_0 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
                *( N * )( p_0 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
                i = E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, i );
            }
            return i;
        }
        struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
        N l_ = E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
        struct E_mem_Q_blk_Z_free free_p_;
        N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
        if( ~max )
        {   // Szukanie wolnego bloku przyległego od dołu.
            N min = 0;
            N max_0 = max;
            N free_i = max / 2;
            O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                {   if( free_p[ free_i ].l >= l_ )
                    {   free_p[ free_i ].l -= l_;
                        if( !free_p[ free_i ].l )
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].p -= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                        E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p
                        , E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                        , E_main_S_kernel_args.mem_blk.allocated[ table_i ].n * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                        );
                        N i = E_main_S_kernel_args.mem_blk.allocated[ table_i ].n;
                        *( P * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
                        *( N * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                        if(p)
                            i = E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, i );
                        return i;
                    }
                    break;
                }
                if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                {   if( free_i == min )
                        break;
                    max = free_i - 1;
                    free_i = max - ( free_i - min ) / 2;
                }else
                {   if( free_i == max )
                        break;
                    min = free_i + 1;
                    free_i = min + ( max - free_i ) / 2;
                }
            }
            // Szukanie wolnego bloku przyległego od góry.
            min = 0;
            max = max_0;
            free_i = max / 2;
            O{  if( p_0 + l_0 == free_p[ free_i ].p )
                {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u )
                    {   if( free_p[ free_i ].l -= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u )
                            free_p[ free_i ].p += E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                        else
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        *( P * )( p_0 + l_0 + rel_addr_p ) = p;
                        *( N * )( p_0 + l_0 + rel_addr_l ) = l;
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                        return p ? E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, E_main_S_kernel_args.mem_blk.allocated[ table_i ].n - 1 ) : E_main_S_kernel_args.mem_blk.allocated[ table_i ].n - 1;
                    }
                    break;
                }
                if( free_p[ free_i ].p > p_0 + l_0 )
                {   if( free_i == min )
                        break;
                    max = free_i - 1;
                    free_i = max - ( free_i - min ) / 2;
                }else
                {   if( free_i == max )
                        break;
                    min = free_i + 1;
                    free_i = min + ( max - free_i ) / 2;
                }
            }
        }
    }else
    {   if( i != E_main_S_kernel_args.mem_blk.allocated[ table_i ].n )
        {   if(p)
            {   *( P * )( p_0 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
                *( N * )( p_0 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
                i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( i, ~0 );
            }
            return i;
        }
        struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
        N l_ = E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
        struct E_mem_Q_blk_Z_free free_p_;
        N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
        if( ~max )
        {   // Szukanie wolnego bloku przyległego od dołu.
            N min = 0;
            N max_0 = max;
            N free_i = max / 2;
            O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                {   if( free_p[ free_i ].l >= l_ )
                    {   free_p[ free_i ].l -= l_;
                        if( !free_p[ free_i ].l )
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].p -= l_;
                        E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p
                        , E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + l_
                        , E_main_S_kernel_args.mem_blk.allocated[ table_i ].n * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
                        );
                        E_main_S_kernel_args.mem_blk.allocated = (P)E_main_S_kernel_args.mem_blk.allocated[ table_i - 1 ].p;
                        N i = E_main_S_kernel_args.mem_blk.allocated[ table_i ].n;
                        *( P * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
                        *( N * )( E_main_S_kernel_args.mem_blk.allocated[ table_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                        if(p)
                            i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( i, ~0 );
                        return i;
                    }
                    break;
                }
                if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                {   if( free_i == min )
                        break;
                    max = free_i - 1;
                    free_i = max - ( free_i - min ) / 2;
                }else
                {   if( free_i == max )
                        break;
                    min = free_i + 1;
                    free_i = min + ( max - free_i ) / 2;
                }
            }
            // Szukanie wolnego bloku przyległego od góry.
            min = free_i;
            max = max_0;
            free_i = max / 2;
            O{  if( p_0 + l_0 == free_p[ free_i ].p )
                {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u )
                    {   free_p[ free_i ].l -= E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                        if( free_p[ free_i ].l )
                            free_p[ free_i ].p += E_main_S_kernel_args.mem_blk.allocated[ table_i ].u;
                        else
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        *( P * )( p_0 + l_0 + rel_addr_p ) = p;
                        *( N * )( p_0 + l_0 + rel_addr_l ) = l;
                        E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                        return p ? E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( E_main_S_kernel_args.mem_blk.allocated[ table_i ].n - 1, ~0 ) : E_main_S_kernel_args.mem_blk.allocated[ table_i ].n - 1;
                    }
                    break;
                }
                if( free_p[ free_i ].p > p_0 + l_0 )
                {   if( free_i == min )
                        break;
                    max = free_i - 1;
                    free_i = max - ( free_i - min ) / 2;
                }else
                {   if( free_i == max )
                        break;
                    min = free_i + 1;
                    free_i = min + ( max - free_i ) / 2;
                }
            }
        }
    }
    Pc p_1 = E_mem_Q_blk_Q_table_M_from_free( &table_i
    , E_main_S_kernel_args.mem_blk.allocated[ table_i ].u
    , E_main_S_kernel_args.mem_blk.allocated[ table_i ].n + 1
    , E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ? p_0 : 0
    , l_0
    , 0
    , ~0
    );
    if( !p_1 )
        return ~0;
    i = E_main_S_kernel_args.mem_blk.allocated[ table_i ].n - 1;
    if( table_i == E_main_S_kernel_args.mem_blk.free_id )
        if(p)
        {   E_main_S_kernel_args.mem_blk.allocated[ table_i ].n--;
            N i = E_mem_Q_blk_Q_sys_table_R_last( table_i, rel_addr_p ) + 1;
            if( !E_mem_Q_blk_Q_sys_table_f_I_unite( table_i, rel_addr_p, rel_addr_l, p, l ))
            {   if( i != E_main_S_kernel_args.mem_blk.allocated[ table_i ].n )
                {   *( P * )( p_1 + ( E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = 0;
                    *( N * )( p_1 + ( E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = 0;
                }
                *( P * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
                *( N * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
                E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
                i = E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( table_i, rel_addr_p, rel_addr_l, i );
            }else
            {   *( P * )( p_1 + ( E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = 0;
                *( N * )( p_1 + ( E_main_S_kernel_args.mem_blk.allocated[ table_i ].n ) * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = 0;
                E_main_S_kernel_args.mem_blk.allocated[ table_i ].n++;
            }
        }else
        {   *( P * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = 0;
            *( N * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = 0;
        }
    else
    {   *( P * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_p ) = p;
        *( N * )( p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ table_i ].u + rel_addr_l ) = l;
        if(p)
            i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( i, ~0 );
    }
    return i;
}
_internal
void
E_mem_Q_blk_Q_table_I_put_begin( N *allocated_or_table_i
){  E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id[ E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n++ ] = allocated_or_table_i;
}
_internal
void
E_mem_Q_blk_Q_table_I_put_before( N table_i
){  E_main_S_kernel_args.mem_blk.M_from_free_S_table_id[ E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n - 1 ] = table_i;
}
_internal
void
E_mem_Q_blk_Q_table_I_put_after( N table_i
){  for_n( i, E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n )
    {   N *allocated_or_table_i = E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id[i];
        if( E_main_S_kernel_args.mem_blk.M_from_free_S_table_id[ E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n - 1 ] < *allocated_or_table_i
        && table_i >= *allocated_or_table_i
        )
            (*allocated_or_table_i)--;
        else if( E_main_S_kernel_args.mem_blk.M_from_free_S_table_id[ E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n - 1 ] > *allocated_or_table_i
        && table_i <= *allocated_or_table_i
        )
            (*allocated_or_table_i)++;
    }
}
_internal
void
E_mem_Q_blk_Q_table_I_put_end( void
){  E_main_S_kernel_args.mem_blk.M_from_free_S_allocated_id_n--;
}
_internal
N
E_mem_Q_blk_I_default_align( N u
){  N align;
    // Spekulacje.
    if( u > E_mem_Q_blk_S_align_to_all
    && u % E_mem_Q_blk_S_align_to_all == 0
    )
        align = E_mem_Q_blk_S_align_to_all;
    else if( u == E_mem_Q_blk_S_align_to_all )
        align = u;
    else if( u
    && u < E_mem_Q_blk_S_align_to_all
    && E_simple_Z_n_T_power_2(u)
    )
        align = u;
    else
        align = 1;
    return align;
}
// Dla tablicy systemowej “free” ‘alokuje’ tyle “n”, ile żądane, lub więcej.
_internal
P
E_mem_Q_blk_Q_table_M_from_free( N *allocated_or_table_i
, N u
, N n
, P p // Adres uprzedniej zawartości lub 0, gdy brak.
, N l // I rozmiar. Jeśli “p == 0”, to parametr ignorowany.
, N l_rel
, N align
){  N l_1 = n * u;
    Pc p_1;
    S i = 0;
    if(n)
    {   if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id
        && p // Uprzedni obszar tablicy staje się wolnym blokiem.
        )
        {   n++;
            l_1 += u;
        }
        N l_align;
        if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.allocated_id
        || *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id
        )
            l_align = sizeof(N);
        else if( ~align )
            l_align = align;
        else
            l_align = E_mem_Q_blk_I_default_align(u);
        if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id ) // Obszar przed wyrównanym adresem staje się wolnym blokiem.
        {   n++;
            l_1 += u;
        }
        N l_ = ~0;
        N i_found;
        struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
        for_n( free_i, E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n ) // Szukanie wolnego bloku na całą tablicę.
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
            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( i_found );
        if(p)
        {   E_mem_Q_blk_I_copy( p_1 + l_rel, p, l );
            if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.allocated_id )
                E_main_S_kernel_args.mem_blk.allocated = (P)p_1;
        }
        E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].p = p_1;
        *allocated_or_table_i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( *allocated_or_table_i, ~0 );
        E_mem_Q_blk_Q_table_I_put_begin( allocated_or_table_i );
        if( p_1 - old_free_p )
            if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id )
            {   free_p = (P)p_1;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n ].p = old_free_p;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n ].l = p_1 - old_free_p;
                E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n++;
                struct E_mem_Q_blk_Z_free free_p_;
                E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( *allocated_or_table_i, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n - 1 );
                i++;
            }else
            {   E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, old_free_p, p_1 - old_free_p ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
            }
    }else
    {   N allocated_i_sorted_pos;
        if( !( p_1 = E_mem_Q_blk_M_new_0( &allocated_i_sorted_pos )))
            return 0;
        E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].p = p_1;
        *allocated_or_table_i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( *allocated_or_table_i, allocated_i_sorted_pos );
        E_mem_Q_blk_Q_table_I_put_begin( allocated_or_table_i );
    }
    if( !p ) //NDFN Rozpoznanie niebezpośrednie, mimo że jednoznaczne w obecnej implementacji.
        E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].u = u;
    if(p)
        if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id )
        {   struct E_mem_Q_blk_Z_free *free_p = (P)p_1;
            struct E_mem_Q_blk_Z_free free_p_;
            if( !E_mem_Q_blk_Q_sys_table_f_I_unite( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, l ))
            {   free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n ].p = p;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n ].l = l;
                E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n++;
                E_mem_Q_blk_Q_sys_table_f_I_sort_inserted( *allocated_or_table_i, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n - 1 );
                i++;
            }
            N start_i = i;
            for( ; i != 2; i++ )
            {   free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n + i - start_i ].p = 0;
                free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n + i - start_i ].l = 0;
            }
        }else
        {   E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
            struct E_mem_Q_blk_Z_free free_p_;
            if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, l ))
            {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_mem_Q_blk_Q_table_I_put_end();
                return 0;
            }
            E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
        }
    else if( *allocated_or_table_i == E_main_S_kernel_args.mem_blk.free_id )
    {   struct E_mem_Q_blk_Z_free *free_p = (P)p_1;
        N start_i = i;
        for( ; i != 2; i++ )
        {   free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n + i - start_i ].p = 0;
            free_p[ E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n + i - start_i ].l = 0;
        }
    }
    E_main_S_kernel_args.mem_blk.allocated[ *allocated_or_table_i ].n = n;
    E_mem_Q_blk_Q_table_I_put_end();
    return p_1;
}
// Adresy puste znajdują się powyżej ostatniego adresu podstawowego bloku pamięci wirtualnej z wyjątkiem ewentualnie umieszczonej na końcu pamięci zarezerwowanej. Ewentualne dodatkowo ‘zmapowane’ powyżej tego bloku podstawowego bloki pamięci wirtualnej (np. stosy) mogą mieć ten sam adres jak adres pusty, ale z tymi adresami nie będzie przydzielanej pamięci.
_internal
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_new_0( N *allocated_i_sorted_pos
){  Pc p_start = E_main_S_kernel_args.mem_blk.reserved_from_end ? (P)( E_main_S_kernel_args.mem_blk.memory_size - E_main_S_kernel_args.mem_blk.reserved_size ) : (P)E_main_S_kernel_args.mem_blk.memory_size;
    Pc p = (P)( ~0 - 1 );
    struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = allocated_max;
    do
    {   if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p != p )
            break;
        if( --p == p_start - 1 )
            return 0;
    }while( allocated_i-- );
    *allocated_i_sorted_pos = ~allocated_i ? allocated_i : 0;
    return p;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_mem_Q_sys_table_I_reduce( void
){  struct E_mem_Q_blk_Z_allocated allocated_p_;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p_.p - (Pc)&allocated_p_ );
    if( max != E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n - 1 )
    {   N n = E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].n = max + 1;
        struct E_mem_Q_blk_Z_free free_p_;
        if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id
        , (Pc)&free_p_.p - (Pc)&free_p_
        , (Pc)&free_p_.l - (Pc)&free_p_
        , E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].p + ( max + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u
        , ( n - 1 - max ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.allocated_id ].u ))
            return ~0;
    }
    struct E_mem_Q_blk_Z_free free_p_;
    max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
    if( !~max )
        max++;
    if( max != E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n - 1 )
    {   N n = E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n;
        E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].n = max + 1;
        struct E_mem_Q_blk_Z_free free_p_;
        if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id
        , (Pc)&free_p_.p - (Pc)&free_p_
        , (Pc)&free_p_.l - (Pc)&free_p_
        , E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p + ( max + 1 ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u
        , ( n - 1 - max ) * E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].u ))
            return ~0;
    }
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
P
E_mem_Q_blk_M( N l
){  return E_mem_Q_blk_M_tab( 1, l );
}
P
E_mem_Q_blk_M_tab(
  N u
, N n
){  return E_mem_Q_blk_M_align_tab( u, n, ~0 );
}
P
E_mem_Q_blk_M_align( N l
, N align
){  return E_mem_Q_blk_M_align_tab( 1, l, align );
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_align_tab(
  N u
, N n
, N align
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_i = E_mem_Q_blk_Q_sys_table_M_new_id( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p, (Pc)&allocated_p.n - (Pc)&allocated_p, 0, 0 );
    if( !~allocated_i )
        return 0;
    if( !E_mem_Q_blk_Q_table_M_from_free( &allocated_i, u, n, 0, 0, 0, align ))
        return 0;
    return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
}
P
E_mem_Q_blk_M_replace( P p
, N l
){  return E_mem_Q_blk_M_replace_tab( p, 1, l );
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_replace_tab( P p
, N u
, N n
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   E_mem_Q_blk_Q_table_I_put_begin( &allocated_i );
                E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_mem_Q_blk_Q_table_I_put_end();
            }
            if( !E_mem_Q_blk_Q_table_M_from_free( &allocated_i, u, n, 0, 0, 0, ~0 ))
            {   E_mem_Q_blk_Q_sys_table_a_I_move_empty_entry( allocated_i );
                *( P * )p = 0;
                return 0;
            }
            P p_ = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
            *( P * )p = p_;
            return p_;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
__attribute__ ((__malloc__))
P
E_mem_Q_blk_M_split( P p
, N i
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N allocated_i = E_mem_Q_blk_Q_sys_table_M_new_id( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p, (Pc)&allocated_p.n - (Pc)&allocated_p, 0, 0 );
    if( !~allocated_i )
        return 0;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_j = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_j ].p == p )
        {   E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n = E_main_S_kernel_args.mem_blk.allocated[ allocated_j ].n - i;
            E_main_S_kernel_args.mem_blk.allocated[ allocated_j ].n = i;
            E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u = E_main_S_kernel_args.mem_blk.allocated[ allocated_j ].u;
            E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p = (Pc)p + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_j ].u;
            P p_ = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
            E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( allocated_i, ~0 );
            return p_;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
N
E_mem_Q_blk_W( P p
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == p )
        {   N n = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n;
            N u = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            E_mem_Q_blk_Q_sys_table_a_I_move_empty_entry( allocated_i );
            if(n)
            {   struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p, n * u ))
                    return ~0;
            }
            return 0;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > (Pc)p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return ~0;
}
//------------------------------------------------------------------------------
P
E_mem_Q_blk_I_add( P p
, N n
, N *n_prepended
, N *n_appended
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            N l = n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free free_p_;
                N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
                if( ~max )
                {   struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
                    // Szukanie wolnego bloku przyległego od dołu.
                    N min = 0;
                    N max_0 = max;
                    N free_i = max / 2;
                    O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                        {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                            {   l_1 = free_p[ free_i ].l;
                                if( l_1 > l )
                                    l_1 = l;
                                else if( l_1 < l )
                                    l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u );
                                l -= l_1;
                            }
                            break;
                        }
                        if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                        {   if( free_i == min )
                                break;
                            max = free_i - 1;
                            free_i = max - ( free_i - min ) / 2;
                        }else
                        {   if( free_i == max )
                                break;
                            min = free_i + 1;
                            free_i = min + ( max - free_i ) / 2;
                        }
                    }
                    if( !l )
                    {   free_p[ free_i ].l -= l_1;
                        if( !free_p[ free_i ].l )
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                        *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                        if( n_prepended )
                            *n_prepended = n;
                        if( n_appended )
                            *n_appended = 0;
                        return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1;
                    }
                    // Szukanie wolnego bloku przyległego od góry.
                    min = free_i;
                    max = max_0;
                    N free_j = max / 2;
                    O{  if( p_0 + l_0 == free_p[ free_j ].p )
                        {   if( free_p[ free_j ].l < l )
                                break;
                            free_p[ free_j ].l -= l;
                            if( free_p[ free_j ].l )
                                free_p[ free_j ].p += l;
                            else
                                E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                            if( l_1 )
                            {   free_p[ free_i ].l -= l_1;
                                if( !free_p[ free_i ].l )
                                    E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                            }
                            E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                            if( n_prepended )
                                *n_prepended = l_1 / E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                            if( n_appended )
                                *n_appended = l / E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                            return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1 + l_0;
                        }
                        if( free_p[ free_j ].p > p_0 + l_0 )
                        {   if( free_j == min )
                                break;
                            max = free_j - 1;
                            free_j = max - ( free_j - min ) / 2;
                        }else
                        {   if( free_j == max )
                                break;
                            min = free_j + 1;
                            free_j = min + ( max - free_j ) / 2;
                        }
                    }
                    l += l_1; // Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, zabrakło do pełnej liczby od góry.
                }
            }
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( &allocated_i
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            , ~0
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            if( n_prepended )
                *n_prepended = 0;
            if( n_appended )
                *n_appended = n;
            return (Pc)p_1 + l_0;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
_internal
P
E_mem_Q_blk_I_prepend_append( P p
, N n_prepend
, N n_append
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   N l = ( n_prepend + n_append ) * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                N l_1 = 0;
                p_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free free_p_;
                N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
                if( ~max )
                {   struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
                    // Szukanie wolnego bloku przyległego od dołu.
                    N min = 0;
                    N max_0 = max;
                    N free_i = max / 2;
                    O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                        {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                            {   l_1 = free_p[ free_i ].l;
                                if( l_1 > l )
                                    l_1 = l;
                                else if( l_1 < l )
                                    l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u );
                                l -= l_1;
                            }
                            break;
                        }
                        if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                        {   if( free_i == min )
                                break;
                            max = free_i - 1;
                            free_i = max - ( free_i - min ) / 2;
                        }else
                        {   if( free_i == max )
                                break;
                            min = free_i + 1;
                            free_i = min + ( max - free_i ) / 2;
                        }
                    }
                    if( l_1 )
                    {   // Szukanie wolnego bloku przyległego od góry.
                        min = free_i;
                        max = max_0;
                        N free_j = max / 2;
                        O{  if( p_0 + l_0 == free_p[ free_j ].p )
                            {   if( free_p[ free_j ].l >= l )
                                {   E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n_prepend + n_append;
                                    if( l_1
                                    && free_p[ free_i ].l >= n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                                    && free_p[ free_j ].l >= n_append * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                                    )
                                    {   if( free_p[ free_j ].l )
                                            free_p[ free_j ].p += n_append * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                                        else
                                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                        free_p[ free_i ].l -= n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                                        if( !free_p[ free_i ].l )
                                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                        free_p[ free_j ].l -= n_append * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                                        *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                                        return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                                    }
                                    free_p[ free_j ].l -= l;
                                    if( free_p[ free_j ].l )
                                        free_p[ free_j ].p += l;
                                    else
                                        E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                    free_p[ free_i ].l -= l_1;
                                    if( !free_p[ free_i ].l )
                                        E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                    E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p - ( l_1 - n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                                    , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p
                                    , l_0
                                    );
                                    *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                                    return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1;
                                }
                                break;
                            }
                            if( free_p[ free_j ].p > p_0 + l_0 )
                            {   if( free_j == min )
                                    break;
                                max = free_j - 1;
                                free_j = max - ( free_j - min ) / 2;
                            }else
                            {   if( free_j == max )
                                    break;
                                min = free_j + 1;
                                free_j = min + ( max - free_j ) / 2;
                            }
                        }
                        l += l_1; // Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, zabrakło do pełnej liczby od góry.
                    }
                }
            }
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( &allocated_i
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n + n_prepend + n_append
            , p_0
            , l_0
            , n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , ~0
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return (Pc)p_1 + n_prepend * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
P
E_mem_Q_blk_I_append( P p
, N n
, N align
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            Pc p_0 = 0;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free free_p_;
                N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
                if( ~max )
                {   struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
                    // Szukanie wolnego bloku przyległego od góry.
                    N min = 0;
                    N free_i = max / 2;
                    O{  if( p_0 + l_0 == free_p[ free_i ].p )
                        {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                            {   l_1 = free_p[ free_i ].l;
                                if( l_1 > l )
                                    l_1 = l;
                                else if( l_1 < l )
                                    l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u );
                                l -= l_1;
                            }
                            break;
                        }
                        if( free_p[ free_i ].p > p_0 + l_0 )
                        {   if( free_i == min )
                                break;
                            max = free_i - 1;
                            free_i = max - ( free_i - min ) / 2;
                        }else
                        {   if( free_i == max )
                                break;
                            min = free_i + 1;
                            free_i = min + ( max - free_i ) / 2;
                        }
                    }
                    if( !l
                    && ( !~align
                      || E_simple_Z_p_T_aligned_to_v2( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p, align )
                    ))
                    {   free_p[ free_i ].l -= l_1;
                        if( free_p[ free_i ].l )
                            free_p[ free_i ].p += l_1;
                        else
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                        return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_0;
                    }
                    // Szukanie wolnego bloku przyległego od dołu.
                    min = 0;
                    max = free_i;
                    N free_j = max / 2;
                    if( ~align )
                        O{  if( free_p[ free_j ].p + free_p[ free_j ].l == p_0 )
                            {   Pc p_align = E_simple_Z_p_I_align_up_to_v2( free_p[ free_j ].p, align );
                                if( free_p[ free_j ].l >= ( p_align - free_p[ free_j ].p ) + l )
                                {   free_p[ free_j ].l = p_align - free_p[ free_j ].p;
                                    if( !free_p[ free_j ].l )
                                    {   E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                        free_i--;
                                    }
                                    if( l_1 )
                                    {   free_p[ free_i ].l -= l_1;
                                        free_p[ free_i ].l += ( p_0 - p_align ) - l;
                                        if( free_p[ free_i ].l )
                                        {   free_p[ free_i ].p += l_1;
                                            free_p[ free_i ].p -= ( p_0 - p_align ) - l;
                                        }else
                                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                    }else if(( p_0 - p_align ) - l )
                                    {   E_mem_Q_blk_Q_table_I_put_begin( &allocated_i );
                                        E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                                        struct E_mem_Q_blk_Z_free free_p_;
                                        E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_, p_align + l_0 + l, ( p_0 - p_align ) - l );
                                        E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                                        E_mem_Q_blk_Q_table_I_put_end();
                                    }
                                    E_mem_Q_blk_I_copy( p_align, p_0, l_0 );
                                    E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                                    *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p = p_align;
                                    return p_align + l_0;
                                }
                                break;
                            }
                            if( free_p[ free_j ].p + free_p[ free_j ].l > p_0 )
                            {   if( free_j == min )
                                    break;
                                max = free_j - 1;
                                free_j = max - ( free_j - min ) / 2;
                            }else
                            {   if( free_j == max )
                                    break;
                                min = free_j + 1;
                                free_j = min + ( max - free_j ) / 2;
                            }
                        }
                    else
                        O{  if( free_p[ free_j ].p + free_p[ free_j ].l == p_0 )
                            {   if( free_p[ free_j ].l >= l )
                                {   free_p[ free_j ].l -= l;
                                    if( !free_p[ free_j ].l )
                                    {   E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                        free_i--;
                                    }
                                    if( l_1 )
                                    {   free_p[ free_i ].l -= l_1;
                                        if( free_p[ free_i ].l )
                                            free_p[ free_i ].p += l_1;
                                        else
                                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                    }
                                    Pc p_1 = p_0 - l;
                                    E_mem_Q_blk_I_copy( p_1, p_0, l_0 );
                                    E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                                    *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p = p_1;
                                    return p_1 + l_0;
                                }
                                break;
                            }
                            if( free_p[ free_j ].p + free_p[ free_j ].l > p_0 )
                            {   if( free_j == min )
                                    break;
                                max = free_j - 1;
                                free_j = max - ( free_j - min ) / 2;
                            }else
                            {   if( free_j == max )
                                    break;
                                min = free_j + 1;
                                free_j = min + ( max - free_j ) / 2;
                            }
                        }
                    l += l_1; // Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, a zabrakło do pełnej liczby od góry.
                }
            }
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( &allocated_i
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            , align
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return (Pc)p_1 + l_0;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
P
E_mem_Q_blk_I_prepend( P p
, N n
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            Pc p_0 = 0;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   N l_1 = 0;
                p_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free free_p_;
                N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
                if( ~max )
                {   struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
                    // Szukanie wolnego bloku przyległego od dołu.
                    N min = 0;
                    N max_0 = max;
                    N free_i = max / 2;
                    O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                        {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                            {   l_1 = free_p[ free_i ].l;
                                if( l_1 > l )
                                    l_1 = l;
                                else if( l_1 < l )
                                    l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u );
                                l -= l_1;
                            }
                            break;
                        }
                        if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                        {   if( free_i == min )
                                break;
                            max = free_i - 1;
                            free_i = max - ( free_i - min ) / 2;
                        }else
                        {   if( free_i == max )
                                break;
                            min = free_i + 1;
                            free_i = min + ( max - free_i ) / 2;
                        }
                    }
                    if( !l )
                    {   free_p[ free_i ].l -= l_1;
                        if( !free_p[ free_i ].l )
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                        *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                        return p_0;
                    }
                    // Szukanie wolnego bloku przyległego od góry.
                    min = free_i;
                    max = max_0;
                    N free_j = max / 2;
                    O{  if( p_0 + l_0 == free_p[ free_j ].p )
                        {   if( free_p[ free_j ].l >= l )
                            {   if( free_p[ free_j ].l -= l )
                                    free_p[ free_j ].p += l;
                                else
                                    E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                if( l_1 )
                                {   free_p[ free_i ].l -= l_1;
                                    if( !free_p[ free_i ].l )
                                        E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                }
                                E_mem_Q_blk_I_copy( p_0 + l, p_0, l_0 );
                                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                                if( l_1 )
                                    *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                                return p_0 + l;
                            }
                            break;
                        }
                        if( free_p[ free_j ].p > p_0 + l_0 )
                        {   if( free_j == min )
                                break;
                            max = free_j - 1;
                            free_j = max - ( free_j - min ) / 2;
                        }else
                        {   if( free_j == max )
                                break;
                            min = free_j + 1;
                            free_j = min + ( max - free_j ) / 2;
                        }
                    }
                    l += l_1; // Przywraca oryginalną wartość sprzed scalenia od dołu, skoro był blok przyległy od dołu, a zabrakło do pełnej liczby od góry.
                }
            }
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( &allocated_i
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , l
            , ~0
            );
            if( !p_1 )
                return 0;
            *( P * )p = p_1;
            return (Pc)p_1 + l;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
P
E_mem_Q_blk_I_insert( P p
, N i
, N n
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   Pc p_0 = 0;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
            {   N l = n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                N l_1 = 0;
                p_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
                struct E_mem_Q_blk_Z_free free_p_;
                N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_ );
                if( ~max )
                {   struct E_mem_Q_blk_Z_free *free_p = (P)E_main_S_kernel_args.mem_blk.allocated[ E_main_S_kernel_args.mem_blk.free_id ].p;
                    // Szukanie wolnego bloku przyległego od dołu.
                    N min = 0;
                    N max_0 = max;
                    N free_i = max / 2;
                    O{  if( free_p[ free_i ].p + free_p[ free_i ].l == p_0 )
                        {   if( free_p[ free_i ].l >= E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u )
                            {   l_1 = free_p[ free_i ].l;
                                if( l_1 > l )
                                    l_1 = l;
                                else if( l_1 < l )
                                    l_1 = E_simple_Z_n_I_align_down_to_v( l_1, E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u );
                                l -= l_1;
                            }
                            break;
                        }
                        if( free_p[ free_i ].p + free_p[ free_i ].l > p_0 )
                        {   if( free_i == min )
                                break;
                            max = free_i - 1;
                            free_i = max - ( free_i - min ) / 2;
                        }else
                        {   if( free_i == max )
                                break;
                            min = free_i + 1;
                            free_i = min + ( max - free_i ) / 2;
                        }
                    }
                    if( !l )
                    {   free_p[ free_i ].l -= l_1;
                        if( !free_p[ free_i ].l )
                            E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                        E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                        *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                        E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p
                        , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1
                        , i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                        );
                        return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                    }
                    // Szukanie wolnego bloku przyległego od góry.
                    min = free_i;
                    max = max_0;
                    N free_j = max / 2;
                    O{  if( p_0 + l_0 == free_p[ free_j ].p )
                        {   if( free_p[ free_j ].l >= l )
                            {   if( free_p[ free_j ].l -= l )
                                    free_p[ free_j ].p += l;
                                else
                                    E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_j );
                                if( l_1 )
                                {   free_p[ free_i ].l -= l_1;
                                    if( !free_p[ free_i ].l )
                                        E_mem_Q_blk_Q_sys_table_f_I_move_empty_entry( free_i );
                                    *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p -= l_1;
                                }
                                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n += n;
                                if( l_1 )
                                    E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p
                                    , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1
                                    , i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                                    );
                                E_mem_Q_blk_I_copy( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1 + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u + l
                                ,  E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + l_1 + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                                , l_0 - i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                                );
                                return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                            }
                            break;
                        }
                        if( free_p[ free_j ].p > p_0 + l_0 )
                        {   if( free_j == min )
                                break;
                            max = free_j - 1;
                            free_j = max - ( free_j - min ) / 2;
                        }else
                        {   if( free_j == max )
                                break;
                            min = free_j + 1;
                            free_j = min + ( max - free_j ) / 2;
                        }
                    }
                }
            }
            P p_1 = E_mem_Q_blk_Q_table_M_from_free( &allocated_i
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
            , E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n + n
            , p_0
            , l_0
            , 0
            , ~0
            );
            if( !p_1 )
                return 0;
            //TODO Zrobić w “E_mem_Q_blk_Q_table_M_from_free” parametr przesuniecia dla ‘split’ i kopiowania tam odrazu?
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n )
                E_mem_Q_blk_I_copy( (Pc)p_1 + ( i + n ) * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                , (Pc)p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                , l_0 - i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u
                );
            *( P * )p = p_1;
            return (Pc)p_1 + i * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
P
E_mem_Q_blk_I_remove( P p
, N i
, N n
){  struct E_mem_Q_blk_Z_allocated allocated_p;
    N min = 0;
    N max = E_mem_Q_blk_Q_sys_table_R_last( E_main_S_kernel_args.mem_blk.allocated_id, (Pc)&allocated_p.p - (Pc)&allocated_p );
    N allocated_i = max / 2;
    O{  if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p == *( P * )p )
        {   N l = n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            N l_0 = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
            E_mem_Q_blk_Q_table_I_put_begin( &allocated_i );
            if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n == n ) // Usuwany cały blok.
            {   N allocated_i_sorted_pos;
                P p_ = E_mem_Q_blk_M_new_0( &allocated_i_sorted_pos );
                if( !p_ )
                    return 0;
                E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( P * )p
                , l
                ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n = 0;
                *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p = p_;
                allocated_i = E_mem_Q_blk_Q_sys_table_a_I_sort_inserted( allocated_i, allocated_i_sorted_pos );
            }else if( i + n == E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n ) // Usuwane na końcu bloku.
            {   E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( Pc * )p + l_0 - l
                , l
                ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n -= n;
            }else if( !i ) // Usuwane na początku bloku.
            {   E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                P p_ = *( P * )p;
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , p_
                , l
                ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n -= n;
                *( P * )p = E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p += l;
            }else // Usuwane w środku bloku.
            {   Pc p_0 = *( Pc * )p + ( i + n ) * E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].u;
                E_mem_Q_blk_I_copy( p_0 - l
                , p_0
                , *( Pc * )p + l_0 - p_0
                );
                E_mem_Q_blk_Q_table_I_put_before( E_main_S_kernel_args.mem_blk.free_id );
                struct E_mem_Q_blk_Z_free free_p_;
                if( !~E_mem_Q_blk_Q_sys_table_f_P_put( E_main_S_kernel_args.mem_blk.free_id, (Pc)&free_p_.p - (Pc)&free_p_, (Pc)&free_p_.l - (Pc)&free_p_
                , *( Pc * )p + l_0 - l
                , l
                ))
                {   E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                    E_mem_Q_blk_Q_table_I_put_end();
                    return 0;
                }
                E_mem_Q_blk_Q_table_I_put_after( E_main_S_kernel_args.mem_blk.free_id );
                E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].n -= n;
            }
            E_mem_Q_blk_Q_table_I_put_end();
            return E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p;
        }
        if( E_main_S_kernel_args.mem_blk.allocated[ allocated_i ].p > *( Pc * )p )
        {   if( allocated_i == min )
                break;
            max = allocated_i - 1;
            allocated_i = max - ( allocated_i - min ) / 2;
        }else
        {   if( allocated_i == max )
                break;
            min = allocated_i + 1;
            allocated_i = min + ( max - allocated_i ) / 2;
        }
    }
    return (P)~0;
}
/******************************************************************************/
