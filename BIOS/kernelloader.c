/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS kernel boot loader
*         after protected mode initialization
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”             2021‒3‒26 W
*******************************************************************************/
#include "kernelloader.h"
//==============================================================================
#define E_cpu_Z_cr0_S_mp                ( 1UL << 1 )
#define E_cpu_Z_cr0_S_em                ( 1UL << 2 )
#define E_cpu_Z_cr0_S_ts                ( 1UL << 3 )
#define E_cpu_Z_cr0_S_ne                ( 1UL << 5 )
#define E_cpu_Z_cr0_S_wp                ( 1UL << 16 )
#define E_cpu_Z_cr0_S_nw                ( 1UL << 29 )
#define E_cpu_Z_cr0_S_cd                ( 1UL << 30 )
#define E_cpu_Z_cr3_S_pwt               ( 1UL << 3 )
#define E_cpu_Z_cr3_S_pcd               ( 1UL << 4 )
#define E_cpu_Z_cr4_S_vme               ( 1UL << 0 )
#define E_cpu_Z_cr4_S_pvi               ( 1UL << 1 )
#define E_cpu_Z_cr4_S_tsd               ( 1UL << 2 )
#define E_cpu_Z_cr4_S_de                ( 1UL << 3 )
#define E_cpu_Z_cr4_S_mce               ( 1UL << 6 )
#define E_cpu_Z_cr4_S_pge               ( 1UL << 7 )
#define E_cpu_Z_cr4_S_pce               ( 1UL << 8 )
#define E_cpu_Z_cr4_S_osfxsr            ( 1UL << 9 )
#define E_cpu_Z_cr4_S_osxmmexcpt        ( 1UL << 10 )
#define E_cpu_Z_cr4_S_umip              ( 1UL << 11 )
#define E_cpu_Z_cr4_S_fsgsbase          ( 1UL << 16 )
#define E_cpu_Z_cr4_S_pcide             ( 1UL << 17 )
#define E_cpu_Z_cr4_S_osxsave           ( 1UL << 18 )
#define E_cpu_Z_cr4_S_smep              ( 1UL << 20 )
#define E_cpu_Z_cr4_S_smap              ( 1UL << 21 )
#define E_cpu_Z_cr4_S_pke               ( 1UL << 22 )
#define E_cpu_Z_cr4_S_pks               ( 1UL << 24 )
#define E_cpu_Z_cr4_S_uintr             ( 1UL << 25 )
#define E_cpu_Z_cr4_S_lam_sup           ( 1UL << 28 )
#define E_cpu_Z_page_entry_S_present    ( 1UL << 0 )
#define E_cpu_Z_page_entry_S_write      ( 1UL << 1 )
#define E_cpu_Z_page_entry_S_pwt        ( 1UL << 3 )
#define E_cpu_Z_page_entry_S_pcd        ( 1UL << 4 )
#define E_cpu_Z_page_entry_S_pat        ( 1UL << 7 )
#define E_cpu_Z_gdt_Z_data_S_write      ( 1UL << 9 )
#define E_cpu_Z_gdt_Z_type_S_code       ( 1UL << 11 )
#define E_cpu_Z_gdt_S_code_data         ( 1UL << 12 )
#define E_cpu_Z_gdt_S_present           ( 1UL << 15 )
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1UL << 21 )
#define E_cpu_Z_gdt_S_granularity       ( 1UL << 23 )
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1UL << 9 )
#define E_cpu_Z_gdt_Z_type_S_tss        ( 9UL << 8 )
//==============================================================================
#define J_descriptor_low(base,limit,flags) \
  ( (limit) & ~( ~0 << 16 )) \
  | (( (base) & ~( ~0 << 16 )) << 16 ) \
  | (((( (base) >> 16 ) & 0xff ) | ( (limit) & ( 0xf << 16 )) | ( (base) & ( 0xff << 24 )) | E_cpu_Z_gdt_S_present | (flags) ) << 32 )
