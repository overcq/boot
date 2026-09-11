/*******************************************************************************
*   ___   public
*  ¦OUX¦  GNU x86_64 assembly
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         interrupt handlers
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒6‒4 V
*******************************************************************************/
.global E_interrupt_I_external_32
.global E_interrupt_I_external_33
.global E_interrupt_I_external_34
.global E_interrupt_I_external_35
.global E_interrupt_I_external_36
.global E_interrupt_I_external_37
.global E_interrupt_I_external_38
.global E_interrupt_I_external_39
.global E_interrupt_I_external_40
.global E_interrupt_I_external_41
.global E_interrupt_I_external_42
.global E_interrupt_I_external_43
.global E_interrupt_I_external_44
.global E_interrupt_I_external_45
.global E_interrupt_I_external_46
.global E_interrupt_I_external_47
.global E_interrupt_I_external_48
.global E_interrupt_I_external_49
.global E_interrupt_I_external_50
.global E_interrupt_I_external_51
.global E_interrupt_I_external_52
.global E_interrupt_I_external_53
.global E_interrupt_I_external_54
.global E_interrupt_I_external_55
.global E_interrupt_I_external_56
.global E_interrupt_I_external_57
.global E_interrupt_I_external_58
.global E_interrupt_I_external_59
.global E_interrupt_I_external_60
.global E_interrupt_I_external_61
.global E_interrupt_I_external_62
.global E_interrupt_I_external_63
.global E_interrupt_I_external_64
.global E_interrupt_I_external_65
.global E_interrupt_I_external_66
.global E_interrupt_I_external_67
.global E_interrupt_I_external_68
.global E_interrupt_I_external_69
.global E_interrupt_I_external_70
.global E_interrupt_I_external_71
.global E_interrupt_I_external_72
.global E_interrupt_I_external_73
.global E_interrupt_I_external_74
.global E_interrupt_I_external_75
.global E_interrupt_I_external_76
.global E_interrupt_I_external_77
.global E_interrupt_I_external_78
.global E_interrupt_I_external_79
.global E_interrupt_I_external_80
.global E_interrupt_I_external_81
.global E_interrupt_I_external_82
.global E_interrupt_I_external_83
.global E_interrupt_I_external_84
.global E_interrupt_I_external_85
.global E_interrupt_I_external_86
.global E_interrupt_I_external_87
.global E_interrupt_I_external_88
.global E_interrupt_I_external_89
.global E_interrupt_I_external_90
.global E_interrupt_I_external_91
.global E_interrupt_I_external_92
.global E_interrupt_I_external_93
.global E_interrupt_I_external_94
.global E_interrupt_I_external_95
.global E_interrupt_I_external_96
.global E_interrupt_I_external_97
.global E_interrupt_I_external_98
.global E_interrupt_I_external_99
.global E_interrupt_I_external_100
.global E_interrupt_I_external_101
.global E_interrupt_I_external_102
.global E_interrupt_I_external_103
.global E_interrupt_I_external_104
.global E_interrupt_I_external_105
.global E_interrupt_I_external_106
.global E_interrupt_I_external_107
.global E_interrupt_I_external_108
.global E_interrupt_I_external_109
.global E_interrupt_I_external_110
.global E_interrupt_I_external_111
.global E_interrupt_I_external_112
.global E_interrupt_I_external_113
.global E_interrupt_I_external_114
.global E_interrupt_I_external_115
.global E_interrupt_I_external_116
.global E_interrupt_I_external_117
.global E_interrupt_I_external_118
.global E_interrupt_I_external_119
.global E_interrupt_I_external_120
.global E_interrupt_I_external_121
.global E_interrupt_I_external_122
.global E_interrupt_I_external_123
.global E_interrupt_I_external_124
.global E_interrupt_I_external_125
.global E_interrupt_I_external_126
.global E_interrupt_I_external_127
.global E_interrupt_I_external_128
.global E_interrupt_I_external_129
.global E_interrupt_I_external_130
.global E_interrupt_I_external_131
.global E_interrupt_I_external_132
.global E_interrupt_I_external_133
.global E_interrupt_I_external_134
.global E_interrupt_I_external_135
.global E_interrupt_I_external_136
.global E_interrupt_I_external_137
.global E_interrupt_I_external_138
.global E_interrupt_I_external_139
.global E_interrupt_I_external_140
.global E_interrupt_I_external_141
.global E_interrupt_I_external_142
.global E_interrupt_I_external_143
.global E_interrupt_I_external_144
.global E_interrupt_I_external_145
.global E_interrupt_I_external_146
.global E_interrupt_I_external_147
.global E_interrupt_I_external_148
.global E_interrupt_I_external_149
.global E_interrupt_I_external_150
.global E_interrupt_I_external_151
.global E_interrupt_I_external_152
.global E_interrupt_I_external_153
.global E_interrupt_I_external_154
.global E_interrupt_I_external_155
.global E_interrupt_I_external_156
.global E_interrupt_I_external_157
.global E_interrupt_I_external_158
.global E_interrupt_I_external_159
.global E_interrupt_I_external_160
.global E_interrupt_I_external_161
.global E_interrupt_I_external_162
.global E_interrupt_I_external_163
.global E_interrupt_I_external_164
.global E_interrupt_I_external_165
.global E_interrupt_I_external_166
.global E_interrupt_I_external_167
.global E_interrupt_I_external_168
.global E_interrupt_I_external_169
.global E_interrupt_I_external_170
.global E_interrupt_I_external_171
.global E_interrupt_I_external_172
.global E_interrupt_I_external_173
.global E_interrupt_I_external_174
.global E_interrupt_I_external_175
.global E_interrupt_I_external_176
.global E_interrupt_I_external_177
.global E_interrupt_I_external_178
.global E_interrupt_I_external_179
.global E_interrupt_I_external_180
.global E_interrupt_I_external_181
.global E_interrupt_I_external_182
.global E_interrupt_I_external_183
.global E_interrupt_I_external_184
.global E_interrupt_I_external_185
.global E_interrupt_I_external_186
.global E_interrupt_I_external_187
.global E_interrupt_I_external_188
.global E_interrupt_I_external_189
.global E_interrupt_I_external_190
.global E_interrupt_I_external_191
.global E_interrupt_I_external_192
.global E_interrupt_I_external_193
.global E_interrupt_I_external_194
.global E_interrupt_I_external_195
.global E_interrupt_I_external_196
.global E_interrupt_I_external_197
.global E_interrupt_I_external_198
.global E_interrupt_I_external_199
.global E_interrupt_I_external_200
.global E_interrupt_I_external_201
.global E_interrupt_I_external_202
.global E_interrupt_I_external_203
.global E_interrupt_I_external_204
.global E_interrupt_I_external_205
.global E_interrupt_I_external_206
.global E_interrupt_I_external_207
.global E_interrupt_I_external_208
.global E_interrupt_I_external_209
.global E_interrupt_I_external_210
.global E_interrupt_I_external_211
.global E_interrupt_I_external_212
.global E_interrupt_I_external_213
.global E_interrupt_I_external_214
.global E_interrupt_I_external_215
.global E_interrupt_I_external_216
.global E_interrupt_I_external_217
.global E_interrupt_I_external_218
.global E_interrupt_I_external_219
.global E_interrupt_I_external_220
.global E_interrupt_I_external_221
.global E_interrupt_I_external_222
.global E_interrupt_I_external_223
.global E_interrupt_I_external_224
.global E_interrupt_I_external_225
.global E_interrupt_I_external_226
.global E_interrupt_I_external_227
.global E_interrupt_I_external_228
.global E_interrupt_I_external_229
.global E_interrupt_I_external_230
.global E_interrupt_I_external_231
.global E_interrupt_I_external_232
.global E_interrupt_I_external_233
.global E_interrupt_I_external_234
.global E_interrupt_I_external_235
.global E_interrupt_I_external_236
.global E_interrupt_I_external_237
.global E_interrupt_I_external_238
.global E_interrupt_I_external_239
.global E_interrupt_I_external_240
.global E_interrupt_I_external_241
.global E_interrupt_I_external_242
.global E_interrupt_I_external_243
.global E_interrupt_I_external_244
.global E_interrupt_I_external_245
.global E_interrupt_I_external_246
.global E_interrupt_I_external_247
.global E_interrupt_I_external_248
.global E_interrupt_I_external_249
.global E_interrupt_I_external_250
.global E_interrupt_I_external_251
.global E_interrupt_I_external_252
.global E_interrupt_I_external_253
.global E_interrupt_I_external_254
.global E_interrupt_I_external_spurious
//==============================================================================
.text
E_interrupt_I_external:
    xchg    (%rsp),%rdx
    push    %rax
    push    %rcx
    mov     E_interrupt_S_external(%rip),%rax
    mov     (%rax,%rdx,8),%rax
    test    %rax,%rax
    jz      0f
    push    %rdi
    push    %rsi
    push    %r8
    push    %r9
    push    %r10
    push    %r11
    push    %rbp
    mov     %rsp,%rbp
    and     $~0xf,%rsp
    sub     $512,%rsp
    fxsave  (%rsp)
    lea     -4*8(%rsp),%rsp
    cld
    call    *%rax
    lea     4*8(%rsp),%rsp
    fxrstor (%rsp)
    mov     %rbp,%rsp
    pop     %rbp
    pop     %r11
    pop     %r10
    pop     %r9
    pop     %r8
    pop     %rsi
    pop     %rdi
