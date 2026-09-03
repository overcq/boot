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
#define E_cpu_Z_gdt_Z_data_S_write      ( 1UL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_code       ( 1UL << ( 32 + 11 ))
#define E_cpu_Z_gdt_S_code_data         ( 1UL << ( 32 + 12 ))
#define E_cpu_Z_gdt_S_present           ( 1UL << ( 32 + 15 ))
#define E_cpu_Z_gdt_Z_code_S_64bit      ( 1UL << ( 32 + 21 ))
#define E_cpu_Z_gdt_S_granularity       ( 1UL << ( 32 + 23 ))
#define E_cpu_Z_gdt_Z_type_S_ldt        ( 1UL << ( 32 + 9 ))
#define E_cpu_Z_gdt_Z_type_S_tss        ( 9UL << ( 32 + 8 ))
//==============================================================================
N E_pci_I_check_buses(void);
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
typedef void ( *E_main_Z_remap_jump )( N pml4, N delta );
//==============================================================================
extern B E_acpi_S_pic_mode;
//------------------------------------------------------------------------------
extern char E_main_S_relocation_data[];
extern P E_remap_jump_I, E_remap_jump_I_end;
extern P E_mp_init_I, E_mp_init_I_end, E_mp_init_I_reloc_1, E_mp_init_I_reloc_2, E_mp_init_I_reloc_3, E_mp_init_I_reloc_4, E_mp_init_I_reloc_5, E_mp_init_I_reloc_6, E_mp_init_S_gdt_32, E_mp_init_S_gdt, E_mp_init_S_gd_32, E_mp_init_S_gd;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
B E_main_S_sse;
struct E_main_Z_kernel_args E_main_S_kernel_args;
N32 E_main_S_sata_ahci_addresses[8];
N64 E_main_S_ethernet_address, E_main_S_ethernet_eeprom_address;
N8 E_main_S_sata_ahci_n;
N E_main_S_framebuffer_physical, E_main_S_framebuffer_virtual;
struct E_main_Z_memory_map_entry *E_main_S_memory_map;
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
N32
E_interrupt_Q_io_apic_R( N8 i
){  *( volatile N32 * )E_main_S_kernel_args.io_apic_address = i;
    return *( volatile N32 * )(( Pc )E_main_S_kernel_args.io_apic_address + 0x10 );
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_interrupt_I_ipi_init( N32 processor
){  E_main_Q_msr_P( 0x830, ( (N64)processor << 32 ) | ( 5 << 8 ));
}
//==============================================================================
void
E_main_Q_memory_map_I_align( void
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   memory_map->size = E_simple_Z_n_I_align_up_to_v2( memory_map->physical_start % H_oux_E_mem_S_page_size + memory_map->size, H_oux_E_mem_S_page_size );
        memory_map->physical_start = E_simple_Z_n_I_align_down_to_v2( memory_map->physical_start, H_oux_E_mem_S_page_size );
        memory_map++;
    }
}
//DFN Nie usuwa wszystkich przecinających się bloków. Liczy na to, że niektóre bezsensowne przecięcia nie wystąpią.
__attribute__ (( __warn_unused_result__ ))
N
E_main_Q_memory_map_I_remove_overlapped( void
){  struct E_main_Z_memory_map_entry *entry = E_main_S_memory_map;
    while( entry != (P)E_main_Z_memory_table_S_end
    && !entry->size
    )
        entry++;
    if( entry == (P)E_main_Z_memory_table_S_end )
        return 0;
    struct E_main_Z_memory_map_entry *next_entry = entry;
    O{  do
            next_entry++;
        while( next_entry != (P)E_main_Z_memory_table_S_end
        && !next_entry->size
        );
        if( next_entry == (P)E_main_Z_memory_table_S_end )
            break;
        N64 entry_physical_start = entry->physical_start;
        N64 entry_size = entry->size;
        N32 entry_type = entry->type;
        N64 next_entry_physical_start = next_entry->physical_start;
        N64 next_entry_size = next_entry->size;
        if( entry_physical_start + entry_size >= next_entry_physical_start
        && entry_type == next_entry->type
        ) // Bloki są scalane w jeden.
        {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                entry->size = next_entry_physical_start + next_entry_size - entry_physical_start;
            E_mem_Q_blk_I_copy( E_main_S_memory_map + 1, E_main_S_memory_map, ( next_entry - E_main_S_memory_map ) * sizeof( *E_main_S_memory_map ));
            E_main_S_memory_map++;
            entry++;
        }else if( entry_physical_start + entry_size > next_entry_physical_start )
            if(( entry_type != E_main_Z_memory_table_Z_memory_type_S_boot_loader
              && entry_type != E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
              && ( !next_entry->type
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_bad
                || next_entry->type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            ))
            || ( entry_type == E_main_Z_memory_table_Z_memory_type_S_reserved
              && ( next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
            ))
            || ( entry_type == E_main_Z_memory_table_Z_memory_type_S_available
              && ( next_entry->type == E_main_Z_memory_table_Z_memory_type_S_reserved
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
                || next_entry->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            ))) // Pierwszy blok ustępuje drugiemu.
            {   if( entry_physical_start + entry_size > next_entry_physical_start + next_entry_size )
                {   struct E_main_Z_memory_map_entry *new_entry = next_entry + 1;
                    while( new_entry != (P)E_main_Z_memory_table_S_end
                    && new_entry->physical_start < next_entry_physical_start + next_entry_size
                    )
                        new_entry++;
                    E_mem_Q_blk_I_copy( E_main_S_memory_map - 1, E_main_S_memory_map, ( new_entry - E_main_S_memory_map ) * sizeof( *E_main_S_memory_map ));
                    E_main_S_memory_map--;
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
                {   E_mem_Q_blk_I_copy( E_main_S_memory_map + 1, E_main_S_memory_map, ( entry - E_main_S_memory_map ) * sizeof( *E_main_S_memory_map ));
                    E_main_S_memory_map++;
                    entry = next_entry;
                }
            }else if((( !entry_type
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_bad
                || entry_type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
              )
              && next_entry->type != E_main_Z_memory_table_Z_memory_type_S_boot_loader
              && next_entry->type != E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )
            || (( entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
              )
              && ( next_entry->type == E_main_Z_memory_table_Z_memory_type_S_reserved
            ))
            || (( entry_type == E_main_Z_memory_table_Z_memory_type_S_reserved
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
                || entry_type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
              )
              && next_entry->type == E_main_Z_memory_table_Z_memory_type_S_available
            )) // Drugi blok ustępuje pierwszemu.
            {   if( entry_physical_start + entry_size < next_entry_physical_start + next_entry_size )
                {   struct E_main_Z_memory_map_entry *new_entry = next_entry + 1;
                    while( new_entry != (P)E_main_Z_memory_table_S_end
                    && new_entry->physical_start < next_entry_physical_start + next_entry_size
                    )
                        new_entry++;
                    new_entry--;
                    E_mem_Q_blk_I_copy( next_entry, next_entry + 1, ( new_entry - next_entry ) * sizeof( *E_main_S_memory_map ));
                    new_entry->physical_start = entry_physical_start + entry_size;
                    new_entry->size = next_entry_physical_start + next_entry_size - new_entry->physical_start;
                }else
                {   E_mem_Q_blk_I_copy( E_main_S_memory_map + 1, E_main_S_memory_map, ( next_entry - E_main_S_memory_map ) * sizeof( *E_main_S_memory_map ));
                    E_main_S_memory_map++;
                    entry++;
                }
            }else
                return ~0;
        else
            entry = next_entry;
    }
    return 0;
}
void
E_main_Q_memory_map_I_remove_bad( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
        {   if(( E_main_S_memory_map[ i - 1 ].size
              && (( E_main_S_memory_map[ i - 1 ].type
                  && E_main_S_memory_map[ i - 1 ].type < E_main_Z_memory_table_Z_memory_type_S_bad
                )
                || ( E_main_S_memory_map[ i - 1 ].type >= ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
                  && E_main_S_memory_map[ i - 1 ].type <= ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )))
            && (( !E_main_S_memory_map[i].size
                || !E_main_S_memory_map[i].type
                || ( E_main_S_memory_map[i].type >= E_main_Z_memory_table_Z_memory_type_S_bad
                  && ( E_main_S_memory_map[i].type < ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
                    || E_main_S_memory_map[i].type > ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )))
              || E_main_S_memory_map[ i - 1 ].physical_start > E_main_S_memory_map[i].physical_start
            ))
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_S_memory_map[ i - 1 ], E_main_S_memory_map[i] );
                new_n = i;
            }
        }
        n = new_n;
    }
    n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
    for_n( i, n )
        if( E_main_S_memory_map[i].size
        && (( E_main_S_memory_map[i].type
            && E_main_S_memory_map[i].type < E_main_Z_memory_table_Z_memory_type_S_bad
          )
          || ( E_main_S_memory_map[i].type >= ( N32 )E_main_Z_memory_table_Z_memory_type_S_boot_loader
            && E_main_S_memory_map[i].type <= ( N32 )E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )))
            break;
    E_main_S_memory_map = &E_main_S_memory_map[i];
}
//------------------------------------------------------------------------------
void
E_main_Q_memory_map_I_sort_physical( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
            if( E_main_S_memory_map[ i - 1 ].physical_start > E_main_S_memory_map[i].physical_start )
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_S_memory_map[ i - 1 ], E_main_S_memory_map[i] );
                new_n = i;
            }
        n = new_n;
    }
}
void
E_main_Q_memory_map_I_join_physical( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
    for_n( i, n - 1 )
        if( E_main_S_memory_map[i].type == E_main_S_memory_map[ i + 1 ].type
        && E_main_S_memory_map[i].physical_start + E_main_S_memory_map[i].size == E_main_S_memory_map[ i + 1 ].physical_start
        )
        {   E_main_S_memory_map[i].size += E_main_S_memory_map[ i + 1 ].size;
            E_mem_Q_blk_I_copy( E_main_S_memory_map + 1, E_main_S_memory_map, ( i + 1 ) * sizeof( *E_main_S_memory_map ));
            E_main_S_memory_map++;
            i--;
            n--;
        }
}
void
E_main_Q_memory_map_I_sort_virtual( void
){  N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
    while( n > 1 )
    {   N new_n = 0;
        for( N i = 1; i != n; i++ )
            if( E_main_S_memory_map[ i - 1 ].virtual_start > E_main_S_memory_map[i].virtual_start )
            {   J_swap( struct E_main_Z_memory_map_entry, E_main_S_memory_map[ i - 1 ], E_main_S_memory_map[i] );
                new_n = i;
            }
        n = new_n;
    }
}
P
E_main_Z_p_I_to_virtual_pre( P p
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    if( (N)p < E_main_S_boot_loader_end )
        return p;
    if( (N)p < 0xe0000 )
        return 0;
    if( (N)p < 0x100000 )
        return (P)( E_main_S_boot_loader_end + (N)p - 0xe0000 );
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if(( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        )
        && (N)p >= memory_map->physical_start
        && (N)p < memory_map->physical_start + memory_map->size
        )
            return (P)( memory_map->virtual_start + ( (N)p - memory_map->physical_start ));
        memory_map++;
    }
    return 0;
}
P
E_main_Z_p_I_to_virtual( P p
, N memory_table_S_end
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)memory_table_S_end )
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
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
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
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_reserved_size_from_start( void
){  N size = 0;
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_size_0( void
){  N size = H_oux_E_mem_S_page_size;
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
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
E_main_Q_memory_map_R_size_test( void
){  N size = 0;
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
            size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_size( void
){  N size = H_oux_E_mem_S_page_size;
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   size += memory_map->size;
        memory_map++;
    }
    return size;
}
N
E_main_Q_memory_map_R_saved_n( 
){  N n = 0;
    struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->size
        && ( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved //TODO Czy potrzebne?
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim //TODO Czy potrzebne?
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
          || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        ))
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
            if( E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size != loader_start )
            {   memory_map_new_entry_ = --E_main_S_memory_map;
                memory_map_new_entry_->type = memory_map->type;
                memory_map_new_entry_->virtual_start = E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size;
                memory_map_new_entry_->size = loader_start - memory_map_new_entry_->virtual_start;
                memory_map_new_entry_->physical_start = memory_map->physical_start + memory_map->size;
            }
            if( *next_virtual_address != loader_start )
            {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_S_memory_map;
                memory_map_new_entry->type = memory_map->type;
                memory_map_new_entry->size = *next_virtual_address - ( memory_map->virtual_start + memory_map->size );
                if( E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size != loader_start )
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
            {   memory_map_new_entry_ = --E_main_S_memory_map;
                memory_map_new_entry_->type = memory_map->type;
                memory_map_new_entry_->virtual_start = loader_end;
                memory_map_new_entry_->size = E_main_S_kernel_args.processor_start_page - memory_map_new_entry_->virtual_start;
                memory_map_new_entry_->physical_start = memory_map->physical_start + memory_map->size;
            }
            if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
            {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_S_memory_map;
                memory_map_new_entry->type = memory_map->type;
                memory_map_new_entry->size = *next_virtual_address - ( memory_map->virtual_start + memory_map->size );
                if( loader_end != E_main_S_kernel_args.processor_start_page )
                {   memory_map_new_entry->size -= memory_map_new_entry_->size;
                    memory_map_new_entry->physical_start = memory_map_new_entry_->physical_start + memory_map_new_entry_->size;
                }else
                    memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
                memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size;
            }
        }
        *next_virtual_address += H_oux_E_mem_S_page_size + loader_end - loader_start;
        *processor_start_page_computed = yes;
        *loader_computed = yes;
    }else if( !*processor_start_page_computed
    && *next_virtual_address >= E_main_S_kernel_args.processor_start_page
    )
    {   if( *next_virtual_address != E_main_S_kernel_args.processor_start_page )
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_S_memory_map;
            memory_map_new_entry->type = memory_map->type;
            memory_map_new_entry->size = *next_virtual_address - E_main_S_kernel_args.processor_start_page;
            memory_map->size -= memory_map_new_entry->size;
            memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
            memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size;
        }
        *next_virtual_address += H_oux_E_mem_S_page_size;
        *processor_start_page_computed = yes;
    }else if( !*loader_computed
    && *next_virtual_address >= loader_start
    )
    {   if( *next_virtual_address != loader_start )
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_S_memory_map;
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
){  struct E_main_Z_memory_map_entry *memory_map_orig = E_main_S_memory_map;
    struct E_main_Z_memory_map_entry *memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
        || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
        )
            memory_map->virtual_start = memory_map->physical_start;
        memory_map++;
    }
    B processor_start_page_computed = no, loader_computed = no;
    N next_virtual_address = H_oux_E_mem_S_page_size;
    memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
            E_main_Q_memory_map_I_set_virtual_0_I_entry( memory_map
            , E_main_S_boot_loader_start, E_main_S_boot_loader_end
            , &next_virtual_address
            , &processor_start_page_computed
            , &loader_computed
            );
        memory_map++;
    }
    memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
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
        {   struct E_main_Z_memory_map_entry *memory_map_new_entry = --E_main_S_memory_map;
            memory_map_new_entry->type = memory_map->type;
            memory_map_new_entry->size = *next_virtual_address - E_main_S_kernel_args.processor_start_page;
            memory_map->size -= memory_map_new_entry->size;
            memory_map_new_entry->physical_start = memory_map->physical_start + memory_map->size;
            memory_map_new_entry->virtual_start = E_main_S_kernel_args.processor_start_page + H_oux_E_mem_S_page_size;
        }
        *next_virtual_address += H_oux_E_mem_S_page_size;
        *processor_start_page_computed = yes;
    }
}
__attribute__ (( __warn_unused_result__ ))
N
E_main_Q_memory_map_I_set_virtual( B reserved_from_end
){  N startup_page_virtual_address;
    struct E_main_Z_memory_map_entry *memory_map_orig = E_main_S_memory_map;
    struct E_main_Z_memory_map_entry *memory_map = memory_map_orig;
    while( memory_map != (P)E_main_Z_memory_table_S_end )
    {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page )
        {   startup_page_virtual_address = memory_map->virtual_start = memory_map->physical_start;
            break;
        }
        memory_map++;
    }
    B processor_start_page_computed = no;
    N next_virtual_address = H_oux_E_mem_S_page_size;
    if( reserved_from_end )
    {   memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
                break;
            }
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
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
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_reserved
            || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
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
        if( next_virtual_address < startup_page_virtual_address + H_oux_E_mem_S_page_size )
            return ~0;
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_available )
                E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
            memory_map++;
        }
        memory_map = memory_map_orig;
        while( memory_map != (P)E_main_Z_memory_table_S_end )
        {   if( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
            {   E_main_Q_memory_map_I_set_virtual_I_entry( memory_map
                , &next_virtual_address
                , &processor_start_page_computed
                );
                break;
            }
            memory_map++;
        }
    }
    return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void
