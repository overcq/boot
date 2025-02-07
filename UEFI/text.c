/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         text
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒7 L
*******************************************************************************/
#include "fileloader.h"
//==============================================================================
_inline
N
E_simple_Z_n_I_mod_i2( N n
, N i
){  return n & ( _v( n, ~0 ) >> ( sizeof(n) * 8 - i ));
}
//==============================================================================
Pc16
E_text_Z_n_N_s( Pc16 s_end
, N n
, N l
, N base
){  n = E_simple_Z_n_I_mod_i2( n, l * 8 );
    do
    {   N k = n % base;
        *--s_end = k < 10
        ? L'0' + k
        : L'A' + k - 10;
    }while( n /= base );
    return s_end;
}
N
E_text_Z_n_N_s_G( N n
, N l
, N base
){  N i = 0;
    n = E_simple_Z_n_I_mod_i2( n, l * 8 );
    do
    {   i++;
    }while( n /= base );
    return i;
}
/******************************************************************************/