0:  mov     $0x80b,%ecx
    xor     %edx,%edx
    xor     %eax,%eax
    wrmsr
    pop     %rcx
    pop     %rax
    pop     %rdx
E_interrupt_I_external_spurious:
    iretq
//------------------------------------------------------------------------------
E_interrupt_I_external_32:
    pushq   $0
    jmp     E_interrupt_I_external
E_interrupt_I_external_33:
    pushq   $1
    jmp     E_interrupt_I_external
E_interrupt_I_external_34:
    pushq   $2
    jmp     E_interrupt_I_external
E_interrupt_I_external_35:
    pushq   $3
    jmp     E_interrupt_I_external
E_interrupt_I_external_36:
    pushq   $4
    jmp     E_interrupt_I_external
E_interrupt_I_external_37:
    pushq   $5
    jmp     E_interrupt_I_external
E_interrupt_I_external_38:
    pushq   $6
    jmp     E_interrupt_I_external
E_interrupt_I_external_39:
    pushq   $7
    jmp     E_interrupt_I_external
E_interrupt_I_external_40:
    pushq   $8
    jmp     E_interrupt_I_external
E_interrupt_I_external_41:
    pushq   $9
    jmp     E_interrupt_I_external
E_interrupt_I_external_42:
    pushq   $10
    jmp     E_interrupt_I_external