E_main_I_allocate_page_table_I_next_page( B reserved_from_end
, struct E_main_Z_memory_map_entry **entry
, N *size
){  if( reserved_from_end )
        if( !*size )
        {   ( *entry )--;
            while(( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_boot_loader
            || ( *entry )->type == E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
            )
                ( *entry )--;
            *size = ( *entry )->size - H_oux_E_mem_S_page_size;
        }else
            *size -= H_oux_E_mem_S_page_size;
    else
        if(( *size += H_oux_E_mem_S_page_size ) == ( *entry )->size )
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
){  if(( *size += H_oux_E_mem_S_page_size ) == ( *entry )->size )
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
    : "r" (pml4)
    : "memory"
    );
}
__attribute__ (( __warn_unused_result__ ))
N
E_main_I_allocate_page_table_pre( void
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    N table_address = E_main_S_boot_loader_start; // Start na obecnej tablicy pamięci, malejąco.
    volatile N *pml4 = (P)( table_address -= H_oux_E_mem_S_page_size );
    N physical_address = -H_oux_E_mem_S_page_size;
    B end = no;
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    memory_map--;
    for_n( pml4_i, table_n )
        if( !end )
        {   volatile N *pdpt = (P)( table_address -= H_oux_E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   volatile N *pd = (P)( table_address -= H_oux_E_mem_S_page_size );
                    for_n( pd_i, table_n )
                    {   if( !end )
                        {   if( pd_i )
                                return ~0;
                            volatile N *pt = (P)( table_address -= H_oux_E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    physical_address += H_oux_E_mem_S_page_size;
                                    if( physical_address == E_main_S_boot_loader_end )
                                        physical_address = 0xe0000;
                                    else if( physical_address == 0x100000
                                    || ( physical_address > 0x100000
                                      && physical_address == memory_map->physical_start + memory_map->size
                                    ))
                                    {   while( ++memory_map != (P)E_main_Z_memory_table_S_end )
                                            if(( memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
                                              || memory_map->type == E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
                                            )
                                            && memory_map->physical_start >= 0x100000
                                            )
                                                break;
                                        if( memory_map == (P)E_main_Z_memory_table_S_end )
                                        {   end = yes;
                                            pt_i--;
                                            continue;
                                        }
                                        physical_address = memory_map->physical_start;
                                        memory_map->virtual_start = virtual_address;
                                    }
                                    pt[ pt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | physical_address;
                                }else
                                    pt[ pt_i ] = 0;
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pt;
                        }else
                            pd[ pd_i ] = 0;
                    }
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pd;
                }else
                    pdpt[ pdpt_i ] = 0;
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | (N)pdpt;
        }else
            pml4[ pml4_i ] = 0;
    E_main_Q_cr3_P( (N)pml4 );
    return 0;
}
void
E_main_I_allocate_page_table_0_I_allocate( N max_memory
, struct E_main_Z_memory_map_entry **memory_map_entry_available
, N *size
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    E_main_I_allocate_page_table_I_next_page( no, memory_map_entry_available, size );
    N pml4_physical = ( *memory_map_entry_available )->physical_start + *size;
    volatile N *pml4 = (P)(( *memory_map_entry_available )->virtual_start + *size );
    N physical_size = -H_oux_E_mem_S_page_size;
    B end = no;
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
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
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i  << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory - H_oux_E_mem_S_page_size )
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
, N *start_end_address
){  N max_memory_step = 64 * 1024 * 1024; // Maksimum 64 MiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 140 KiB.
    N max_memory_ = J_min( max_memory_step, max_memory );
    N table_address = (N)E_simple_Z_p_I_align_down_to_v2( E_main_S_memory_map, H_oux_E_mem_S_page_size ); // Start poniżej tablicy pamięci, malejąco.
    struct E_main_Z_memory_map_entry *memory_map_ = E_main_S_memory_map;
    volatile N *pml4 = (P)( table_address -= H_oux_E_mem_S_page_size );
    N physical_size = -H_oux_E_mem_S_page_size;
    B end = no;
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
        if( !end )
        {   volatile N *pdpt = (P)( table_address -= H_oux_E_mem_S_page_size );
            for_n( pdpt_i, table_n )
                if( !end )
                {   volatile N *pd = (P)( table_address -= H_oux_E_mem_S_page_size );
                    for_n( pd_i, table_n )
                        if( !end )
                        {   volatile N *pt = (P)( table_address -= H_oux_E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory_ - H_oux_E_mem_S_page_size )
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
    struct E_main_Z_memory_map_entry *memory_map_entry_available = E_main_S_memory_map;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available++;
    N size = -H_oux_E_mem_S_page_size;
    if( max_memory <= max_memory_step )
        goto End;
    max_memory_step = 16UL * 1024 * 1024 * 1024; // Maksimum 16 GiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 32 MiB.
    E_main_I_allocate_page_table_0_I_allocate( J_min( max_memory_step, max_memory )
    , &memory_map_entry_available, &size
    );
    if( max_memory <= max_memory_step )
        goto End;
    max_memory_step = 4UL * 1024 * 1024 * 1024 * 1024; // Maksimum 4 TiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 8 GiB.
    E_main_I_allocate_page_table_0_I_allocate( J_min( max_memory_step, max_memory )
    , &memory_map_entry_available, &size
    );
    if( max_memory <= max_memory_step )
        goto End;
    E_main_I_allocate_page_table_0_I_allocate( max_memory // Maksimum 256 TiB pamięci. Tablice stron pamięci zajmują maksymalnie ok. 513 GiB.
    , &memory_map_entry_available, &size
    );
End:*start_end_address = memory_map_entry_available->virtual_start + size + H_oux_E_mem_S_page_size;
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
, N *pml4_
, N *start_end_address
, N *additional_pages
, N memory_size_0
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    struct E_main_Z_memory_map_entry *memory_map_entry_available;
    memory_map_entry_available = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - 1;
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available--;
    N size = memory_map_entry_available->size - H_oux_E_mem_S_page_size;
    N pml4_physical = memory_map_entry_available->physical_start + size;
    volatile N *pml4 = (P)( memory_size_0 -= H_oux_E_mem_S_page_size );
    N physical_size = -H_oux_E_mem_S_page_size;
    B end = no;
    *additional_pages = 0;
    N additional_pages_ = 1UL << 15; //CONF Maksymalna liczba (‘guard pages’ stosów) ‹zadań› w systemie.
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
    {   if( !end
        || additional_pages_
        )
        {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
            N pdpt_physical = memory_map_entry_available->physical_start + size;
            volatile N *pdpt = (P)( memory_size_0 -= H_oux_E_mem_S_page_size );
            for_n( pdpt_i, table_n )
            {   if( !end
                || additional_pages_
                )
                {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
                    N pd_physical = memory_map_entry_available->physical_start + size;
                    volatile N *pd = (P)( memory_size_0 -= H_oux_E_mem_S_page_size );
                    for_n( pd_i, table_n )
                    {   if( !end
                        || additional_pages_
                        )
                        {   E_main_I_allocate_page_table_I_next_page( yes, &memory_map_entry_available, &size );
                            N pt_physical = memory_map_entry_available->physical_start + size;
                            volatile N *pt = (P)( memory_size_0 -= H_oux_E_mem_S_page_size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory - H_oux_E_mem_S_page_size )
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
                                            || physical_address == E_main_S_sata_ahci_addresses[i] + H_oux_E_mem_S_page_size
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
                                        || ( physical_address >= E_main_S_ethernet_address
                                          && physical_address < E_main_S_ethernet_address + 0x20000
                                        )
                                        || physical_address == E_main_S_ethernet_eeprom_address
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pwt | E_cpu_Z_page_entry_S_pcd;
                                        else if( physical_address >= E_simple_Z_n_I_align_down_to_v2( E_main_S_framebuffer_physical, H_oux_E_mem_S_page_size )
                                        && physical_address < E_simple_Z_n_I_align_up_to_v2( E_main_S_framebuffer_physical + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, H_oux_E_mem_S_page_size )
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pat;
                                    }
                                }else
                                {   pt[ pt_i ] = 0;
                                    if( additional_pages_ )
                                        additional_pages_--;
                                    ( *additional_pages )++;
                                }
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pt_physical;
                        }else
                            pd[ pd_i ] = 0;
                    }
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pd_physical;
                }else
                    pdpt[ pdpt_i ] = 0;
            }
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pdpt_physical;
        }else
            pml4[ pml4_i ] = 0;
    }
    *pml4_ = pml4_physical;
    *start_end_address = memory_map_entry_available->virtual_start + size;
}
void
E_main_I_allocate_page_table( N max_memory
, N *pml4_
, N *start_end_address
, N *additional_pages
){  struct E_main_Z_memory_map_entry *memory_map = E_main_S_memory_map;
    //DFN Jest co najmniej jeden blok pamięci typu “E_main_Z_memory_table_Z_memory_type_S_available”.
    struct E_main_Z_memory_map_entry *memory_map_entry_available;
    memory_map_entry_available = memory_map;
    while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_available )
        memory_map_entry_available++;
    N size = 0;
    N pml4_physical = memory_map_entry_available->physical_start + size;
    volatile N *pml4 = (P)(N)( memory_map_entry_available->virtual_start + size );
    N physical_size = -H_oux_E_mem_S_page_size;
    B end = no;
    *additional_pages = 0;
    N additional_pages_ = 1UL << 15; //CONF Maksymalna liczba (‘guard pages’ stosów) ‹zadań› w systemie.
    const N table_n = H_oux_E_mem_S_page_size / sizeof(N);
    for_n( pml4_i, table_n )
    {   if( !end
        || additional_pages_
        )
        {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
            N pdpt_physical = memory_map_entry_available->physical_start + size;
            volatile N *pdpt = (P)(N)( memory_map_entry_available->virtual_start + size );
            for_n( pdpt_i, table_n )
            {   if( !end
                || additional_pages_
                )
                {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
                    N pd_physical = memory_map_entry_available->physical_start + size;
                    volatile N *pd = (P)(N)( memory_map_entry_available->virtual_start + size );
                    for_n( pd_i, table_n )
                    {   if( !end
                        || additional_pages_
                        )
                        {   E_main_I_allocate_page_table_I_next_page( no, &memory_map_entry_available, &size );
                            N pt_physical = memory_map_entry_available->physical_start + size;
                            volatile N *pt = (P)(N)( memory_map_entry_available->virtual_start + size );
                            for_n( pt_i, table_n )
                                if( !end )
                                {   N virtual_address = ( pml4_i << 39 ) | ( pdpt_i << 30 ) | ( pd_i << 21 ) | ( pt_i * H_oux_E_mem_S_page_size );
                                    if( virtual_address == max_memory - H_oux_E_mem_S_page_size )
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
                                            || physical_address == E_main_S_sata_ahci_addresses[i] + H_oux_E_mem_S_page_size
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
                                        || ( physical_address >= E_main_S_ethernet_address
                                          && physical_address < E_main_S_ethernet_address + 0x20000
                                        )
                                        || physical_address == E_main_S_ethernet_eeprom_address
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pwt | E_cpu_Z_page_entry_S_pcd;
                                        else if( physical_address >= E_simple_Z_n_I_align_down_to_v2( E_main_S_framebuffer_physical, H_oux_E_mem_S_page_size )
                                        && physical_address < E_simple_Z_n_I_align_up_to_v2( E_main_S_framebuffer_physical + E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8, H_oux_E_mem_S_page_size )
                                        )
                                            pt[ pt_i ] |= E_cpu_Z_page_entry_S_pat;
                                    }
                                }else
                                {   pt[ pt_i ] = 0;
                                    if( additional_pages_ )
                                        additional_pages_--;
                                    ( *additional_pages )++;
                                }
                            pd[ pd_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pt_physical;
                        }else
                            pd[ pd_i ] = 0;
                    }
                    pdpt[ pdpt_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pd_physical;
                }else
                    pdpt[ pdpt_i ] = 0;
            }
            pml4[ pml4_i ] = E_cpu_Z_page_entry_S_present | E_cpu_Z_page_entry_S_write | pdpt_physical;
        }else
            pml4[ pml4_i ] = 0;
    }
    E_main_Q_cr3_P( pml4_physical );
    *pml4_ = (N)pml4;
    *start_end_address = memory_map_entry_available->virtual_start + size + H_oux_E_mem_S_page_size;
}
//------------------------------------------------------------------------------
__attribute__ ((__noreturn__))
void
main( struct E_main_Z_memory_map_entry *memory_map
, struct E_main_Z_video *video
){  E_main_S_memory_map = memory_map;
    N32 eax = 1, ebx, ecx, edx;
    __asm__ volatile (
    "\n" "cpuid"
    : "+a" (eax), "=b" (ebx), "=c" (ecx), "=d" (edx)
    );
    E_main_S_sse = edx & ( 1 << 25 );
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
    if( K_error( E_main_I_allocate_page_table_pre() ))
        goto End;
    if( K_error( E_acpi_I_search() ))
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
    v = E_main_Q_msr_R( 0x1b );
    v |= ( 1 << 11 ) | ( 1 << 10 );
    E_main_Q_msr_P( 0x1b, v );
    E_main_S_sata_ahci_n = 0;
    // Przygotowanie tablicy zakresów pamięci.
    if( K_error( E_pci_I_check_buses() ))
        goto End;
    E_main_S_kernel_args.processor_start_page = 0xf000;
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry )
    { E_main_S_kernel_args.processor_start_page
    , H_oux_E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
    };
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry )
    { E_main_S_boot_loader_start
    , E_main_S_boot_loader_end - E_main_S_boot_loader_start
    , E_main_Z_memory_table_Z_memory_type_S_boot_loader
    };

    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ 0x80000
    //, 0x80000
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ 0x100000
    //, 0x20000
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ 0x17d000
    //, 0x3000
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ 0x19e000
    //, 0x2000
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ 0x140000
    //, 0x40000
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    { 0
    , E_main_S_kernel_args.processor_start_page
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    };
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN Potrzebne tylko dla symulatora Vmware. Powinny być realne zakresy zastrzeżone.
    { E_main_S_boot_loader_end
    , 0x200000 - E_main_S_boot_loader_end
    , E_main_Z_memory_table_Z_memory_type_S_reserved
    };

    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) // ISA
    //{ 0xf00000
    //, 0x100000
    //, E_main_Z_memory_table_Z_memory_type_S_reserved
    //};
    if( E_main_S_kernel_args.pcie_base_address )
        *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) // PCI Express ECAM
        { (N)E_main_S_kernel_args.pcie_base_address
        , 256 * 32 * 8 * 4096
        , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        };
    if( E_main_S_kernel_args.acpi.hpet.address.address )
        *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) // HPET
        { E_main_S_kernel_args.acpi.hpet.address.address
        , H_oux_E_mem_S_page_size
        , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
        };
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) // TPM
    //{ 0xfed40000
    //, 5 * H_oux_E_mem_S_page_size
    //, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    //};
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry )
    { E_main_S_framebuffer_physical
    , E_main_S_kernel_args.framebuffer.height * E_main_S_kernel_args.framebuffer.pixels_per_scan_line * E_main_S_kernel_args.framebuffer.bits_per_pixel / 8
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry )
    { (N)E_main_S_kernel_args.local_apic_address
    , H_oux_E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    *--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry )
    { (N)E_main_S_kernel_args.io_apic_address
    , H_oux_E_mem_S_page_size
    , E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
    };
    E_main_Q_memory_map_I_align();
    //N top_of_memory = E_main_Q_memory_map_R_start_of_memory();
    //*--E_main_S_memory_map = ( struct E_main_Z_memory_map_entry ) //NDFN
    //{ top_of_memory -= 0x800000
    //, 0x800000
    //, E_main_Z_memory_table_Z_memory_type_S_reserved
    //};

    E_main_Q_memory_map_I_sort_physical();
    if( K_error( E_main_Q_memory_map_I_remove_overlapped() ))
        goto End;
    E_main_Q_memory_map_I_remove_bad();

