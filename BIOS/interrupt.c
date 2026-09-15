/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         interrupt handlers
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒6‒4 K
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
void E_interrupt_I_external_32(void);
void E_interrupt_I_external_33(void);
void E_interrupt_I_external_34(void);
void E_interrupt_I_external_35(void);
void E_interrupt_I_external_36(void);
void E_interrupt_I_external_37(void);
void E_interrupt_I_external_38(void);
void E_interrupt_I_external_39(void);
void E_interrupt_I_external_40(void);
void E_interrupt_I_external_41(void);
void E_interrupt_I_external_42(void);
void E_interrupt_I_external_43(void);
void E_interrupt_I_external_44(void);
void E_interrupt_I_external_45(void);
void E_interrupt_I_external_46(void);
void E_interrupt_I_external_47(void);
void E_interrupt_I_external_48(void);
void E_interrupt_I_external_49(void);
void E_interrupt_I_external_50(void);
void E_interrupt_I_external_51(void);
void E_interrupt_I_external_52(void);
void E_interrupt_I_external_53(void);
void E_interrupt_I_external_54(void);
void E_interrupt_I_external_55(void);
void E_interrupt_I_external_56(void);
void E_interrupt_I_external_57(void);
void E_interrupt_I_external_58(void);
void E_interrupt_I_external_59(void);
void E_interrupt_I_external_60(void);
void E_interrupt_I_external_61(void);
void E_interrupt_I_external_62(void);
void E_interrupt_I_external_63(void);
void E_interrupt_I_external_64(void);
void E_interrupt_I_external_65(void);
void E_interrupt_I_external_66(void);
void E_interrupt_I_external_67(void);
void E_interrupt_I_external_68(void);
void E_interrupt_I_external_69(void);
void E_interrupt_I_external_70(void);
void E_interrupt_I_external_71(void);
void E_interrupt_I_external_72(void);
void E_interrupt_I_external_73(void);
void E_interrupt_I_external_74(void);
void E_interrupt_I_external_75(void);
void E_interrupt_I_external_76(void);
void E_interrupt_I_external_77(void);
void E_interrupt_I_external_78(void);
void E_interrupt_I_external_79(void);
void E_interrupt_I_external_80(void);
void E_interrupt_I_external_81(void);
void E_interrupt_I_external_82(void);
void E_interrupt_I_external_83(void);
void E_interrupt_I_external_84(void);
void E_interrupt_I_external_85(void);
void E_interrupt_I_external_86(void);
void E_interrupt_I_external_87(void);
void E_interrupt_I_external_88(void);
void E_interrupt_I_external_89(void);
void E_interrupt_I_external_90(void);
void E_interrupt_I_external_91(void);
void E_interrupt_I_external_92(void);
void E_interrupt_I_external_93(void);
void E_interrupt_I_external_94(void);
void E_interrupt_I_external_95(void);
void E_interrupt_I_external_96(void);
void E_interrupt_I_external_97(void);
void E_interrupt_I_external_98(void);
void E_interrupt_I_external_99(void);
void E_interrupt_I_external_100(void);
void E_interrupt_I_external_101(void);
void E_interrupt_I_external_102(void);
void E_interrupt_I_external_103(void);
void E_interrupt_I_external_104(void);
void E_interrupt_I_external_105(void);
void E_interrupt_I_external_106(void);
void E_interrupt_I_external_107(void);
void E_interrupt_I_external_108(void);
void E_interrupt_I_external_109(void);
void E_interrupt_I_external_110(void);
void E_interrupt_I_external_111(void);
void E_interrupt_I_external_112(void);
void E_interrupt_I_external_113(void);
void E_interrupt_I_external_114(void);
void E_interrupt_I_external_115(void);
void E_interrupt_I_external_116(void);
void E_interrupt_I_external_117(void);
void E_interrupt_I_external_118(void);
void E_interrupt_I_external_119(void);
void E_interrupt_I_external_120(void);
void E_interrupt_I_external_121(void);
void E_interrupt_I_external_122(void);
void E_interrupt_I_external_123(void);
void E_interrupt_I_external_124(void);
void E_interrupt_I_external_125(void);
void E_interrupt_I_external_126(void);
void E_interrupt_I_external_127(void);
void E_interrupt_I_external_128(void);
void E_interrupt_I_external_129(void);
void E_interrupt_I_external_130(void);
void E_interrupt_I_external_131(void);
void E_interrupt_I_external_132(void);
void E_interrupt_I_external_133(void);
void E_interrupt_I_external_134(void);
void E_interrupt_I_external_135(void);
void E_interrupt_I_external_136(void);
void E_interrupt_I_external_137(void);
void E_interrupt_I_external_138(void);
void E_interrupt_I_external_139(void);
void E_interrupt_I_external_140(void);
void E_interrupt_I_external_141(void);
void E_interrupt_I_external_142(void);
void E_interrupt_I_external_143(void);
void E_interrupt_I_external_144(void);
void E_interrupt_I_external_145(void);
void E_interrupt_I_external_146(void);
void E_interrupt_I_external_147(void);
void E_interrupt_I_external_148(void);
void E_interrupt_I_external_149(void);
void E_interrupt_I_external_150(void);
void E_interrupt_I_external_151(void);
void E_interrupt_I_external_152(void);
void E_interrupt_I_external_153(void);
void E_interrupt_I_external_154(void);
void E_interrupt_I_external_155(void);
void E_interrupt_I_external_156(void);
void E_interrupt_I_external_157(void);
void E_interrupt_I_external_158(void);
void E_interrupt_I_external_159(void);
void E_interrupt_I_external_160(void);
void E_interrupt_I_external_161(void);
void E_interrupt_I_external_162(void);
void E_interrupt_I_external_163(void);
void E_interrupt_I_external_164(void);
void E_interrupt_I_external_165(void);
void E_interrupt_I_external_166(void);
void E_interrupt_I_external_167(void);
void E_interrupt_I_external_168(void);
void E_interrupt_I_external_169(void);
void E_interrupt_I_external_170(void);
void E_interrupt_I_external_171(void);
void E_interrupt_I_external_172(void);
void E_interrupt_I_external_173(void);
void E_interrupt_I_external_174(void);
void E_interrupt_I_external_175(void);
void E_interrupt_I_external_176(void);
void E_interrupt_I_external_177(void);
void E_interrupt_I_external_178(void);
void E_interrupt_I_external_179(void);
void E_interrupt_I_external_180(void);
void E_interrupt_I_external_181(void);
void E_interrupt_I_external_182(void);
void E_interrupt_I_external_183(void);
void E_interrupt_I_external_184(void);
void E_interrupt_I_external_185(void);
void E_interrupt_I_external_186(void);
void E_interrupt_I_external_187(void);
void E_interrupt_I_external_188(void);
void E_interrupt_I_external_189(void);
void E_interrupt_I_external_190(void);
void E_interrupt_I_external_191(void);
void E_interrupt_I_external_192(void);
void E_interrupt_I_external_193(void);
void E_interrupt_I_external_194(void);
void E_interrupt_I_external_195(void);
void E_interrupt_I_external_196(void);
void E_interrupt_I_external_197(void);
void E_interrupt_I_external_198(void);
void E_interrupt_I_external_199(void);
void E_interrupt_I_external_200(void);
void E_interrupt_I_external_201(void);
void E_interrupt_I_external_202(void);
void E_interrupt_I_external_203(void);
void E_interrupt_I_external_204(void);
void E_interrupt_I_external_205(void);
void E_interrupt_I_external_206(void);
void E_interrupt_I_external_207(void);
void E_interrupt_I_external_208(void);
void E_interrupt_I_external_209(void);
void E_interrupt_I_external_210(void);
void E_interrupt_I_external_211(void);
void E_interrupt_I_external_212(void);
void E_interrupt_I_external_213(void);
void E_interrupt_I_external_214(void);
void E_interrupt_I_external_215(void);
void E_interrupt_I_external_216(void);
void E_interrupt_I_external_217(void);
void E_interrupt_I_external_218(void);
void E_interrupt_I_external_219(void);
void E_interrupt_I_external_220(void);
void E_interrupt_I_external_221(void);
void E_interrupt_I_external_222(void);
void E_interrupt_I_external_223(void);
void E_interrupt_I_external_224(void);
void E_interrupt_I_external_225(void);
void E_interrupt_I_external_226(void);
void E_interrupt_I_external_227(void);
void E_interrupt_I_external_228(void);
void E_interrupt_I_external_229(void);
void E_interrupt_I_external_230(void);
void E_interrupt_I_external_231(void);
void E_interrupt_I_external_232(void);
void E_interrupt_I_external_233(void);
void E_interrupt_I_external_234(void);
void E_interrupt_I_external_235(void);
void E_interrupt_I_external_236(void);
void E_interrupt_I_external_237(void);
void E_interrupt_I_external_238(void);
void E_interrupt_I_external_239(void);
void E_interrupt_I_external_240(void);
void E_interrupt_I_external_241(void);
void E_interrupt_I_external_242(void);
void E_interrupt_I_external_243(void);
void E_interrupt_I_external_244(void);
void E_interrupt_I_external_245(void);
void E_interrupt_I_external_246(void);
void E_interrupt_I_external_247(void);
void E_interrupt_I_external_248(void);
void E_interrupt_I_external_249(void);
void E_interrupt_I_external_250(void);
void E_interrupt_I_external_251(void);
void E_interrupt_I_external_252(void);
void E_interrupt_I_external_253(void);
void E_interrupt_I_external_254(void);
void E_interrupt_I_external_spurious(void);
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N64 E_main_Q_msr_R( N32 );
void E_main_Q_msr_P( N32 , N64 );
//==============================================================================
struct __attribute__ ((packed)) E_interrupt_Z_id
{ N16 limit;
  N base;
};
//==============================================================================
extern struct E_main_Z_kernel_args E_main_S_kernel_args;
extern struct E_interrupt_Z_id S_id;
//==============================================================================
N *E_interrupt_S_idt;
E_interrupt_S_external_Z *E_interrupt_S_external;
N8 E_interrupt_S_gsi_ex_n, E_interrupt_S_gsi_next;
//==============================================================================
N32
E_interrupt_Q_io_apic_R( N8 i
){  *( volatile N32 * )E_main_S_kernel_args.io_apic_address = i;
    return *( volatile N32 * )( (Pc)E_main_S_kernel_args.io_apic_address + 0x10 );
}
void
E_interrupt_Q_io_apic_P( N8 i
, N32 v
){  *( volatile N32 * )E_main_S_kernel_args.io_apic_address = i;
    *( volatile N32 * )( (Pc)E_main_S_kernel_args.io_apic_address + 0x10 ) = v;
}
void
E_interrupt_Q_io_apic_P_gsi( N8 i
, N64 v
){  E_interrupt_Q_io_apic_P( 0x10 + i * 2, ( v | ( 1 << 16 )) & 0xffffffff );
    E_interrupt_Q_io_apic_P( 0x10 + i * 2 + 1, v >> 32 );
    E_interrupt_Q_io_apic_P( 0x10 + i * 2, v & 0xffffffff );
}
//------------------------------------------------------------------------------
void
E_interrupt_I_ipi_init( N32 processor
){  if( E_main_S_kernel_args.x2apic )
        E_main_Q_msr_P( 0x830, (( N64 )processor << 32 ) | ( 5 << 8 ));
    else
    {   *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0x310 ) = processor << 24;
        *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0x300 ) = 5 << 8;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define E_interrupt_Q_io_apic_J_gsi_v( vector, delivery_mode, polarity, trigger_mode, destination ) \
    ( (vector) | ( (N)( delivery_mode ) << 8 ) | ( (N)(polarity) << 13 ) | ( (N)( trigger_mode ) << 15 ) | ( (N)(destination) << 56 ))