E_interrupt_I_external_43:
    pushq   $11
    jmp     E_interrupt_I_external
E_interrupt_I_external_44:
    pushq   $12
    jmp     E_interrupt_I_external
E_interrupt_I_external_45:
    pushq   $13
    jmp     E_interrupt_I_external
E_interrupt_I_external_46:
    pushq   $14
    jmp     E_interrupt_I_external
E_interrupt_I_external_47:
    pushq   $15
    jmp     E_interrupt_I_external
E_interrupt_I_external_48:
    pushq   $16
    jmp     E_interrupt_I_external
E_interrupt_I_external_49:
    pushq   $17
    jmp     E_interrupt_I_external
E_interrupt_I_external_50:
    pushq   $18
    jmp     E_interrupt_I_external
E_interrupt_I_external_51:
    pushq   $19
    jmp     E_interrupt_I_external
E_interrupt_I_external_52:
    pushq   $20
    jmp     E_interrupt_I_external
E_interrupt_I_external_53:
    pushq   $21
    jmp     E_interrupt_I_external
E_interrupt_I_external_54:
    pushq   $22
    jmp     E_interrupt_I_external
E_interrupt_I_external_55:
    pushq   $23
    jmp     E_interrupt_I_external
E_interrupt_I_external_56:
    pushq   $24
    jmp     E_interrupt_I_external
