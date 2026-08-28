/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         VGA driver
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒16 K
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
//==============================================================================
N32
E_vga_Z_video_color_M(
  N8 red
, N8 green
, N8 blue
){  return ( (N32)blue << E_main_S_kernel_args.framebuffer.blue_shift )
    | ( (N32)green << E_main_S_kernel_args.framebuffer.green_shift )
    | ( (N32)red << E_main_S_kernel_args.framebuffer.red_shift );
}
N8
E_vga_Z_video_color_R_red( N32 video_color
){  return ( video_color >> E_main_S_kernel_args.framebuffer.red_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.red_size ) - 1 );
}
N8
E_vga_Z_video_color_R_green( N32 video_color
){  return ( video_color >> E_main_S_kernel_args.framebuffer.green_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.green_size ) - 1 );
}
N8
E_vga_Z_video_color_R_blue( N32 video_color
){  return ( video_color >> E_main_S_kernel_args.framebuffer.blue_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.blue_size ) - 1 );
}
N32
E_vga_Z_video_color_M_gray( N8 luminance
){  return E_vga_Z_video_color_M( luminance, luminance, luminance );
}
N32
E_vga_R_video_color( N32 color
){  return ((( color & 0xff ) * (( 1 << E_main_S_kernel_args.framebuffer.blue_size ) - 1 ) / 255 ) << E_main_S_kernel_args.framebuffer.blue_shift )
    | (((( color >> 8 ) & 0xff ) * (( 1 << E_main_S_kernel_args.framebuffer.green_size ) - 1 ) / 255 ) << E_main_S_kernel_args.framebuffer.green_shift )
    | (((( color >> 16 ) & 0xff ) * (( 1 << E_main_S_kernel_args.framebuffer.red_size ) - 1 ) / 255 ) << E_main_S_kernel_args.framebuffer.red_shift );
}
N32
E_vga_R_color( N32 video_color
){  return ((( video_color >> E_main_S_kernel_args.framebuffer.blue_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.blue_size ) - 1 )) * 255 / (( 1 << E_main_S_kernel_args.framebuffer.blue_size ) - 1 ))
    | (((( video_color >> E_main_S_kernel_args.framebuffer.green_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.green_size ) - 1 )) << 8 ) * 255 / (( 1 << E_main_S_kernel_args.framebuffer.green_size ) - 1 ))
    | (((( video_color >> E_main_S_kernel_args.framebuffer.red_shift ) & (( 1 << E_main_S_kernel_args.framebuffer.red_size ) - 1 )) << 16 ) * 255 / (( 1 << E_main_S_kernel_args.framebuffer.red_size ) - 1 ));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N32
