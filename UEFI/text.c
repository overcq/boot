/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         text
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒7 L
*******************************************************************************/
#include "fileloader.h"
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
Pc
E_text_Z_su_R_u( Pc s
, U *u
){  if( (S8)*s >= 0 )
    {   *u = *s;
        return s + 1;
    }
    U v = ~(S8)*s;
    N n = E_asm_I_bsr(v);
    if( n == ~0
    || n == 6
    || n < 3
    )
    {   *u = ~0;
        return s;
    }
    v = *s & (( 1 << n ) - 1 );
    n = 6 - n;
    for_n( i, n )
    {   if(( *++s & 0xc0 ) != 0x80 )
        {   *u = ~0;
            return s;
        }
        v <<= 6;
        v |= *s & 0x3f;
    }
    *u = (( n == 1 && v > 0x7f )
      || ( n == 2 && v > 0x7ff )
      || ( n == 3 && v > 0xffff )
    )
    && v <= 0x10ffff
    && ( v < 0xd800
    || v > 0xdfff
    )
    ? v
    : ~0;
    return s + 1;
}
/******************************************************************************/