E_interrupt_I_external_57:
    pushq   $25
    jmp     E_interrupt_I_external
E_interrupt_I_external_58:
    pushq   $26
    jmp     E_interrupt_I_external
E_interrupt_I_external_59:
    pushq   $27
    jmp     E_interrupt_I_external
E_interrupt_I_external_60:
    pushq   $28
    jmp     E_interrupt_I_external
E_interrupt_I_external_61:
    pushq   $29
    jmp     E_interrupt_I_external
E_interrupt_I_external_62:
    pushq   $30
    jmp     E_interrupt_I_external
E_interrupt_I_external_63:
    pushq   $31
    jmp     E_interrupt_I_external
E_interrupt_I_external_64:
    pushq   $32
    jmp     E_interrupt_I_external
E_interrupt_I_external_65:
    pushq   $33
    jmp     E_interrupt_I_external
E_interrupt_I_external_66:
    pushq   $34
    jmp     E_interrupt_I_external
E_interrupt_I_external_67:
    pushq   $35
    jmp     E_interrupt_I_external
E_interrupt_I_external_68:
    pushq   $36
    jmp     E_interrupt_I_external
E_interrupt_I_external_69:
    pushq   $37
    jmp     E_interrupt_I_external
E_interrupt_I_external_70:
    pushq   $38
    jmp     E_interrupt_I_external
E_interrupt_I_external_71:
    pushq   $39
    jmp     E_interrupt_I_external
E_interrupt_I_external_72:
    pushq   $40
    jmp     E_interrupt_I_external
E_interrupt_I_external_73:
    pushq   $41
    jmp     E_interrupt_I_external
E_interrupt_I_external_74:
    pushq   $42
    jmp     E_interrupt_I_external
E_interrupt_I_external_75:
    pushq   $43
    jmp     E_interrupt_I_external
E_interrupt_I_external_76:
    pushq   $44
    jmp     E_interrupt_I_external
E_interrupt_I_external_77:
    pushq   $45
    jmp     E_interrupt_I_external
E_interrupt_I_external_78:
    pushq   $46
    jmp     E_interrupt_I_external
E_interrupt_I_external_79:
    pushq   $47
    jmp     E_interrupt_I_external
E_interrupt_I_external_80:
    pushq   $48
    jmp     E_interrupt_I_external
E_interrupt_I_external_81:
    pushq   $49
    jmp     E_interrupt_I_external
E_interrupt_I_external_82:
    pushq   $50
    jmp     E_interrupt_I_external
E_interrupt_I_external_83:
    pushq   $51
    jmp     E_interrupt_I_external
E_interrupt_I_external_84:
    pushq   $52
    jmp     E_interrupt_I_external
E_interrupt_I_external_85:
    pushq   $53
    jmp     E_interrupt_I_external
E_interrupt_I_external_86:
    pushq   $54
    jmp     E_interrupt_I_external
E_interrupt_I_external_87:
    pushq   $55
    jmp     E_interrupt_I_external
E_interrupt_I_external_88:
    pushq   $56
    jmp     E_interrupt_I_external
E_interrupt_I_external_89:
    pushq   $57
    jmp     E_interrupt_I_external
E_interrupt_I_external_90:
    pushq   $58
    jmp     E_interrupt_I_external
E_interrupt_I_external_91:
    pushq   $59
    jmp     E_interrupt_I_external