//N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
//for_n( i, n - 1 )
    //if( E_main_S_memory_map[i].physical_start + E_main_S_memory_map[i].size > E_main_S_memory_map[ i + 1 ].physical_start )
        //O{  __asm__ volatile (
            //"\n" "pause"
            //);
        //}
//O{  __asm__ volatile (
    //"\n" "hlt"
    //);
//}

//N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
//for_n( i, n )
//{   if( E_main_S_memory_map[i].type == E_main_Z_memory_table_Z_memory_type_S_boot_loader )
        //continue;
    //if(( E_main_S_memory_map[i].physical_start <= E_main_S_boot_loader_start
      //&& E_main_S_memory_map[i].physical_start + E_main_S_memory_map[i].size >= E_main_S_boot_loader_end
    //)
    //|| ( E_main_S_memory_map[i].physical_start <= E_main_S_boot_loader_start
      //&& E_main_S_memory_map[i].physical_start + E_main_S_memory_map[i].size > E_main_S_boot_loader_start
    //)
    //|| ( E_main_S_memory_map[i].physical_start < E_main_S_boot_loader_end
      //&& E_main_S_memory_map[i].physical_start + E_main_S_memory_map[i].size >= E_main_S_boot_loader_end
    //))
        //O{  __asm__ volatile (
            //"\n" "pause"
            //);
        //}