//==============================================================================
struct __attribute__ (( __packed__ )) E_main_Z_video
{ N32 p;
  N16 line_width;
  N16 width, height;
  N8 bits_per_pixel;
  N8 blue_size, blue_start;
  N8 green_size, green_start;
  N8 red_size, red_start;
};
struct __attribute__ ((packed)) E_interrupt_Z_gd
{ N16 limit;
  N base;
};
struct __attribute__ (( __packed__ )) E_main_I_tss
{ N32 reserved_0;
  N rsp[3];
  N ist[8];
  N reserved_1;
  N16 reserved_2;
  N16 io_map_base_address;
};
struct __attribute__ (( __packed__ )) Q_elf_Z_rela_entry
{ N64 offset;
  N32 type, sym;
  N64 addend;
};
struct __attribute__ (( __packed__ )) Q_exe_Z_rela_plt_entry
{ N64 offset;
  N32 sym;
};
struct __attribute__ (( __packed__ )) Q_exe_Z_export_entry
{ N32 sym;
  N64 offset;
};
typedef void ( *E_main_Z_remap_jump )( N E_main_S_pml4, N delta );
struct E_main_Z_kernel_data
{ struct Q_elf_Z_rela_entry *rela;
  struct Q_exe_Z_rela_plt_entry *rela_plt;
  struct Q_exe_Z_export_entry *exports;
  Pc dynstr;
  Pc got;
  Pc text;
  Pc data;
  P entry;
};
//==============================================================================
extern B E_acpi_S_pic_mode;
extern P E_acpi_S_apic_content;
extern N E_acpi_S_apic_content_l;
//------------------------------------------------------------------------------
extern struct E_interrupt_Z_gd S_gd;
extern N S_gdt[], S_ldt[];
//------------------------------------------------------------------------------
extern char E_main_S_relocation_data[];
extern P E_remap_jump_I, E_remap_jump_I_end;
extern P E_mp_init_I, E_mp_init_I_end, E_mp_init_I_reloc_1, E_mp_init_I_reloc_2, E_mp_init_I_reloc_3, E_mp_init_I_reloc_4, E_mp_init_I_reloc_5, E_mp_init_I_reloc_6, E_mp_init_I_reloc_7, E_mp_init_I_reloc_8, E_mp_init_S_gdt_32, E_mp_init_S_gdt, E_mp_init_S_gd_32, E_mp_init_S_gd;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct E_main_Z_kernel_args E_main_S_kernel_args;
struct E_main_I_tss E_main_S_tss;
N32 E_main_S_sata_ahci_addresses[8];
N8 E_main_S_sata_ahci_n;
N64 E_main_S_ethernet_address, E_main_S_ethernet_eeprom_address;
N E_main_S_framebuffer_physical, E_main_S_framebuffer_virtual;
struct E_main_Z_memory_map_entry *E_main_Z_memory_table_S;
N E_main_Z_memory_table_S_end;
N E_main_Z_memory_table_pre_S_virtual_address;
N E_main_S_pml4, E_main_S_start_end_address;
//==============================================================================
void
E_main_I_out_8( N16 port
, N8 v
){  __asm__ volatile (
    "\n" "out   %0,%1"
    :
    : "a" (v), "d" (port)
    );
}
N32
E_main_I_in_32( N16 port
){  N32 v;
    __asm__ volatile (
    "\n" "in    %1,%0"
    : "=a" (v)
    : "d" (port)
    );
    return v;
}
void
E_main_I_out_32( N16 port
, N32 v
){  __asm__ volatile (
    "\n" "out   %0,%1"
    :
    : "a" (v), "d" (port)
    );
}
//------------------------------------------------------------------------------
N64
E_main_Q_msr_R( N32 i
){  N32 l, h;
    __asm__ volatile (
    "\n" "rdmsr"
    : "=a" (l), "=d" (h)
    : "c" (i)
    );
    return (( N64 )h << 32 ) | l;
}
void
E_main_Q_msr_P( N32 i
, N64 v
){  __asm__ volatile (
    "\n" "wrmsr"
    :
    : "c" (i), "d" ( v >> 32 ), "a" ( v & 0xffffffff )
    );
}
//==============================================================================
void
E_main_Q_memory_map_I_align( void
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   memory_map->size = E_simple_Z_n_I_align_up_to_v2( memory_map->physical_start % E_mem_S_page_size + memory_map->size, E_mem_S_page_size );
        memory_map->physical_start = E_simple_Z_n_I_align_down_to_v2( memory_map->physical_start, E_mem_S_page_size );
        memory_map++;
    }
}
//DFN Nie usuwa wszystkich przecinających się bloków. Liczy na to, że niektóre bezsensowne przecięcia nie wystąpią.
__attribute__ (( __warn_unused_result__ ))
N
E_main_Q_memory_map_I_remove_overlapped( void
){  struct E_main_Z_memory_map_entry *entry = E_main_Z_memory_table_S;
    while( entry != (P)E_main_Z_memory_table_S_orig_end
    && !entry->size
    )
        entry++;
    if( entry == (P)E_main_Z_memory_table_S_orig_end )
        return 0;
    struct E_main_Z_memory_map_entry *next_entry = entry;
    O{  do
            next_entry++;
        while( next_entry != (P)E_main_Z_memory_table_S_orig_end
        && !next_entry->size
        );
        if( next_entry == (P)E_main_Z_memory_table_S_orig_end )
            break;
        N64 entry_physical_start = entry->physical_start;
        N64 entry_size = entry->size;
        N32 entry_type = entry->type;
        N64 next_entry_physical_start = next_entry->physical_start;
        N64 next_entry_size = next_entry->size;
        N32 next_entry_type = next_entry->type;
        if( entry_physical_start + entry_size >= next_entry_physical_start
        && entry_type == next_entry_type
        ) // Bloki są scalane w jeden.
        {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                entry->size = next_entry_physical_start + next_entry_size - entry_physical_start;
            E_mem_Q_blk_I_copy( E_main_Z_memory_table_S + 1, E_main_Z_memory_table_S, ( next_entry - E_main_Z_memory_table_S ) * sizeof( *E_main_Z_memory_table_S ));
            E_main_Z_memory_table_S++;
            entry++;
        }else if( entry_physical_start + entry_size > next_entry_physical_start )
            if( entry_type == E_main_Z_memory_table_Z_memory_type_S_available
            || ( entry_type != E_main_Z_memory_table_Z_memory_type_S_boot_loader
              && entry_type != E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
              && ( !next_entry_type
                || next_entry_type == E_main_Z_memory_table_Z_memory_type_S_bad
                || next_entry_type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            ))
            || ( entry_type == E_main_Z_memory_table_Z_memory_type_S_reserved
              && ( next_entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || next_entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || next_entry_type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
            ))
            ) // Pierwszy blok ustępuje drugiemu.
            {   if( entry_physical_start + entry_size > next_entry_physical_start + next_entry_size )
                {   struct E_main_Z_memory_map_entry *new_entry = next_entry + 1;
                    while( new_entry != (P)E_main_Z_memory_table_S_orig_end
                    && new_entry->physical_start < next_entry_physical_start + next_entry_size
                    )
                        new_entry++;
                    E_mem_Q_blk_I_copy( E_main_Z_memory_table_S - 1, E_main_Z_memory_table_S, ( new_entry - E_main_Z_memory_table_S ) * sizeof( *E_main_Z_memory_table_S ));
                    E_main_Z_memory_table_S--;
                    entry--;
                    next_entry--;
                    new_entry--;
                    new_entry->physical_start = next_entry_physical_start + next_entry_size;
                    new_entry->size = entry_physical_start + entry_size - new_entry->physical_start;
                    new_entry->type = entry_type;
                }
                if( entry_physical_start != next_entry_physical_start )
                    entry->size = next_entry_physical_start - entry_physical_start;
                else
                {   E_mem_Q_blk_I_copy( E_main_Z_memory_table_S + 1, E_main_Z_memory_table_S, ( entry - E_main_Z_memory_table_S ) * sizeof( *E_main_Z_memory_table_S ));
                    if( entry == E_main_Z_memory_table_S )
                    {   E_main_Z_memory_table_S++;
                        goto Entry_next;
                    }
                    E_main_Z_memory_table_S++;
                    struct E_main_Z_memory_map_entry *entry_ = entry;
                    while( entry_ != (P)E_main_Z_memory_table_S
                    && !entry_->size
                    )
                        entry_--;
                    if( entry_ != entry
                    && entry_->size
                    )
                        entry = entry_;
                    else
                        goto Entry_next;
                }
            }else if( next_entry_type == E_main_Z_memory_table_Z_memory_type_S_available
            || (( !entry_type
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_bad
                || entry_type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
              )
              && next_entry_type != E_main_Z_memory_table_Z_memory_type_S_boot_loader
              && next_entry_type != E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )
            || (( entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
              )
              && ( next_entry_type == E_main_Z_memory_table_Z_memory_type_S_reserved
            ))
            ) // Drugi blok ustępuje pierwszemu.
            {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                {   struct E_main_Z_memory_map_entry *new_entry = next_entry + 1;
                    while( new_entry != (P)E_main_Z_memory_table_S_orig_end
                    && new_entry->physical_start < entry_physical_start + entry_size
                    )
                        new_entry++;
                    new_entry--;
                    E_mem_Q_blk_I_copy( next_entry, next_entry + 1, ( new_entry - next_entry ) * sizeof( *E_main_Z_memory_table_S ));
                    new_entry->physical_start = entry_physical_start + entry_size;
                    new_entry->size = next_entry_physical_start + next_entry_size - new_entry->physical_start;
                    new_entry->type = next_entry_type;
                    if( new_entry - next_entry )
                        next_entry--;
                }else
                {   E_mem_Q_blk_I_copy( E_main_Z_memory_table_S + 1, E_main_Z_memory_table_S, ( next_entry - E_main_Z_memory_table_S ) * sizeof( *E_main_Z_memory_table_S ));
                    E_main_Z_memory_table_S++;
                    entry++;
                }
            }else
                return ~0;
        else
Entry_next:
        {   do
                entry++;
            while( !entry->size );
            next_entry = entry;
        }
    }
    return 0;
}
void
E_main_Q_memory_map_I_remove_bad( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_orig_end - E_main_Z_memory_table_S;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if(( E_main_Z_memory_table_S[ i - 1 ].size
              && (( E_main_Z_memory_table_S[ i - 1 ].type
                  && E_main_Z_memory_table_S[ i - 1 ].type < E_main_Z_memory_table_Z_memory_type_S_bad
                )
                || ( E_main_Z_memory_table_S[ i - 1 ].type >= ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
                  && E_main_Z_memory_table_S[ i - 1 ].type <= ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )))
            && (( !E_main_Z_memory_table_S[i].size
                || !E_main_Z_memory_table_S[i].type
                || ( E_main_Z_memory_table_S[i].type >= E_main_Z_memory_table_Z_memory_type_S_bad
                  && ( E_main_Z_memory_table_S[i].type < ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
                    || E_main_Z_memory_table_S[i].type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )))
              || E_main_Z_memory_table_S[ i - 1 ].physical_start > E_main_Z_memory_table_S[i].physical_start
            ))
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_Z_memory_table_S[ i - 1 ], E_main_Z_memory_table_S[i] );
                new_n = i;
            }
        }
        n = new_n;
    }
    n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_orig_end - E_main_Z_memory_table_S;
    for_n( i, n )
        if( E_main_Z_memory_table_S[i].size
        && (( E_main_Z_memory_table_S[i].type
            && E_main_Z_memory_table_S[i].type < E_main_Z_memory_table_Z_memory_type_S_bad
          )
          || ( E_main_Z_memory_table_S[i].type >= ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
            && E_main_Z_memory_table_S[i].type <= ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )))
            break;
    E_main_Z_memory_table_S = &E_main_Z_memory_table_S[i];
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_sort_physical( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_Z_memory_table_S;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
            if( E_main_Z_memory_table_S[ i - 1 ].physical_start > E_main_Z_memory_table_S[i].physical_start )
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_Z_memory_table_S[ i - 1 ], E_main_Z_memory_table_S[i] );
                new_n = i;
            }
        n = new_n;
    }
}
void
E_main_Q_memory_map_I_join_physical( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_orig_end - E_main_Z_memory_table_S;
    for_n( i, n - 1 )
        if( E_main_Z_memory_table_S[i].type == E_main_Z_memory_table_S[ i + 1 ].type
        && E_main_Z_memory_table_S[i].physical_start + E_main_Z_memory_table_S[i].size == E_main_Z_memory_table_S[ i + 1 ].physical_start
        )
        {   E_main_Z_memory_table_S[i].size += E_main_Z_memory_table_S[ i + 1 ].size;
            E_mem_Q_blk_I_copy( E_main_Z_memory_table_S + 1, E_main_Z_memory_table_S, ( i + 1 ) * sizeof( *E_main_Z_memory_table_S ));
            E_main_Z_memory_table_S++;
            i--;
            n--;
        }
}
void
E_main_Q_memory_map_I_sort_virtual( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_orig_end - E_main_Z_memory_table_S;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
            if( E_main_Z_memory_table_S[ i - 1 ].virtual_start > E_main_Z_memory_table_S[i].virtual_start )
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_Z_memory_table_S[ i - 1 ], E_main_Z_memory_table_S[i] );
                new_n = i;
            }
        n = new_n;
    }
}
P
E_main_Z_p_I_to_virtual_pre( P p
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    if( (N)p < E_main_S_boot_loader_orig_end )
        return p;
    if( (N)p < 0xe0000 )
        return (P)~0;
    if( (N)p < 0x100000 )
        return (P)( E_main_S_boot_loader_orig_end + (N)p - 0xe0000 );
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if(( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        )
        && (N)p >= memory_map->physical_start
        && (N)p < memory_map->physical_start + memory_map->size
        )
            return (P)( memory_map->virtual_start + ( (N)p - memory_map->physical_start ));
        memory_map++;
    }
    return (P)~0;
}
P
E_main_Z_p_I_to_virtual( P p
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( (N)p >= memory_map->physical_start
        && (N)p < memory_map->physical_start + memory_map->size
        )
            return (P)( memory_map->virtual_start + ( (N)p - memory_map->physical_start ));
        memory_map++;
    }
    return 0;
}
P
E_main_Z_p_I_to_physical( P p
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( (N)p >= memory_map->virtual_start
        && (N)p < memory_map->virtual_start + memory_map->size
        )
            return (P)( memory_map->physical_start + ( (N)p - memory_map->virtual_start ));
        memory_map++;
    }
    return 0;
}
//------------------------------------------------------------------------------
N
E_main_Q_memory_map_R_reserved_size( void
){  N size = 0;
    struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_size_0( void
){  N size = E_mem_S_page_size;
    struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_size( void
){  N size = E_mem_S_page_size;
    struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_saved_n( void
){  N n = 0;
    struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            n++;
        memory_map++;
    }
    return n;
}
void
E_main_Q_memory_map_I_set_virtual_0_I_entry( struct E_main_Z_memory_map_entry *memory_map
, N loader_start
, N loader_end
, N *next_virtual_address
, B *processor_start_page_computed
, B *loader_computed
){  memory_map->virtual_start = *next_virtual_address;
    *next_virtual_address += memory_map->size;
    if( !*processor_start_page_computed
    && !*loader_computed
    && (( E_main_S_kernel_args.processor_start_page < loader_start
        && *next_virtual_address >= loader_start
      )
      || ( !( E_main_S_kernel_args.processor_start_page < loader_start )
        && *next_virtual_address >= E_main_S_kernel_args.processor_start_page
    )))
    {   if( E_main_S_kernel_args.processor_start_page < loader_start )
        {   memory_map->size = E_main_S_kernel_args.processor_start_page - memory_map->virtual_start;
            struct E_main_Z_memory_map_entry *memory_map_new_entry_;
            if( E_main_S_kernel_args.processor_start_page + E_mem_S_page_size != loader_start )
            {   memory_map_new_entry_ = --E_main_Z_memory_table_S;
                memory_map_new_entry_->type = memory_map->type;
                memory_map_new_entry_->virtual_start = E_main_S_kernel_args.processor_start_page + E_mem_S_page_size;
                memory_map_new_entry_->size = loader_start - memory_map_new_entry_->virtual_start;
                memory_map_new_entry_->physical_start = memory_map->physical_start + memory_map->size;
            }
            if( *next_virtual_address != loader_start )
            {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_Z_memory_table_S;
                memory_map_new_entry->type = memory_map->type;
                memory_map_new_entry->size = *next_virtual_address - ( memory_map->virtual_start + memory_map->size );
                if( E_main_S_kernel_args.processor_start_page + E_mem_S_page_size != loader_start )
                {   memory_map_new_entry->size -= memory_map_new_entry_->size;
                    memory_map_new_entry->physical_start = memory_map_new_entry_->physical_start + memory_map_new_entry_->size;
                }else
                    memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
                memory_map_new_entry->virtual_start = loader_end;
            }
        }else
        {   memory_map->size = loader_start - memory_map->virtual_start;
            struct E_main_Z_memory_map_entry *memory_map_new_entry_;
            if( loader_end != E_main_S_kernel_args.processor_start_page )
            {   memory_map_new_entry_ = --E_main_Z_memory_table_S;
                memory_map_new_entry_->type = memory_map->type;
                memory_map_new_entry_->virtual_start = loader_end;
                memory_map_new_entry_->size = E_main_S_kernel_args.processor_start_page - memory_map_new_entry_->virtual_start;
                memory_map_new_entry_->physical_start = memory_map->physical_start + memory_map->size;
            }
            if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
            {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_Z_memory_table_S;
                memory_map_new_entry->type = memory_map->type;
                memory_map_new_entry->size = *next_virtual_address - ( memory_map->virtual_start + memory_map->size );
                if( loader_end != E_main_S_kernel_args.processor_start_page )
                {   memory_map_new_entry->size -= memory_map_new_entry_->size;
                    memory_map_new_entry->physical_start = memory_map_new_entry_->physical_start + memory_map_new_entry_->size;
                }else
                    memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
                memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + E_mem_S_page_size;
            }
        }
        *next_virtual_address += E_mem_S_page_size + loader_end - loader_start;
        *processor_start_page_computed = yes;
        *loader_computed = yes;
    }else if( !*processor_start_page_computed
    && *next_virtual_address >= E_main_S_kernel_args.processor_start_page
    )
    {   if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_Z_memory_table_S;
            memory_map_new_entry->type = memory_map->type;
            memory_map_new_entry->size = *next_virtual_address - E_main_S_kernel_args.processor_start_page;
            memory_map->size -= memory_map_new_entry->size;
            memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
            memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + E_mem_S_page_size;
        }
        *next_virtual_address += E_mem_S_page_size;
        *processor_start_page_computed = yes;
    }else if( !*loader_computed
    && *next_virtual_address >= loader_start
    )
    {   if( *next_virtual_address != loader_start )
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_Z_memory_table_S;
            memory_map_new_entry->type = memory_map->type;
            memory_map_new_entry->size = *next_virtual_address - loader_start;
            memory_map->size -= memory_map_new_entry->size;
            memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
            memory_map_new_entry->virtual_start = loader_end;
        }
        *next_virtual_address += loader_end - loader_start;
        *loader_computed = yes;
    }
}
void
E_main_Q_memory_map_I_set_virtual_0( void
){  struct E_main_Z_memory_map_entry *memory_map_orig = E_main_Z_memory_table_S;
    struct E_main_Z_memory_map_entry *memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            memory_map->virtual_start = memory_map->physical_start;
        memory_map++;
    }
    B processor_start_page_computed = no, loader_computed = no;
    N next_virtual_address = E_mem_S_page_size;
    memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
            E_main_Q_memory_map_I_set_virtual_0_I_entry( memory_map
            , E_main_S_boot_loader_orig_start, E_main_S_boot_loader_orig_end
            , &next_virtual_address
            , &processor_start_page_computed
            , &loader_computed
            );
        memory_map++;
    }
    memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type != E_main_Z_memory_table_Z_memory_type_S_available
        && memory_map->type != E_main_Z_memory_table_Z_memory_type_S_boot_loader
        && memory_map->type != E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            memory_map->virtual_start = ~0;
        memory_map++;
    }
}
void
E_main_Q_memory_map_I_set_virtual_I_entry( struct E_main_Z_memory_map_entry *memory_map
, N *next_virtual_address
, B *processor_start_page_computed
){  memory_map->virtual_start = *next_virtual_address;
    *next_virtual_address += memory_map->size;
    if( !*processor_start_page_computed
    && *next_virtual_address >= E_main_S_kernel_args.processor_start_page
    )
    {   if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_Z_memory_table_S;
            memory_map_new_entry->type = memory_map->type;
            memory_map_new_entry->size = *next_virtual_address - E_main_S_kernel_args.processor_start_page;
            memory_map->size -= memory_map_new_entry->size;
            memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
            memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + E_mem_S_page_size;
        }
        *next_virtual_address += E_mem_S_page_size;
        *processor_start_page_computed = yes;
    }
}
void
E_main_Q_memory_map_I_set_virtual( B reserved_from_end
){  struct E_main_Z_memory_map_entry *memory_map_orig = E_main_Z_memory_table_S;
    struct E_main_Z_memory_map_entry *memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page )
        {   memory_map->virtual_start = memory_map->physical_start;
            break;
        }
        memory_map++;
    }
    B processor_start_page_computed = no;
    N next_virtual_address = E_mem_S_page_size;
    if( reserved_from_end )
    {   memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
            || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
            || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
            )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            }
            memory_map++;
        }
    }else
    {   memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
            || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
            || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
            )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            }
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
    }
    memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved )
            memory_map->virtual_start = ~0;
        memory_map++;
    }
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_main_I_allocate_page_table_I_next_page( B reserved_from_end
, struct E_main_Z_memory_map_entry **entry
, N *size
){  if( reserved_from_end )
    {   if( !*size )
        {   ( *entry )--;
            while(( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
            || ( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )
                ( *entry )--;
            *size = ( *entry )->size;
        }
        *size -= E_mem_S_page_size;
    }else
        if(( *size += E_mem_S_page_size ) == ( *entry )->size )
        {   ( *entry )++;
            while(( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
            || ( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )
                ( *entry )++;
            *size = 0;
        }
}
void
E_main_I_allocate_page_table_I_next_physical_address( struct E_main_Z_memory_map_entry **entry
, N *size
){  if(( *size += E_mem_S_page_size ) == ( *entry )->size )
    {   ( *entry )++;
        *size = 0;
    }
}
_inline
void
E_main_Q_cr3_P( N pml4
){  __asm__ volatile (
    "\n" "mov   %0,%%cr3"
    :
    : "r" ( pml4 )
    : "memory"
    );
}
__attribute__ (( __warn_unused_result__ ))
N
E_main_I_allocate_page_table_pre( void
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    N table_address = E_main_S_boot_loader_orig_start; // Start na obecnej tablicy pamięci, malejąco.
    volatile N *pml4 = (P)( table_address -= E_mem_S_page_size );
    N physical_address = -E_mem_S_page_size;
    B end = no;
    const N table_n = E_mem_S_page_size / sizeof(N);
    memory_map--;
    for_n( pml4_i, table_n )
        if( !end )
        {   volatile N *pdpt = (P)( table_address -= E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   volatile N *pd = (P)( table_address -= E_mem_S_page_size );
                    for_n( pd_i, table_n )
                        if( !end )
                        {   if( pd_i )
                                return ~0;
                            volatile N *pt = (P)( table_address -= E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    physical_address += E_mem_S_page_size;
                                    if( physical_address == E_main_S_boot_loader_orig_end )
                                        physical_address = 0xe0000;
                                    else if( physical_address == 0x100000
                                    || ( physical_address > 0x100000
                                      && physical_address == memory_map->physical_start + memory_map->size
                                    ))
                                    {   while( ++memory_map != (P)E_main_Z_memory_table_S_orig_end )
                                            if(( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                                              || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                                            )
                                            && memory_map->physical_start >= 0x100000
                                            )
                                                break;
                                        if( memory_map == (P)E_main_Z_memory_table_S_orig_end )
                                        {   end = yes;
                                            pt_i--;
                                            E_main_Z_memory_table_pre_S_virtual_address = virtual_address;
                                            continue;
                                        }
                                        physical_address = memory_map->physical_start;
                                        memory_map->virtual_start = virtual_address;
                                    }
                                    pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                }else
                                    pt[ pt_i ] = 0;
                            if( !end
                            && physical_address == memory_map->physical_start + memory_map->size - E_mem_S_page_size
                            )
                            {   while( ++memory_map != (P)E_main_Z_memory_table_S_orig_end )
                                    if(( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                                      || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                                    )
                                    && memory_map->physical_start >= 0x100000
                                    )
                                        break;
                                if( memory_map == (P)E_main_Z_memory_table_S_orig_end )
                                {   end = yes;
                                    E_main_Z_memory_table_pre_S_virtual_address = ~0;
                                }
                            }
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pt;
                        }else
                            pd[ pd_i ] = 0;
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pd;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pdpt;
        }else
            pml4[ pml4_i ] = 0;
    E_main_Q_cr3_P( (N)pml4 );
    return 0;
}
__attribute__ (( __warn_unused_result__ ))
N
E_main_I_complete_page_table_pre( void
){  if( !~E_main_Z_memory_table_pre_S_virtual_address )
        return ~0;
    N table_address = E_main_S_boot_loader_orig_start; // Start na obecnej tablicy pamięci, malejąco.
    volatile N *pml4 = (P)( table_address -= E_mem_S_page_size );
    N physical_address = E_main_Z_memory_table_S->physical_start - E_mem_S_page_size;
    E_main_Z_memory_table_S->virtual_start = E_main_Z_memory_table_pre_S_virtual_address;
    B end = no;
    const N table_n = E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
        if( !end )
        {   volatile N *pdpt = (P)( table_address -= E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   volatile N *pd = (P)( table_address -= E_mem_S_page_size );
                    for_n( pd_i, table_n )
                        if( !end )
                        {   if( pd_i )
                                return ~0;
                            volatile N *pt = (P)( table_address -= E_mem_S_page_size );
                            N pt_i = ( E_main_Z_memory_table_pre_S_virtual_address / E_mem_S_page_size ) & (( 1 << 9 ) - 1 );
                            for( ; pt_i != table_n; pt_i++ )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    physical_address += E_mem_S_page_size;
                                    if( physical_address == E_main_Z_memory_table_S->physical_start + E_main_Z_memory_table_S->size )
                                    {   E_main_Z_memory_table_pre_S_virtual_address = virtual_address;
                                        goto End;
                                    }
                                    pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                }
                            if( !end
                            && physical_address == E_main_Z_memory_table_S->physical_start + E_main_Z_memory_table_S->size - E_mem_S_page_size
                            )
                                E_main_Z_memory_table_pre_S_virtual_address = ~0;
                        }
                }
        }
End:E_main_Q_cr3_P( (N)pml4 );
    return 0;
}
void
E_main_I_allocate_page_table_0_I_allocate( N max_memory
, struct E_main_Z_memory_map_entry **memory_map_entry_available
, N *size
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    E_main_I_allocate_page_table_I_next_page( no, memory_map_entry_available, size );
    N pml4_physical = ( *memory_map_entry_available )->physical_start + *size;
    volatile N *pml4 = (P)(( *memory_map_entry_available )->virtual_start + *size );
    N physical_size = -E_mem_S_page_size;
    B end = no;
    const N table_n = E_mem_S_page_size / sizeof(N);
    max_memory -= E_mem_S_page_size;
    for_n( pml4_i, table_n )
        if( !end )
        {   E_main_I_allocate_page_table_I_next_page( no, memory_map_entry_available, size );
            N pdpt_physical = ( *memory_map_entry_available )->physical_start + *size;
            volatile N *pdpt = (P)(( * memory_map_entry_available )->virtual_start + *size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   E_main_I_allocate_page_table_I_next_page( no, memory_map_entry_available, size );
                    N pd_physical = ( *memory_map_entry_available )->physical_start + *size;
                    volatile N *pd = (P)(( *memory_map_entry_available )->virtual_start + *size );
                    for_n( pd_i, table_n )
                        if( !end )
                        {   E_main_I_allocate_page_table_I_next_page( no, memory_map_entry_available, size );
                            N pt_physical = ( *memory_map_entry_available )->physical_start + *size;
                            volatile N *pt = (P)(( *memory_map_entry_available )->virtual_start + *size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i  << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    if( virtual_address == max_memory )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   E_main_I_allocate_page_table_I_next_physical_address( &memory_map, &physical_size );
                                        N physical_address = memory_map->physical_start + physical_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                    }
                                }else
                                    pt[ pt_i ] = 0;
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pt_physical;
                        }else
                            pd[ pd_i ] = 0;
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pd_physical;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pdpt_physical;
        }else
            pml4[ pml4_i ] = 0;
    E_main_Q_cr3_P( pml4_physical );
}
void
E_main_I_allocate_page_table_0( N max_memory
){  N max_memory_step = 16 * 1024 * 1024; // Maksimum 16 MiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 35 KiB.
    N max_memory_ = J_min( max_memory_step, max_memory ) - E_mem_S_page_size;
    N table_address = (N)E_simple_Z_p_I_align_down_to_v2( E_main_Z_memory_table_S, E_mem_S_page_size ); // Start poniżej tablicy pamięci, malejąco.
    struct E_main_Z_memory_map_entry *memory_map_ = E_main_Z_memory_table_S;
    volatile N *pml4 = (P)( table_address -= E_mem_S_page_size );
    N physical_size = -E_mem_S_page_size;
    B end = no;
    const N table_n = E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
        if( !end )
        {   volatile N *pdpt = (P)( table_address -= E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   volatile N *pd = (P)( table_address -= E_mem_S_page_size );
                    for_n( pd_i, table_n )
                        if( !end )
                        {   volatile N *pt = (P)( table_address -= E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    if( virtual_address == max_memory_ )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   E_main_I_allocate_page_table_I_next_physical_address( &memory_map_, &physical_size );
                                        N physical_address = memory_map_->physical_start + physical_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                    }
                                }else
                                    pt[ pt_i ] = 0;
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pt;
                        }else
                            pd[ pd_i ] = 0;
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pd;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pdpt;
        }else
            pml4[ pml4_i ] = 0;
    E_main_Q_cr3_P( (N)pml4 );
    struct E_main_Z_memory_map_entry *memory_map_entry_available = E_main_Z_memory_table_S;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available++;
    N size = -E_mem_S_page_size;
    if( max_memory <= max_memory_step )
        goto End;
    max_memory_step = 2UL * 1024 * 1024 * 1024; // Maksimum 2 GiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 4 MiB.
    E_main_I_allocate_page_table_0_I_allocate( J_min( max_memory_step, max_memory )
    , &memory_map_entry_available, &size
    );
    if( max_memory <= max_memory_step )
        goto End;
    max_memory_step = 256UL * 1024 * 1024 * 1024; // Maksimum 256 GiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 512 MiB.
    E_main_I_allocate_page_table_0_I_allocate( J_min( max_memory_step, max_memory )
    , &memory_map_entry_available, &size
    );
    if( max_memory <= max_memory_step )
        goto End;
    max_memory_step = 16UL * 1024 * 1024 * 1024 * 1024; // Maksimum 16 TiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 64 GiB.
    E_main_I_allocate_page_table_0_I_allocate( J_min( max_memory_step, max_memory )
    , &memory_map_entry_available, &size
    );
    if( max_memory <= max_memory_step )
        goto End;
    E_main_I_allocate_page_table_0_I_allocate( max_memory // Maksimum 256 TiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 513 GiB.
    , &memory_map_entry_available, &size
    );
End:E_main_S_start_end_address = memory_map_entry_available->virtual_start + size + E_mem_S_page_size;
}
void
E_main_Q_loader_I_relocate( N loader_start_0
, N loader_start
){  struct Q_elf_Z_rela_entry *image_relocation = (P)E_main_S_relocation_data;
    O{  switch( image_relocation->type )
        { case 0:
                return;
          case 1: // R_X86_64_64
          case 8: // R_X86_64_RELATIVE
                *( N64 * )( loader_start_0 + image_relocation->offset ) = loader_start + image_relocation->addend;
                break;
          case 10: // R_X86_64_32
          case 11: // R_X86_64_32S
                *( N32 * )( loader_start_0 + image_relocation->offset ) = loader_start + image_relocation->addend;
                break;
        }
        image_relocation++;
    }
}
void
E_main_I_allocate_page_table_1( N max_memory
, N memory_size_0
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    struct E_main_Z_memory_map_entry *memory_map_entry_available = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_orig_end - 1;
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available--;
    N size = memory_map_entry_available->size - E_mem_S_page_size;
    N pml4_physical = memory_map_entry_available->physical_start + size;
    volatile N *pml4 = (P)( memory_size_0 -= E_mem_S_page_size );
    N physical_size = -E_mem_S_page_size;
    B end = no;
    E_main_S_kernel_args.additional_pages = 0;
    N additional_pages_ = 1UL << 15; //CONF Maksymalna liczba (‘guard pages’ stosów) ‹zadań› w systemie.
    const N table_n = E_mem_S_page_size / sizeof(N);
    max_memory -= E_mem_S_page_size;
    for_n( pml4_i, table_n )
        if( !end
        || additional_pages_
        )
        {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
            N pdpt_physical = memory_map_entry_available->physical_start + size;
            volatile N *pdpt = (P)( memory_size_0 -= E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end
                || additional_pages_
                )
                {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
                    N pd_physical = memory_map_entry_available->physical_start + size;
                    volatile N *pd = (P)( memory_size_0 -= E_mem_S_page_size );
                    for_n( pd_i, table_n )
                        if( !end
                        || additional_pages_
                        )
                        {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
                            N pt_physical = memory_map_entry_available->physical_start + size;
                            volatile N *pt = (P)( memory_size_0 -= E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    if( virtual_address == max_memory )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   E_main_I_allocate_page_table_I_next_physical_address( &memory_map, &physical_size );
                                        N physical_address = memory_map->physical_start + physical_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                        B sata_ahci = no;
                                        for_n( i, E_main_S_sata_ahci_n )
                                            if( physical_address == E_main_S_sata_ahci_addresses[i]
                                            || physical_address == E_main_S_sata_ahci_addresses[i] + E_mem_S_page_size
                                            )
                                            {   sata_ahci = yes;
                                                break;
                                            }
                                        if( sata_ahci
                                        || physical_address == (N)E_main_S_kernel_args.local_apic_address
                                        || physical_address == (N)E_main_S_kernel_args.io_apic_address
                                        || ( E_main_S_kernel_args.pcie_base_address
                                          && physical_address >= (N)E_main_S_kernel_args.pcie_base_address
                                          && physical_address < (N)E_main_S_kernel_args.pcie_base_address + 256 * 32 * 8 * 4096
                                        )
                                        || ( E_main_S_ethernet_address
                                          && physical_address >= E_main_S_ethernet_address
                                          && physical_address < E_main_S_ethernet_address + 0x20000
                                        )
                                        || ( E_main_S_ethernet_eeprom_address
                                          && physical_address == E_main_S_ethernet_eeprom_address
                                        ))
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pwt | E_cpu_Z_page_entry_S_pcd;
                                        else if( physical_address >= E_simple_Z_n_I_align_down_to_v2( E_main_S_framebuffer_physical, E_mem_S_page_size )
                                        && physical_address < E_simple_Z_n_I_align_up_to_v2( E_main_S_framebuffer_physical + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, E_mem_S_page_size )
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pat;
                                    }
                                }else
                                {   pt[ pt_i ] = 0;
                                    if( additional_pages_ )
                                        additional_pages_--;
                                    E_main_S_kernel_args.additional_pages++;
                                }
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pt_physical;
                        }else
                            pd[ pd_i ] = 0;
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pd_physical;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pdpt_physical;
        }else
            pml4[ pml4_i ] = 0;
    E_main_S_pml4 = pml4_physical;
    E_main_S_start_end_address = memory_map_entry_available->virtual_start + size;
}
void
E_main_I_allocate_page_table( N max_memory
){  struct E_main_Z_memory_map_entry *memory_map = E_main_Z_memory_table_S;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    struct E_main_Z_memory_map_entry *memory_map_entry_available = E_main_Z_memory_table_S;
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available++;
    N size = 0;
    N pml4_physical = memory_map_entry_available->physical_start + size;
    volatile N *pml4 = (P)(N)( memory_map_entry_available->virtual_start + size );
    N physical_size = -E_mem_S_page_size;
    B end = no;
    E_main_S_kernel_args.additional_pages = 0;
    N additional_pages_ = 1UL << 15; //CONF Maksymalna liczba (‘guard pages’ stosów) ‹zadań› w systemie.
    const N table_n = E_mem_S_page_size / sizeof(N);
    max_memory -= E_mem_S_page_size;
    for_n( pml4_i, table_n )
        if( !end
        || additional_pages_
        )
        {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
            N pdpt_physical = memory_map_entry_available->physical_start + size;
            volatile N *pdpt = (P)(N)( memory_map_entry_available->virtual_start + size );
            for_n( pdpt_i, table_n )
                if( !end
                || additional_pages_
                )
                {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
                    N pd_physical = memory_map_entry_available->physical_start + size;
                    volatile N *pd = (P)(N)( memory_map_entry_available->virtual_start + size );
                    for_n( pd_i, table_n )
                        if( !end
                        || additional_pages_
                        )
                        {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
                            N pt_physical = memory_map_entry_available->physical_start + size;
                            volatile N *pt = (P)(N)( memory_map_entry_available->virtual_start + size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * E_mem_S_page_size );
                                    if( virtual_address == max_memory )
                                        end = yes;
                                    if( !virtual_address )
                                        pt[ pt_i ] = 0;
                                    else
                                    {   E_main_I_allocate_page_table_I_next_physical_address( &memory_map, &physical_size );
                                        N physical_address = memory_map->physical_start + physical_size;
                                        pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                        B sata_ahci = no;
                                       for_n( i, E_main_S_sata_ahci_n )
                                            if( physical_address == E_main_S_sata_ahci_addresses[i]
                                            || physical_address == E_main_S_sata_ahci_addresses[i] + E_mem_S_page_size
                                            )
                                            {   sata_ahci = yes;
                                                break;
                                            }
                                        if( sata_ahci
                                        || physical_address == (N)E_main_S_kernel_args.local_apic_address
                                        || physical_address == (N)E_main_S_kernel_args.io_apic_address
                                        || ( E_main_S_kernel_args.pcie_base_address
                                          && physical_address >= (N)E_main_S_kernel_args.pcie_base_address
                                          && physical_address < (N)E_main_S_kernel_args.pcie_base_address + 256 * 32 * 8 * 4096
                                        )
                                        || ( E_main_S_ethernet_address
                                          && physical_address >= E_main_S_ethernet_address
                                          && physical_address < E_main_S_ethernet_address + 0x20000
                                        )
                                        || ( E_main_S_ethernet_eeprom_address
                                          && physical_address == E_main_S_ethernet_eeprom_address
                                        ))
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pwt | E_cpu_Z_page_entry_S_pcd;
                                        else if( physical_address >= E_simple_Z_n_I_align_down_to_v2( E_main_S_framebuffer_physical, E_mem_S_page_size )
                                        && physical_address < E_simple_Z_n_I_align_up_to_v2( E_main_S_framebuffer_physical + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, E_mem_S_page_size )
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pat;
                                    }
                                }else
                                {   pt[ pt_i ] = 0;
                                    if( additional_pages_ )
                                        additional_pages_--;
                                    E_main_S_kernel_args.additional_pages++;
                                }
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pt_physical;
                        }else
                            pd[ pd_i ] = 0;
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pd_physical;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pdpt_physical;
        }else
            pml4[ pml4_i ] = 0;
    E_main_Q_cr3_P( pml4_physical );
    E_main_S_pml4 = (N)pml4;
    E_main_S_start_end_address = memory_map_entry_available->virtual_start + size + E_mem_S_page_size;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_main_M_madt_I_source_override_sort_gsi( struct E_main_Z_apic_source_override *apic_source_override
, N apic_source_override_n
){  N n = apic_source_override_n;
    while( n > 1 )
    {   struct E_main_Z_apic_source_override *entry_prev = &apic_source_override[0];
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if( entry_prev->gsi > apic_source_override[i].gsi )
            {   J_swap( struct E_main_Z_apic_source_override, *entry_prev, apic_source_override[i] );
                new_n = i;
            }
            entry_prev = &apic_source_override[i];
        }
        n = new_n;
    }
}
void
E_main_M_madt_I_source_override_sort_source( struct E_main_Z_apic_source_override *apic_source_override
, N apic_source_override_n
){  N n = apic_source_override_n;
    while( n > 1 )
    {   struct E_main_Z_apic_source_override *entry_prev = &apic_source_override[0];
        N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if( entry_prev->source > apic_source_override[i].source )
            {   J_swap( struct E_main_Z_apic_source_override, *entry_prev, apic_source_override[i] );
                new_n = i;
            }
            entry_prev = &apic_source_override[i];
        }
        n = new_n;
    }
}
N
E_main_M_madt( Pc table
, N l
){  N apic_source_override_n = 0;
    struct E_main_Z_apic_source_override *apic_source_override;
    Mt_( apic_source_override, apic_source_override_n );
    Kp( apic_source_override )
        return ~0;
    while(l)
    {   switch( (N8)table[0] )
        { case 2: // source override
            {   struct H_acpi_Z_madt_Z_source_override *source_override = (P)&table[0];
                N n_prepended;
                if( !E_mem_Q_blk_I_add( &apic_source_override, 1, &n_prepended, 0 ))
                {   K_( ~1, W( apic_source_override ));
                    return ~0;
                }
                N i = n_prepended ? 0 : apic_source_override_n;
                apic_source_override[i].source = source_override->source;
                apic_source_override[i].gsi = source_override->gsi;
                apic_source_override[i].flags = source_override->flags & 0xff;
                apic_source_override_n++;
                break;
            }
        }
        l -= (N8)table[1];
        table += (N8)table[1];
    }
    // Sprawdzenie, czy nie ma duplikatów ‘source override’.
    if( apic_source_override_n > 1 )
    {   E_main_M_madt_I_source_override_sort_source( apic_source_override, apic_source_override_n );
        N source = apic_source_override[0].source;
        for_n( i, apic_source_override_n - 1 )
        {   if( apic_source_override[ 1 + i ].source == source )
            {   K_( ~1, W( apic_source_override ));
                return ~0;
            }
            source = apic_source_override[ 1 + i ].source;
        }
        E_main_M_madt_I_source_override_sort_gsi( apic_source_override, apic_source_override_n );
        N gsi = apic_source_override[0].gsi;
        for_n_( i, apic_source_override_n - 1 )
        {   if( apic_source_override[ 1 + i ].gsi == gsi )
            {   K_( ~1, W( apic_source_override ));
                return ~0;
            }
            gsi = apic_source_override[ 1 + i ].gsi;
        }
    }
    E_main_S_kernel_args.gsi_n = (( E_interrupt_Q_io_apic_R(1) >> 16 ) & 0xff ) + 1;
    if( E_main_S_kernel_args.gsi_n < 17 )
        return ~0;
    Mt_( E_main_S_kernel_args.gsi, E_main_S_kernel_args.gsi_n );
    Kp( E_main_S_kernel_args.gsi )
    {   K_( ~1, W( apic_source_override ));
        return ~0;
    }
    N apic_source_override_i = 0;
    for_n( i, E_main_S_kernel_args.gsi_n )
        if( apic_source_override_i == apic_source_override_n
        || i != apic_source_override[ apic_source_override_i ].gsi
        )
        {   E_main_S_kernel_args.gsi[i].source = i;
            E_main_S_kernel_args.gsi[i].flags = 0;
        }else
        {   E_main_S_kernel_args.gsi[i].source = apic_source_override[ apic_source_override_i ].source;
            E_main_S_kernel_args.gsi[i].flags = apic_source_override[ apic_source_override_i ].flags;
            if( i != E_main_S_kernel_args.gsi[i].source )
                E_main_S_kernel_args.gsi[ E_main_S_kernel_args.gsi[i].source ].source = ~0;
            apic_source_override_i++;
        }
    K_( ~1, W( apic_source_override ));
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
__attribute__ ((__noreturn__))
void
main( struct E_main_Z_memory_map_entry *memory_map
, struct E_main_Z_video *video
){  N32 eax = 0, ebx, ecx, edx;
    __asm__ volatile (
    "\n" "cpuid"
    : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    );
    if( ebx != 0x756e6547
    || edx != 0x49656e69
    || ecx != 0x6c65746e
    )
        goto End;
    eax = 1;
    __asm__ volatile (
    "\n" "cpuid"
    : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    );
    if(( edx
      ^ (( 1 << 16 ) | ( 1 << 15 ) | ( 1 << 9 ) | ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 0 )))
      & (( 1 << 16 ) | ( 1 << 15 ) | ( 1 << 9 ) | ( 1 << 5 ) | ( 1 << 4 ) | ( 1 << 0 )) // PAT, CMOV, APIC, MSR, TSC, FPU
    )
        goto End;
    E_main_S_kernel_args.sse = !!( edx & ( 1 << 25 ));
    E_main_S_kernel_args.x2apic = !!( ecx & ( 1 << 21 ));
    E_main_Z_memory_table_S = memory_map;
    E_main_S_framebuffer_physical = (N)video->p;
    E_main_S_kernel_args.framebuffer.width = video->width;
    E_main_S_kernel_args.framebuffer.height = video->height;
    E_main_S_kernel_args.framebuffer.bits_per_pixel = video->bits_per_pixel;
    E_main_S_kernel_args.framebuffer.pixels_per_scan_line = video->line_width / ( E_main_S_kernel_args.framebuffer.bits_per_pixel / 8 );
    E_main_S_kernel_args.framebuffer.blue_size = video->blue_size;
    E_main_S_kernel_args.framebuffer.blue_shift = video->blue_start;
    E_main_S_kernel_args.framebuffer.green_size = video->green_size;
    E_main_S_kernel_args.framebuffer.green_shift = video->green_start;
    E_main_S_kernel_args.framebuffer.red_size = video->red_size;
    E_main_S_kernel_args.framebuffer.red_shift = video->red_start;
    E_main_Q_memory_map_I_align();
    N r = E_main_I_allocate_page_table_pre();
    if( K_error(r) )
        goto End;
    r = E_acpi_I_search();
    if( K_error(r) )
        goto End;
    if( E_acpi_S_pic_mode )
    {   E_main_I_out_8( 0x21, 0xff );
        E_main_I_out_8( 0xa1, 0xff );
    }
    // Przygotowanie ‘write‐combining’ (dla ‘framebuffera’).
    N64 v = E_main_Q_msr_R( 0x277 );
    v &= ~0x70000;
    v |= 0x10000;
    E_main_Q_msr_P( 0x277, v );
    // Włączenie x2APIC.
    if( E_main_S_kernel_args.x2apic )
    {   v = E_main_Q_msr_R( 0x1b );
        v |= ( 1 << 11 ) | ( 1 << 10 );
        E_main_Q_msr_P( 0x1b, v );
    }
    // Przygotowanie tablicy zakresów pamięci.
    E_main_S_sata_ahci_n = 0;
    E_main_S_ethernet_address = E_main_S_ethernet_eeprom_address = 0;
    r = E_pci_I_check_buses();
    if( K_error(r) )
        goto End;
    E_main_S_kernel_args.processor_start_page = 0xf000;
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) //NDFN
    { 0
    , 0x2000
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    };
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
    { E_main_S_kernel_args.processor_start_page
    , E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
    };
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
    { E_main_S_boot_loader_orig_start
    , E_main_S_boot_loader_orig_end - E_main_S_boot_loader_orig_start
    , E_main_Z_memory_table_Z_memory_type_S_boot_loader
    };
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) //NDFN 0x200000 potrzebne tylko dla symulatora Vmware. Powinny być realne zakresy zastrzeżone.
    { E_main_S_boot_loader_orig_end
    , 0x100000 - E_main_S_boot_loader_orig_end
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    };
    //*--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) // ISA
    //{ 0xf00000
    //, 0x100000
    //, E_main_Z_memory_table_Z_memory_type_S_reserved
    //};
    if( E_main_S_kernel_args.pcie_base_address )
        *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) // PCI Express ECAM
        { (N)E_main_S_kernel_args.pcie_base_address
        , 256 * 32 * 8 * 4096
        , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        };
    if( E_main_S_kernel_args.acpi.hpet.address.address )
        *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) // HPET
        { E_main_S_kernel_args.acpi.hpet.address.address
        , E_mem_S_page_size
        , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        };
    //*--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) // TPM
    //{ 0xfed40000
    //, 5 * E_mem_S_page_size
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
    { E_main_S_framebuffer_physical
    , E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
    { (N)E_main_S_kernel_args.local_apic_address
    , E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
    { (N)E_main_S_kernel_args.io_apic_address
    , E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    E_main_Q_memory_map_I_align();
    E_main_Z_memory_table_S_end = E_main_Z_memory_table_S_orig_end;
    E_main_Q_memory_map_I_sort_physical();
    r = E_main_Q_memory_map_I_remove_overlapped();
    if( K_error(r) )
        goto End;
    E_main_Q_memory_map_I_remove_bad();
    E_main_Q_memory_map_I_set_virtual_0();
    E_main_Q_memory_map_I_sort_virtual();
    N memory_size_0 = E_main_Q_memory_map_R_size_0();
    if( memory_size_0 < E_mem_S_page_size + E_main_S_boot_loader_orig_end - E_main_S_boot_loader_orig_start + 2 * 1024 * 1024 ) //NDFN
        goto End;
    E_main_I_allocate_page_table_0( memory_size_0 );
    N reserved_size = E_main_Q_memory_map_R_reserved_size();
    B reserved_from_end = no; //CONF
    N memory_size = E_main_Q_memory_map_R_size();
    if( reserved_from_end
    && memory_size - reserved_size - E_mem_S_page_size > 0x100000000UL - E_mem_S_page_size
    )
        reserved_from_end = no;
    N loader_start_0 = E_simple_Z_n_I_align_down_to_v2( memory_size_0 / 2, E_mem_S_page_size );
    N loader_start_physical = (N)E_main_Z_p_I_to_physical( (P)loader_start_0 );
    memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_orig_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
        {   memory_map->size = E_main_S_boot_loader_orig_first_end - E_main_S_boot_loader_orig_start;
            *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry )
            { E_main_S_boot_loader_orig_second_start
            , E_main_S_boot_loader_orig_end - E_main_S_boot_loader_orig_second_start
            , E_main_Z_memory_table_Z_memory_type_S_boot_loader
            };
            break;
        }
        memory_map++;
    }
    *--E_main_Z_memory_table_S = ( struct E_main_Z_memory_map_entry ) // SMM
    { E_main_S_boot_loader_orig_first_end
    , E_main_S_boot_loader_orig_second_start - E_main_S_boot_loader_orig_first_end
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    };
    E_main_Q_memory_map_I_sort_physical();
    E_main_Q_memory_map_I_join_physical();
    E_main_Q_memory_map_I_set_virtual( reserved_from_end );
    E_main_Q_memory_map_I_sort_virtual();
    E_main_S_kernel_args.boot_loader = E_main_Z_p_I_to_virtual( (P)loader_start_physical );
    E_main_I_allocate_page_table_1( memory_size, memory_size_0 );
    E_main_Z_remap_jump remap_jump = (P)(N)E_main_S_kernel_args.processor_start_page;
    E_mem_Q_blk_I_copy( remap_jump, &E_remap_jump_I, ( Pc )&E_remap_jump_I_end - ( Pc )&E_remap_jump_I );
    E_mem_Q_blk_I_copy( (P)loader_start_0
    , (P)E_main_S_boot_loader_orig_start
    , E_main_S_boot_loader_orig_first_end - E_main_S_boot_loader_orig_start
    );
    E_mem_Q_blk_I_copy( ( Pc )loader_start_0 + E_main_S_boot_loader_orig_second_start - E_main_S_boot_loader_orig_start
    , (P)E_main_S_boot_loader_orig_second_start
    , E_main_S_boot_loader_orig_end - E_main_S_boot_loader_orig_second_start
    );
    E_main_Q_loader_I_relocate( loader_start_0, (N)E_main_S_kernel_args.boot_loader );
    remap_jump( E_main_S_pml4, (N)E_main_S_kernel_args.boot_loader - E_main_S_boot_loader_orig_start );
    __asm__ volatile ( "" ::: "rax", "rbx", "rcx", "rdx", "rsi", "rdi", "rbp", "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15", "memory" );
    E_main_Z_memory_table_S = (P)( (N)E_main_S_kernel_args.boot_loader + (N)E_main_Z_memory_table_S - E_main_S_boot_loader_orig_start );
    E_main_Z_memory_table_S_end = (N)E_main_S_kernel_args.boot_loader + E_main_Z_memory_table_S_orig_end - E_main_S_boot_loader_orig_start;
    if( reserved_from_end )
        E_main_S_pml4 = (N)E_main_Z_p_I_to_virtual( (P)E_main_S_pml4 );
    else
        E_main_I_allocate_page_table( memory_size );
    E_main_S_kernel_args.framebuffer.p = E_main_Z_p_I_to_virtual( (P)E_main_S_framebuffer_physical );
    if( !E_main_S_kernel_args.framebuffer.p )
        goto End;
    E_mem_M_0( memory_size );
    r = E_font_M();
    if( K_error(r) )
        goto End;
    E_vga_I_fill_rect( 0, 0, E_main_S_kernel_args.framebuffer.width, E_main_S_kernel_args.framebuffer.height, E_vga_R_video_color( E_vga_S_background_color ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 38, E_main_S_kernel_args.framebuffer.height / 2 - 37, 38 + 34, 37 + 36, E_vga_R_video_color( 0x4e864f ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2 - 50, E_main_S_kernel_args.framebuffer.height / 2 + 4 + 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10 - 13, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_vga_I_fill_rect( E_main_S_kernel_args.framebuffer.width / 2, E_main_S_kernel_args.framebuffer.height / 2 - 10, 48, 5, E_vga_R_video_color( 0x2b2b2b ));
    E_font_I_print( "OUX/C+ OS boot loader ©overcq <overcq@int.pl> http://github.com/overcq\n" );
    E_acpi_S_apic_content = E_main_Z_p_I_to_virtual( E_acpi_S_apic_content );
    E_main_S_kernel_args.io_apic_address = E_main_Z_p_I_to_virtual( E_main_S_kernel_args.io_apic_address );
    r = E_main_M_madt( E_acpi_S_apic_content, E_acpi_S_apic_content_l );
    if( K_error(r) )
        goto End;
    P interrupt_stack = E_mem_Q_blk_M_align_tab( E_mem_S_page_size, 2, E_mem_S_page_size );
    if( K_error( interrupt_stack ))
        goto End;
    _0_( &E_main_S_tss );
    E_main_S_tss.ist[1] = (N)interrupt_stack + 2 * E_mem_S_page_size;
    E_main_S_tss.io_map_base_address = ~0;
    S_gdt[3] = J_descriptor_low( (N)&S_ldt[0], 8 - 1, E_cpu_Z_gdt_Z_type_S_ldt );
    S_gdt[4] = (N)&S_ldt[0] >> 32;
    S_gdt[5] = J_descriptor_low( (N)&E_main_S_tss, sizeof( E_main_S_tss ) - 1, E_cpu_Z_gdt_Z_type_S_tss );
    S_gdt[6] = (N)&E_main_S_tss >> 32;
    S_gd.base = (N)&S_gdt[0];
    S_gd.limit = 7 * sizeof(N) - 1;
    __asm__ volatile (
    "\n" "lgdt  %0"
    "\n" "mov   $3 << 3,%%ax"
    "\n" "lldt  %%ax"
    "\n" "mov   $5 << 3,%%ax"
    "\n" "ltr   %%ax"
    :
    : "g" ( S_gd )
    : "ax"
    );
    r = E_interrupt_M();
    if( K_error(r) )
        goto End;
    __asm__ volatile (
    "\n" "sti"
    );
    E_main_S_kernel_args.local_apic_address = E_main_Z_p_I_to_virtual( E_main_S_kernel_args.local_apic_address );
    r = E_pci_I_check_buses_1();
    if( K_error(r) )
    {   __asm__ volatile (
        "\n" "cli"
        );
        goto End;
    }
    E_main_S_kernel_args.page_table = (P)( reserved_from_end ? E_main_S_start_end_address : E_main_S_pml4 );
    N kernel_size = E_ouxfs_Q_kernel_R_size();
    E_main_S_kernel_args.kernel = (P)( reserved_from_end
    ? (N)E_main_S_kernel_args.page_table - E_simple_Z_n_I_align_up_to_v2( kernel_size, E_mem_S_page_size )
    : E_main_S_start_end_address );
    N page_table_size = ( reserved_from_end ? E_main_S_pml4 + E_mem_S_page_size : (N)E_main_S_kernel_args.kernel ) - (N)E_main_S_kernel_args.page_table;
    E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n();
    N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
    E_main_S_kernel_args.memory_map = (P)( reserved_from_end
    ? (N)E_main_S_kernel_args.kernel - memory_map_size
    : E_simple_Z_n_I_align_up_to_v2( (N)E_main_S_kernel_args.kernel + kernel_size, sizeof(N) ));
    E_main_S_kernel_args.stack_pages = 1 + 1; //CONF Co najmniej 2 strony pamięci, ponieważ jedna będzie zmieniona na ‘guard page’.
    N stack_size = E_main_S_kernel_args.stack_pages * E_mem_S_page_size;
    E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
    ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, E_mem_S_page_size ) - stack_size
    : memory_size - stack_size
    );
    r = E_ouxfs_Q_kernel_I_read( E_main_S_kernel_args.kernel );
    if( K_error(r) )
    {   __asm__ volatile (
        "\n" "cli"
        );
        goto End;
    }
    __asm__ volatile (
    "\n" "cli"
    );
    Pn kernel_p = E_main_S_kernel_args.kernel;
    if( !E_mem_Q_blk_T_eq( kernel_p, "OUXEXE", 6 ))
        goto End;
    kernel_p = (P)(( Pc )kernel_p + 6 );
    struct E_main_Z_kernel_data kernel_data;
    kernel_data.rela_plt = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.exports = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.dynstr = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.got = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.text = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.data = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.entry = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.rela = (P)kernel_p;
    if( (N)kernel_data.rela > (N)kernel_data.rela_plt
    || (N)kernel_data.rela_plt != (N)kernel_data.exports
    || (N)kernel_data.exports > (N)kernel_data.dynstr
    || (N)kernel_data.dynstr > (N)kernel_data.got
    || (N)kernel_data.got > (N)kernel_data.text
    || (N)kernel_data.text > (N)kernel_data.data
    || (N)kernel_data.data > (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, E_mem_S_page_size )
    || (N)kernel_data.entry < (N)kernel_data.text
    || (N)kernel_data.entry >= (N)kernel_data.data
    )
        goto End;
    for_n( i, ( (N)kernel_data.rela_plt - (N)kernel_data.rela ) / sizeof( *kernel_data.rela ))
        if( kernel_data.rela[i].offset < (N)kernel_data.got - (N)E_main_S_kernel_args.kernel
        || kernel_data.rela[i].offset >= kernel_size //NDFN Być może ‘relokacje’ mogą być tylko w GOT.
        )
            goto End;
    for_n_( i, ( (N)kernel_data.dynstr - (N)kernel_data.exports ) / sizeof( *kernel_data.exports ))
        if( kernel_data.exports[i].offset < (N)kernel_data.text - (N)E_main_S_kernel_args.kernel
        || kernel_data.exports[i].offset >= (N)E_main_S_kernel_args.kernel + E_simple_Z_n_I_align_up_to_v2( kernel_size, E_mem_S_page_size )
        )
            goto End;
    // ‘Relokacja’ kernela.
    kernel_p = (P)(( Pc )E_main_S_kernel_args.kernel + 6 );
    kernel_data.rela_plt = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.exports = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.dynstr = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.got = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.text = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.data = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.entry = (P)( (N)E_main_S_kernel_args.kernel + *kernel_p );
    kernel_p++;
    kernel_data.rela = (P)kernel_p;
    for_n_( i, ( (N)kernel_data.rela_plt - (N)kernel_data.rela ) / sizeof( *kernel_data.rela ))
        switch( kernel_data.rela[i].type )
        { case 6:
                *(Pn)( (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].offset ) += (N)E_main_S_kernel_args.kernel;
                break;
          case 8:
                *(Pn)( (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].offset ) = (N)E_main_S_kernel_args.kernel + kernel_data.rela[i].addend;
                break;
        }
    r = E_mem_M( reserved_from_end, (N)E_main_S_kernel_args.kernel_stack, stack_size, (N)E_main_S_kernel_args.memory_map, memory_map_size, (N)E_main_S_kernel_args.page_table, page_table_size, (N)E_main_S_kernel_args.kernel, kernel_size, memory_size, reserved_size );
    if( K_error(r) )
        goto End;
    Mt_( E_main_S_kernel_args.processor_proc, E_main_S_kernel_args.processor_n - 1 );
    if( K_error( E_main_S_kernel_args.processor_proc )
    || !E_main_S_kernel_args.processor_proc
    )
        goto End;
    E_mem_Q_blk_I_copy( (P)(N)E_main_S_kernel_args.processor_start_page, &E_mp_init_I, ( Pc )&E_mp_init_I_end - ( Pc )&E_mp_init_I );
    Pc p = (P)(N)E_main_S_kernel_args.processor_start_page;
    i = ( Pc )&E_mp_init_I_reloc_1 - ( Pc )&E_mp_init_I + 1;
    *( N16 * )( p + i ) = E_main_S_kernel_args.processor_start_page >> 4;
    i = ( Pc )&E_mp_init_I_reloc_2 - ( Pc )&E_mp_init_I + 2;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + i + 6;
    i = ( Pc )&E_mp_init_I_reloc_3 - ( Pc )&E_mp_init_I + 1;
    *( N32 * )( p + i ) = (N)E_main_Z_p_I_to_physical( (P)E_main_S_pml4 );;
    i = ( Pc )&E_mp_init_I_reloc_4 - ( Pc )&E_mp_init_I + 3;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gd - ( Pc )&E_mp_init_I;
    i = ( Pc )&E_mp_init_I_reloc_5 - ( Pc )&E_mp_init_I + 1;
    *( N32 * )( p + i ) = E_main_S_kernel_args.processor_start_page + i + 6;
    i = ( Pc )&E_mp_init_I_reloc_6 - ( Pc )&E_mp_init_I + 1;
    *( N8 * )( p + i ) = E_main_S_kernel_args.x2apic;
    i = ( Pc )&E_mp_init_I_reloc_7 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = (N)E_main_S_kernel_args.local_apic_address;
    i = ( Pc )&E_mp_init_I_reloc_8 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = (N)E_main_S_kernel_args.processor_proc;
    i = ( Pc )&E_mp_init_S_gd_32 - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gdt_32 - ( Pc )&E_mp_init_I;
    i = ( Pc )&E_mp_init_S_gd - ( Pc )&E_mp_init_I + 2;
    *( N * )( p + i ) = E_main_S_kernel_args.processor_start_page + ( Pc )&E_mp_init_S_gdt - ( Pc )&E_mp_init_I;
    for_n_( i, E_main_S_kernel_args.processor_n - 1 )
    {   E_main_S_kernel_args.processor_proc[i] = (P)~0UL;
        E_interrupt_I_ipi_init( 1 + i );
    }
    E_main_Q_memory_map_I_sort_physical();
    struct H_oux_E_mem_Z_memory_map *my_memory_map = E_main_S_kernel_args.memory_map;
    memory_map = E_main_Z_memory_table_S;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
        {   my_memory_map->physical_start = memory_map->physical_start;
            my_memory_map->virtual_start = memory_map->virtual_start;
            my_memory_map->pages = memory_map->size / E_mem_S_page_size;
            my_memory_map++;
        }
        memory_map++;
    }
    _0( &E_main_S_kernel_args.uefi_runtime_services, sizeof( E_main_S_kernel_args.uefi_runtime_services ));
    E_main_S_kernel_args.acpi.dsdt_content = E_main_Z_p_I_to_virtual( E_main_S_kernel_args.acpi.dsdt_content );
    for_n_( i, E_main_S_kernel_args.acpi.ssdt_content_n )
        E_main_S_kernel_args.acpi.ssdt_content[i].address = E_main_Z_p_I_to_virtual( E_main_S_kernel_args.acpi.ssdt_content[i].address );
    if( E_main_S_kernel_args.acpi.pm1a_control_block_mmio )
        E_main_S_kernel_args.acpi.pm1a_control_block = (N)E_main_Z_p_I_to_virtual( (P)E_main_S_kernel_args.acpi.pm1a_control_block );
    r = E_main_M_madt( E_acpi_S_apic_content, E_acpi_S_apic_content_l );
    if( K_error(r) )
        goto End;
    __asm__ volatile (
    "\n" "mov   %%cr0,%%rax"
    "\n" "and   %0,%%rax"
    "\n" "or    %1,%%rax"
    "\n" "mov   %%rax,%%cr0"
    "\n" "mov   %%cr4,%%rax"
    "\n" "and   %2,%%rax"
    "\n" "or    %3,%%rax"
    "\n" "mov   %%rax,%%cr4"
    "\n" "mov   %%cr8,%%rax"
    "\n" "and   $~0xf,%%rax"
    "\n" "mov   %%rax,%%cr8"
    :
    : "i" ( ~( E_cpu_Z_cr0_S_ts | E_cpu_Z_cr0_S_nw | E_cpu_Z_cr0_S_cd ))
    , "i" ( E_cpu_Z_cr0_S_ne | E_cpu_Z_cr0_S_wp )
    , "i" ( ~( E_cpu_Z_cr4_S_pvi | E_cpu_Z_cr4_S_tsd | E_cpu_Z_cr4_S_pge | E_cpu_Z_cr4_S_pcide | E_cpu_Z_cr4_S_smep | E_cpu_Z_cr4_S_smap | E_cpu_Z_cr4_S_pke | E_cpu_Z_cr4_S_pks | E_cpu_Z_cr4_S_uintr | E_cpu_Z_cr4_S_lam_sup ))
    , "i" ( E_cpu_Z_cr4_S_de | E_cpu_Z_cr4_S_mce | E_cpu_Z_cr4_S_pce | E_cpu_Z_cr4_S_fsgsbase )
    : "rax"
    );
    // Przed wyrzuceniem z pamięci programu ‘boot loadera’ ‘kernel’ potrzebuje przenieść dostarczone dane, ustawić LDT, IDT, TSS.
    __asm__ volatile (
    "\n" "mov   %0,%%rsp"
    "\n" "jmp   *%1"
    :
    : "g" ( (N)E_main_S_kernel_args.kernel_stack + stack_size ), "r" ( kernel_data.entry ), "D" ( &E_main_S_kernel_args )
    );
    __builtin_unreachable();
End:O{  __asm__ volatile (
        "\n" "hlt"
        );
    }
    __builtin_unreachable();
}
/******************************************************************************/