E_interrupt_I_external_92:
    pushq   $60
    jmp     E_interrupt_I_external
E_interrupt_I_external_93:
    pushq   $61
    jmp     E_interrupt_I_external
E_interrupt_I_external_94:
    pushq   $62
    jmp     E_interrupt_I_external
E_interrupt_I_external_95:
    pushq   $63
    jmp     E_interrupt_I_external
E_interrupt_I_external_96:
    pushq   $64
    jmp     E_interrupt_I_external
E_interrupt_I_external_97:
    pushq   $65
    jmp     E_interrupt_I_external
E_interrupt_I_external_98:
    pushq   $66
    jmp     E_interrupt_I_external
E_interrupt_I_external_99:
    pushq   $67
    jmp     E_interrupt_I_external
E_interrupt_I_external_100:
    pushq   $68
    jmp     E_interrupt_I_external
E_interrupt_I_external_101:
    pushq   $69
    jmp     E_interrupt_I_external
E_interrupt_I_external_102:
    pushq   $70
    jmp     E_interrupt_I_external
E_interrupt_I_external_103:
    pushq   $71
    jmp     E_interrupt_I_external
E_interrupt_I_external_104:
    pushq   $72
    jmp     E_interrupt_I_external
E_interrupt_I_external_105:
    pushq   $73
    jmp     E_interrupt_I_external
E_interrupt_I_external_106:
    pushq   $74
    jmp     E_interrupt_I_external
E_interrupt_I_external_107:
    pushq   $75
    jmp     E_interrupt_I_external
E_interrupt_I_external_108:
    pushq   $76
    jmp     E_interrupt_I_external
E_interrupt_I_external_109:
    pushq   $77
    jmp     E_interrupt_I_external
E_interrupt_I_external_110:
    pushq   $78
    jmp     E_interrupt_I_external
E_interrupt_I_external_111:
    pushq   $79
    jmp     E_interrupt_I_external
E_interrupt_I_external_112:
    pushq   $80
    jmp     E_interrupt_I_external
E_interrupt_I_external_113:
    pushq   $81
    jmp     E_interrupt_I_external
E_interrupt_I_external_114:
    pushq   $82
    jmp     E_interrupt_I_external
E_interrupt_I_external_115:
    pushq   $83
    jmp     E_interrupt_I_external
E_interrupt_I_external_116:
    pushq   $84
    jmp     E_interrupt_I_external
E_interrupt_I_external_117:
    pushq   $85
    jmp     E_interrupt_I_external
E_interrupt_I_external_118:
    pushq   $86
    jmp     E_interrupt_I_external
E_interrupt_I_external_119:
    pushq   $87
    jmp     E_interrupt_I_external
E_interrupt_I_external_120:
    pushq   $88
    jmp     E_interrupt_I_external
E_interrupt_I_external_121:
    pushq   $89
    jmp     E_interrupt_I_external
E_interrupt_I_external_122:
    pushq   $90
    jmp     E_interrupt_I_external
E_interrupt_I_external_123:
    pushq   $91
    jmp     E_interrupt_I_external
E_interrupt_I_external_124:
    pushq   $92
    jmp     E_interrupt_I_external
E_interrupt_I_external_125:
    pushq   $93
    jmp     E_interrupt_I_external
E_interrupt_I_external_126:
    pushq   $94
    jmp     E_interrupt_I_external
E_interrupt_I_external_127:
    pushq   $95
    jmp     E_interrupt_I_external
E_interrupt_I_external_128:
    pushq   $96
    jmp     E_interrupt_I_external
E_interrupt_I_external_129:
    pushq   $97
    jmp     E_interrupt_I_external
E_interrupt_I_external_130:
    pushq   $98
    jmp     E_interrupt_I_external
E_interrupt_I_external_131:
    pushq   $99
    jmp     E_interrupt_I_external
E_interrupt_I_external_132:
    pushq   $100
    jmp     E_interrupt_I_external