void
E_interrupt_Q_io_apic_I_enable( N8 i
){  N polarity = ( E_main_S_kernel_args.gsi[i].flags & 3 ) == 3;
    N trigger_mode = (( E_main_S_kernel_args.gsi[i].flags >> 2 ) & 3 ) == 3;
    E_interrupt_Q_io_apic_P_gsi( i, E_interrupt_Q_io_apic_J_gsi_v( 32 + E_interrupt_S_gsi_ex_n + i, 0, polarity, trigger_mode, 0 ));
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_interrupt_P_gsi( N8 irq
, E_interrupt_S_external_Z proc
){  for_n( i, E_main_S_kernel_args.gsi_n )
        if( E_main_S_kernel_args.gsi[i].source == irq )
        {   E_interrupt_S_external[ E_interrupt_S_gsi_ex_n + i ] = proc;
            break;
        }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define E_interrupt_J_interrupt_descriptor_low( selector, offset, stack ) (( (N)(offset) & (( 1UL << 16 ) - 1 )) | ( (N)(selector) << 16 ) | ( (N)(stack) << 32 ) | ( 0xeUL << ( 32 + 8 )) | ( 1UL << ( 32 + 15 )) | ((( (N)(offset) >> 16 ) & (( 1UL << 16 ) - 1 )) << ( 32 + 16 )))
#define E_interrupt_J_interrupt_descriptor( i, procedure, stack ) \
    {   E_interrupt_S_idt[ (i) * 2 ] = E_interrupt_J_interrupt_descriptor_low( 1 << 3, (N)&(procedure), (stack) ); \
        E_interrupt_S_idt[ (i) * 2 + 1 ] = (N)&(procedure) >> 32; \
    }
N8
E_interrupt_R_gsi_next( void
){  if( E_interrupt_S_gsi_next < E_interrupt_S_gsi_ex_n )
        return E_interrupt_S_gsi_next++;
    for( ; E_interrupt_S_gsi_next != E_interrupt_S_gsi_ex_n + 16; E_interrupt_S_gsi_next++ )
        if( !~(S8)E_main_S_kernel_args.gsi[ E_interrupt_S_gsi_next - E_interrupt_S_gsi_ex_n ].source )
            return E_interrupt_S_gsi_next++;
    return ~0;
}
N8
E_interrupt_R_free_external( void
){  for_n( i, E_main_S_kernel_args.gsi_n )
        if( !E_interrupt_S_external[ E_interrupt_S_gsi_ex_n + i ] )
            return E_interrupt_S_gsi_ex_n + i;
    return ~0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N
E_interrupt_M( void
){  E_interrupt_S_gsi_next = 0;
    E_interrupt_S_gsi_ex_n = 0;
    N idt_n = 32 + E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n + 1;
    Mt_( E_interrupt_S_idt, idt_n * 2 );
    Kp( E_interrupt_S_idt )
        return ~0;
    E_interrupt_J_interrupt_descriptor( 0, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 1, E_interrupt_I_external_spurious, 1 );
    //E_interrupt_J_interrupt_descriptor( 2, E_interrupt_I_external_spurious, 1 );
    E_interrupt_S_idt[ 2 * 2 ] = 0;
    E_interrupt_J_interrupt_descriptor( 3, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 4, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 5, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 6, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 7, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 8, E_interrupt_I_external_spurious, 1 );
    E_interrupt_S_idt[ 9 * 2 ] = 0;
    E_interrupt_J_interrupt_descriptor( 10, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 11, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 12, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 13, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 14, E_interrupt_I_external_spurious, 1 );
    E_interrupt_S_idt[ 15 * 2 ] = 0;
    E_interrupt_J_interrupt_descriptor( 16, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 17, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 18, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 19, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 20, E_interrupt_I_external_spurious, 1 );
    E_interrupt_J_interrupt_descriptor( 21, E_interrupt_I_external_spurious, 1 );
    for( N i = 22; i != 32; i++ )
        E_interrupt_S_idt[ i * 2 ] = 0;
    Mt_( E_interrupt_S_external, E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n );
    Kp( E_interrupt_S_external )
    {   K_( ~1, W( E_interrupt_S_idt ));
        return ~0;
    }
    _0t_( E_interrupt_S_external, E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n );
    for( N i = 32; i != 32 + E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n; i++ )
        switch(i)
        { case 32:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_32, 1 );
                break;
          case 33:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_33, 1 );
                break;
          case 34:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_34, 1 );
                break;
          case 35:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_35, 1 );
                break;
          case 36:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_36, 1 );
                break;
          case 37:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_37, 1 );
                break;
          case 38:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_38, 1 );
                break;
          case 39:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_39, 1 );
                break;
          case 40:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_40, 1 );
                break;
          case 41:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_41, 1 );
                break;
          case 42:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_42, 1 );
                break;
          case 43:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_43, 1 );
                break;
          case 44:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_44, 1 );
                break;
          case 45:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_45, 1 );
                break;
          case 46:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_46, 1 );
                break;
          case 47:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_47, 1 );
                break;
          case 48:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_48, 1 );
                break;
          case 49:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_49, 1 );
                break;
          case 50:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_50, 1 );
                break;
          case 51:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_51, 1 );
                break;
          case 52:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_52, 1 );
                break;
          case 53:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_53, 1 );
                break;
          case 54:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_54, 1 );
                break;
          case 55:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_55, 1 );
                break;
          case 56:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_56, 1 );
                break;
          case 57:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_57, 1 );
                break;
          case 58:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_58, 1 );
                break;
          case 59:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_59, 1 );
                break;
          case 60:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_60, 1 );
                break;
          case 61:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_61, 1 );
                break;
          case 62:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_62, 1 );
                break;
          case 63:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_63, 1 );
                break;
          case 64:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_64, 1 );
                break;
          case 65:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_65, 1 );
                break;
          case 66:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_66, 1 );
                break;
          case 67:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_67, 1 );
                break;
          case 68:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_68, 1 );
                break;
          case 69:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_69, 1 );
                break;
          case 70:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_70, 1 );
                break;
          case 71:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_71, 1 );
                break;
          case 72:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_72, 1 );
                break;
          case 73:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_73, 1 );
                break;
          case 74:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_74, 1 );
                break;
          case 75:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_75, 1 );
                break;
          case 76:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_76, 1 );
                break;
          case 77:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_77, 1 );
                break;
          case 78:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_78, 1 );
                break;
          case 79:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_79, 1 );
                break;
          case 80:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_80, 1 );
                break;
          case 81:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_81, 1 );
                break;
          case 82:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_82, 1 );
                break;
          case 83:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_83, 1 );
                break;
          case 84:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_84, 1 );
                break;
          case 85:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_85, 1 );
                break;
          case 86:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_86, 1 );
                break;
          case 87:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_87, 1 );
                break;
          case 88:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_88, 1 );
                break;
          case 89:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_89, 1 );
                break;
          case 90:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_90, 1 );
                break;
          case 91:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_91, 1 );
                break;
          case 92:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_92, 1 );
                break;
          case 93:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_93, 1 );
                break;
          case 94:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_94, 1 );
                break;
          case 95:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_95, 1 );
                break;
          case 96:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_96, 1 );
                break;
          case 97:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_97, 1 );
                break;
          case 98:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_98, 1 );
                break;
          case 99:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_99, 1 );
                break;
          case 100:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_100, 1 );
                break;
          case 101:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_101, 1 );
                break;
          case 102:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_102, 1 );
                break;
          case 103:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_103, 1 );
                break;
          case 104:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_104, 1 );
                break;
          case 105:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_105, 1 );
                break;
          case 106:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_106, 1 );
                break;
          case 107:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_107, 1 );
                break;
          case 108:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_108, 1 );
                break;
          case 109:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_109, 1 );
                break;
          case 110:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_110, 1 );
                break;
          case 111:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_111, 1 );
                break;
          case 112:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_112, 1 );
                break;
          case 113:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_113, 1 );
                break;
          case 114:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_114, 1 );
                break;
          case 115:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_115, 1 );
                break;
          case 116:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_116, 1 );
                break;
          case 117:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_117, 1 );
                break;
          case 118:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_118, 1 );
                break;
          case 119:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_119, 1 );
                break;
          case 120:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_120, 1 );
                break;
          case 121:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_121, 1 );
                break;
          case 122:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_122, 1 );
                break;
          case 123:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_123, 1 );
                break;
          case 124:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_124, 1 );
                break;
          case 125:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_125, 1 );
                break;
          case 126:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_126, 1 );
                break;
          case 127:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_127, 1 );
                break;
          case 128:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_128, 1 );
                break;
          case 129:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_129, 1 );
                break;
          case 130:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_130, 1 );
                break;
          case 131:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_131, 1 );
                break;
          case 132:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_132, 1 );
                break;
          case 133:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_133, 1 );
                break;
          case 134:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_134, 1 );
                break;
          case 135:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_135, 1 );
                break;
          case 136:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_136, 1 );
                break;
          case 137:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_137, 1 );
                break;
          case 138:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_138, 1 );
                break;
          case 139:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_139, 1 );
                break;
          case 140:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_140, 1 );
                break;
          case 141:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_141, 1 );
                break;
          case 142:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_142, 1 );
                break;
          case 143:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_143, 1 );
                break;
          case 144:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_144, 1 );
                break;
          case 145:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_145, 1 );
                break;
          case 146:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_146, 1 );
                break;
          case 147:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_147, 1 );
                break;
          case 148:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_148, 1 );
                break;
          case 149:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_149, 1 );
                break;
          case 150:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_150, 1 );
                break;
          case 151:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_151, 1 );
                break;
          case 152:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_152, 1 );
                break;
          case 153:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_153, 1 );
                break;
          case 154:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_154, 1 );
                break;
          case 155:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_155, 1 );
                break;
          case 156:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_156, 1 );
                break;
          case 157:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_157, 1 );
                break;
          case 158:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_158, 1 );
                break;
          case 159:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_159, 1 );
                break;
          case 160:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_160, 1 );
                break;
          case 161:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_161, 1 );
                break;
          case 162:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_162, 1 );
                break;
          case 163:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_163, 1 );
                break;
          case 164:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_164, 1 );
                break;
          case 165:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_165, 1 );
                break;
          case 166:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_166, 1 );
                break;
          case 167:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_167, 1 );
                break;
          case 168:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_168, 1 );
                break;
          case 169:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_169, 1 );
                break;
          case 170:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_170, 1 );
                break;
          case 171:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_171, 1 );
                break;
          case 172:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_172, 1 );
                break;
          case 173:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_173, 1 );
                break;
          case 174:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_174, 1 );
                break;
          case 175:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_175, 1 );
                break;
          case 176:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_176, 1 );
                break;
          case 177:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_177, 1 );
                break;
          case 178:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_178, 1 );
                break;
          case 179:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_179, 1 );
                break;
          case 180:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_180, 1 );
                break;
          case 181:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_181, 1 );
                break;
          case 182:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_182, 1 );
                break;
          case 183:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_183, 1 );
                break;
          case 184:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_184, 1 );
                break;
          case 185:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_185, 1 );
                break;
          case 186:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_186, 1 );
                break;
          case 187:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_187, 1 );
                break;
          case 188:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_188, 1 );
                break;
          case 189:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_189, 1 );
                break;
          case 190:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_190, 1 );
                break;
          case 191:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_191, 1 );
                break;
          case 192:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_192, 1 );
                break;
          case 193:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_193, 1 );
                break;
          case 194:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_194, 1 );
                break;
          case 195:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_195, 1 );
                break;
          case 196:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_196, 1 );
                break;
          case 197:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_197, 1 );
                break;
          case 198:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_198, 1 );
                break;
          case 199:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_199, 1 );
                break;
          case 200:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_200, 1 );
                break;
          case 201:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_201, 1 );
                break;
          case 202:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_202, 1 );
                break;
          case 203:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_203, 1 );
                break;
          case 204:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_204, 1 );
                break;
          case 205:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_205, 1 );
                break;
          case 206:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_206, 1 );
                break;
          case 207:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_207, 1 );
                break;
          case 208:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_208, 1 );
                break;
          case 209:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_209, 1 );
                break;
          case 210:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_210, 1 );
                break;
          case 211:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_211, 1 );
                break;
          case 212:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_212, 1 );
                break;
          case 213:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_213, 1 );
                break;
          case 214:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_214, 1 );
                break;
          case 215:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_215, 1 );
                break;
          case 216:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_216, 1 );
                break;
          case 217:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_217, 1 );
                break;
          case 218:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_218, 1 );
                break;
          case 219:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_219, 1 );
                break;
          case 220:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_220, 1 );
                break;
          case 221:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_221, 1 );
                break;
          case 222:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_222, 1 );
                break;
          case 223:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_223, 1 );
                break;
          case 224:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_224, 1 );
                break;
          case 225:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_225, 1 );
                break;
          case 226:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_226, 1 );
                break;
          case 227:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_227, 1 );
                break;
          case 228:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_228, 1 );
                break;
          case 229:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_229, 1 );
                break;
          case 230:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_230, 1 );
                break;
          case 231:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_231, 1 );
                break;
          case 232:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_232, 1 );
                break;
          case 233:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_233, 1 );
                break;
          case 234:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_234, 1 );
                break;
          case 235:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_235, 1 );
                break;
          case 236:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_236, 1 );
                break;
          case 237:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_237, 1 );
                break;
          case 238:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_238, 1 );
                break;
          case 239:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_239, 1 );
                break;
          case 240:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_240, 1 );
                break;
          case 241:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_241, 1 );
                break;
          case 242:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_242, 1 );
                break;
          case 243:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_243, 1 );
                break;
          case 244:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_244, 1 );
                break;
          case 245:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_245, 1 );
                break;
          case 246:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_246, 1 );
                break;
          case 247:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_247, 1 );
                break;
          case 248:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_248, 1 );
                break;
          case 249:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_249, 1 );
                break;
          case 250:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_250, 1 );
                break;
          case 251:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_251, 1 );
                break;
          case 252:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_252, 1 );
                break;
          case 253:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_253, 1 );
                break;
          case 254:
                E_interrupt_J_interrupt_descriptor( i, E_interrupt_I_external_254, 1 );
                break;
        }
    E_interrupt_J_interrupt_descriptor( 32 + E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n, E_interrupt_I_external_spurious, 1 );
    S_id.base = (N)&E_interrupt_S_idt[0];
    S_id.limit = idt_n * 2 * sizeof( *E_interrupt_S_idt ) - 1;
    __asm__ volatile (
    "\n" "lidt  %0"
    :
    : "g" ( S_id )
    );
    if( E_main_S_kernel_args.x2apic )
        E_main_Q_msr_P( 0x80f, ( E_main_Q_msr_R( 0x80f ) & ~0xff ) | 0x100 | ( 32 + E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n )); // Ustaw numer przerwania nieoczekiwanego i włącz APIC.
    else
        *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0xf0 ) = ( *( volatile N32 * )(( Pc )E_main_S_kernel_args.local_apic_address + 0xf0 ) & ~0xff ) | 0x100 | ( 32 + E_interrupt_S_gsi_ex_n + E_main_S_kernel_args.gsi_n );
    for_n( i, E_main_S_kernel_args.gsi_n )
        if( ~(S8)E_main_S_kernel_args.gsi[i].source )
            E_interrupt_Q_io_apic_I_enable(i);
    return 0;
}
/******************************************************************************/
