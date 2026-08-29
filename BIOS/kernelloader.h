/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   BIOS boot loader
*         include
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 e
*******************************************************************************/
#include <stdarg.h>
//==============================================================================
typedef unsigned char       N8;
typedef signed char         S8;
typedef unsigned short      N16;
typedef short               S16;
typedef unsigned            N32;
typedef int                 S32;
typedef unsigned long       N64;
typedef long                S64;
typedef _Bool               B;
typedef char                C;
typedef N64                 N;
typedef S64                 S;
typedef N32                 U;
typedef double              F;
typedef void                *P;
typedef C                   *Pc;
typedef N                   *Pn;
typedef unsigned __int128   N128;
typedef __int128            S128;
//------------------------------------------------------------------------------
#define false                               0
#define true                                1
#define no                                  false
#define yes                                 true
#define _v(a,v)                             (( (a) ^ (a) ) | (v) )
//------------------------------------------------------------------------------
#define _J_ab(a,b)                          a##b
#define J_ab(a,b)                           _J_ab(a,b)
#define J_a_b(a,b)                          J_ab(J_ab(a,_),b)
#define J_s0_R_l(s)                         ( sizeof(s) - 1 )
#define J_a_R_n(a)                          ( sizeof(a) / sizeof( (a)[0] ))
//------------------------------------------------------------------------------
#define J_swap(type,a,b)                    { type J_autogen(_) = a; a = b; b = J_autogen(_); }
#define J_min(a,b)                          ( (a) > (b) ? (b) : (a) )
#define J_max(a,b)                          ( (a) < (b) ? (b) : (a) )
#define J_min_max(a,b,c)                    ( J_min( (a), J_max( (b), (c) )))
#define J_abs(v)                            ( (v) < 0 ? -(v) : (v) )
//------------------------------------------------------------------------------
#define J_autogen_S                         _autogen
#define J_autogen(a)                        J_a_b( a, J_autogen_S )
#define J_autogen_line(a)                   J_autogen( J_a_b( a, __LINE__ ))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define O                                   while(yes)
#define for_n_(i_var,n)                     for( i_var = 0; i_var != (n); i_var++ )
#define for_n(i_var,n)                      N i_var; for_n_(i_var,(n))
#define for_n_rev_(i_var,n)                 i_var = (n); while( i_var-- )
#define for_n_rev(i_var,n)                  N for_n_rev_(i_var,(n))
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _0(pointer_variable,l)              E_mem_Q_blk_P_fill_c( (pointer_variable), l, 0 )
#define _0_(pointer_variable)               _0( (pointer_variable), sizeof( *(pointer_variable) ))
#define _0t_(pointer_variable,n)            _0( (pointer_variable), (n) * sizeof( *(pointer_variable) ))
#define M(l)                                E_mem_Q_blk_M(l)
#define Mt(u,n)                             E_mem_Q_blk_M_tab((u),(n))
#define W(pointer_variable)                 E_mem_Q_blk_W( pointer_variable )
#define M_(pointer_variable)                pointer_variable = M( sizeof( *( pointer_variable )))
#define Mt_(pointer_variable,n)             pointer_variable = Mt( sizeof( *( pointer_variable )), (n) )
#define W_(pointer_variable)                ( W( pointer_variable ), pointer_variable = 0 )
#define W_tab_(pointer_variable)            ( E_mem_Q_tab_W( pointer_variable ), pointer_variable = 0 )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _inline                             static __attribute__ (( __always_inline__, __unused__ ))
#define _internal                           static
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define K_error(error) \
  ( (S)(error) < 0 \
  && (S)(error) > ~5 \
  )
// Emisja ‹zdarzenia› w procedurze — dla otaczania wywołań procedur, które zwracają kod błędu.
#define K(statement) \
  N J_autogen_line(r) = (statement); \
  if( K_error( J_autogen_line(r) ) \
  && ~J_autogen_line(r) \
  ) \
      return J_autogen_line(r); \
  if( ~J_autogen_line(r) ) \
  { \
  }else
// Emisja ‹zdarzenia› w procedurze — dla otaczania wywołań procedur, które zwracają adres.
#define Kp(statement) \
  N J_autogen_line(r) = (N)(statement); \
  if( K_error( J_autogen_line(r) )) \
      return J_autogen_line(r); \
  if( J_autogen_line(r) ) \
  { \
  }else