E_interrupt_I_external_133:
    pushq   $101
    jmp     E_interrupt_I_external
E_interrupt_I_external_134:
    pushq   $102
    jmp     E_interrupt_I_external
E_interrupt_I_external_135:
    pushq   $103
    jmp     E_interrupt_I_external
E_interrupt_I_external_136:
    pushq   $104
    jmp     E_interrupt_I_external
E_interrupt_I_external_137:
    pushq   $105
    jmp     E_interrupt_I_external
E_interrupt_I_external_138:
    pushq   $106
    jmp     E_interrupt_I_external
E_interrupt_I_external_139:
    pushq   $107
    jmp     E_interrupt_I_external
E_interrupt_I_external_140:
    pushq   $108
    jmp     E_interrupt_I_external
E_interrupt_I_external_141:
    pushq   $109
    jmp     E_interrupt_I_external
E_interrupt_I_external_142:
    pushq   $110
    jmp     E_interrupt_I_external
E_interrupt_I_external_143:
    pushq   $111
    jmp     E_interrupt_I_external
E_interrupt_I_external_144:
    pushq   $112
    jmp     E_interrupt_I_external
E_interrupt_I_external_145:
    pushq   $113
    jmp     E_interrupt_I_external
E_interrupt_I_external_146:
    pushq   $114
    jmp     E_interrupt_I_external
E_interrupt_I_external_147:
    pushq   $115
    jmp     E_interrupt_I_external
E_interrupt_I_external_148:
    pushq   $116
    jmp     E_interrupt_I_external
E_interrupt_I_external_149:
    pushq   $117
    jmp     E_interrupt_I_external
E_interrupt_I_external_150:
    pushq   $118
    jmp     E_interrupt_I_external
E_interrupt_I_external_151:
    pushq   $119
    jmp     E_interrupt_I_external
E_interrupt_I_external_152:
    pushq   $120
    jmp     E_interrupt_I_external
E_interrupt_I_external_153:
    pushq   $121
    jmp     E_interrupt_I_external
E_interrupt_I_external_154:
    pushq   $122
    jmp     E_interrupt_I_external
E_interrupt_I_external_155:
    pushq   $123
    jmp     E_interrupt_I_external
E_interrupt_I_external_156:
    pushq   $124
    jmp     E_interrupt_I_external
E_interrupt_I_external_157:
    pushq   $125
    jmp     E_interrupt_I_external
E_interrupt_I_external_158:
    pushq   $126
    jmp     E_interrupt_I_external
E_interrupt_I_external_159:
    pushq   $127
    jmp     E_interrupt_I_external
E_interrupt_I_external_160:
    pushq   $128
    jmp     E_interrupt_I_external
E_interrupt_I_external_161:
    pushq   $129
    jmp     E_interrupt_I_external
E_interrupt_I_external_162:
    pushq   $130
    jmp     E_interrupt_I_external
E_interrupt_I_external_163:
    pushq   $131
    jmp     E_interrupt_I_external
E_interrupt_I_external_164:
    pushq   $132
    jmp     E_interrupt_I_external
E_interrupt_I_external_165:
    pushq   $133
    jmp     E_interrupt_I_external
E_interrupt_I_external_166:
    pushq   $134
    jmp     E_interrupt_I_external
E_interrupt_I_external_167:
    pushq   $135
    jmp     E_interrupt_I_external
E_interrupt_I_external_168:
    pushq   $136
    jmp     E_interrupt_I_external
E_interrupt_I_external_169:
    pushq   $137
    jmp     E_interrupt_I_external
E_interrupt_I_external_170:
    pushq   $138
    jmp     E_interrupt_I_external
E_interrupt_I_external_171:
    pushq   $139
    jmp     E_interrupt_I_external
E_interrupt_I_external_172:
    pushq   $140
    jmp     E_interrupt_I_external
E_interrupt_I_external_173:
    pushq   $141
    jmp     E_interrupt_I_external