E_vga_R_pixel(
  N32 x
, N32 y
){  volatile N8 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 + x * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
    return E_main_S_kernel_args.framebuffer.bits_per_pixel == 16 ? *( volatile N16 * )video_address : *( volatile N32 * )video_address;
}
void
E_vga_P_pixel(
  N32 x
, N32 y
, N32 video_color
){  volatile N8 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 + x * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
    if( E_main_S_kernel_args.framebuffer.bits_per_pixel == 16 )
        *( volatile N16 * )video_address = video_color;
    else
        *( volatile N32 * )video_address = video_color;
}
void
E_vga_I_set_pixel_aa(
  N32 x
, N32 y
, N32 video_color
, F brightness
, N get_pixel
){  N background_red, background_green, background_blue;
    N n = 0;
    N p[8];
    p[0] = ( get_pixel & E_vga_Z_aa_pixel_S_e ) && x + 1 < E_main_S_kernel_args.framebuffer.width ? E_vga_R_pixel( x + 1, y ) : video_color;
    p[1] = ( get_pixel & E_vga_Z_aa_pixel_S_se ) && x + 1 < E_main_S_kernel_args.framebuffer.width && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_pixel( x + 1, y + 1 ) : video_color;
    p[2] = ( get_pixel & E_vga_Z_aa_pixel_S_s ) && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_pixel( x, y + 1 ) : video_color;
    p[3] = ( get_pixel & E_vga_Z_aa_pixel_S_sw ) && x > 0 && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_pixel( x - 1, y + 1 ) : video_color;
    p[4] = ( get_pixel & E_vga_Z_aa_pixel_S_w ) && x > 0 ? E_vga_R_pixel( x - 1, y ) : video_color;
    p[5] = ( get_pixel & E_vga_Z_aa_pixel_S_nw ) && x > 0 && y > 0 ? E_vga_R_pixel( x - 1, y - 1 ) : video_color;
    p[6] = ( get_pixel & E_vga_Z_aa_pixel_S_n ) && y > 0 ? E_vga_R_pixel( x, y - 1 ) : video_color;
    p[7] = ( get_pixel & E_vga_Z_aa_pixel_S_ne ) && x + 1 < E_main_S_kernel_args.framebuffer.width && y > 0 ? E_vga_R_pixel( x + 1, y - 1 ) : video_color;
    background_red = background_green = background_blue = 0;
    for_n( i, 8 )
        if( p[i] != video_color )
        {   background_red += E_vga_Z_video_color_R_red( p[i] );
            background_green += E_vga_Z_video_color_R_green( p[i] );
            background_blue += E_vga_Z_video_color_R_red( p[i] );
            n++;
        }
    if(n)
    {   background_red /= n;
        background_green /= n;
        background_blue /= n;
    }else
    {   background_red = E_vga_Z_video_color_R_red( E_vga_S_background_color );
        background_green = E_vga_Z_video_color_R_green( E_vga_S_background_color );
        background_blue = E_vga_Z_video_color_R_blue( E_vga_S_background_color );
    }
    N red = E_vga_Z_video_color_R_red( video_color );
    N green = E_vga_Z_video_color_R_green( video_color );
    N blue = E_vga_Z_video_color_R_blue( video_color );
    if( red > background_red )
        red = background_red + ( red - background_red ) * brightness;
    else
        red = background_red - ( background_red - red ) * brightness;
    if( green > background_green )
        green = background_green + ( green - background_green ) * brightness;
    else
        green = background_green - ( background_green - green ) * brightness;
    if( blue > background_blue )
        blue = background_blue + ( blue - background_blue ) * brightness;
    else
        blue = background_blue - ( background_blue - blue ) * brightness;
    E_vga_P_pixel( x, y, E_vga_Z_video_color_M( red, green, blue ));
}
void
E_vga_I_draw_rect(
  N32 x
, N32 y
, N32 width
, N32 height
, N32 video_color
){  volatile N8 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 + x * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
    if( E_main_S_kernel_args.framebuffer.bits_per_pixel == 16 )
    {   for_n( x_i, width )
            (( volatile N16 * )video_address )[ x_i ] = video_color;
        video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        for_n( y_i, height - 2 )
        {   (( volatile N16 * )video_address )[0] = video_color;
            (( volatile N16 * )video_address )[ width - 1 ] = video_color;
            video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        }
        for_n_( x_i, width )
            (( volatile N16 * )video_address )[ x_i ] = video_color;
    }else
    {   for_n( x_i, width )
            (( volatile N32 * )video_address )[ x_i ] = video_color;
        video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        for_n( y_i, height - 2 )
        {   (( volatile N32 * )video_address )[0] = video_color;
            (( volatile N32 * )video_address )[ width - 1 ] = video_color;
            video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        }
        for_n_( x_i, width )
            (( volatile N32 * )video_address )[ x_i ] = video_color;
    }
}
void
E_vga_I_fill_rect(
  N32 x
, N32 y
, N32 width
, N32 height
, N32 video_color
){  volatile N8 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 + x * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
    if( E_main_S_kernel_args.framebuffer.bits_per_pixel == 16 )
    {   for_n( y_i, height )
        {   for_n( x_i, width )
                (( volatile N16 * )video_address )[ x_i ] = video_color;
            video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        }
    }else
    {   for_n( y_i, height )
        {   for_n( x_i, width )
                (( volatile N32 * )video_address )[ x_i ] = video_color;
            video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8;
        }
    }
}
/******************************************************************************/