// Emisja ‹zdarzenia› w bloku wyjścia procedury — dla otaczania wywołań procedur, które zwracają kod błędu.
#define K_(error,statement) \
  N J_autogen_line(r) = (statement); \
  if( !K_error( J_autogen_line(r) )) \
  { \
  }else \
      return J_autogen_line(r) < (error) ? J_autogen_line(r) : (error)
// Emisja ‹zdarzenia› w bloku wyjścia procedury — dla otaczania wywołań procedur, które zwracają adres.
#define Kp_(error,statement) \
  N J_autogen_line(r) = (N)(statement); \
  if( K_error( J_autogen_line(r) )) \
      return J_autogen_line(r) < (error) ? J_autogen_line(r) : (error); \
  if( J_autogen_line(r) ) \
  { \
  }else \
      return (error)
//------------------------------------------------------------------------------
// Emisja ‹zdarzenia› w procedurze zwracającej adres — dla otaczania wywołań procedur, które zwracają kod błędu.
#define KP(statement) \
  N J_autogen_line(r) = (statement); \
  if( K_error( J_autogen_line(r) ) \
  && ~J_autogen_line(r) \
  ) \
      return (P)J_autogen_line(r); \
  if( ~J_autogen_line(r) ) \
  { \
  }else
// Emisja ‹zdarzenia› w procedurze zwracającej adres — dla otaczania wywołań procedur, które zwracają adres.
#define KPp(statement) \
  N J_autogen_line(r) = (N)(statement); \
  if( K_error( J_autogen_line(r) )) \
      return (P)J_autogen_line(r); \
  if( J_autogen_line(r) ) \
  { \
  }else
// Emisja ‹zdarzenia› w bloku wyjścia procedury zwracającej adres — dla otaczania wywołań procedur, które zwracają kod błędu.
#define KP_(error,statement) \
  N J_autogen_line(r) = (statement); \
  if( !K_error( J_autogen_line(r) )) \
  { \
  }else \
      return (P)( J_autogen_line(r) < (error) ? J_autogen_line(r) : (error) )
// Emisja ‹zdarzenia› w bloku wyjścia procedury zwracającej adres — dla otaczania wywołań procedur, które zwracają adres.
#define KPp_(error,statement) \
  N J_autogen_line(r) = (N)(statement); \
  if( K_error( J_autogen_line(r) )) \
      return (P)( J_autogen_line(r) < (error) ? J_autogen_line(r) : (error) ); \
  if( J_autogen_line(r) ) \
  { \
  }else \
      return (P)(error)