//}
//O{  __asm__ volatile (
    //"\n" "hlt"
    //);
//}

    E_main_Q_memory_map_I_set_virtual_0();
    E_main_Q_memory_map_I_sort_virtual();
    //if( top_of_memory < 2 * 1024 * 1024 ) // NDFN
        //goto End;

//N n = ( struct E_main_Z_memory_map_entry * )E_main_Z_memory_table_S_end - E_main_S_memory_map;
//for_n( i, n - 1 )
    //if( E_main_S_memory_map[i].type == E_main_Z_memory_table_Z_memory_type_S_available
    //&& E_main_S_memory_map[ i + 1 ].type == E_main_Z_memory_table_Z_memory_type_S_available
    //&& E_main_S_memory_map[i].physical_start > E_main_S_memory_map[ i + 1 ].physical_start
    //)
        //O{  __asm__ volatile (
            //"\n" "pause"
            //);
        //}
//O{  __asm__ volatile (
    //"\n" "hlt"
    //);
//}

    N memory_size_0 = E_main_Q_memory_map_R_size_0();
    N start_end_address;
    E_main_I_allocate_page_table_0( memory_size_0, &start_end_address );
    if( start_end_address < E_main_S_boot_loader_start )
    {   struct E_main_Z_memory_map_entry *memory_map_entry_available = E_main_S_memory_map;
        while( memory_map_entry_available->type != E_main_Z_memory_table_Z_memory_type_S_boot_loader )
            memory_map_entry_available++;
        memory_map_entry_available++;
        start_end_address = memory_map_entry_available->virtual_start;
    }
    N loader_start_0 = start_end_address;
    N loader_start_physical = (N)E_main_Z_p_I_to_physical( (P)loader_start_0 );
    //N reserved_size = E_main_Q_memory_map_R_reserved_size();
    B reserved_from_end = yes; //CONF
    N memory_size = E_main_Q_memory_map_R_size();
    //N reserved_size_from_start;
    //if( reserved_from_end )
    //{   reserved_size_from_start = E_main_Q_memory_map_R_reserved_size_from_start();
        //if( memory_size - ( reserved_size - reserved_size_from_start ) - H_oux_E_mem_S_page_size > 0x100000000UL - H_oux_E_mem_S_page_size )
            //reserved_from_end = no;
    //}
    E_main_Q_memory_map_I_sort_physical();
    E_main_Q_memory_map_I_join_physical();
    if( K_error( E_main_Q_memory_map_I_set_virtual( reserved_from_end )))
        goto End;
    E_main_Q_memory_map_I_sort_virtual();
    N loader_start = (N)E_main_Z_p_I_to_virtual( (P)loader_start_physical, E_main_Z_memory_table_S_end );
    N pml4;
    E_main_I_allocate_page_table_1( memory_size, &pml4, &start_end_address, &E_main_S_kernel_args.additional_pages, memory_size_0 );
    E_main_Z_remap_jump remap_jump = (P)(N)E_main_S_kernel_args.processor_start_page;
    E_mem_Q_blk_I_copy( remap_jump, &E_remap_jump_I, ( Pc )&E_remap_jump_I_end - ( Pc )&E_remap_jump_I );
    E_mem_Q_blk_I_copy( (P)loader_start_0, (P)E_main_S_boot_loader_start, E_main_S_boot_loader_end - E_main_S_boot_loader_start );
    E_main_Q_loader_I_relocate( loader_start_0, loader_start );
    remap_jump( pml4, loader_start - E_main_S_boot_loader_start );
    E_main_S_memory_map = (P)( loader_start + ( Pc )E_main_S_memory_map - E_main_S_boot_loader_start );
    N memory_table_S_end = loader_start + E_main_Z_memory_table_S_end - E_main_S_boot_loader_start;
    if( reserved_from_end )
        pml4 = (N)E_main_Z_p_I_to_virtual( (P)pml4, memory_table_S_end );
    else
        E_main_I_allocate_page_table( memory_size, &pml4, &start_end_address, &E_main_S_kernel_args.additional_pages );
    E_main_S_kernel_args.framebuffer.p = E_main_Z_p_I_to_virtual( (P)E_main_S_framebuffer_physical, memory_table_S_end );
    if( !E_main_S_kernel_args.framebuffer.p )
        goto End;

    if( K_error( E_mem_M_test( reserved_from_end, start_end_address, loader_start, memory_size )))
        goto End;
    if( K_error( E_font_M() ))
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
    E_font_I_print_hex( pml4 );
    N n = ( struct E_main_Z_memory_map_entry * )memory_table_S_end - E_main_S_memory_map;
    for_n( i, n )
        if( E_main_S_memory_map[i].type == E_main_Z_memory_table_Z_memory_type_S_available )
        {   E_font_I_print( ",t=" );
            E_font_I_print_hex( E_main_S_memory_map[i].type );
            E_font_I_print( ",a=" );
            E_font_I_print_hex( E_main_S_memory_map[i].virtual_start );
            E_font_I_print( "," );
            E_font_I_print_hex( E_main_S_memory_map[i].physical_start );
            E_font_I_print( ",s=" );
            E_font_I_print_hex( E_main_S_memory_map[i].size );
        }
    __asm__ volatile (
    "\n" "sfence"
    );
