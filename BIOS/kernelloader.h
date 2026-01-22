/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS file boot loader
*         include
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒12 f
*******************************************************************************/
typedef unsigned char       N8;
typedef signed char         S8;
typedef unsigned short      N16;
typedef short               S16;
typedef unsigned            N32;
typedef int                 S32;
typedef unsigned long       N64;
typedef long                S64;
typedef _Bool               B;
typedef char                C;
typedef N64                 N;
typedef S64                 S;
typedef N32                 U;
typedef double              F;
typedef void                *P;
typedef C                   *Pc;
typedef N                   *Pn;
//------------------------------------------------------------------------------
#define false                               0
#define true                                1
#define no                                  false
#define yes                                 true
#define _v(a,v)                             (( (a) ^ (a) ) | (v) )
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
#define for_n_rev_(i_var,n)                 i_var = (n); while( i_var-- )
#define for_n_rev(i_var,n)                  N for_n_rev_(i_var,(n))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _0(pointer_variable,l)              E_mem_Q_blk_P_fill_c( (pointer_variable), l, 0 )
#define _0_(pointer_variable)               _0( (pointer_variable), sizeof( *(pointer_variable) ))
#define M(l)                                E_mem_Q_blk_M(l)
#define Mt(u,n)                             E_mem_Q_blk_M_tab((u),(n))
#define W(pointer_variable)                 E_mem_Q_blk_W( pointer_variable )
#define M_(pointer_variable)                pointer_variable = M( sizeof( *( pointer_variable )))
#define Mt_(pointer_variable,n)             pointer_variable = Mt( sizeof( *( pointer_variable )), (n) )
#define W_(pointer_variable)                ( W( pointer_variable ), pointer_variable = 0 )
#define W_tab_(pointer_variable)            ( E_mem_Q_tab_W( pointer_variable ), pointer_variable = 0 )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _inline                             static __attribute__ ((__always_inline__,__unused__))
#define _internal                           static
//==============================================================================
#include "simple.h"
//==============================================================================
#define H_oux_E_mem_S_page_size             0x1000
#define E_memory_S_start                    0x100000
//==============================================================================
#define E_main_Z_memory_start               0x10000
#define E_main_Z_memory_table_end           0x80000
enum
{ E_main_Z_memory_table_Z_memory_type_S_available = 1
, E_main_Z_memory_table_Z_memory_type_S_reserved
, E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
, E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
, E_main_Z_memory_table_Z_memory_type_S_bad
, E_main_Z_memory_table_Z_memory_type_S_boot_loader
};
struct __attribute__(( __packed__ ))  E_main_Z_memory_table_entry
{ Pc address;
  N64 size;
  N32 type;
  N32 extended_attributes;
};
struct __attribute__(( __packed__ )) E_main_Z_video
{ N32 framebuffer;
  N16 line_width;
  N16 width, height;
  N8 bpp;
  N8 red_start, red_size, green_start, green_size, blue_start, blue_size;
};
//==============================================================================
Pc E_text_Z_su_R_u( Pc, U * );
//==============================================================================
void E_mem_M( struct E_main_Z_memory_table_entry * );
void E_mem_M_free( struct E_main_Z_memory_table_entry * );
B E_mem_Q_blk_T_eq( P, P, N );
void E_mem_Q_blk_I_copy( P, P, N );
void E_mem_Q_blk_P_fill_c( P, N, C );
P E_mem_Q_blk_M(N);
P E_mem_Q_blk_M_tab( N, N );
P E_mem_Q_blk_M_align( N, N );
P E_mem_Q_blk_M_align_tab( N, N, N );
P E_mem_Q_blk_M_replace_tab( P, N, N );
P E_mem_Q_blk_M_replace( P, N );
P E_mem_Q_blk_M_split( P, N );
N E_mem_Q_blk_W(P);
P E_mem_Q_blk_I_add( P, N, N *, N * );
P E_mem_Q_blk_I_prepend_append( P, N, N );
P E_mem_Q_blk_I_append( P, N, N );
P E_mem_Q_blk_I_prepend( P, N );
P E_mem_Q_blk_I_insert( P, N, N );
P E_mem_Q_blk_I_remove( P, N, N );
//==============================================================================
#define E_vga_S_background_color            0xdbdbdb
#define E_vga_S_text_color                  0
enum E_vga_Z_aa_pixel
{ E_vga_Z_aa_pixel_S_e = 1 << 0,
  E_vga_Z_aa_pixel_S_se = 1 << 1,
  E_vga_Z_aa_pixel_S_s = 1 << 2,
  E_vga_Z_aa_pixel_S_sw = 1 << 3,
  E_vga_Z_aa_pixel_S_w = 1 << 4,
  E_vga_Z_aa_pixel_S_nw = 1 << 5,
  E_vga_Z_aa_pixel_S_n = 1 << 6,
  E_vga_Z_aa_pixel_S_ne = 1 << 7
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
extern struct E_main_Z_video *E_vga_S_video;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N E_vga_Z_color_M( N8, N8, N8 );
N8 E_vga_Z_color_R_red(N32);
N8 E_vga_Z_color_R_green(N32);
N8 E_vga_Z_color_R_blue(N32);
N E_vga_Z_color_M_gray(N8);
N E_vga_R_video_color(N);
N E_vga_R_pixel( N, N );
void E_vga_P_pixel( N, N, N );
void E_vga_I_set_pixel_aa( N, N, N, F, N );
void E_vga_I_draw_rect( N, N, N, N, N );
void E_vga_I_fill_rect( N, N, N, N, N );
//==============================================================================
N E_font_M(void);
N E_font_I_draw( N, N, N, U );
void E_font_I_print_nl(void);
void E_font_I_print_u(U);
N E_font_I_print(Pc);
void E_font_I_print_hex(N);
/******************************************************************************/