E_interrupt_I_external_174:
    pushq   $142
    jmp     E_interrupt_I_external
E_interrupt_I_external_175:
    pushq   $143
    jmp     E_interrupt_I_external
E_interrupt_I_external_176:
    pushq   $144
    jmp     E_interrupt_I_external
E_interrupt_I_external_177:
    pushq   $145
    jmp     E_interrupt_I_external
E_interrupt_I_external_178:
    pushq   $146
    jmp     E_interrupt_I_external
E_interrupt_I_external_179:
    pushq   $147
    jmp     E_interrupt_I_external
E_interrupt_I_external_180:
    pushq   $148
    jmp     E_interrupt_I_external
E_interrupt_I_external_181:
    pushq   $149
    jmp     E_interrupt_I_external
E_interrupt_I_external_182:
    pushq   $150
    jmp     E_interrupt_I_external
E_interrupt_I_external_183:
    pushq   $151
    jmp     E_interrupt_I_external
E_interrupt_I_external_184:
    pushq   $152
    jmp     E_interrupt_I_external
E_interrupt_I_external_185:
    pushq   $153
    jmp     E_interrupt_I_external
E_interrupt_I_external_186:
    pushq   $154
    jmp     E_interrupt_I_external
E_interrupt_I_external_187:
    pushq   $155
    jmp     E_interrupt_I_external
E_interrupt_I_external_188:
    pushq   $156
    jmp     E_interrupt_I_external
E_interrupt_I_external_189:
    pushq   $157
    jmp     E_interrupt_I_external
E_interrupt_I_external_190:
    pushq   $158
    jmp     E_interrupt_I_external
E_interrupt_I_external_191:
    pushq   $159
    jmp     E_interrupt_I_external
E_interrupt_I_external_192:
    pushq   $160
    jmp     E_interrupt_I_external
E_interrupt_I_external_193:
    pushq   $161
    jmp     E_interrupt_I_external
E_interrupt_I_external_194:
    pushq   $162
    jmp     E_interrupt_I_external
E_interrupt_I_external_195:
    pushq   $163
    jmp     E_interrupt_I_external
E_interrupt_I_external_196:
    pushq   $164
    jmp     E_interrupt_I_external
E_interrupt_I_external_197:
    pushq   $165
    jmp     E_interrupt_I_external
E_interrupt_I_external_198:
    pushq   $166
    jmp     E_interrupt_I_external
E_interrupt_I_external_199:
    pushq   $167
    jmp     E_interrupt_I_external
E_interrupt_I_external_200:
    pushq   $168
    jmp     E_interrupt_I_external
E_interrupt_I_external_201:
    pushq   $169
    jmp     E_interrupt_I_external
E_interrupt_I_external_202:
    pushq   $170
    jmp     E_interrupt_I_external
E_interrupt_I_external_203:
    pushq   $171
    jmp     E_interrupt_I_external
E_interrupt_I_external_204:
    pushq   $172
    jmp     E_interrupt_I_external
E_interrupt_I_external_205:
    pushq   $173
    jmp     E_interrupt_I_external
E_interrupt_I_external_206:
    pushq   $174
    jmp     E_interrupt_I_external
E_interrupt_I_external_207:
    pushq   $175
    jmp     E_interrupt_I_external
E_interrupt_I_external_208:
    pushq   $176
    jmp     E_interrupt_I_external
E_interrupt_I_external_209:
    pushq   $177
    jmp     E_interrupt_I_external
E_interrupt_I_external_210:
    pushq   $178
    jmp     E_interrupt_I_external
E_interrupt_I_external_211:
    pushq   $179
    jmp     E_interrupt_I_external
E_interrupt_I_external_212:
    pushq   $180
    jmp     E_interrupt_I_external
E_interrupt_I_external_213:
    pushq   $181
    jmp     E_interrupt_I_external
