/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   kernel
*         font
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒16 L
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
//==============================================================================
struct E_font_Q_font_Z_bitmap
{ U u;
  N8 width;
  N8 *bitmap;
};
struct
{ struct E_font_Q_font_Z_bitmap *bitmap;
  N32 default_i;
  N32 bitmap_n;
  N8 height;
}font;
N32 E_font_S_x, E_font_S_y;
N32 E_font_S_color = E_vga_S_text_color;
N8 E_font_S_size = 1;
N8 E_font_S_thickness = 1;
//==============================================================================
N
E_font_M( void
){  E_font_S_x = E_font_S_thickness + 1;
    E_font_S_y = E_font_S_size + 1;
    font.height = 8;
    font.default_i = 63;
    font.bitmap_n = 177;
    Mt_( font.bitmap, font.bitmap_n );
    if( !font.bitmap )
        return ~0;
    for_n( i, font.bitmap_n )
    {   N8 *bitmap;
        switch(i)
        { case 0:
            {   font.bitmap[i].u = ' ';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 1:
            {   font.bitmap[i].u = '!';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 3
                , 3
                , 3
                , 0
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 2:
            {   font.bitmap[i].u = '\"';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 3:
            {   font.bitmap[i].u = '#';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0, 3, 0
                , 3, 3, 3, 3, 3
                , 0, 3, 0, 3, 0
                , 0, 3, 0, 3, 0
                , 3, 3, 3, 3, 3
                , 0, 3, 0, 3, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 4:
            {   font.bitmap[i].u = '$';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0
                , 1, 3, 2
                , 2, 0, 0
                , 1, 3, 1
                , 0, 0, 2
                , 2, 3, 1
                , 0, 3, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 5:
            {   font.bitmap[i].u = '%';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0, 3
                , 3, 0, 3, 1
                , 2, 0, 3, 0
                , 0, 3, 0, 2
                , 1, 3, 0, 3
                , 3, 0, 3, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 6:
            {   font.bitmap[i].u = '&';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1, 0
                , 3, 0, 3, 0
                , 1, 3, 1, 0
                , 3, 0, 1, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 7:
            {   font.bitmap[i].u = '\'';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 3
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 8:
            {   font.bitmap[i].u = '(';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 3, 0
                , 3, 0
                , 3, 0
                , 3, 0
                , 0, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 9:
            {   font.bitmap[i].u = ')';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0
                , 0, 3
                , 0, 3
                , 0, 3
                , 0, 3
                , 3, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 10:
            {   font.bitmap[i].u = '*';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 1, 3, 1
                , 3, 3, 3
                , 1, 3, 1
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 11:
            {   font.bitmap[i].u = '+';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 3, 0
                , 3, 3, 3
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 12:
            {   font.bitmap[i].u = ',';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 3
                , 2, 1
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 13:
            {   font.bitmap[i].u = '-';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 14:
            {   font.bitmap[i].u = '.';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 0
                , 0
                , 0
                , 0
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 15:
            {   font.bitmap[i].u = '/';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 3
                , 0, 0, 2, 1
                , 0, 0, 3, 0
                , 0, 3, 0, 0
                , 1, 2, 0, 0
                , 3, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 16:
            {   font.bitmap[i].u = '0';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 1, 3, 3
                , 3, 3, 1, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 17:
            {   font.bitmap[i].u = '1';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 3
                , 0, 3, 3
                , 3, 1, 3
                , 1, 0, 3
                , 0, 0, 3
                , 0, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 18:
            {   font.bitmap[i].u = '2';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 0, 0, 0, 3
                , 0, 0, 3, 0
                , 0, 3, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 19:
            {   font.bitmap[i].u = '3';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 2, 3, 3, 2
                , 0, 0, 0, 3
                , 0, 3, 3, 2
                , 0, 0, 0, 3
                , 0, 0, 0, 3
                , 2, 3, 3, 2
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 20:
            {   font.bitmap[i].u = '4';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 3, 2
                , 0, 3, 1, 3
                , 3, 1, 0, 3
                , 2, 3, 3, 3
                , 0, 0, 0, 3
                , 0, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 21:
            {   font.bitmap[i].u = '5';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 3, 0, 0, 0
                , 2, 3, 3, 1
                , 0, 0, 0, 3
                , 0, 0, 0, 3
                , 2, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 22:
            {   font.bitmap[i].u = '6';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 2, 3
                , 2, 1, 0, 0
                , 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 23:
            {   font.bitmap[i].u = '7';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 0, 0, 1, 2
                , 0, 0, 3, 0
                , 0, 1, 2, 0
                , 0, 3, 0, 0
                , 3, 1, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 24:
            {   font.bitmap[i].u = '8';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 25:
            {   font.bitmap[i].u = '9';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 3
                , 0, 0, 1, 2
                , 3, 2, 1, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 26:
            {   font.bitmap[i].u = ':';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 0
                , 3
                , 0
                , 0
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 27:
            {   font.bitmap[i].u = ';';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 0, 3
                , 0, 0
                , 0, 0
                , 0, 3
                , 2, 1
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 28:
            {   font.bitmap[i].u = '<';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 3
                , 0, 3, 0
                , 3, 0, 0
                , 3, 0, 0
                , 0, 3, 0
                , 0, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 29:
            {   font.bitmap[i].u = '=';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 30:
            {   font.bitmap[i].u = '>';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0
                , 0, 3, 0
                , 0, 0, 3
                , 0, 0, 3
                , 0, 3, 0
                , 3, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 31:
            {   font.bitmap[i].u = '?';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 2, 0, 3
                , 0, 0, 3
                , 0, 3, 0
                , 0, 0, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 32:
            {   font.bitmap[i].u = '@';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 3, 1
                , 3, 0, 1, 1, 3
                , 3, 0, 2, 2, 3
                , 3, 0, 2, 2, 2
                , 3, 0, 0, 0, 0
                , 1, 3, 3, 3, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 33:
            {   font.bitmap[i].u = 'A';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 3, 0
                , 3, 1, 1, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 34:
            {   font.bitmap[i].u = 'B';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 35:
            {   font.bitmap[i].u = 'C';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 1, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 36:
            {   font.bitmap[i].u = 'D';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 37:
            {   font.bitmap[i].u = 'E';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 38:
            {   font.bitmap[i].u = 'F';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 39:
            {   font.bitmap[i].u = 'G';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 0, 3, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 2
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 40:
            {   font.bitmap[i].u = 'H';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 41:
            {   font.bitmap[i].u = 'I';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 3
                , 3
                , 3
                , 3
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 42:
            {   font.bitmap[i].u = 'J';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 3
                , 0, 0, 3
                , 0, 0, 3
                , 0, 0, 3
                , 3, 0, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 43:
            {   font.bitmap[i].u = 'K';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 3
                , 3, 0, 3, 0
                , 3, 3, 0, 0
                , 3, 3, 0, 0
                , 3, 0, 3, 0
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 44:
            {   font.bitmap[i].u = 'L';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0
                , 3, 0, 0
                , 3, 0, 0
                , 3, 0, 0
                , 3, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 45:
            {   font.bitmap[i].u = 'M';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0, 3
                , 3, 2, 0, 2, 3
                , 3, 3, 1, 3, 3
                , 3, 1, 3, 1, 3
                , 3, 0, 3, 0, 3
                , 3, 0, 0, 0, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 46:
            {   font.bitmap[i].u = 'N';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 3
                , 3, 2, 0, 3
                , 3, 3, 1, 3
                , 3, 1, 3, 3
                , 3, 0, 2, 3
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 47:
            {   font.bitmap[i].u = 'O';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 48:
            {   font.bitmap[i].u = 'P';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 49:
            {   font.bitmap[i].u = 'Q';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 2, 0
                , 0, 0, 1, 2
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 50:
            {   font.bitmap[i].u = 'R';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 3, 0, 3, 0
                , 3, 0, 1, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 51:
            {   font.bitmap[i].u = 'S';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 2
                , 2, 0, 0, 0
                , 1, 3, 0, 0
                , 0, 0, 3, 1
                , 0, 0, 0, 2
                , 2, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 52:
            {   font.bitmap[i].u = 'T';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3
                , 0, 3, 0
                , 0, 3, 0
                , 0, 3, 0
                , 0, 3, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 53:
            {   font.bitmap[i].u = 'U';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 54:
            {   font.bitmap[i].u = 'V';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 2, 1, 2
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 55:
            {   font.bitmap[i].u = 'W';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0, 3
                , 3, 0, 2, 0, 3
                , 3, 0, 3, 0, 2
                , 3, 0, 3, 0, 3
                , 2, 1, 2, 1, 2
                , 0, 3, 0, 3, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 56:
            {   font.bitmap[i].u = 'X';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3
                , 3, 0, 3
                , 0, 3, 0
                , 0, 3, 0
                , 3, 0, 3
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 57:
            {   font.bitmap[i].u = 'Y';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3
                , 3, 0, 3
                , 3, 1, 3
                , 0, 3, 0
                , 0, 3, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 58:
            {   font.bitmap[i].u = 'Z';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 0, 0, 1, 2
                , 0, 0, 3, 0
                , 0, 3, 0, 0
                , 2, 1, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 59:
            {   font.bitmap[i].u = '[';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3
                , 3, 0
                , 3, 0
                , 3, 0
                , 3, 0
                , 3, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 60:
            {   font.bitmap[i].u = '\\';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0
                , 0, 2, 0, 0
                , 0, 3, 0, 0
                , 0, 0, 3, 0
                , 0, 0, 2, 0
                , 0, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 61:
            {   font.bitmap[i].u = ']';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3
                , 0, 3
                , 0, 3
                , 0, 3
                , 0, 3
                , 3, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 62:
            {   font.bitmap[i].u = '^';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 63:
            {   font.bitmap[i].u = '_';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 64:
            {   font.bitmap[i].u = '`';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 2, 0
                , 1, 3
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 65:
            {   font.bitmap[i].u = 'a';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 1, 3, 3, 1
                , 0, 0, 0, 3
                , 1, 3, 3, 3
                , 3, 0, 0, 3
                , 1, 3, 2, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 66:
            {   font.bitmap[i].u = 'b';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 2, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 67:
            {   font.bitmap[i].u = 'c';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 1, 3, 3
                , 3, 0, 0
                , 3, 0, 0
                , 1, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 68:
            {   font.bitmap[i].u = 'd';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 3
                , 0, 0, 0, 3
                , 1, 3, 2, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 69:
            {   font.bitmap[i].u = 'e';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 1, 3, 3, 1
                , 3, 0, 0, 3
                , 2, 3, 3, 2
                , 3, 0, 0, 0
                , 1, 3, 3, 2
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 70:
            {   font.bitmap[i].u = 'f';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 2, 3
                , 0, 3, 0
                , 3, 3, 3
                , 0, 3, 0
                , 0, 3, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 71:
            {   font.bitmap[i].u = 'g';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 1, 3, 3, 2
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 2
                , 0, 0, 0, 3
                , 2, 3, 3, 2
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 72:
            {   font.bitmap[i].u = 'h';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0
                , 3, 0, 0, 0
                , 2, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 73:
            {   font.bitmap[i].u = 'i';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 0
                , 3
                , 3
                , 3
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 74:
            {   font.bitmap[i].u = 'j';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 0, 0
                , 0, 3
                , 0, 3
                , 0, 3
                , 0, 3
                , 0, 3
                , 3, 2
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 75:
            {   font.bitmap[i].u = 'k';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0, 0
                , 3, 0, 0, 3
                , 3, 0, 3, 1
                , 3, 3, 3, 0
                , 3, 0, 1, 2
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 76:
            {   font.bitmap[i].u = 'l';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0
                , 3, 0
                , 3, 0
                , 3, 0
                , 3, 0
                , 2, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 77:
            {   font.bitmap[i].u = 'm';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 3, 3, 2, 3, 2
                , 3, 0, 3, 0, 3
                , 3, 0, 3, 0, 3
                , 3, 0, 3, 0, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 78:
            {   font.bitmap[i].u = 'n';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 3, 2
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 79:
            {   font.bitmap[i].u = 'o';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 2, 3, 2
                , 3, 0, 3
                , 3, 0, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 80:
            {   font.bitmap[i].u = 'p';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 3, 3, 3, 1
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 3, 2, 3, 1
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 81:
            {   font.bitmap[i].u = 'q';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 1, 3, 3, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 2, 3
                , 0, 0, 0, 3
                , 0, 0, 0, 3
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 82:
            {   font.bitmap[i].u = 'r';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 2, 3
                , 3, 0
                , 3, 0
                , 3, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 83:
            {   font.bitmap[i].u = 's';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 1, 3, 2
                , 2, 0, 0
                , 1, 3, 1
                , 0, 0, 2
                , 2, 3, 1
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 84:
            {   font.bitmap[i].u = 't';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0
                , 0, 3, 0
                , 3, 3, 3
                , 0, 3, 0
                , 0, 3, 0
                , 0, 2, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 85:
            {   font.bitmap[i].u = 'u';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 2, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 86:
            {   font.bitmap[i].u = 'v';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 0, 3
                , 3, 0, 3
                , 2, 1, 2
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 87:
            {   font.bitmap[i].u = 'w';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 3, 0, 2, 0, 2
                , 3, 0, 3, 0, 3
                , 2, 1, 2, 1, 2
                , 0, 3, 0, 3, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 88:
            {   font.bitmap[i].u = 'x';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 0, 3
                , 0, 3, 0
                , 0, 3, 0
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 89:
            {   font.bitmap[i].u = 'y';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 0, 3
                , 3, 0, 3
                , 2, 1, 2
                , 0, 3, 0
                , 1, 2, 0
                , 3, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 90:
            {   font.bitmap[i].u = 'z';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 3, 3
                , 0, 2, 1
                , 1, 2, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 91:
            {   font.bitmap[i].u = '{';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 2, 1
                , 0, 3, 0
                , 1, 2, 0
                , 3, 2, 0
                , 0, 3, 0
                , 0, 2, 1
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 92:
            {   font.bitmap[i].u = '|';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 3
                , 3
                , 3
                , 3
                , 3
                , 3
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 93:
            {   font.bitmap[i].u = '}';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2, 0
                , 0, 3, 0
                , 0, 2, 1
                , 0, 2, 3
                , 0, 3, 0
                , 1, 2, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 94:
            {   font.bitmap[i].u = '~';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 1, 3, 0, 3
                , 3, 0, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 95:
            {   font.bitmap[i].u = L'¡';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 3
                , 0
                , 3
                , 3
                , 3
                , 3
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 96:
            {   font.bitmap[i].u = L'¢';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 2, 0
                , 0, 3, 3, 3
                , 2, 1, 2, 0
                , 3, 0, 2, 0
                , 2, 1, 2, 0
                , 0, 3, 3, 3
                , 0, 0, 2, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 97:
            {   font.bitmap[i].u = L'¦';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 3
                , 1
                , 1
                , 3
                , 3
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 98:
            {   font.bitmap[i].u = L'©';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 3, 1
                , 3, 1, 2, 0, 3
                , 3, 2, 0, 0, 3
                , 3, 1, 2, 0, 3
                , 1, 3, 3, 3, 1
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 99:
            {   font.bitmap[i].u = L'«';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 3, 0, 3
                , 3, 0, 3, 0
                , 0, 3, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 100:
            {   font.bitmap[i].u = L'®';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 1, 2, 3
                , 3, 1, 2, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 101:
            {   font.bitmap[i].u = L'°';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 3, 0, 3
                , 1, 3, 1
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 102:
            {   font.bitmap[i].u = L'±';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 3, 0
                , 3, 3, 3
                , 0, 3, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 103:
            {   font.bitmap[i].u = L'²';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 2, 0, 3
                , 0, 3, 0
                , 3, 2, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 104:
            {   font.bitmap[i].u = L'³';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 2, 3, 2
                , 0, 0, 3
                , 0, 3, 2
                , 0, 0, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 105:
            {   font.bitmap[i].u = L'µ';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 2, 3, 3
                , 3, 0, 0
                , 3, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 106:
            {   font.bitmap[i].u = L'·';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 0
                , 0
                , 3
                , 0
                , 0
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 107:
            {   font.bitmap[i].u = L'»';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 3, 0, 3, 0
                , 0, 3, 0, 3
                , 3, 0, 3, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 108:
            {   font.bitmap[i].u = L'¿';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 3, 0
                , 3, 0, 0
                , 3, 0, 2
                , 1, 3, 1
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 109:
            {   font.bitmap[i].u = L'Ó';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 1
                , 3, 0, 2, 3
                , 3, 2, 0, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 1, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 110:
            {   font.bitmap[i].u = L'×';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 3, 0, 0, 3
                , 0, 3, 3, 0
                , 0, 3, 3, 0
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 111:
            {   font.bitmap[i].u = L'ó';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 3
                , 0, 3, 0
                , 2, 3, 2
                , 3, 0, 3
                , 3, 0, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 112:
            {   font.bitmap[i].u = L'÷';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 3, 0
                , 0, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 113:
            {   font.bitmap[i].u = L'Ą';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 3, 0
                , 3, 1, 1, 3
                , 3, 0, 0, 3
                , 3, 3, 3, 3
                , 3, 0, 0, 3
                , 3, 0, 0, 3
                , 0, 0, 1, 2
                , 0, 0, 2, 1
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 114:
            {   font.bitmap[i].u = L'ą';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 1, 3, 3, 1
                , 0, 0, 0, 3
                , 1, 3, 3, 3
                , 3, 0, 0, 3
                , 1, 3, 2, 3
                , 0, 0, 3, 0
                , 0, 0, 2, 3
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 115:
            {   font.bitmap[i].u = L'Ć';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 3
                , 3, 0, 2, 0
                , 3, 0, 2, 0
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 1, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 116:
            {   font.bitmap[i].u = L'ć';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 3
                , 0, 3, 0
                , 1, 3, 3
                , 3, 0, 0
                , 3, 0, 0
                , 1, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 117:
            {   font.bitmap[i].u = L'Ę';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 3, 3, 3
                , 3, 0, 0, 0
                , 3, 0, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 1, 2
                , 0, 0, 2, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 118:
            {   font.bitmap[i].u = L'ę';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 1, 3, 3, 1
                , 3, 0, 0, 3
                , 2, 3, 3, 2
                , 3, 0, 0, 0
                , 1, 3, 3, 2
                , 0, 0, 3, 0
                , 0, 0, 2, 3
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 119:
            {   font.bitmap[i].u = L'Ł';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 0
                , 3, 0, 3
                , 2, 3, 0
                , 3, 0, 0
                , 3, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 120:
            {   font.bitmap[i].u = L'ł';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0
                , 0, 3, 3
                , 1, 3, 1
                , 3, 3, 0
                , 0, 3, 0
                , 0, 2, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 121:
            {   font.bitmap[i].u = L'Ń';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3, 2
                , 3, 2, 0, 3
                , 3, 3, 1, 3
                , 3, 1, 3, 3
                , 3, 0, 2, 3
                , 3, 0, 0, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 122:
            {   font.bitmap[i].u = L'ń';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 3
                , 0, 3, 0
                , 3, 3, 2
                , 3, 0, 3
                , 3, 0, 3
                , 3, 0, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 123:
            {   font.bitmap[i].u = L'Ś';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 3, 2
                , 2, 0, 0, 3
                , 1, 3, 1, 0
                , 0, 0, 3, 1
                , 0, 0, 0, 2
                , 2, 3, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 124:
            {   font.bitmap[i].u = L'ś';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 3
                , 1, 3, 2
                , 2, 0, 0
                , 1, 3, 1
                , 0, 0, 2
                , 2, 3, 1
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 125:
            {   font.bitmap[i].u = L'Ź';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 3, 0, 0, 2
                , 2, 0, 3, 0
                , 0, 3, 0, 0
                , 2, 1, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 126:
            {   font.bitmap[i].u = L'ź';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 2
                , 0, 2, 0
                , 3, 3, 3
                , 0, 2, 1
                , 1, 2, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 127:
            {   font.bitmap[i].u = L'Ż';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 3
                , 0, 0, 1, 2
                , 2, 1, 3, 0
                , 0, 3, 1, 2
                , 2, 1, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 128:
            {   font.bitmap[i].u = L'ż';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0
                , 0, 0, 0
                , 3, 3, 3
                , 0, 2, 1
                , 1, 2, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 129:
            {   font.bitmap[i].u = L'՚';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 3
                , 2
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 130:
            {   font.bitmap[i].u = L' ';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 131:
            {   font.bitmap[i].u = L' ';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 132:
            {   font.bitmap[i].u = L'‐';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 0, 0
                , 3, 3
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 133:
            {   font.bitmap[i].u = L'‒';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 134:
            {   font.bitmap[i].u = L'–';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 3, 3, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 135:
            {   font.bitmap[i].u = L'—';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 3, 3, 3, 3, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 136:
            {   font.bitmap[i].u = L'―';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 3, 3, 3, 3, 3
                , 3, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 137:
            {   font.bitmap[i].u = L'‘';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2
                , 3, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 138:
            {   font.bitmap[i].u = L'’';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 2, 1
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 139:
            {   font.bitmap[i].u = L'‚';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 3
                , 2, 1
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 140:
            {   font.bitmap[i].u = L'‛';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0
                , 1, 2
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 141:
            {   font.bitmap[i].u = L'“';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2, 1, 2
                , 3, 0, 3, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 142:
            {   font.bitmap[i].u = L'”';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0, 3
                , 2, 1, 2, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 143:
            {   font.bitmap[i].u = L'„';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 3, 0, 3
                , 2, 1, 2, 1
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 144:
            {   font.bitmap[i].u = L'‟';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3, 0
                , 1, 2, 1, 2
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 145:
            {   font.bitmap[i].u = L'•';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 2, 3, 2
                , 3, 3, 3
                , 2, 3, 2
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 146:
            {   font.bitmap[i].u = L'‣';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 2, 0
                , 3, 3, 3
                , 3, 2, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 147:
            {   font.bitmap[i].u = L'…';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 3, 0, 3, 0, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 148:
            {   font.bitmap[i].u = L'′';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 3, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 149:
            {   font.bitmap[i].u = L'‹';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 0, 3
                , 3, 0
                , 0, 3
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 150:
            {   font.bitmap[i].u = L'›';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 3, 0
                , 0, 3
                , 3, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 151:
            {   font.bitmap[i].u = L'‽';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 2, 2, 3
                , 0, 2, 3
                , 0, 3, 0
                , 0, 0, 0
                , 0, 3, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 152:
            {   font.bitmap[i].u = L'⁂';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 3, 1, 0
                , 0, 3, 3, 3, 0
                , 0, 1, 3, 1, 0
                , 1, 3, 1, 3, 1
                , 3, 3, 3, 3, 3
                , 1, 3, 1, 3, 1
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 153:
            {   font.bitmap[i].u = L'⁃';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0
                , 0, 0
                , 2, 2
                , 3, 3
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 154:
            {   font.bitmap[i].u = L'⁄';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 2, 3
                , 0, 0, 0, 3, 0
                , 0, 0, 3, 1, 0
                , 0, 1, 3, 0, 0
                , 0, 3, 0, 0, 0
                , 3, 2, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 155:
            {   font.bitmap[i].u = L'⁅';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3
                , 3, 0
                , 3, 3
                , 3, 0
                , 3, 0
                , 3, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 156:
            {   font.bitmap[i].u = L'⁆';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3
                , 0, 3
                , 3, 3
                , 0, 3
                , 0, 3
                , 3, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 157:
            {   font.bitmap[i].u = L'⁋';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 1
                , 3, 0, 3, 3
                , 3, 0, 3, 1
                , 3, 0, 3, 0
                , 3, 0, 3, 0
                , 3, 0, 3, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 158:
            {   font.bitmap[i].u = L'⁑';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 1, 3, 1
                , 3, 3, 3
                , 1, 3, 1
                , 1, 3, 1
                , 3, 3, 3
                , 1, 3, 1
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 159:
            {   font.bitmap[i].u = L' ';
                font.bitmap[i].width = 1;
                N8 *bitmap_ = ( N8 [] )
                { 0
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                , 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 160:
            {   font.bitmap[i].u = L'€';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 1, 3, 3
                , 1, 3, 0, 0
                , 3, 2, 2, 0
                , 3, 2, 2, 0
                , 1, 3, 0, 0
                , 0, 1, 3, 3
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 161:
            {   font.bitmap[i].u = L'№';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0, 3, 0, 0
                , 3, 1, 3, 0, 0
                , 3, 3, 3, 2, 3
                , 3, 2, 3, 2, 3
                , 3, 1, 3, 0, 0
                , 3, 0, 3, 2, 2
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 162:
            {   font.bitmap[i].u = L'℠';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 2, 0, 3
                , 3, 0, 3, 2, 3
                , 0, 3, 2, 3, 3
                , 3, 0, 3, 0, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 163:
            {   font.bitmap[i].u = L'™';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 3, 3, 3, 0, 3
                , 0, 3, 2, 2, 3
                , 0, 3, 2, 3, 3
                , 0, 3, 2, 0, 3
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 164:
            {   font.bitmap[i].u = L'⇢';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 3, 0
                , 3, 0, 3, 3, 3
                , 0, 0, 0, 3, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 165:
            {   font.bitmap[i].u = L'−';
                font.bitmap[i].width = 3;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0
                , 0, 0, 0
                , 3, 3, 3
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                , 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 166:
            {   font.bitmap[i].u = L'≈';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0
                , 1, 3, 1, 3
                , 3, 1, 3, 1
                , 0, 0, 0, 0
                , 1, 3, 1, 3
                , 3, 1, 3, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 167:
            {   font.bitmap[i].u = L'〈';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 1, 1
                , 3, 0
                , 3, 0
                , 1, 1
                , 0, 3
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 168:
            {   font.bitmap[i].u = L'〉';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 3, 0
                , 1, 1
                , 0, 3
                , 0, 3
                , 1, 1
                , 3, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 169:
            {   font.bitmap[i].u = L'⏎';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 3, 3
                , 0, 3, 0, 3, 3
                , 1, 3, 3, 3, 3
                , 3, 0, 0, 0, 3
                , 1, 3, 3, 3, 3
                , 0, 3, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 170:
            {   font.bitmap[i].u = L'✓';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 3
                , 0, 0, 0, 3, 0
                , 0, 0, 0, 2, 0
                , 0, 0, 3, 0, 0
                , 3, 0, 2, 0, 0
                , 0, 3, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 171:
            {   font.bitmap[i].u = L'❛';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2
                , 3, 0
                , 3, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 172:
            {   font.bitmap[i].u = L'❜';
                font.bitmap[i].width = 2;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3
                , 0, 3
                , 2, 1
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                , 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 173:
            {   font.bitmap[i].u = L'❝';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2, 1, 2
                , 3, 0, 3, 0
                , 3, 0, 3, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 174:
            {   font.bitmap[i].u = L'❞';
                font.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 0, 3, 0, 3
                , 0, 3, 0, 3
                , 2, 1, 2, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 175:
            {   font.bitmap[i].u = L'➪';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 3, 0, 0
                , 3, 3, 3, 3, 0
                , 3, 0, 0, 0, 3
                , 3, 3, 3, 3, 0
                , 0, 0, 3, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
          case 176:
            {   font.bitmap[i].u = L'〃';
                font.bitmap[i].width = 5;
                N8 *bitmap_ = ( N8 [] )
                { 0, 0, 0, 0, 0
                , 0, 0, 3, 0, 3
                , 0, 3, 0, 3, 0
                , 0, 3, 0, 3, 0
                , 3, 0, 3, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                , 0, 0, 0, 0, 0
                };
                bitmap = M( font.bitmap[i].width * font.height );
                if( !bitmap )
                    return ~2;
                E_mem_Q_blk_I_copy( bitmap, bitmap_, font.bitmap[i].width * font.height );
                break;
            }
        }
        Mt_( font.bitmap[i].bitmap, font.bitmap[i].width * font.height / 4 + ( font.bitmap[i].width * font.height % 4 ? 1 : 0 ));
        if( !font.bitmap[i].bitmap )
        {   for_n( j, i )
                W( font.bitmap[j].bitmap );
            W( font.bitmap );
            return ~0;
        }
        N8 c;
        for_n( j, font.bitmap[i].width * font.height )
        {   if( j % 4 == 0 )
                c = 0;
            c |= bitmap[j] << ( j % 4 * 2 );
            if( j % 4 == 3 )
                font.bitmap[i].bitmap[ j / 4 ] = c;
        }
        if( j % 4 == 1 || j % 4 == 2 || j % 4 == 3 )
            font.bitmap[i].bitmap[ j / 4 + 1 ] = c;
    }
    return 0;
}
void
E_font_W( void
){  for_n( i, font.bitmap_n )
        W( font.bitmap[i].bitmap );
    W( font.bitmap );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_font_I_draw( U u
, N32 x
, N32 y
, N32 video_color
, N8 size
, N8 thickness
){  size++;
    thickness++;
    N32 min = 0;
    N32 max = font.bitmap_n - 1;
    N32 i = max / 2;
    O{  if( font.bitmap[i].u == u )
        {   N j = 0;
            C c;
            N32 x_, y_ = y;
            for_n( font_y, font.height )
            {   x_ = x;
                for_n( font_x, font.bitmap[i].width )
                {   if( j % 4 == 0 )
                        c = font.bitmap[i].bitmap[ j / 4 ];
                    if(( c >> ( j % 4 * 2 )) & 3 )
                    {   F brightness;
                        switch(( c >> ( j % 4 * 2 )) & 3 )
                        { case 1:
                              brightness = (F)( (N8)~0 >> 2 ) / (N8)~0;
                              break;
                          case 2:
                              brightness = (F)( (N8)~0 >> 1 ) / (N8)~0;
                              break;
                          case 3:
                              brightness = 1;
                              break;
                        }
                        for_n( j, size )
                        {   for_n( i, thickness )
                                E_vga_I_set_pixel_aa(
                                  x_ + i, y_ + j
                                , video_color
                                , brightness
                                , ~0
                                );
                        }
                    }
                    x_ += thickness;
                    j++;
                }
                y_ += size;
            }
            return thickness * font.bitmap[i].width;
        }
        if( font.bitmap[i].u > u )
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
    return ~0;
}
void
E_font_I_scroll_fwd( N dy
){  E_mem_Q_blk_I_copy( (P)E_main_S_kernel_args.framebuffer.p
    , (P)( E_main_S_kernel_args.framebuffer.p + dy * E_main_S_kernel_args.framebuffer.pixels_per_scan_line )
    , ( E_main_S_kernel_args.framebuffer.height - dy ) * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * sizeof( *E_main_S_kernel_args.framebuffer.p )
    );
    E_vga_I_fill_rect( 0, E_main_S_kernel_args.framebuffer.height - dy, E_main_S_kernel_args.framebuffer.width, dy, E_vga_R_video_color( E_vga_S_background_color ));
}
void
E_font_I_print_nl( void
){  E_font_S_x = E_font_S_size + 1;
    if( E_font_S_y + ( E_font_S_size + 1 ) * ( font.height + E_font_S_size + 1 ) > E_main_S_kernel_args.framebuffer.height )
        E_font_I_scroll_fwd( E_font_S_size + 1 + font.height );
    else
        E_font_S_y += font.height + E_font_S_size + 1;
}
void
E_font_I_print_u( U u
){  if( u == '\n' )
    {   E_font_I_print_nl();
        return;
    }
    N dx;
    N32 min = 0;
    N32 max = font.bitmap_n - 1;
    N32 i = max / 2;
    O{  if( font.bitmap[i].u == u )
            break;
        if( font.bitmap[i].u > u )
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
    if( font.bitmap[i].u == u )
        dx = font.bitmap[i].width;
    else
    {   u = font.bitmap[ font.default_i ].u;
        dx = font.bitmap[ font.default_i ].width;
    }
    dx *= E_font_S_thickness + 1;
    if( E_font_S_x + dx + E_font_S_thickness + 1 > E_main_S_kernel_args.framebuffer.width )
        E_font_I_print_nl();
    E_font_I_draw( u, E_font_S_x, E_font_S_y, E_font_S_color, E_font_S_size, E_font_S_thickness );
    E_font_S_x += dx + E_font_S_thickness + 1;
}
N
E_font_I_print( Pc s
){  while( *s )
    {   U u;
        Pc s_ = E_text_Z_su_R_u( s, &u );
        if( s_ == s )
            return ~0;
        s = s_;
        if( ~u )
            E_font_I_print_u(u);
    }
    __asm__ volatile (
    "\n"    "sfence"
    );
    return 0;
}
void
E_font_I_print_hex( N n
){  E_font_I_print( "0x" );
    for_n_rev( i, sizeof(N) * 2 )
    {   U u = ( n >> ( i * 4 )) & 0xf;
        if( u < 10 )
            u += '0';
        else
            u += 'a' - 10;
        E_font_I_print_u(u);
        if( i
        && i % 4 == 0
        )
            E_font_I_print_u( '\'' );
    }
}
/******************************************************************************/
