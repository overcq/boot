/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         E_font_S
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
}E_font_S;
N32 E_font_S_x, E_font_S_y;
N32 E_font_S_color = E_vga_S_text_color;
N8 E_font_S_size = 1;
N8 E_font_S_thickness = 1;
//==============================================================================
N
E_font_M( void
){  E_font_S_x = E_font_S_thickness + 1;
    E_font_S_y = E_font_S_size + 1;
    E_font_S.height = 8;
    E_font_S.default_i = 63;
    E_font_S.bitmap_n = 177;
    Mt_( E_font_S.bitmap, E_font_S.bitmap_n );
    Kp( E_font_S.bitmap )
        return ~0;
    for_n( i, E_font_S.bitmap_n )
    {   N8 *bitmap;
        switch(i)
        { case 0:
            {   E_font_S.bitmap[i].u = ' ';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 1:
            {   E_font_S.bitmap[i].u = '!';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 2:
            {   E_font_S.bitmap[i].u = '\"';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 3:
            {   E_font_S.bitmap[i].u = '#';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 4:
            {   E_font_S.bitmap[i].u = '$';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 5:
            {   E_font_S.bitmap[i].u = '%';
                E_font_S.bitmap[i].width = 4;
                N8 *bitmap_ = ( N8 [] )
                { 1, 2, 1, 0
                , 2, 0, 1, 3
                , 1, 1, 3, 0
                , 0, 3, 1, 1
                , 3, 1, 0, 2
                , 0, 1, 2, 1
                , 0, 0, 0, 0
                , 0, 0, 0, 0
                };
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 6:
            {   E_font_S.bitmap[i].u = '&';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 7:
            {   E_font_S.bitmap[i].u = '\'';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 8:
            {   E_font_S.bitmap[i].u = '(';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 9:
            {   E_font_S.bitmap[i].u = ')';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 10:
            {   E_font_S.bitmap[i].u = '*';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 11:
            {   E_font_S.bitmap[i].u = '+';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 12:
            {   E_font_S.bitmap[i].u = ',';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 13:
            {   E_font_S.bitmap[i].u = '-';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 14:
            {   E_font_S.bitmap[i].u = '.';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 15:
            {   E_font_S.bitmap[i].u = '/';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 16:
            {   E_font_S.bitmap[i].u = '0';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 17:
            {   E_font_S.bitmap[i].u = '1';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 18:
            {   E_font_S.bitmap[i].u = '2';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 19:
            {   E_font_S.bitmap[i].u = '3';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 20:
            {   E_font_S.bitmap[i].u = '4';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 21:
            {   E_font_S.bitmap[i].u = '5';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 22:
            {   E_font_S.bitmap[i].u = '6';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 23:
            {   E_font_S.bitmap[i].u = '7';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 24:
            {   E_font_S.bitmap[i].u = '8';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 25:
            {   E_font_S.bitmap[i].u = '9';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 26:
            {   E_font_S.bitmap[i].u = ':';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 27:
            {   E_font_S.bitmap[i].u = ';';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 28:
            {   E_font_S.bitmap[i].u = '<';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 29:
            {   E_font_S.bitmap[i].u = '=';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 30:
            {   E_font_S.bitmap[i].u = '>';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 31:
            {   E_font_S.bitmap[i].u = '?';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 32:
            {   E_font_S.bitmap[i].u = '@';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 33:
            {   E_font_S.bitmap[i].u = 'A';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 34:
            {   E_font_S.bitmap[i].u = 'B';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 35:
            {   E_font_S.bitmap[i].u = 'C';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 36:
            {   E_font_S.bitmap[i].u = 'D';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 37:
            {   E_font_S.bitmap[i].u = 'E';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 38:
            {   E_font_S.bitmap[i].u = 'F';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 39:
            {   E_font_S.bitmap[i].u = 'G';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 40:
            {   E_font_S.bitmap[i].u = 'H';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 41:
            {   E_font_S.bitmap[i].u = 'I';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 42:
            {   E_font_S.bitmap[i].u = 'J';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 43:
            {   E_font_S.bitmap[i].u = 'K';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 44:
            {   E_font_S.bitmap[i].u = 'L';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 45:
            {   E_font_S.bitmap[i].u = 'M';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 46:
            {   E_font_S.bitmap[i].u = 'N';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 47:
            {   E_font_S.bitmap[i].u = 'O';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 48:
            {   E_font_S.bitmap[i].u = 'P';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 49:
            {   E_font_S.bitmap[i].u = 'Q';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 50:
            {   E_font_S.bitmap[i].u = 'R';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 51:
            {   E_font_S.bitmap[i].u = 'S';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 52:
            {   E_font_S.bitmap[i].u = 'T';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 53:
            {   E_font_S.bitmap[i].u = 'U';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 54:
            {   E_font_S.bitmap[i].u = 'V';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 55:
            {   E_font_S.bitmap[i].u = 'W';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 56:
            {   E_font_S.bitmap[i].u = 'X';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 57:
            {   E_font_S.bitmap[i].u = 'Y';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 58:
            {   E_font_S.bitmap[i].u = 'Z';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 59:
            {   E_font_S.bitmap[i].u = '[';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 60:
            {   E_font_S.bitmap[i].u = '\\';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 61:
            {   E_font_S.bitmap[i].u = ']';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 62:
            {   E_font_S.bitmap[i].u = '^';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 63:
            {   E_font_S.bitmap[i].u = '_';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 64:
            {   E_font_S.bitmap[i].u = '`';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 65:
            {   E_font_S.bitmap[i].u = 'a';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 66:
            {   E_font_S.bitmap[i].u = 'b';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 67:
            {   E_font_S.bitmap[i].u = 'c';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 68:
            {   E_font_S.bitmap[i].u = 'd';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 69:
            {   E_font_S.bitmap[i].u = 'e';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 70:
            {   E_font_S.bitmap[i].u = 'f';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 71:
            {   E_font_S.bitmap[i].u = 'g';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 72:
            {   E_font_S.bitmap[i].u = 'h';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 73:
            {   E_font_S.bitmap[i].u = 'i';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 74:
            {   E_font_S.bitmap[i].u = 'j';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 75:
            {   E_font_S.bitmap[i].u = 'k';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 76:
            {   E_font_S.bitmap[i].u = 'l';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 77:
            {   E_font_S.bitmap[i].u = 'm';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 78:
            {   E_font_S.bitmap[i].u = 'n';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 79:
            {   E_font_S.bitmap[i].u = 'o';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 80:
            {   E_font_S.bitmap[i].u = 'p';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 81:
            {   E_font_S.bitmap[i].u = 'q';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 82:
            {   E_font_S.bitmap[i].u = 'r';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 83:
            {   E_font_S.bitmap[i].u = 's';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 84:
            {   E_font_S.bitmap[i].u = 't';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 85:
            {   E_font_S.bitmap[i].u = 'u';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 86:
            {   E_font_S.bitmap[i].u = 'v';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 87:
            {   E_font_S.bitmap[i].u = 'w';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 88:
            {   E_font_S.bitmap[i].u = 'x';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 89:
            {   E_font_S.bitmap[i].u = 'y';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 90:
            {   E_font_S.bitmap[i].u = 'z';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 91:
            {   E_font_S.bitmap[i].u = '{';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 92:
            {   E_font_S.bitmap[i].u = '|';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 93:
            {   E_font_S.bitmap[i].u = '}';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 94:
            {   E_font_S.bitmap[i].u = '~';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 95:
            {   E_font_S.bitmap[i].u = L'¡';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 96:
            {   E_font_S.bitmap[i].u = L'¢';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 97:
            {   E_font_S.bitmap[i].u = L'¦';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 98:
            {   E_font_S.bitmap[i].u = L'©';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 99:
            {   E_font_S.bitmap[i].u = L'«';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 100:
            {   E_font_S.bitmap[i].u = L'®';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 101:
            {   E_font_S.bitmap[i].u = L'°';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 102:
            {   E_font_S.bitmap[i].u = L'±';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 103:
            {   E_font_S.bitmap[i].u = L'²';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 104:
            {   E_font_S.bitmap[i].u = L'³';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 105:
            {   E_font_S.bitmap[i].u = L'µ';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 106:
            {   E_font_S.bitmap[i].u = L'·';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 107:
            {   E_font_S.bitmap[i].u = L'»';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 108:
            {   E_font_S.bitmap[i].u = L'¿';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 109:
            {   E_font_S.bitmap[i].u = L'Ó';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 110:
            {   E_font_S.bitmap[i].u = L'×';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 111:
            {   E_font_S.bitmap[i].u = L'ó';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 112:
            {   E_font_S.bitmap[i].u = L'÷';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 113:
            {   E_font_S.bitmap[i].u = L'Ą';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 114:
            {   E_font_S.bitmap[i].u = L'ą';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 115:
            {   E_font_S.bitmap[i].u = L'Ć';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 116:
            {   E_font_S.bitmap[i].u = L'ć';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 117:
            {   E_font_S.bitmap[i].u = L'Ę';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 118:
            {   E_font_S.bitmap[i].u = L'ę';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 119:
            {   E_font_S.bitmap[i].u = L'Ł';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 120:
            {   E_font_S.bitmap[i].u = L'ł';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 121:
            {   E_font_S.bitmap[i].u = L'Ń';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 122:
            {   E_font_S.bitmap[i].u = L'ń';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 123:
            {   E_font_S.bitmap[i].u = L'Ś';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 124:
            {   E_font_S.bitmap[i].u = L'ś';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 125:
            {   E_font_S.bitmap[i].u = L'Ź';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 126:
            {   E_font_S.bitmap[i].u = L'ź';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 127:
            {   E_font_S.bitmap[i].u = L'Ż';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 128:
            {   E_font_S.bitmap[i].u = L'ż';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 129:
            {   E_font_S.bitmap[i].u = L'՚';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 130:
            {   E_font_S.bitmap[i].u = L' ';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 131:
            {   E_font_S.bitmap[i].u = L' ';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 132:
            {   E_font_S.bitmap[i].u = L'‐';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 133:
            {   E_font_S.bitmap[i].u = L'‒';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 134:
            {   E_font_S.bitmap[i].u = L'–';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 135:
            {   E_font_S.bitmap[i].u = L'—';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 136:
            {   E_font_S.bitmap[i].u = L'―';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 137:
            {   E_font_S.bitmap[i].u = L'‘';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 138:
            {   E_font_S.bitmap[i].u = L'’';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 139:
            {   E_font_S.bitmap[i].u = L'‚';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 140:
            {   E_font_S.bitmap[i].u = L'‛';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 141:
            {   E_font_S.bitmap[i].u = L'“';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 142:
            {   E_font_S.bitmap[i].u = L'”';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 143:
            {   E_font_S.bitmap[i].u = L'„';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 144:
            {   E_font_S.bitmap[i].u = L'‟';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 145:
            {   E_font_S.bitmap[i].u = L'•';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 146:
            {   E_font_S.bitmap[i].u = L'‣';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 147:
            {   E_font_S.bitmap[i].u = L'…';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 148:
            {   E_font_S.bitmap[i].u = L'′';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 149:
            {   E_font_S.bitmap[i].u = L'‹';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 150:
            {   E_font_S.bitmap[i].u = L'›';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 151:
            {   E_font_S.bitmap[i].u = L'‽';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 152:
            {   E_font_S.bitmap[i].u = L'⁂';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 153:
            {   E_font_S.bitmap[i].u = L'⁃';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 154:
            {   E_font_S.bitmap[i].u = L'⁄';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 155:
            {   E_font_S.bitmap[i].u = L'⁅';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 156:
            {   E_font_S.bitmap[i].u = L'⁆';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 157:
            {   E_font_S.bitmap[i].u = L'⁋';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 158:
            {   E_font_S.bitmap[i].u = L'⁑';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 159:
            {   E_font_S.bitmap[i].u = L' ';
                E_font_S.bitmap[i].width = 1;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 160:
            {   E_font_S.bitmap[i].u = L'€';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 161:
            {   E_font_S.bitmap[i].u = L'№';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 162:
            {   E_font_S.bitmap[i].u = L'℠';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 163:
            {   E_font_S.bitmap[i].u = L'™';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 164:
            {   E_font_S.bitmap[i].u = L'⇢';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 165:
            {   E_font_S.bitmap[i].u = L'−';
                E_font_S.bitmap[i].width = 3;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 166:
            {   E_font_S.bitmap[i].u = L'≈';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 167:
            {   E_font_S.bitmap[i].u = L'〈';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 168:
            {   E_font_S.bitmap[i].u = L'〉';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 169:
            {   E_font_S.bitmap[i].u = L'⏎';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 170:
            {   E_font_S.bitmap[i].u = L'✓';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 171:
            {   E_font_S.bitmap[i].u = L'❛';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 172:
            {   E_font_S.bitmap[i].u = L'❜';
                E_font_S.bitmap[i].width = 2;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 173:
            {   E_font_S.bitmap[i].u = L'❝';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 174:
            {   E_font_S.bitmap[i].u = L'❞';
                E_font_S.bitmap[i].width = 4;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 175:
            {   E_font_S.bitmap[i].u = L'➪';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
          case 176:
            {   E_font_S.bitmap[i].u = L'〃';
                E_font_S.bitmap[i].width = 5;
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
                bitmap = M( E_font_S.bitmap[i].width * E_font_S.height );
                Kp_( ~2, bitmap );
                E_mem_Q_blk_I_copy( bitmap, bitmap_, E_font_S.bitmap[i].width * E_font_S.height );
                break;
            }
        }
        Mt_( E_font_S.bitmap[i].bitmap, E_font_S.bitmap[i].width * E_font_S.height / 4 + ( E_font_S.bitmap[i].width * E_font_S.height % 4 ? 1 : 0 ));
        Kp_( ~2, E_font_S.bitmap[i].bitmap );
        N8 c;
        for_n( j, E_font_S.bitmap[i].width * E_font_S.height )
        {   if( j % 4 == 0 )
                c = 0;
            c |= bitmap[j] << ( j % 4 * 2 );
            if( j % 4 == 3 )
                E_font_S.bitmap[i].bitmap[ j / 4 ] = c;
        }
        if( j % 4 == 1 || j % 4 == 2 || j % 4 == 3 )
            E_font_S.bitmap[i].bitmap[ j / 4 + 1 ] = c;
        K_( ~2, W(bitmap) );
    }
    return 0;
}
N
E_font_W( void
){  for_n( i, E_font_S.bitmap_n )
    {   K_( ~2, W( E_font_S.bitmap[i].bitmap ));
    }
    K_( ~1, W( E_font_S.bitmap ));
    return 0;
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
    N32 max = E_font_S.bitmap_n - 1;
    N32 i = max / 2;
    O{  if( E_font_S.bitmap[i].u == u )
        {   N j = 0;
            C c;
            N32 x_, y_ = y;
            for_n( font_y, E_font_S.height )
            {   x_ = x;
                for_n( font_x, E_font_S.bitmap[i].width )
                {   if( j % 4 == 0 )
                        c = E_font_S.bitmap[i].bitmap[ j / 4 ];
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
            return thickness * E_font_S.bitmap[i].width;
        }
        if( E_font_S.bitmap[i].u > u )
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
){  for_n( i, dy - 1 )
        E_mem_Q_blk_I_copy( (P)( E_main_S_kernel_args.framebuffer.p + i * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 )
        , (P)( E_main_S_kernel_args.framebuffer.p + ( i + 1 ) * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 )
        , E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8
        );
    for_n_( i, E_main_S_kernel_args.framebuffer.height - dy - 1 )
        E_mem_Q_blk_I_copy( (P)( E_main_S_kernel_args.framebuffer.p + ( dy + i ) * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 )
        , (P)( E_main_S_kernel_args.framebuffer.p + ( dy + i + 1 ) * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 )
        , E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8
        );
    E_vga_I_fill_rect( 0, E_main_S_kernel_args.framebuffer.height - dy, E_main_S_kernel_args.framebuffer.width, dy, E_vga_R_video_color( E_vga_S_background_color ));
}
void
E_font_I_print_nl( void
){  E_font_S_x = E_font_S_size + 1;
    if( E_font_S_y + ( E_font_S_size + 1 ) * E_font_S.height + E_font_S_size + 1 > E_main_S_kernel_args.framebuffer.height )
        E_font_I_scroll_fwd(( E_font_S_size + 1 ) * E_font_S.height + E_font_S_size + 1 );
    else
        E_font_S_y += ( E_font_S_size + 1 ) * E_font_S.height + E_font_S_size + 1;
}
void
E_font_I_print_u( U u
){  if( u == '\n' )
    {   E_font_I_print_nl();
        return;
    }
    N dx;
    N32 min = 0;
    N32 max = E_font_S.bitmap_n - 1;
    N32 i = max / 2;
    O{  if( E_font_S.bitmap[i].u == u )
            break;
        if( E_font_S.bitmap[i].u > u )
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
    if( E_font_S.bitmap[i].u == u )
        dx = E_font_S.bitmap[i].width;
    else
    {   u = E_font_S.bitmap[ E_font_S.default_i ].u;
        dx = E_font_S.bitmap[ E_font_S.default_i ].width;
    }
    dx *= E_font_S_thickness + 1;
    if( E_font_S_x + dx + E_font_S_thickness + 1 > E_main_S_kernel_args.framebuffer.width )
        E_font_I_print_nl();
    E_font_I_draw( u, E_font_S_x, E_font_S_y, E_vga_R_video_color( E_font_S_color ), E_font_S_size, E_font_S_thickness );
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
    "\n" "mfence"
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