E_interrupt_I_external_214:
    pushq   $182
    jmp     E_interrupt_I_external
E_interrupt_I_external_215:
    pushq   $183
    jmp     E_interrupt_I_external
E_interrupt_I_external_216:
    pushq   $184
    jmp     E_interrupt_I_external
E_interrupt_I_external_217:
    pushq   $185
    jmp     E_interrupt_I_external
E_interrupt_I_external_218:
    pushq   $186
    jmp     E_interrupt_I_external
E_interrupt_I_external_219:
    pushq   $187
    jmp     E_interrupt_I_external
E_interrupt_I_external_220:
    pushq   $188
    jmp     E_interrupt_I_external
E_interrupt_I_external_221:
    pushq   $189
    jmp     E_interrupt_I_external
E_interrupt_I_external_222:
    pushq   $190
    jmp     E_interrupt_I_external
E_interrupt_I_external_223:
    pushq   $191
    jmp     E_interrupt_I_external
E_interrupt_I_external_224:
    pushq   $192
    jmp     E_interrupt_I_external
E_interrupt_I_external_225:
    pushq   $193
    jmp     E_interrupt_I_external
E_interrupt_I_external_226:
    pushq   $194
    jmp     E_interrupt_I_external
E_interrupt_I_external_227:
    pushq   $195
    jmp     E_interrupt_I_external
E_interrupt_I_external_228:
    pushq   $196
    jmp     E_interrupt_I_external
E_interrupt_I_external_229:
    pushq   $197
    jmp     E_interrupt_I_external
E_interrupt_I_external_230:
    pushq   $198
    jmp     E_interrupt_I_external
E_interrupt_I_external_231:
    pushq   $199
    jmp     E_interrupt_I_external
E_interrupt_I_external_232:
    pushq   $200
    jmp     E_interrupt_I_external
E_interrupt_I_external_233:
    pushq   $201
    jmp     E_interrupt_I_external
E_interrupt_I_external_234:
    pushq   $202
    jmp     E_interrupt_I_external
E_interrupt_I_external_235:
    pushq   $203
    jmp     E_interrupt_I_external
E_interrupt_I_external_236:
    pushq   $204
    jmp     E_interrupt_I_external
E_interrupt_I_external_237:
    pushq   $205
    jmp     E_interrupt_I_external
E_interrupt_I_external_238:
    pushq   $206
    jmp     E_interrupt_I_external
E_interrupt_I_external_239:
    pushq   $207
    jmp     E_interrupt_I_external
E_interrupt_I_external_240:
    pushq   $208
    jmp     E_interrupt_I_external
E_interrupt_I_external_241:
    pushq   $209
    jmp     E_interrupt_I_external
E_interrupt_I_external_242:
    pushq   $210
    jmp     E_interrupt_I_external
E_interrupt_I_external_243:
    pushq   $211
    jmp     E_interrupt_I_external
E_interrupt_I_external_244:
    pushq   $212
    jmp     E_interrupt_I_external
E_interrupt_I_external_245:
    pushq   $213
    jmp     E_interrupt_I_external
E_interrupt_I_external_246:
    pushq   $214
    jmp     E_interrupt_I_external
E_interrupt_I_external_247:
    pushq   $215
    jmp     E_interrupt_I_external
E_interrupt_I_external_248:
    pushq   $216
    jmp     E_interrupt_I_external
E_interrupt_I_external_249:
    pushq   $217
    jmp     E_interrupt_I_external
E_interrupt_I_external_250:
    pushq   $218
    jmp     E_interrupt_I_external
E_interrupt_I_external_251:
    pushq   $219
    jmp     E_interrupt_I_external
E_interrupt_I_external_252:
    pushq   $220
    jmp     E_interrupt_I_external
E_interrupt_I_external_253:
    pushq   $221
    jmp     E_interrupt_I_external
E_interrupt_I_external_254:
    pushq   $222
    jmp     E_interrupt_I_external
/******************************************************************************/
