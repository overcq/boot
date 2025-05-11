/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS file boot loader
*         font
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒5‒16 L
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
struct
{ N height;
  N bitmap_n;
  struct E_font_Q_font_Z_bitmap *bitmap;
}font;
struct E_font_Q_font_Z_bitmap
{ U u;
  N width;
  N8 *bitmap;
};
N E_font_S_x = 1;
N E_font_S_y = 1;
N E_font_S_color = E_vga_S_text_color;
//==============================================================================
N
E_font_M( void
){  font.height = 8;
    font.bitmap_n = 177;
    Mt_( font.bitmap, font.bitmap_n );
    if( !font.bitmap )
        return ~0;
    for_n( i, font.bitmap_n )
    {   N8 *bitmap;
        switch(i)
        { case 0:
              font.bitmap[i].u = '_';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 1:
              font.bitmap[i].u = 'A';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 3, 3, 0
              , 3, 1, 1, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 2:
              font.bitmap[i].u = L'Ą';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 3, 3, 0
              , 3, 1, 1, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 0, 0, 1, 2
              , 0, 0, 2, 1
              })[0];
              break;
          case 3:
              font.bitmap[i].u = 'B';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 4:
              font.bitmap[i].u = 'C';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 1, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 5:
              font.bitmap[i].u = L'Ć';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 3
              , 3, 0, 2, 0
              , 3, 0, 2, 0
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 1, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 6:
              font.bitmap[i].u = 'D';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 7:
              font.bitmap[i].u = 'E';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 8:
              font.bitmap[i].u = L'Ę';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 1, 2
              , 0, 0, 2, 0
              })[0];
              break;
          case 9:
              font.bitmap[i].u = 'F';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 10:
              font.bitmap[i].u = 'G';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 3
              , 3, 0, 0, 0
              , 3, 0, 3, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 2
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 11:
              font.bitmap[i].u = 'H';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 12:
              font.bitmap[i].u = 'I';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 3
              , 3
              , 3
              , 3
              , 3
              , 0
              , 0
              })[0];
              break;
          case 13:
              font.bitmap[i].u = 'J';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 3
              , 0, 0, 3
              , 0, 0, 3
              , 0, 0, 3
              , 3, 0, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 14:
              font.bitmap[i].u = 'K';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 3
              , 3, 0, 3, 0
              , 3, 3, 0, 0
              , 3, 3, 0, 0
              , 3, 0, 3, 0
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 15:
              font.bitmap[i].u = 'L';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 0
              , 3, 0, 0
              , 3, 0, 0
              , 3, 0, 0
              , 3, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 16:
              font.bitmap[i].u = L'Ł';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 0
              , 3, 0, 3
              , 2, 3, 0
              , 3, 0, 0
              , 3, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 17:
              font.bitmap[i].u = 'M';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 3, 0, 0, 0, 3
              , 3, 2, 0, 2, 3
              , 3, 3, 1, 3, 3
              , 3, 1, 3, 1, 3
              , 3, 0, 3, 0, 3
              , 3, 0, 0, 0, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 18:
              font.bitmap[i].u = 'N';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 3
              , 3, 2, 0, 3
              , 3, 3, 1, 3
              , 3, 1, 3, 3
              , 3, 0, 2, 3
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 19:
              font.bitmap[i].u = L'Ń';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 3, 2
              , 3, 2, 0, 3
              , 3, 3, 1, 3
              , 3, 1, 3, 3
              , 3, 0, 2, 3
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 20:
              font.bitmap[i].u = 'O';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 21:
              font.bitmap[i].u = L'Ó';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 2, 3
              , 3, 2, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 22:
              font.bitmap[i].u = 'P';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 23:
              font.bitmap[i].u = 'Q';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 2, 0
              , 0, 0, 1, 2
              })[0];
              break;
          case 24:
              font.bitmap[i].u = 'R';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 3, 0, 3, 0
              , 3, 0, 1, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 25:
              font.bitmap[i].u = 'S';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 2
              , 2, 0, 0, 0
              , 1, 3, 0, 0
              , 0, 0, 3, 1
              , 0, 0, 0, 2
              , 2, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 26:
              font.bitmap[i].u = L'Ś';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 2
              , 2, 0, 0, 3
              , 1, 3, 1, 0
              , 0, 0, 3, 1
              , 0, 0, 0, 2
              , 2, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 27:
              font.bitmap[i].u = 'T';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 3, 3
              , 0, 3, 0
              , 0, 3, 0
              , 0, 3, 0
              , 0, 3, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 28:
              font.bitmap[i].u = 'U';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 29:
              font.bitmap[i].u = 'V';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 2, 1, 2
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 30:
              font.bitmap[i].u = 'W';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 3, 0, 0, 0, 3
              , 3, 0, 2, 0, 3
              , 3, 0, 3, 0, 2
              , 3, 0, 3, 0, 3
              , 2, 1, 2, 1, 2
              , 0, 3, 0, 3, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 31:
              font.bitmap[i].u = 'X';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 3
              , 3, 0, 3
              , 0, 3, 0
              , 0, 3, 0
              , 3, 0, 3
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 32:
              font.bitmap[i].u = 'Y';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 3
              , 3, 0, 3
              , 3, 1, 3
              , 0, 3, 0
              , 0, 3, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 33:
              font.bitmap[i].u = 'Z';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 0, 0, 1, 2
              , 0, 0, 3, 0
              , 0, 3, 0, 0
              , 2, 1, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 34:
              font.bitmap[i].u = L'Ź';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 3, 0, 0, 2
              , 2, 0, 3, 0
              , 0, 3, 0, 0
              , 2, 1, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 35:
              font.bitmap[i].u = L'Ż';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 0, 0, 1, 2
              , 2, 1, 3, 0
              , 0, 3, 1, 2
              , 2, 1, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 36:
              font.bitmap[i].u = 'a';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 1, 3, 3, 1
              , 0, 0, 0, 3
              , 1, 3, 3, 3
              , 3, 0, 0, 3
              , 1, 3, 2, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 37:
              font.bitmap[i].u = L'ą';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 1, 3, 3, 1
              , 0, 0, 0, 3
              , 1, 3, 3, 3
              , 3, 0, 0, 3
              , 1, 3, 2, 3
              , 0, 0, 3, 0
              , 0, 0, 2, 3
              })[0];
              break;
          case 38:
              font.bitmap[i].u = 'b';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 0
              , 3, 0, 0, 0
              , 3, 2, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 39:
              font.bitmap[i].u = 'c';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 1, 3, 3
              , 3, 0, 0
              , 3, 0, 0
              , 1, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 40:
              font.bitmap[i].u = L'ć';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 1, 3
              , 0, 3, 0
              , 1, 3, 3
              , 3, 0, 0
              , 3, 0, 0
              , 1, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 41:
              font.bitmap[i].u = 'd';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 3
              , 0, 0, 0, 3
              , 1, 3, 2, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 42:
              font.bitmap[i].u = 'e';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 1, 3, 3, 1
              , 3, 0, 0, 3
              , 2, 3, 3, 2
              , 3, 0, 0, 0
              , 1, 3, 3, 2
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 43:
              font.bitmap[i].u = L'ę';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 1, 3, 3, 1
              , 3, 0, 0, 3
              , 2, 3, 3, 2
              , 3, 0, 0, 0
              , 1, 3, 3, 2
              , 0, 0, 3, 0
              , 0, 0, 2, 3
              })[0];
              break;
          case 44:
              font.bitmap[i].u = 'f';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 2, 3
              , 0, 3, 0
              , 3, 3, 3
              , 0, 3, 0
              , 0, 3, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 45:
              font.bitmap[i].u = 'g';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 1, 3, 3, 2
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 2
              , 0, 0, 0, 3
              , 2, 3, 3, 2
              })[0];
              break;
          case 46:
              font.bitmap[i].u = 'h';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 0
              , 3, 0, 0, 0
              , 2, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 47:
              font.bitmap[i].u = 'i';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 0
              , 3
              , 3
              , 3
              , 3
              , 0
              , 0
              })[0];
              break;
          case 48:
              font.bitmap[i].u = 'j';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 0, 0
              , 0, 3
              , 0, 3
              , 0, 3
              , 0, 3
              , 0, 3
              , 3, 2
              })[0];
              break;
          case 49:
              font.bitmap[i].u = 'k';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 0
              , 3, 0, 0, 3
              , 3, 0, 3, 1
              , 3, 3, 3, 0
              , 3, 0, 1, 2
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 50:
              font.bitmap[i].u = 'l';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 0
              , 3, 0
              , 3, 0
              , 3, 0
              , 3, 0
              , 2, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 51:
              font.bitmap[i].u = L'ł';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 3, 0
              , 0, 3, 3
              , 1, 3, 1
              , 3, 3, 0
              , 0, 3, 0
              , 0, 2, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 52:
              font.bitmap[i].u = 'm';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 3, 3, 2, 3, 2
              , 3, 0, 3, 0, 3
              , 3, 0, 3, 0, 3
              , 3, 0, 3, 0, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 53:
              font.bitmap[i].u = 'n';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 3, 2
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 54:
              font.bitmap[i].u = L'ń';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 1, 3
              , 0, 3, 0
              , 3, 3, 2
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 55:
              font.bitmap[i].u = 'o';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 2, 3, 2
              , 3, 0, 3
              , 3, 0, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 56:
              font.bitmap[i].u = L'ó';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 1, 3
              , 0, 3, 0
              , 2, 3, 2
              , 3, 0, 3
              , 3, 0, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 57:
              font.bitmap[i].u = 'p';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 3, 2, 3, 1
              , 3, 0, 0, 0
              , 3, 0, 0, 0
              })[0];
              break;
          case 58:
              font.bitmap[i].u = 'q';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 1, 3, 3, 3
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 2, 3
              , 0, 0, 0, 3
              , 0, 0, 0, 3
              })[0];
              break;
          case 59:
              font.bitmap[i].u = 'r';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 2, 3
              , 3, 0
              , 3, 0
              , 3, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 60:
              font.bitmap[i].u = 's';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 1, 3, 2
              , 2, 0, 0
              , 1, 3, 1
              , 0, 0, 2
              , 2, 3, 1
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 61:
              font.bitmap[i].u = L'ś';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 3
              , 1, 3, 2
              , 2, 0, 0
              , 1, 3, 1
              , 0, 0, 2
              , 2, 3, 1
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 62:
              font.bitmap[i].u = 't';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 3, 0
              , 0, 3, 0
              , 3, 3, 3
              , 0, 3, 0
              , 0, 3, 0
              , 0, 2, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 63:
              font.bitmap[i].u = 'u';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 2, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 64:
              font.bitmap[i].u = 'v';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 0, 3
              , 3, 0, 3
              , 2, 1, 2
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 65:
              font.bitmap[i].u = 'w';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 3, 0, 2, 0, 2
              , 3, 0, 3, 0, 3
              , 2, 1, 2, 1, 2
              , 0, 3, 0, 3, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 66:
              font.bitmap[i].u = 'x';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 0, 3
              , 0, 3, 0
              , 0, 3, 0
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 67:
              font.bitmap[i].u = 'y';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 0, 3
              , 3, 0, 3
              , 2, 1, 2
              , 0, 3, 0
              , 1, 2, 0
              , 3, 0, 0
              })[0];
              break;
          case 68:
              font.bitmap[i].u = 'z';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 3, 3
              , 0, 2, 1
              , 1, 2, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 69:
              font.bitmap[i].u = L'ź';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 1, 2
              , 0, 2, 0
              , 3, 3, 3
              , 0, 2, 1
              , 1, 2, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 70:
              font.bitmap[i].u = L'ż';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 3, 0
              , 0, 0, 0
              , 3, 3, 3
              , 0, 2, 1
              , 1, 2, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 71:
              font.bitmap[i].u = ' ';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 72:
              font.bitmap[i].u = '0';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 1, 3, 3
              , 3, 3, 1, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 73:
              font.bitmap[i].u = '1';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 3
              , 0, 3, 3
              , 3, 1, 3
              , 1, 0, 3
              , 0, 0, 3
              , 0, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 74:
              font.bitmap[i].u = '2';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 0, 0, 0, 3
              , 0, 0, 3, 0
              , 0, 3, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 75:
              font.bitmap[i].u = '3';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 2, 3, 3, 2
              , 0, 0, 0, 3
              , 0, 3, 3, 2
              , 0, 0, 0, 3
              , 0, 0, 0, 3
              , 2, 3, 3, 2
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 76:
              font.bitmap[i].u = '4';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 3, 2
              , 0, 3, 1, 3
              , 3, 1, 0, 3
              , 2, 3, 3, 3
              , 0, 0, 0, 3
              , 0, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 77:
              font.bitmap[i].u = '5';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 3, 0, 0, 0
              , 2, 3, 3, 1
              , 0, 0, 0, 3
              , 0, 0, 0, 3
              , 2, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 78:
              font.bitmap[i].u = '6';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 1, 2, 3
              , 2, 1, 0, 0
              , 3, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 79:
              font.bitmap[i].u = '7';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 3
              , 0, 0, 1, 2
              , 0, 0, 3, 0
              , 0, 1, 2, 0
              , 0, 3, 0, 0
              , 3, 1, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 80:
              font.bitmap[i].u = '8';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 81:
              font.bitmap[i].u = '9';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 0, 0, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 3
              , 0, 0, 1, 2
              , 3, 2, 1, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 82:
              font.bitmap[i].u = '!';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 3
              , 3
              , 3
              , 0
              , 3
              , 0
              , 0
              })[0];
              break;
          case 83:
              font.bitmap[i].u = '\"';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 3
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 84:
              font.bitmap[i].u = '#';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 3, 0, 3, 0
              , 3, 3, 3, 3, 3
              , 0, 3, 0, 3, 0
              , 0, 3, 0, 3, 0
              , 3, 3, 3, 3, 3
              , 0, 3, 0, 3, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 85:
              font.bitmap[i].u = '$';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 3, 0
              , 1, 3, 2
              , 2, 0, 0
              , 1, 3, 1
              , 0, 0, 2
              , 2, 3, 1
              , 0, 3, 0
              , 0, 0, 0
              })[0];
              break;
          case 86:
              font.bitmap[i].u = '%';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 3, 0, 3
              , 3, 0, 3, 1
              , 2, 0, 3, 0
              , 0, 3, 0, 2
              , 1, 3, 0, 3
              , 3, 0, 3, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 87:
              font.bitmap[i].u = '&';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 1, 0
              , 3, 0, 3, 0
              , 1, 3, 1, 0
              , 3, 0, 1, 3
              , 3, 0, 0, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 88:
              font.bitmap[i].u = '\'';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 3
              , 0
              , 0
              , 0
              , 0
              , 0
              , 0
              })[0];
              break;
          case 89:
              font.bitmap[i].u = '(';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 3, 0
              , 3, 0
              , 3, 0
              , 3, 0
              , 0, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 90:
              font.bitmap[i].u = ')';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 0
              , 0, 3
              , 0, 3
              , 0, 3
              , 0, 3
              , 3, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 91:
              font.bitmap[i].u = '*';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 1, 3, 1
              , 3, 3, 3
              , 1, 3, 1
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 92:
              font.bitmap[i].u = '+';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 3, 0
              , 3, 3, 3
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 93:
              font.bitmap[i].u = ',';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 3
              , 2, 1
              , 0, 0
              })[0];
              break;
          case 94:
              font.bitmap[i].u = '-';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 95:
              font.bitmap[i].u = '.';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 0
              , 0
              , 0
              , 0
              , 0
              , 3
              , 0
              , 0
              })[0];
              break;
          case 96:
              font.bitmap[i].u = '/';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 3
              , 0, 0, 2, 1
              , 0, 0, 3, 0
              , 0, 3, 0, 0
              , 1, 2, 0, 0
              , 3, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 97:
              font.bitmap[i].u = ':';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 0
              , 0
              , 3
              , 0
              , 0
              , 3
              , 0
              , 0
              })[0];
              break;
          case 98:
              font.bitmap[i].u = ';';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 0, 3
              , 0, 0
              , 0, 0
              , 0, 3
              , 2, 1
              , 0, 0
              })[0];
              break;
          case 99:
              font.bitmap[i].u = '<';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 3
              , 0, 3, 0
              , 3, 0, 0
              , 3, 0, 0
              , 0, 3, 0
              , 0, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 100:
              font.bitmap[i].u = '=';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 101:
              font.bitmap[i].u = '>';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 3, 0, 0
              , 0, 3, 0
              , 0, 0, 3
              , 0, 0, 3
              , 0, 3, 0
              , 3, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 102:
              font.bitmap[i].u = '?';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 2, 0, 3
              , 0, 0, 3
              , 0, 3, 0
              , 0, 0, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 103:
              font.bitmap[i].u = '@';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 1, 3, 3, 3, 1
              , 3, 0, 1, 1, 3
              , 3, 0, 2, 2, 3
              , 3, 0, 2, 2, 2
              , 3, 0, 0, 0, 0
              , 1, 3, 3, 3, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 104:
              font.bitmap[i].u = '[';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 3
              , 3, 0
              , 3, 0
              , 3, 0
              , 3, 0
              , 3, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 105:
              font.bitmap[i].u = '\\';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 0, 0
              , 0, 2, 0, 0
              , 0, 3, 0, 0
              , 0, 0, 3, 0
              , 0, 0, 2, 0
              , 0, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 106:
              font.bitmap[i].u = ']';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 3
              , 0, 3
              , 0, 3
              , 0, 3
              , 0, 3
              , 3, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 107:
              font.bitmap[i].u = '^';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 3, 0, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 108:
              font.bitmap[i].u = '`';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 2, 0
              , 1, 3
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 109:
              font.bitmap[i].u = '{';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 2, 1
              , 0, 3, 0
              , 1, 2, 0
              , 3, 2, 0
              , 0, 3, 0
              , 0, 2, 1
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 110:
              font.bitmap[i].u = '|';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 3
              , 3
              , 3
              , 3
              , 3
              , 3
              , 0
              })[0];
              break;
          case 111:
              font.bitmap[i].u = '}';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 2, 0
              , 0, 3, 0
              , 0, 2, 1
              , 0, 2, 3
              , 0, 3, 0
              , 1, 2, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 112:
              font.bitmap[i].u = '~';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 1, 3, 0, 3
              , 3, 0, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 113:
              font.bitmap[i].u = L'′';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 3, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 114:
              font.bitmap[i].u = L'−';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 115:
              font.bitmap[i].u = L'¡';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 0
              , 3
              , 0
              , 3
              , 3
              , 3
              , 3
              , 0
              })[0];
              break;
          case 116:
              font.bitmap[i].u = L'‽';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 2, 2, 3
              , 0, 2, 3
              , 0, 3, 0
              , 0, 0, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 117:
              font.bitmap[i].u = L'²';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 2, 0, 3
              , 0, 3, 0
              , 3, 2, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 118:
              font.bitmap[i].u = L'➪';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 3, 0, 0
              , 3, 3, 3, 3, 0
              , 3, 0, 0, 0, 3
              , 3, 3, 3, 3, 0
              , 0, 0, 3, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 119:
              font.bitmap[i].u = L'³';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 2, 3, 2
              , 0, 0, 3
              , 0, 3, 2
              , 0, 0, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 120:
              font.bitmap[i].u = L'№';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 3, 0, 3, 0, 0
              , 3, 1, 3, 0, 0
              , 3, 3, 3, 2, 3
              , 3, 2, 3, 2, 3
              , 3, 1, 3, 0, 0
              , 3, 0, 3, 2, 2
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 121:
              font.bitmap[i].u = L' ';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 0
              , 0
              , 0
              , 0
              , 0
              , 0
              , 0
              , 0
              })[0];
              break;
          case 122:
              font.bitmap[i].u = L'‒';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 123:
              font.bitmap[i].u = L'⁄';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 2, 3
              , 0, 0, 0, 3, 0
              , 0, 0, 3, 1, 0
              , 0, 1, 3, 0, 0
              , 0, 3, 0, 0, 0
              , 3, 2, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 124:
              font.bitmap[i].u = L'÷';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 3, 0
              , 0, 0, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 125:
              font.bitmap[i].u = L'°';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 3, 0, 3
              , 1, 3, 1
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 126:
              font.bitmap[i].u = L'〃';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 3, 0, 3
              , 0, 3, 0, 3, 0
              , 0, 3, 0, 3, 0
              , 3, 0, 3, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 127:
              font.bitmap[i].u = L'⇢';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 3, 0
              , 3, 0, 3, 3, 3
              , 0, 0, 0, 3, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 128:
              font.bitmap[i].u = L'×';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 3, 0, 0, 3
              , 0, 3, 3, 0
              , 0, 3, 3, 0
              , 3, 0, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 129:
              font.bitmap[i].u = L'⁑';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 1, 3, 1
              , 3, 3, 3
              , 1, 3, 1
              , 1, 3, 1
              , 3, 3, 3
              , 1, 3, 1
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 130:
              font.bitmap[i].u = L'⁂';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 1, 3, 1, 0
              , 0, 3, 3, 3, 0
              , 0, 1, 3, 1, 0
              , 1, 3, 1, 3, 1
              , 3, 3, 3, 3, 3
              , 1, 3, 1, 3, 1
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 131:
              font.bitmap[i].u = L'–';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 3, 3, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 132:
              font.bitmap[i].u = L'—';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 3, 3, 3, 3, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 133:
              font.bitmap[i].u = L'≈';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 1, 3, 1, 3
              , 3, 1, 3, 1
              , 0, 0, 0, 0
              , 1, 3, 1, 3
              , 3, 1, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 134:
              font.bitmap[i].u = L'±';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 3, 0
              , 3, 3, 3
              , 0, 3, 0
              , 3, 3, 3
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 135:
              font.bitmap[i].u = L'⁃';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 2, 2
              , 3, 3
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 136:
              font.bitmap[i].u = L'―';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 3, 3, 3, 3, 3
              , 3, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 137:
              font.bitmap[i].u = L'‣';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 2, 0
              , 3, 3, 3
              , 3, 2, 0
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 138:
              font.bitmap[i].u = L'•';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 2, 3, 2
              , 3, 3, 3
              , 2, 3, 2
              , 0, 0, 0
              , 0, 0, 0
              , 0, 0, 0
              })[0];
              break;
          case 139:
              font.bitmap[i].u = L'®';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 3, 3, 1
              , 3, 1, 2, 3
              , 3, 1, 2, 3
              , 1, 3, 3, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 140:
              font.bitmap[i].u = L'℠';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 3, 2, 0, 3
              , 3, 0, 3, 2, 3
              , 0, 3, 2, 3, 3
              , 3, 0, 3, 0, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 141:
              font.bitmap[i].u = L'©';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 1, 3, 3, 3, 1
              , 3, 1, 2, 0, 3
              , 3, 2, 0, 0, 3
              , 3, 1, 2, 0, 3
              , 1, 3, 3, 3, 1
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 142:
              font.bitmap[i].u = L'™';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 3, 3, 3, 0, 3
              , 0, 3, 2, 2, 3
              , 0, 3, 2, 3, 3
              , 0, 3, 2, 0, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 143:
              font.bitmap[i].u = L'µ';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 3, 0, 3
              , 3, 0, 3
              , 3, 0, 3
              , 2, 3, 3
              , 3, 0, 0
              , 3, 0, 0
              })[0];
              break;
          case 144:
              font.bitmap[i].u = L'❛';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 1, 2
              , 3, 0
              , 3, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 145:
              font.bitmap[i].u = L'❝';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 2, 1, 2
              , 3, 0, 3, 0
              , 3, 0, 3, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 146:
              font.bitmap[i].u = L'❜';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 0, 3
              , 2, 1
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 147:
              font.bitmap[i].u = L'❞';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 3, 0, 3
              , 0, 3, 0, 3
              , 2, 1, 2, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 148:
              font.bitmap[i].u = L'⁅';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 3
              , 3, 0
              , 3, 3
              , 3, 0
              , 3, 0
              , 3, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 149:
              font.bitmap[i].u = L'〈';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 1, 1
              , 3, 0
              , 3, 0
              , 1, 1
              , 0, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 150:
              font.bitmap[i].u = L'⁆';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 3
              , 0, 3
              , 3, 3
              , 0, 3
              , 0, 3
              , 3, 3
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 151:
              font.bitmap[i].u = L'〉';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 0
              , 1, 1
              , 0, 3
              , 0, 3
              , 1, 1
              , 3, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 152:
              font.bitmap[i].u = L' ';
              font.bitmap[i].width = 0;
              break;
          case 153:
              font.bitmap[i].u = L'¦';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 3
              , 1
              , 1
              , 3
              , 3
              , 0
              , 0
              })[0];
              break;
          case 154:
              font.bitmap[i].u = L'‐';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 0, 0
              , 3, 3
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 155:
              font.bitmap[i].u = L'՚';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 3
              , 2
              , 0
              , 0
              , 0
              , 0
              , 0
              , 0
              })[0];
              break;
          case 156:
              font.bitmap[i].u = L'‛';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 3, 0
              , 1, 2
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 157:
              font.bitmap[i].u = L'‟';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 0, 3, 0
              , 1, 2, 1, 2
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 158:
              font.bitmap[i].u = L'‚';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 3
              , 2, 1
              , 0, 0
              })[0];
              break;
          case 159:
              font.bitmap[i].u = L'„';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 3, 0, 3
              , 2, 1, 2, 1
              , 0, 0, 0, 0
              })[0];
              break;
          case 160:
              font.bitmap[i].u = L'‘';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 1, 2
              , 3, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 161:
              font.bitmap[i].u = L'“';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 1, 2, 1, 2
              , 3, 0, 3, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 162:
              font.bitmap[i].u = L'’';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 3
              , 2, 1
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 163:
              font.bitmap[i].u = L'”';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 3, 0, 3
              , 2, 1, 2, 1
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 164:
              font.bitmap[i].u = L'⏎';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 3, 3
              , 0, 3, 0, 3, 3
              , 1, 3, 3, 3, 3
              , 3, 0, 0, 0, 3
              , 1, 3, 3, 3, 3
              , 0, 3, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 165:
              font.bitmap[i].u = L'⁋';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 3, 3, 3, 1
              , 3, 0, 3, 3
              , 3, 0, 3, 1
              , 3, 0, 3, 0
              , 3, 0, 3, 0
              , 3, 0, 3, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 166:
              font.bitmap[i].u = L'✓';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 3
              , 0, 0, 0, 3, 0
              , 0, 0, 0, 2, 0
              , 0, 0, 3, 0, 0
              , 3, 0, 2, 0, 0
              , 0, 3, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 167:
              font.bitmap[i].u = L'·';
              font.bitmap[i].width = 1;
              bitmap = &((N8 [])
              { 0
              , 0
              , 0
              , 3
              , 0
              , 0
              , 0
              , 0
              })[0];
              break;
          case 168:
              font.bitmap[i].u = L'…';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 3, 0, 3, 0, 3
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 169:
              font.bitmap[i].u = L'‹';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 0, 3
              , 3, 0
              , 0, 3
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 170:
              font.bitmap[i].u = L'«';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 3, 0, 3
              , 3, 0, 3, 0
              , 0, 3, 0, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 171:
              font.bitmap[i].u = L'›';
              font.bitmap[i].width = 2;
              bitmap = &((N8 [])
              { 0, 0
              , 0, 0
              , 3, 0
              , 0, 3
              , 3, 0
              , 0, 0
              , 0, 0
              , 0, 0
              })[0];
              break;
          case 172:
              font.bitmap[i].u = L'»';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 0, 0
              , 0, 0, 0, 0
              , 3, 0, 3, 0
              , 0, 3, 0, 3
              , 3, 0, 3, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 173:
              font.bitmap[i].u = L' ';
              font.bitmap[i].width = 5;
              bitmap = &((N8 [])
              { 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              , 0, 0, 0, 0, 0
              })[0];
              break;
          case 174:
              font.bitmap[i].u = L'¿';
              font.bitmap[i].width = 3;
              bitmap = &((N8 [])
              { 0, 0, 0
              , 0, 0, 0
              , 0, 3, 0
              , 0, 0, 0
              , 0, 3, 0
              , 3, 0, 0
              , 3, 0, 2
              , 1, 3, 1
              })[0];
              break;
          case 175:
              font.bitmap[i].u = L'€';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 1, 3, 3
              , 1, 3, 0, 0
              , 3, 2, 2, 0
              , 3, 2, 2, 0
              , 1, 3, 0, 0
              , 0, 1, 3, 3
              , 0, 0, 0, 0
              , 0, 0, 0, 0
              })[0];
              break;
          case 176:
              font.bitmap[i].u = L'¢';
              font.bitmap[i].width = 4;
              bitmap = &((N8 [])
              { 0, 0, 2, 0
              , 0, 3, 3, 3
              , 2, 1, 2, 0
              , 3, 0, 2, 0
              , 2, 1, 2, 0
              , 0, 3, 3, 3
              , 0, 0, 2, 0
              , 0, 0, 0, 0
              })[0];
              break;
        }
        Mt_( font.bitmap[i].bitmap, font.bitmap[i].width * font.height / 4 + ( font.bitmap[i].width * font.height % 4 ? 1 : 0 ));
        if( !font.bitmap[i].bitmap )
            return ~0;
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
N
E_font_I_draw(
  N x
, N y
, N color
, U u
){  for_n( i, font.bitmap_n )
        if( font.bitmap[i].u == u )
        {   N j = 0;
            C c;
            N x_, y_ = y;
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
                        E_vga_I_set_pixel_aa(
                          x_, y_
                        , color
                        , brightness
                        , 0
                        );
                    }
                    x_++;
                    j++;
                }
                y_++;
            }
            return font.bitmap[i].width;
        }
    return ~0;
}
void
E_font_I_scroll( N dy
){  E_mem_Q_blk_I_copy( (P)(N)E_vga_S_video->framebuffer, (Pc)(N)E_vga_S_video->framebuffer + dy * E_vga_S_video->line_width, E_vga_S_video->framebuffer + ( E_vga_S_video->height - dy ) * E_vga_S_video->line_width );
    E_vga_I_fill_rect( 0, E_vga_S_video->height - dy, E_vga_S_video->width, dy, E_vga_R_video_color( E_font_S_color ));
}
void
E_font_I_print_nl( void
){  E_font_S_x = 1;
    if( E_font_S_y + font.height + 1 > E_vga_S_video->height )
    {   E_font_I_scroll( font.height + 1 - ( E_vga_S_video->height - E_font_S_y ));
        E_font_S_y = E_vga_S_video->height - font.height + 1;
    }else
        E_font_S_y += font.height + 1;
}
void
E_font_I_print_u( U u
){  if( u == '\n' )
    {   E_font_I_print_nl();
        return;
    }
    N dx = 0;
    for_n( i, font.bitmap_n )
        if( font.bitmap[i].u == u )
        {   dx = font.bitmap[i].width;
            break;
        }
    if( !dx )
    {   u = font.bitmap[0].u;
        dx = font.bitmap[0].width;
    }
    if( E_font_S_x + dx + 1 > E_vga_S_video->width )
        E_font_I_print_nl();
    E_font_I_draw( E_font_S_x, E_font_S_y, E_font_S_color, u );
    E_font_S_x += dx + 1;
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
    }
}
/******************************************************************************/
