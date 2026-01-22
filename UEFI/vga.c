/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   kernel
*         VGA driver
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒16 K
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
//==============================================================================
N32
E_vga_Z_color_M(
  N8 red
, N8 green
, N8 blue
){  return ( (N32)blue << E_main_S_kernel_args.framebuffer.pixel_shifts.blue )
    | ( (N32)green << E_main_S_kernel_args.framebuffer.pixel_shifts.green )
    | ( (N32)red << E_main_S_kernel_args.framebuffer.pixel_shifts.red );
}
N8
E_vga_Z_color_R_red( N32 color
){  return ( color >> E_main_S_kernel_args.framebuffer.pixel_shifts.red ) & 0xff;
}
N8
E_vga_Z_color_R_green( N32 color
){  return ( color >> E_main_S_kernel_args.framebuffer.pixel_shifts.green ) & 0xff;
}
N8
E_vga_Z_color_R_blue( N32 color
){  return ( color >> E_main_S_kernel_args.framebuffer.pixel_shifts.blue ) & 0xff;
}
N32
E_vga_Z_color_M_gray( N8 luminance
){  return E_vga_Z_color_M( luminance, luminance, luminance );
}
N32
E_vga_R_video_color( N32 color
){  return ((( color >> 16 ) & 0xff ) << E_main_S_kernel_args.framebuffer.pixel_shifts.blue )
    | ((( color >> 8 ) & 0xff ) << E_main_S_kernel_args.framebuffer.pixel_shifts.green )
    | (( color & 0xff ) << E_main_S_kernel_args.framebuffer.pixel_shifts.red );
}
N32
E_vga_R_color( N32 video_color
){  return ((( video_color >> E_main_S_kernel_args.framebuffer.pixel_shifts.blue ) & 0xff ) << 16 )
    | ((( video_color >> E_main_S_kernel_args.framebuffer.pixel_shifts.green ) & 0xff ) << 8 )
    | (( video_color >> E_main_S_kernel_args.framebuffer.pixel_shifts.red ) & 0xff );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N32
E_vga_R_pixel(
  N32 x
, N32 y
){  volatile N32 *video_address = E_main_S_kernel_args.framebuffer.p + E_main_S_kernel_args.framebuffer.pixels_per_scan_line * y + x;
    return *video_address;
}
void
E_vga_P_pixel(
  N32 x
, N32 y
, N32 video_color
){  volatile N32 *video_address = E_main_S_kernel_args.framebuffer.p + E_main_S_kernel_args.framebuffer.pixels_per_scan_line * y + x;
    *video_address = video_color;
}
void
E_vga_I_set_pixel_aa(
  N32 x
, N32 y
, N32 color
, F brightness
, N get_pixel
){  N background_red, background_green, background_blue;
    N n = 0;
    N p[8];
    p[0] = ( get_pixel & E_vga_Z_aa_pixel_S_e ) && x + 1 < E_main_S_kernel_args.framebuffer.width ? E_vga_R_color( E_vga_R_pixel( x + 1, y )) : color;
    p[1] = ( get_pixel & E_vga_Z_aa_pixel_S_se ) && x + 1 < E_main_S_kernel_args.framebuffer.width && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_color( E_vga_R_pixel( x + 1, y + 1 )) : color;
    p[2] = ( get_pixel & E_vga_Z_aa_pixel_S_s ) && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_color( E_vga_R_pixel( x, y + 1 )) : color;
    p[3] = ( get_pixel & E_vga_Z_aa_pixel_S_sw ) && x > 0 && y + 1 < E_main_S_kernel_args.framebuffer.height ? E_vga_R_color( E_vga_R_pixel( x - 1, y + 1 )) : color;
    p[4] = ( get_pixel & E_vga_Z_aa_pixel_S_w ) && x > 0 ? E_vga_R_color( E_vga_R_pixel( x - 1, y )) : color;
    p[5] = ( get_pixel & E_vga_Z_aa_pixel_S_nw ) && x > 0 && y > 0 ? E_vga_R_color( E_vga_R_pixel( x - 1, y - 1 )) : color;
    p[6] = ( get_pixel & E_vga_Z_aa_pixel_S_n ) && y > 0 ? E_vga_R_color( E_vga_R_pixel( x, y - 1 )) : color;
    p[7] = ( get_pixel & E_vga_Z_aa_pixel_S_ne ) && x + 1 < E_main_S_kernel_args.framebuffer.width && y > 0 ? E_vga_R_color( E_vga_R_pixel( x + 1, y - 1 )) : color;
    background_red = background_green = background_blue = 0;
    for_n( i, 8 )
        if( p[i] != color )
        {   background_red += E_vga_Z_color_R_red( p[i] );
            background_green += E_vga_Z_color_R_green( p[i] );
            background_blue += E_vga_Z_color_R_red( p[i] );
            n++;
        }
    if(n)
    {   background_red /= n;
        background_green /= n;
        background_blue /= n;
    }else
    {   background_red = E_vga_Z_color_R_red( E_vga_S_background_color );
        background_green = E_vga_Z_color_R_green( E_vga_S_background_color );
        background_blue = E_vga_Z_color_R_blue( E_vga_S_background_color );
    }
    N red = E_vga_Z_color_R_red(color);
    N green = E_vga_Z_color_R_green(color);
    N blue = E_vga_Z_color_R_blue(color);
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
    E_vga_P_pixel( x, y, E_vga_R_video_color( E_vga_Z_color_M( red, green, blue )));
}
void
E_vga_I_draw_rect(
  N32 x
, N32 y
, N32 width
, N32 height
, N32 video_color
){  volatile N32 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line + x;
    for_n( x_i, width )
        video_address[ x_i ] = video_color;
    video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line;
    for_n( y_i, height - 2 )
    {   video_address[0] = video_color;
        video_address[ width - 1 ] = video_color;
        video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line;
    }
    for_n_( x_i, width )
        video_address[ x_i ] = video_color;
}
void
E_vga_I_fill_rect(
  N32 x
, N32 y
, N32 width
, N32 height
, N32 video_color
){  volatile N32 *video_address = E_main_S_kernel_args.framebuffer.p + y * E_main_S_kernel_args.framebuffer.pixels_per_scan_line + x;
    for_n( y_i, height )
    {   for_n( x_i, width )
            video_address[ x_i ] = video_color;
        video_address += E_main_S_kernel_args.framebuffer.pixels_per_scan_line;
    }
}
/******************************************************************************/