//==============================================================================
#include "simple.h"
//==============================================================================
#define H_oux_E_mem_S_page_size             0x1000
#define E_main_Z_memory_table_S_end         0x7e000
//==============================================================================
struct E_datetime_Z
{ N16 year;
  N8 month;
  N8 day;
  N8 hour;
  N8 minute;
  N8 second;
  N32 nanosecond;
};
//==============================================================================
N E_mem_M_test( N, N );
N E_mem_M( B, N, N, N, N, N, N, N, N, N, N, N );
B E_mem_Q_blk_T_eq( P, P, N );
void E_mem_Q_blk_I_copy( P, P, N );
void E_mem_Q_blk_P_fill_c( P, N, C );
P E_mem_Q_blk_M(N);
P E_mem_Q_blk_M_tab( N, N );
P E_mem_Q_blk_M_align( N, N );
P E_mem_Q_blk_M_align_tab( N, N, N );
P E_mem_Q_blk_M_replace_tab( P, N, N );
P E_mem_Q_blk_M_replace( P, N );
P E_mem_Q_blk_M_split( P, N );
N E_mem_Q_blk_W(P);
P E_mem_Q_blk_I_add( P, N, N *, N * );
P E_mem_Q_blk_I_prepend_append( P, N, N );
P E_mem_Q_blk_I_append( P, N, N );
P E_mem_Q_blk_I_prepend( P, N );
P E_mem_Q_blk_I_insert( P, N, N );
P E_mem_Q_blk_I_remove( P, N, N );
//==============================================================================
enum
{ E_main_Z_memory_table_Z_memory_type_S_available = 1
, E_main_Z_memory_table_Z_memory_type_S_reserved
, E_main_Z_memory_table_Z_memory_type_S_acpi_reclaim
, E_main_Z_memory_table_Z_memory_type_S_acpi_nvs
, E_main_Z_memory_table_Z_memory_type_S_bad
, E_main_Z_memory_table_Z_memory_type_S_boot_loader = 0xffff0000
, E_main_Z_memory_table_Z_memory_type_S_memory_mapped_io
, E_main_Z_memory_table_Z_memory_type_S_processor_startup_page
};
struct __attribute__(( __packed__ )) E_main_Z_memory_map_entry
{ N64 physical_start;
  N64 size;
  N32 type;
  N64 virtual_start;
};
//------------------------------------------------------------------------------
struct H_uefi_Z_table_header
{ N64 signature;
  N32 revision;
  N32 header_l;
  N32 crc32;
  N32 reserved;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define H_uefi_Z_guid_S_acpi_table { 0x8868e871, 0xe4f1, 0x11d3, { 0xbc, 0x22, 0, 0x80, 0xc7, 0x3c, 0x88, 0x81 } }
#define H_uefi_Z_guid_S_acpi1_table { 0xeb9d2d30, 0x2d88, 0x11d3, { 0x91, 0x16, 0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#define H_uefi_Z_guid_S_sal_system_table { 0xeb9d2d32, 0x2d88, 0x11d3, { 0x9a, 0x16, 0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#define H_uefi_Z_guid_S_smbios_table { 0xeb9d2d31, 0x2d88, 0x11d3, { 0x9a, 0x16, 0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#define H_uefi_Z_guid_S_smbios3_table { 0xf2fd1544, 0x9794, 0x4a2c, { 0x99, 0x2e, 0xe5, 0xbb, 0xcf, 0x20, 0xe3, 0x94 } }
#define H_uefi_Z_guid_S_mps_table { 0xeb9d2d2f, 0x2d88, 0x11d3, { 0x9a, 0x16, 0, 0x90, 0x27, 0x3f, 0xc1, 0x4d } }
#define H_uefi_Z_guid_S_dtb_table { 0xb1b621d5, 0xf19c, 0x41a5, { 0x83, 0xb, 0xd9, 0x15, 0x2c, 0x69, 0xaa, 0xe0 } }
#define H_uefi_Z_guid_S_rt_properties_table { 0xeb66918a, 0x7eef, 0x402a, { 0x84, 0x2e, 0x93, 0x1d, 0x21, 0xc3, 0x8a, 0xe9 } }
#define H_uefi_Z_guid_S_memory_attributes_table { 0xdcfa911d, 0x26eb, 0x469f, { 0xa2, 0x20, 0x38, 0xb7, 0xdc, 0x46, 0x12, 0x20 } }
#define H_uefi_Z_guid_S_conformance_profiles_table { 0x36122546, 0xf7e7, 0x4c8f, { 0xbd, 0x9b, 0xeb, 0x85, 0x25, 0xb5, 0xc, 0xb } }
#define H_uefi_Z_guid_S_memory_range_capsule { 0xde9f0ec, 0x88b6, 0x428f, { 0x97, 0x7a, 0x25, 0x8f, 0x1d, 0xe, 0x5e, 0x72 } }
#define H_uefi_Z_guid_S_debug_image_info_table { 0x49152e77, 0x1ada, 0x4764, { 0xb7, 0xa2, 0x7a, 0xfe, 0xfe, 0xd9, 0x5e, 0x8b } }
#define H_uefi_Z_guid_S_system_resource_table { 0xb122a263, 0x3661, 0x4f68, { 0x99, 0x29, 0x78, 0xf8, 0xb0, 0xd6, 0x21, 0x80 } }
#define H_uefi_Z_guid_S_image_security_database { 0xd719b2cb, 0x3d3a, 0x4596, { 0xa3, 0xbc, 0xda, 0xd0, 0xe, 0x67, 0x65, 0x6f } }
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_local_apic
{ N8 type;
  N8 l;
  N8 processor;
  N8 id;
  N32 flags;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_io_apic
{ N8 type;
  N8 l;
  N8 id;
  N8 reserved;
  N32 address;
  N32 gsi_base;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_source_override
{ N8 type;
  N8 l;
  N8 bus;
  N8 source;
  N32 gsi;
  N16 flags;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_madt_Z_local_apic_nmi
{ N8 type;
  N8 l;
  N8 processor;
  N16 flags;
  N8 int_;
};
struct E_main_Z_apic_source_override
{ N8 source;
  N8 gsi;
  N8 flags;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_rsdp
{ C signature[8];
  N8 checksum;
  C OEM_id[6];
  N8 revision;
  N32 RSDT_address;
  N32 length;
  N64 XSDT_address;
  N8 checksum_2;
  N8 reserved[3];
};
struct __attribute__ (( __packed__ )) H_acpi_Z_table_header
{ C signature[4];
  N32 length;
  N8 revision;
  N8 checksum;
  C OEM_id[6];
  C OEM_table_id[8];
  N32 OEM_revision;
  N32 creator_id;
  N32 creator_revision;
};
enum H_acpi_Z_generic_address_Z_space
{ H_acpi_Z_generic_address_Z_space_S_memory
, H_acpi_Z_generic_address_Z_space_S_io
, H_acpi_Z_generic_address_Z_space_S_pci_configuration
, H_acpi_Z_generic_address_Z_space_S_embedded_controller
, H_acpi_Z_generic_address_Z_space_S_smbus
, H_acpi_Z_generic_address_Z_space_S_pci_bar_target
, H_acpi_Z_generic_address_Z_space_S_ipmi
, H_acpi_Z_generic_address_Z_space_S_gpio
, H_acpi_Z_generic_address_Z_space_S_generic_serial_bus
, H_acpi_Z_generic_address_Z_space_S_platform_com_channel
, H_acpi_Z_generic_address_Z_space_S_platform_runtime_machanism
, H_acpi_Z_generic_address_Z_space_S_functional_fixed_hw = 0x7f
};
enum H_acpi_Z_generic_address_Z_access_size
{ H_acpi_Z_generic_address_Z_access_size_S_undefined
, H_acpi_Z_generic_address_Z_access_size_S_1
, H_acpi_Z_generic_address_Z_access_size_S_2
, H_acpi_Z_generic_address_Z_access_size_S_4
, H_acpi_Z_generic_address_Z_access_size_S_8
};
struct __attribute__ (( __packed__ )) H_acpi_Z_generic_address
{ N8 space;
  N8 width;
  N8 offset;
  N8 access_size;
  N64 address;
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_xsdt
{ struct H_acpi_Z_table_header header;
  N64 table_address[];
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_apic
{ struct H_acpi_Z_table_header header;
  N32 local_interrupt_controler;
  N32 flags;
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_dmar
{ struct H_acpi_Z_table_header header;
  N8 host_addreass_width;
  N8 flags;
  N8 reserved[10];
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_facs_v0
{ C signature[4];
  N32 length;
  N32 hardware_signature;
  N32 firmware_waking_vector;
  N32 global_lock;
  N32 flags;
  N8 reserved_1[8];
  N8 version;
  N8 reserved_2[31];
};
struct __attribute__ (( __packed__ )) H_acpi_Z_facs_v1
{ C signature[4];
  N32 length;
  N32 hardware_signature;
  N32 firmware_waking_vector;
  N32 global_lock;
  N32 flags;
  N64 ex_firmware_waking_vector;
  N8 version;
  N8 reserved[31];
};
struct __attribute__ (( __packed__ )) H_acpi_Z_facs
{ C signature[4];
  N32 length;
  N32 hardware_signature;
  N32 firmware_waking_vector;
  N32 global_lock;
  N32 flags;
  N64 ex_firmware_waking_vector;
  N8 version;
  N8 reserved_1[3];
  N32 OS_flags;
  N8 reserved_2[24];
};
//------------------------------------------------------------------------------
enum H_acpi_Z_fadt_Z_preferred_pm_profile
{ H_acpi_Z_fadt_Z_preferred_pm_profile_S_unspecified
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_desktop
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_mobile
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_workstation
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_enterprise_server
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_soho_server
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_appliance_pc
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_performance_server
, H_acpi_Z_fadt_Z_preferred_pm_profile_S_tablet
};
struct __attribute__ (( __packed__ )) H_acpi_Z_fadt_v3
{ struct H_acpi_Z_table_header header;
  N32 facs;
  N32 dsdt;
  N8 reserved_1;
  N8 preferred_pm_profile;
  N16 SCI_interrupt;
  N32 SMI_command;
  N8 SMI_ACPI_enable, SMI_ACPI_disable;
  N8 SMI_S4BIOS_request;
  N8 SMI_PSTATE_ACPI_control;
  N32 PM1a_event_block, PM1b_event_block;
  N32 PM1a_control_block, PM1b_control_block;
  N32 PM2_control_block;
  N32 PM_timer_block;
  N32 GPE0_block, GPE1_block;
  N8 PM1_event_length, PM1_control_length, PM2_control_length, PM_timer_length, GPE0_block_length, GPE1_block_length;
  N8 GPE1_base;
  N8 SMI_CST_ACPI_control;
  N16 PM_level2_latency, PM_level3_latency;
  N16 memory_cache_flush_size, memory_cache_flush_stride;
  N8 CPU_duty_offset, CPU_duty_width;
  N8 CMOS_alarm_day, CMOS_alarm_month;
  N8 CMOS_century;
  N16 IA_PC_boot_architecture_flags;
  N8 reserved_2;
  N32 flags;
  struct H_acpi_Z_generic_address reset_register;
  N8 reset_value;
  N8 reserved[3];
  N64 ex_facs;
  N64 ex_dsdt;
  struct H_acpi_Z_generic_address ex_PM1a_event_block, ex_PM1b_event_block;
  struct H_acpi_Z_generic_address ex_PM1a_control_block, ex_PM1b_control_block;
  struct H_acpi_Z_generic_address ex_PM2_control_block;
  struct H_acpi_Z_generic_address ex_PM_timer_block;
  struct H_acpi_Z_generic_address ex_GPE0_block, ex_GPE1_block;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_fadt
{ struct H_acpi_Z_table_header header;
  N32 facs;
  N32 dsdt;
  N8 reserved_1;
  N8 preferred_pm_profile;
  N16 SCI_interrupt;
  N32 SMI_command;
  N8 SMI_ACPI_enable, SMI_ACPI_disable;
  N8 SMI_S4BIOS_request;
  N8 SMI_PSTATE_ACPI_control;
  N32 PM1a_event_block, PM1b_event_block;
  N32 PM1a_control_block, PM1b_control_block;
  N32 PM2_control_block;
  N32 PM_timer_block;
  N32 GPE0_block, GPE1_block;
  N8 PM1_event_length, PM1_control_length, PM2_control_length, PM_timer_length, GPE0_block_length, GPE1_block_length;
  N8 GPE1_base;
  N8 SMI_CST_ACPI_control;
  N16 PM_level2_latency, PM_level3_latency;
  N16 memory_cache_flush_size, memory_cache_flush_stride;
  N8 CPU_duty_offset, CPU_duty_width;
  N8 CMOS_alarm_day, CMOS_alarm_month;
  N8 CMOS_century;
  N16 IA_PC_boot_architecture_flags;
  N8 reserved_2;
  N32 flags;
  struct H_acpi_Z_generic_address reset_register;
  N8 reset_value;
  N16 ARM_boot_architecture_flags;
  N8 table_minor_version;
  N64 ex_facs;
  N64 ex_dsdt;
  struct H_acpi_Z_generic_address ex_PM1a_event_block, ex_PM1b_event_block;
  struct H_acpi_Z_generic_address ex_PM1a_control_block, ex_PM1b_control_block;
  struct H_acpi_Z_generic_address ex_PM2_control_block;
  struct H_acpi_Z_generic_address ex_PM_timer_block;
  struct H_acpi_Z_generic_address ex_GPE0_block, ex_GPE1_block;
  struct H_acpi_Z_generic_address sleep_control_register, sleep_status_register;
  N64 hypervisor_vendor_identity;
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_hpet
{ struct H_acpi_Z_table_header header;
  N8 hardware_rev_id;
  N8 comparator_count   :5;
  N8 counter_size       :1;
  N8 reserved           :1;
  N8 legacy_replacement :1;
  N16 pci_vendor_id;
  struct H_acpi_Z_generic_address address;
  N8 hpet_number;
  N16 minimum_tick;
  N8 page_protection;
};
struct H_oux_Z_hpet
{ N8 comparator_count   :5;
  N8 counter_size       :1;
  N8 legacy_replacement :1;
  struct H_acpi_Z_generic_address address;
  N8 hpet_number;
  N16 minimum_tick;
  N8 page_protection;
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_mcfg_entry
{ N64 base_address;
  N16 pci_segment;
  N8 start_bus;
  N8 end_bus;
  N32 reserved;
};
struct __attribute__ (( __packed__ )) H_acpi_Z_mcfg
{ struct H_acpi_Z_table_header header;
  N64 reserved;
  struct H_acpi_Z_mcfg_entry table[];
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_waet
{ struct H_acpi_Z_table_header header;
  N32 flags;
};
//------------------------------------------------------------------------------
struct __attribute__ (( __packed__ )) H_acpi_Z_wsmt
{ struct H_acpi_Z_table_header header;
  N32 flags;
};
//==============================================================================
#define E_mem_Q_blk_S_free_n_init       4
#define E_mem_Q_blk_S_allocated_n_init  6
struct H_oux_E_mem_Z_memory_map
{ N physical_start;
  N virtual_start;
  N pages;
};
struct E_mem_Q_blk_Z_free
{ Pc p;
  N l;
};
struct E_mem_Q_blk_Z_allocated
{ Pc p;
  N n;
  N u;
  P context_ip;
};
struct E_mem_blk_Z
{ struct E_mem_Q_blk_Z_allocated *allocated;
  N free_id, allocated_id;
  N *M_from_free_S_allocated_id[2];
  N M_from_free_S_table_id[2];
  N M_from_free_S_allocated_id_n;
  N memory_size;
  N reserved_size;
  B reserved_from_end;
};
//==============================================================================
struct H_main_Z_framebuffer
{ volatile N8 *p;
  N32 width, height;
  N32 pixels_per_scan_line;
  N8 bits_per_pixel;
  N8 blue_size, blue_shift;
  N8 green_size, green_shift;
  N8 red_size, red_shift;
};
struct H_main_Z_uefi_runtime_services
{ P R_time;
  P P_time;
  P R_wakeup_time;
  P P_wakeup_time;
  P R_variable;
  P R_next_variable_name;
  P P_variable;
  P R_next_high_monotonic_count;
  P reset_system;
  P update_capsule;
  P R_capsule_capabilities;
  P R_variable_info;
};
struct H_main_Z_kernel_Z_acpi
{ P dmar_content;
  N dmar_content_l;
  P dsdt_content;
  N dsdt_content_l;
  P facs;
  struct H_oux_Z_hpet hpet;
  struct
  { P address;
    N l;
  }ssdt_contents[4];
  N ssdt_contents_n;
  unsigned virt_guest_rtc_good                :1;
  unsigned virt_guest_pm_good                 :1;
  unsigned smm_validate_fixed_comm_buffers    :1;
  unsigned smm_validate_nested_ptr            :1;
  unsigned smm_system_resource_protection     :1;
};
struct E_interrupt_Z_gsi
{ N8 source;
  N8 flags;
};
struct E_main_Z_kernel_args
{ struct E_mem_blk_Z mem_blk;
  struct H_oux_E_mem_Z_memory_map *memory_map;
  N memory_map_n;
  P boot_loader;
  P kernel;
  P page_table;
  N additional_pages;
  P kernel_stack;
  N stack_pages;
  P pcie_base_address;
  struct H_main_Z_framebuffer framebuffer;
  struct H_main_Z_uefi_runtime_services uefi_runtime_services;
  struct H_main_Z_kernel_Z_acpi acpi;
  P local_apic_address;
  P io_apic_address;
  struct E_interrupt_Z_gsi *gsi;
  P *processor_proc;
  N32 processor_start_page;
  N32 processor_n;
  N8 gsi_n;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct E_base_Z_image_dos_header
{ N16 magic; // Magic number
  N16 cblp; // Bytes on last page of file
  N16 cp; // Pages in file
  N16 crlc; // Relocations
  N16 cparhdr; // Size of header in paragraphs
  N16 minalloc; // Minimum extra paragraphs needed
  N16 maxalloc; // Maximum extra paragraphs needed
  N16 ss; // Initial (relative) SS value
  N16 sp; // Initial SP value
  N16 csum; // Checksum
  N16 ip; // Initial IP value
  N16 cs; // Initial (relative) CS value
  N16 lfarlc; // File address of relocation table
  N16 ovno; // Overlay number
  N16 res[4]; // Reserved words
  N16 oemid; // OEM identifier (for e_oeminfo)
  N16 e_oeminfo; // OEM information; oemid specific
  N16 res_2[10]; // Reserved words
  N32 lfanew; // File address of new exe header
};
struct E_base_Z_image_file_header
{ N16 machine;
  N16 number_of_sections;
  N32 time_date_stamp;
  N32 pointer_to_symbol_table;
  N32 number_of_symbols;
  N16 size_of_optional_header;
  N16 characteristics;
};
struct E_base_Z_image_data_directory
{ N32 virtual_address;
  N32 size;
};
struct E_base_Z_image_optional_header64
{ N16 magic;
  N8 major_linker_version;
  N8 minor_linker_version;
  N32 size_of_code;
  N32 size_of_initialized_data;
  N32 size_of_uninitialized_data;
  N32 address_of_entry_point;
  N32 base_of_code;
  N64 image_base;
  N32 section_alignment;
  N32 file_alignment;
  N16 major_operating_system_version;
  N16 minor_operating_system_version;
  N16 major_image_version;
  N16 minor_image_version;
  N16 major_subsystem_version;
  N16 minor_subsystem_version;
  N32 win32_version_value;
  N32 size_of_image;
  N32 size_of_headers;
  N32 checksum;
  N16 subsystem;
  N16 dll_characteristics;
  N64 size_of_stack_reserve;
  N64 size_of_stack_commit;
  N64 size_of_heap_reserve;
  N64 size_of_heap_commit;
  N32 loader_flags;
  N32 number_of_rva_and_sizes;
  struct E_base_Z_image_data_directory data_directory[16];
};
struct E_base_Z_image_nt_headers64
{ N32 signature;
  struct E_base_Z_image_file_header file_header;
  struct E_base_Z_image_optional_header64 optional_header;
};
struct E_base_Z_image_relocation
{ N32 virtual_address;
  N32 size_of_block;
  N16 entries[];
};
//==============================================================================
N E_acpi_I_search( struct E_main_Z_memory_map_entry * );
//==============================================================================
N E_font_M(void);
N E_font_W(void);
N E_font_I_draw( U, N32, N32, N32, N8, N8 );
void E_font_I_print_nl(void);
void E_font_I_print_u(U);
N E_font_I_print( Pc );
void E_font_I_print_hex(N);
//==============================================================================
Pc E_mem_Q_mask_M( N );
B E_mem_Q_mask_R( Pc, N );
N E_mem_Q_mask_R_first_clear( Pc, N );
void E_mem_Q_mask_P_set( Pc, N );
void E_mem_Q_mask_P_clear( Pc, N );
N E_mem_Q_mask_I_resize( Pc *, N, N );
//==============================================================================
B E_text_Z_c_T_alpha(C);
B E_text_Z_c_T_quote(C);
C E_text_Z_c_I_lower(C);
C E_text_Z_c_I_upper(C);
Pc E_text_Z_s0_I_lower( Pc );
Pc E_text_Z_s0_I_upper( Pc );
Pc E_text_Z_s_I_lower( Pc, Pc );
Pc E_text_Z_s_I_upper( Pc, Pc );
B E_text_Z_s0_T_eq_s0( Pc, Pc );
B E_text_Z_s0_T_eq_case_s0( Pc, Pc );
N E_text_Z_s0_T_starts_s0( Pc, Pc );
N E_text_Z_s0_T_starts_case_s0( Pc, Pc );
B E_text_Z_s0_T_ends_s0( Pc, Pc );
B E_text_Z_s_T_ends_s0( Pc, Pc, Pc );
S E_text_Z_s0_I_cmp_s0( Pc, Pc );
S E_text_Z_sl_T_cmp( Pc, Pc, N );
N E_text_Z_s_T_starts_s0( Pc, Pc, Pc );
N E_text_Z_s_T_starts_case_s0( Pc, Pc, Pc );
B E_text_Z_s_T_eq_s0( Pc, Pc, Pc );
B E_text_Z_s_T_eq_case_s0( Pc, Pc, Pc );
B E_text_Z_s_T_eq_s( Pc, Pc, Pc, Pc );
B E_text_Z_sl_T_eq( Pc, Pc, N );
B E_text_Z_s_T_eq_case_s( Pc, Pc, Pc, Pc );
B E_text_Z_sl_T_eq_case( Pc, Pc, N );
Pc E_text_Z_s0_R_end_0_le( Pc, N );
B E_text_Z_s0_T_l_0_le( Pc, N );
Pc E_text_Z_s_R_search_0( Pc, Pc );
Pc E_text_Z_sl_R_search_0( Pc, N );
Pc E_text_Z_s_R_search_c( Pc, Pc, C );
Pc E_text_Z_s_R_search_c_( Pc, C );
Pc E_text_Z_s0_R_search_c( Pc, C );
Pc E_text_Z_s_R_search_last_c( Pc, Pc, C );
Pc E_text_Z_s0_R_search_last_c_( Pc, C );
Pc E_text_Z_sl_R_search_last_c( Pc, N, C );
Pc E_text_Z_s_R_search_s( Pc, Pc, Pc, Pc );
Pc E_text_Z_s_R_search_s0( Pc, Pc, Pc );
Pc E_text_Z_s0_R_search_s0( Pc, Pc );
N E_text_Z_s_N_n( Pc, Pc, Pc *, N );
N E_text_Z_s0_N_n( Pc, Pc *, N );
S E_text_Z_s_N_sn( Pc, Pc, Pc *, N );
S E_text_Z_s0_N_sn( Pc, Pc *, N );
struct E_datetime_Z E_text_Z_s_N_datetime( Pc, Pc, Pc * );
struct E_datetime_Z E_text_Z_s0_N_datetime( Pc, Pc * );
Pc E_text_Z_datetime_N_s0( struct E_datetime_Z );
Pc E_text_Z_n_N_s( Pc, N, N );
N E_text_Z_n_N_s_G( N, N );
Pc E_text_Z_n_N_bcd( Pc, N, N );
N E_text_Z_n_N_bcd_G( N );
Pc E_text_Z_ns_N_s( Pc, S, N );
N E_text_Z_ns_N_s_G( S, N );
Pc E_text_Z_s_P_c_fill( Pc, Pc, C );
void E_text_Z_s0_P_lower( Pc );
void E_text_Z_s_P_lower( Pc, Pc );
void E_text_Z_s0_P_upper( Pc );
void E_text_Z_s_P_upper( Pc, Pc );
Pc E_text_Z_s_P_copy_s( Pc, Pc, Pc );
Pc E_text_Z_s_P_copy_s_0( Pc, Pc, Pc );
Pc E_text_Z_s_P_copy_sl_0( Pc, Pc, N );
Pc E_text_Z_s_P_copy_s0( Pc, Pc );
Pc E_text_Z_s_P_copy_s0_0( Pc, Pc );
void E_text_Z_sl_P_rev( Pc, N );
Pc E_text_Z_s0_M_duplicate( Pc );
Pc E_text_Z_sl_M_duplicate( Pc, N );
Pc E_text_Z_s_I_append_c( Pc *, C );
Pc E_text_Z_s_I_append_s( Pc *, Pc, Pc );
Pc E_text_Z_s_I_s_append_0( Pc *, Pc, Pc );
Pc E_text_Z_s_I_append_sl( Pc *, Pc, N );
Pc E_text_Z_s_I_append_sl_0( Pc *, Pc, N );
Pc E_text_Z_s_I_append_s0( Pc *, Pc );
Pc E_text_Z_s_I_append_s0_0( Pc *, Pc );
Pc E_text_Z_s_I_prepend_s( Pc *, Pc, Pc );
Pc E_text_Z_s_I_prepend_s0( Pc *, Pc );
N E_text_Z_s0_I_clear( Pc * );
Pc E_text_Z_s0_I_append_c( Pc *, C );
Pc E_text_Z_s0_I_append_s0( Pc *, Pc );
Pc E_text_Z_s0_I_append_s( Pc *, Pc, Pc );
N E_text_Z_su0_R_n( Pc );
N E_text_Z_u_R_su( U, Pc );
N E_text_Z_u_R_su_G( U );
Pc E_text_Z_su_R_u( Pc, U * );
Pc E_text_Z_su_R_u_rev( Pc, U * );
B E_text_Z_su_T_correct( Pc );
N E_text_I_vsprintf( Pc *, Pc, va_list );
//==============================================================================
#define E_vga_S_background_color            0xdbdbdb
#define E_vga_S_text_color                  0
enum E_vga_Z_aa_pixel
{ E_vga_Z_aa_pixel_S_e = 1 << 0,
  E_vga_Z_aa_pixel_S_se = 1 << 1,
  E_vga_Z_aa_pixel_S_s = 1 << 2,
  E_vga_Z_aa_pixel_S_sw = 1 << 3,
  E_vga_Z_aa_pixel_S_w = 1 << 4,
  E_vga_Z_aa_pixel_S_nw = 1 << 5,
  E_vga_Z_aa_pixel_S_n = 1 << 6,
  E_vga_Z_aa_pixel_S_ne = 1 << 7
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
N32 E_vga_Z_color_M( N8, N8, N8 );
N8 E_vga_Z_color_R_red( N32 );
N8 E_vga_Z_color_R_green( N32 );
N8 E_vga_Z_color_R_blue( N32 );
N32 E_vga_Z_color_M_gray( N8 );
N32 E_vga_R_video_color( N32 );
N32 E_vga_R_pixel( N32, N32 );
void E_vga_P_pixel( N32, N32, N32 );
void E_vga_I_set_pixel_aa( N32, N32, N32, F, N );
void E_vga_I_draw_rect( N32, N32, N32, N32, N32 );
void E_vga_I_fill_rect( N32, N32, N32, N32, N32 );
/******************************************************************************/
