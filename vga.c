/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   file boot loader
*         VGA driver
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒16 K
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
struct E_main_Z_video *video;
//==============================================================================
N
E_vga_Z_color_M(
  N8 red
, N8 green
, N8 blue
){  return ( red << 16 ) | ( green << 8 ) | blue;
}
N8
E_vga_Z_color_R_red( N32 color
){  return ( color >> 16 ) & 0xff;
}
N8
E_vga_Z_color_R_green( N32 color
){  return ( color >> 8 ) & 0xff;
}
N8
E_vga_Z_color_R_blue( N32 color
){  return color & 0xff;
}
N
E_x_Z_color_M_gray( N8 luminance
){  return E_vga_Z_color_M( luminance, luminance, luminance );
}
N
E_vga_R_video_color( N color
){  N video_color;
    if( video->bpp == 16
    || video->bpp == 15
    )
        video_color = ((( color >> 16 ) * (( 1 << video->red_size ) - 1 ) / 255 ) << video->red_start )
        | (((( color >> 8 ) & 0xff ) * (( 1 << video->green_size ) - 1 ) / 255 ) << video->green_start )
        | ((( color & 0xff ) * (( 1 << video->blue_size ) - 1 ) / 255 ) << video->blue_start );
    else
        video_color = (( color >> 16 ) << video->red_start )
        | ((( color >> 8 ) & 0xff ) << video->green_start )
        | (( color & 0xff ) << video->blue_start );
    return video_color;
}
N
E_vga_R_color( N video_color
){  if( video->bpp == 16
    || video->bpp == 15
    )
        return (((( video_color >> video->red_start ) & (( 1 << video->red_size ) - 1 )) * 255 / (( 1 << video->red_size ) - 1 )) << 16 )
        | (((( video_color >> video->green_start ) & (( 1 << video->green_size ) - 1 )) * 255 / (( 1 << video->green_size ) - 1 )) << 8 )
        | ((( video_color >> video->blue_start ) & (( 1 << video->blue_size ) - 1 )) * 255 / (( 1 << video->blue_size ) - 1 ));
    return ((( video_color >> video->red_start ) & 0xff ) << 16 )
    | ((( video_color >> video->green_start ) & 0xff ) << 8 )
    | (( video_color >> video->blue_start ) & 0xff );
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_vga_R_pixel(
  N x
, N y
){  Pc video_address = (Pc)(N)video->framebuffer + video->line_width * y;
    if( video->bpp == 16
    || video->bpp == 15
    )
        return (( N16 * )video_address)[x];
    return (( N32 * )video_address)[x];
}
void
E_vga_P_pixel(
  N x
, N y
, N video_color
){  Pc video_address = (Pc)(N)video->framebuffer + video->line_width * y;
    if( video->bpp == 16
    || video->bpp == 15
    )
        (( N16 * )video_address)[x] = video_color;
    else
        (( N32 * )video_address)[x] = video_color;
}
void
E_vga_I_set_pixel_aa(
  N x
, N y
, N color
, F brightness
, N get_pixel
){  N background_red, background_green, background_blue;
    N n = 0;
    N p[8];
    p[0] = ( get_pixel & E_x_Z_aa_pixel_S_e ) && x + 1 < video->width ? E_vga_R_color( E_vga_R_pixel( x + 1, y )) : color;
    p[1] = ( get_pixel & E_x_Z_aa_pixel_S_se ) && x + 1 < video->width && y + 1 < video->height ? E_vga_R_color( E_vga_R_pixel( x + 1, y + 1 )) : color;
    p[2] = ( get_pixel & E_x_Z_aa_pixel_S_s ) && y + 1 < video->height ? E_vga_R_color( E_vga_R_pixel( x, y + 1 )) : color;
    p[3] = ( get_pixel & E_x_Z_aa_pixel_S_sw ) && x > 0 && y + 1 < video->height ? E_vga_R_color( E_vga_R_pixel( x - 1, y + 1 )) : color;
    p[4] = ( get_pixel & E_x_Z_aa_pixel_S_w ) && x > 0 ? E_vga_R_color( E_vga_R_pixel( x - 1, y )) : color;
    p[5] = ( get_pixel & E_x_Z_aa_pixel_S_nw ) && x > 0 && y > 0 ? E_vga_R_color( E_vga_R_pixel( x - 1, y - 1 )) : color;
    p[6] = ( get_pixel & E_x_Z_aa_pixel_S_n ) && y > 0 ? E_vga_R_color( E_vga_R_pixel( x, y - 1 )) : color;
    p[7] = ( get_pixel & E_x_Z_aa_pixel_S_ne ) && x + 1 < video->width && y > 0 ? E_vga_R_color( E_vga_R_pixel( x + 1, y - 1 )) : color;
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
  N x
, N y
, N width
, N height
, N video_color
){  Pc video_address = (Pc)(N)video->framebuffer + video->line_width * y;
    if( video->bpp == 16
    || video->bpp == 15
    )
    {   video_address = (Pc)&(( N16 * )video_address)[x];
        for_n( x_i, width )
            (( N16 * )video_address)[ x_i ] = video_color;
        video_address += video->line_width;
        for_n( y_i, height - 2 )
        {   (( N16 * )video_address)[0] = video_color;
            (( N16 * )video_address)[ width - 1 ] = video_color;
            video_address += video->line_width;
        }
        for_n_( x_i, width )
            (( N16 * )video_address)[ x_i ] = video_color;
    }else
    {   video_address = (Pc)&(( N16 * )video_address)[x];
        for_n( x_i, width )
            (( N32 * )video_address)[ x_i ] = video_color;
        video_address += video->line_width;
        for_n( y_i, height - 2 )
        {   (( N32 * )video_address)[0] = video_color;
            (( N32 * )video_address)[ width - 1 ] = video_color;
            video_address += video->line_width;
        }
        for_n_( x_i, width )
            (( N32 * )video_address)[ x_i ] = video_color;
    }
}
void
E_vga_I_fill_rect(
  N x
, N y
, N width
, N height
, N video_color
){  Pc video_address = (Pc)(N)video->framebuffer + video->line_width * y;
    if( video->bpp == 16
    || video->bpp == 15
    )
    {   video_address = (Pc)&(( N16 * )video_address)[x];
        for_n( y_i, height )
        {   for_n( x_i, width )
                (( N16 * )video_address)[ x_i ] = video_color;
            video_address += video->line_width;
        }
    }else
    {   video_address = (Pc)&(( N32 * )video_address)[x];
        for_n( y_i, height )
        {   for_n( x_i, width )
                (( N32 * )video_address)[ x_i ] = video_color;
            video_address += video->line_width;
        }
    }
}
/******************************************************************************/