/*
    N page_table_size = reserved_from_end
    ? memory_size - ( reserved_size - reserved_size_from_start ) - start_end_address
    : start_end_address - ( reserved_size_from_start > 0x10000 ? reserved_size_from_start : 0x10000 );
    E_main_S_kernel_args.page_table = (P)( reserved_from_end ? start_end_address : start_end_address - page_table_size );

    E_main_S_kernel_args.memory_map_n = E_main_Q_memory_map_R_saved_n();
    N memory_map_size = E_main_S_kernel_args.memory_map_n * sizeof( *E_main_S_kernel_args.memory_map );
    E_main_S_kernel_args.memory_map = (P)( reserved_from_end
    ? ( Pc )E_main_S_kernel_args.page_table - memory_map_size
    : ( Pc )E_main_S_kernel_args.page_table + page_table_size
    );
    E_main_S_kernel_args.stack_pages = 1 + 1; //CONF Co najmniej 2 strony pamięci, ponieważ jedna będzie zmieniona na ‘guard page’.
    N stack_size = E_main_S_kernel_args.stack_pages * H_oux_E_mem_S_page_size;
    E_main_S_kernel_args.kernel_stack = (P)( reserved_from_end
    ? E_simple_Z_n_I_align_down_to_v2( (N)E_main_S_kernel_args.memory_map, H_oux_E_mem_S_page_size ) - stack_size
    : memory_size - stack_size
    );
*/
    //TODO Przemapować pamięć dla ciągłego obszaru wolnego, utworzyć tablicę obszarów pamięci dla ‘kernela’, wyszukać tablice ACPI, wczytać z OUXFS i ‘realokować’ ‘kernel’, ustawić flagi rejestrów CR.
    //E_mem_M();
    //E_mem_M_free();
End:O{  __asm__ volatile (
        "\n" "hlt"
        );
    }
    __builtin_unreachable();
}
/******************************************************************************/
