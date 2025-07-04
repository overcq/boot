/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         include
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 e
*******************************************************************************/
typedef unsigned char       N8;
typedef signed char         S8;
typedef unsigned short      N16;
typedef short               S16;
typedef unsigned            N32;
typedef int                 S32;
typedef unsigned long long  N64;
typedef long long           S64;
typedef _Bool               B;
typedef char                C;
typedef N16                 C16;
typedef N64                 N;
typedef S64                 S;
typedef N32                 U;
typedef double              F;
typedef void                *P;
typedef C                   *Pc;
typedef C16                 *Pc16;
typedef N                   *Pn;
    #ifdef __SSE__
typedef unsigned __int128   N128;
typedef __int128            S128;
    #endif
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
#define J_swap(type,a,b)                    { type J_autogen(c) = a; a = b; b = J_autogen(c); }
//------------------------------------------------------------------------------
#define J_autogen_S                         _autogen
#define J_autogen(a)                        J_a_b( a, J_autogen_S )
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
//==============================================================================
#include "simple.h"
//==============================================================================
#define H_oux_E_mem_S_page_size             0x1000
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define H_oux_J_align_up_p(p,t)             E_simple_Z_p_I_align_up_to_v2( p, sizeof(t) )
//==============================================================================
Pc16 E_text_Z_n_N_s( Pc16, N, N, N );
N E_text_Z_n_N_s_G( N, N, N );
//------------------------------------------------------------------------------
N E_mem_M( B, N, N, N, N, N, N, N, N, N, N, N, N, N );
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
#define H_uefi_Z_api __attribute__(( __ms_abi__ ))
#define H_uefi_S_error(e) ( (S)( 1LL << 63 ) | e )
//------------------------------------------------------------------------------
#define H_uefi_Z_error_S_success            0
#define H_uefi_Z_error_S_load_error         H_uefi_S_error(1)
#define H_uefi_Z_error_S_invalid_parameter  H_uefi_S_error(2)
#define H_uefi_Z_error_S_unsupported        H_uefi_S_error(3)
#define H_uefi_Z_error_S_bad_buffer_size    H_uefi_S_error(4)
#define H_uefi_Z_error_S_buffer_too_small   H_uefi_S_error(5)
#define H_uefi_Z_error_S_not_ready          H_uefi_S_error(6)
#define H_uefi_Z_error_S_device_error       H_uefi_S_error(7)
#define H_uefi_Z_error_S_write_protected    H_uefi_S_error(8)
#define H_uefi_Z_error_S_out_of_resoureces  H_uefi_S_error(9)
#define H_uefi_Z_error_S_volume_corrupted   H_uefi_S_error(10)
#define H_uefi_Z_error_S_volume_full        H_uefi_S_error(11)
#define H_uefi_Z_error_S_no_media           H_uefi_S_error(12)
#define H_uefi_Z_error_S_media_changed      H_uefi_S_error(13)
#define H_uefi_Z_error_S_not_found          H_uefi_S_error(14)
#define H_uefi_Z_error_S_access_denied      H_uefi_S_error(15)
#define H_uefi_Z_error_S_no_response        H_uefi_S_error(16)
#define H_uefi_Z_error_S_no_mapping         H_uefi_S_error(17)
#define H_uefi_Z_error_S_timeout            H_uefi_S_error(18)
#define H_uefi_Z_error_S_not_started        H_uefi_S_error(19)
#define H_uefi_Z_error_S_already_started    H_uefi_S_error(20)
#define H_uefi_Z_error_S_aborted            H_uefi_S_error(21)
#define H_uefi_Z_error_S_icmp_error         H_uefi_S_error(22)
#define H_uefi_Z_error_S_tftp_error         H_uefi_S_error(23)
#define H_uefi_Z_error_S_protocol_error     H_uefi_S_error(24)
#define H_uefi_Z_error_S_imcompatible_version H_uefi_S_error(25)
#define H_uefi_Z_error_S_security_violation H_uefi_S_error(26)
#define H_uefi_Z_error_S_crc_error          H_uefi_S_error(27)
#define H_uefi_Z_error_S_end_of_media       H_uefi_S_error(28)
#define H_uefi_Z_error_S_end_of_file        H_uefi_S_error(31)
#define H_uefi_Z_error_S_invalid_language   H_uefi_S_error(32)
#define H_uefi_Z_error_S_compromised_data   H_uefi_S_error(33)
#define H_uefi_Z_error_S_ip_address_conflict H_uefi_S_error(34)
#define H_uefi_Z_error_S_http_error         H_uefi_S_error(35)
//------------------------------------------------------------------------------
struct H_uefi_Z_guid
{ N32 data_1;
  N16 data_2;
  N16 data_3;
  N8 data_4[8];
};
struct H_uefi_Z_table_header
{ N64 signature;
  N32 revision;
  N32 header_l;
  N32 crc32;
  N32 reserved;
};
struct H_uefi_Z_memory_descriptor
{ N32 type;
  N32 pad;
  N64 physical_start;
  N64 virtual_start;
  N64 pages;
  N64 attribute;
};
struct H_uefi_Z_time
{ N16 year;
  N8 month;
  N8 day;
  N8 hour;
  N8 minute;
  N8 second;
  N8 pad_1;
  N32 nanosecond;
  S16 time_zone;
  N8 daylight;
  N8 pad_2;
};
struct H_uefi_Z_time_capabilities
{ N32 resolution;
  N32 accuracy;
  B sets_to_zero;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_uefi_Z_input_key
{ N16 scan_code;
  C16 unicode_char;
};
struct H_uefi_Z_simple_input_interface
{ S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reset )( struct H_uefi_Z_simple_input_interface *this, B extended_verification );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *read_key_stroke )( struct H_uefi_Z_simple_input_interface *this, struct H_uefi_Z_input_key *key );
  P *wait_for_key;
};
struct H_uefi_Z_simple_text_output_mode
{ S32 max_mode;
  S32 mode;
  S32 attribute;
  S32 cursor_column, cursor_row;
  B cursor_visible;
};
struct H_uefi_Z_simple_text_output_interface
{ S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reset )( struct H_uefi_Z_simple_text_output_interface *this, B extended_verification );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *output )( struct H_uefi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *test )( struct H_uefi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_mode )( struct H_uefi_Z_simple_text_output_interface *this, N mode, N *columns, N *rows );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_mode )( struct H_uefi_Z_simple_text_output_interface *this, N mode );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_attribute )( struct H_uefi_Z_simple_text_output_interface *this, N attribute );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *clear_screen )( struct H_uefi_Z_simple_text_output_interface *this );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_cursor_position )( struct H_uefi_Z_simple_text_output_interface *this, N column, N row );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_cursor )( struct H_uefi_Z_simple_text_output_interface *this, B enable );
  struct H_uefi_Z_simple_text_output_mode *mode;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_uefi_Z_capsule_header
{ struct H_uefi_Z_guid guid;
  N32 header_l;
  N32 flags;
  N32 image_size;
};
enum H_uefi_Z_reset
{ H_uefi_Z_reset_Z_cold
, H_uefi_Z_reset_Z_warm
, H_uefi_Z_reset_Z_shutdown
, H_uefi_Z_reset_Z_platform_specific
};
enum H_uefi_Z_allocate
{ H_uefi_Z_allocate_Z_any
, H_uefi_Z_allocate_Z_max_address
, H_uefi_Z_allocate_Z_address
, H_uefi_Z_allocate_Z_n
};
enum H_uefi_Z_memory
{ H_uefi_Z_memory_Z_reserved
, H_uefi_Z_memory_Z_loader_code
, H_uefi_Z_memory_Z_loader_data
, H_uefi_Z_memory_Z_boot_services_code
, H_uefi_Z_memory_Z_boot_services_data
, H_uefi_Z_memory_Z_runtime_services_code
, H_uefi_Z_memory_Z_runtime_services_data
, H_uefi_Z_memory_Z_conventional
, H_uefi_Z_memory_Z_unusable
, H_uefi_Z_memory_Z_acpi_reclaim
, H_uefi_Z_memory_Z_acpi_nvs
, H_uefi_Z_memory_Z_memory_mapped_io
, H_uefi_Z_memory_Z_memory_mapped_io_port_space
, H_uefi_Z_memory_Z_pal_code
, H_uefi_Z_memory_Z_persistent
, H_uefi_Z_memory_Z_unaccepted
, H_uefi_Z_memory_Z_n
, H_uefi_Z_memory_Z_kernel = 0x80000000U
, H_uefi_Z_memory_Z_processor_startup_page
};
#define H_uefi_Z_memory_protection_S_write      0x1000
#define H_uefi_Z_memory_protection_S_read       0x2000
#define H_uefi_Z_memory_protection_S_execute    0x4000
typedef void ( H_uefi_Z_api H_uefi_Z_notify_I )( P event, P context );
enum H_uefi_Z_timer_delay
{ H_uefi_Z_timer_delay_Z_cancel
, H_uefi_Z_timer_delay_Z_periodic
, H_uefi_Z_timer_delay_Z_relative
, H_uefi_Z_timer_delay_Z_n
};
enum H_uefi_Z_inteface
{ H_uefi_Z_inteface_Z_native
, H_uefi_Z_inteface_Z_pcode
};
struct H_uefi_Z_device_path_protocol
{ N8 type;
  N8 subtype;
  N8 length[2];
};
struct H_uefi_Z_device_path
{ struct H_uefi_Z_device_path_protocol header;
  N8 function;
  N8 device;
};
#define H_uefi_Z_open_protocol_Z_attribute_S_by_handle_protocol 1U
struct H_uefi_Z_open_protocol_information_entry
{ P agent_handle;
  P controller_handle;
  N32 attributes;
  N32 open_count;
};
enum H_uefi_Z_locate_search
{ H_uefi_Z_locate_search_Z_all
, H_uefi_Z_locate_search_Z_by_register_notify
, H_uefi_Z_locate_search_Z_by_protocol
};
//------------------------------------------------------------------------------
struct H_uefi_Z_runtime_services
{ struct H_uefi_Z_table_header header;
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_time )( struct H_uefi_Z_time *time, struct H_uefi_Z_time_capabilities *capabilities );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_time )( struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_wakeup_time )( B *enabled, B *pending, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_wakeup_time )( B enable, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_virtual_address_map )( N memory_map_l, N descriptor_size, N32 descriptor_version, struct H_uefi_Z_memory_descriptor *memory_map );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *convert_pointer )( N debug_disposition, P *address );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_variable )( Pc16 name, struct H_uefi_Z_guid *vendor_guid, N32 attrbutes, N *data_l, P *data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_next_variable_name )( N *name_l, Pc16 name, struct H_uefi_Z_guid *vendor_guid );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_variable )( Pc16 name, struct H_uefi_Z_guid *guid, N32 attributes, N data_l, P data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_next_high_monotonic_count )( N32 *count );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reset_system )( enum H_uefi_Z_reset type, N status, N data_l, Pc16 data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *update_capsule )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 scatter_gather_list );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_capsule_capabilities )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 *maximum_capsule_size, enum H_uefi_Z_reset *reset_type );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_variable_info )( N32 attributes, N64 *maximum_variable_storage_size, N64 *remaining_variable_storage_size, N64 *maximum_variable_size );
  
};
struct H_uefi_Z_boot_services
{ struct H_uefi_Z_table_header header;
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *Q_tpl_I_raise )( N tpl );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *Q_tpl_I_restore )( N tpl );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *M_pages )( enum H_uefi_Z_allocate type, enum H_uefi_Z_memory memory_type, N pages_n, N64 *physical_address );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *W_pages )( N64 physical_address, N pages_n );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_memory_map )( N *memory_map_l, struct H_uefi_Z_memory_descriptor *memory_map, N *map_key, N *descriptor_l, N32 *descriptor_version );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *M_pool )( enum H_uefi_Z_memory memory_type, N l, P *buffer );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *W_pool )( P buffer );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *M_event )( N32 type, N tpl, H_uefi_Z_notify_I *notify_proc, P notify_context, P *event );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_timer )( P event, enum H_uefi_Z_timer_delay type, N64 trigger_time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *wait_for_event )( N events_n, P *events, N *index );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *signal_event )( P event );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *W_event )( P event );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *check_event )( P event );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *install_protocol_interface )( P *handle, struct H_uefi_Z_guid *protocol, enum H_uefi_Z_inteface typ, P interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reinstall_protocol_interface )( P handle, struct H_uefi_Z_guid *protocol, P old, P new );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *uninstall_protocol_interface )( P handle, struct H_uefi_Z_guid *protocol, P interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *handle_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *pc_handle_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *register_protocol_notify )( struct H_uefi_Z_guid *protocol, P event, P *registration );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *locate_handle )( struct H_uefi_Z_guid *protocol, P registration, P *interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *locate_device_path )( struct H_uefi_Z_guid *protocol, struct H_uefi_Z_device_path **device_path, P *device );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *install_configuration_table )( struct H_uefi_Z_guid *protocol, P table );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *load_image )( B boot_policy, P parent_image_handle, struct H_uefi_Z_device_path *file_path, P source_buffer, N source_l, P *image_handle );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *start_image )( P handle, N *exit_data_l, Pc16 *exit_data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *exit )( P image_handle, S exit_status, N exit_data_l, Pc16 exit_data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *unload_image )( P handle );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *exit_boot_services )( P image_handle, N map_key );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_next_monotonic_count )( N64 *count );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *stall )( N microseconds );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_watchdog_timer )( N timeout, N64 code, N data_l, Pc16 data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *connect_controller )( P handle, P *driver_image_handle, struct H_uefi_Z_device_path *remaining_device_path, B recursive );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *disconnect_controller )( P handle, P driver_image_handle, P child_handle );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *open_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface, P agent_handle, P controller_handle, N32 attributes );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *close_protocol )( P handle, struct H_uefi_Z_guid *protocol, P agent_handle, P controller_handle );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *open_protocol_information )( P handle, struct H_uefi_Z_guid *protocol, struct H_uefi_Z_open_protocol_information_entry **entry_buffer, N *entry_n );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *protocols_per_handle )( P handle, struct H_uefi_Z_guid **protocol_buffer, N *protocol_n );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *locate_handle_buffer )( enum H_uefi_Z_locate_search type, struct H_uefi_Z_guid *protocol, P search_key, N *handles_n, P **handles_buffer );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *locate_protocol )( struct H_uefi_Z_guid *protocol, P registration, P *interface );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *install_multiple_protocol_interfaces )( P *handle, ... );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *uninstall_multiple_protocol_interfaces )( P handle, ... );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *calculate_crc32 )( P data, N data_l, N32 *crc );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *Q_mem_I_copy )( P destination, P source, N l );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *Q_mem_I_fill )( P buffer, N l, N8 value );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *M_event_ex )( N32 type, N notify_tpl, H_uefi_Z_notify_I *notify_proc, const P notify_context, const struct H_uefi_Z_guid *event_group, P *event );
};
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
struct H_uefi_Z_configuration_table
{ struct H_uefi_Z_guid vendor_guid;
  P vendor_table;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_uefi_Z_system_table
{ struct H_uefi_Z_table_header header;
  Pc16 firmware_vendor;
  N32 firmware_revision;
  P console_in_handle;
  struct H_uefi_Z_simple_input_interface *input;
  P console_out_handle;
  struct H_uefi_Z_simple_text_output_interface *output;
  P standard_error_handle;
  struct H_uefi_Z_simple_text_output_interface *stderr;
  struct H_uefi_Z_runtime_services *runtime_services;
  struct H_uefi_Z_boot_services *boot_services;
  N configuration_table_n;
  struct H_uefi_Z_configuration_table *configuration_table;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
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
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define H_uefi_Z_guid_S_disk_io { 0xce345171, 0xba0b, 0x11d2, { 0x8e, 0x4f, 0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
struct H_uefi_Z_protocol_Z_disk_io
{ N64 revision;
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *read )( struct H_uefi_Z_protocol_Z_disk_io *this, N32 media_id, N64 offset, N l, P buffer );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *write )( struct H_uefi_Z_protocol_Z_disk_io *this, N32 media_id, N64 offset, N l, P buffer );
};
//------------------------------------------------------------------------------
#define H_uefi_Z_guid_S_block_io { 0x964e5b21, 0x6459, 0x11d2, { 0x8e, 0x39, 0, 0xa0, 0xc9, 0x69, 0x72, 0x3b } }
struct H_uefi_Z_protocol_Z_block_io_Z_media
{ N32 media_id;
  B removable, present, logical_partition, read_only, write_caching;
  N32 block_size;
  N32 io_align;
  N64 last_block;
  N64 lowest_aligned_lba;
  N32 logical_blocks_per_physical_block;
  N32 optimal_transfer_lenght_granularity;
};
struct H_uefi_Z_protocol_Z_block_io
{ N64 revision;
  struct H_uefi_Z_protocol_Z_block_io_Z_media *media;
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reset )( struct H_uefi_Z_protocol_Z_block_io *this );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *read )( struct H_uefi_Z_protocol_Z_block_io *this, N32 media_id, N64 lba, N l, P buffer );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *write )( struct H_uefi_Z_protocol_Z_block_io *this, N32 media_id, N64 lba, N l, P buffer );
};
//------------------------------------------------------------------------------
#define H_uefi_Z_guid_S_graphics { 0x9042a9de, 0x23dc, 0x4a38, { 0x96, 0xfb, 0x7a, 0xde, 0xd0, 0x80, 0x51, 0x6a } }
enum H_uefi_Z_pixel_format
{ H_uefi_Z_pixel_format_S_rgb8
, H_uefi_Z_pixel_format_S_bgr8
, H_uefi_Z_pixel_format_S_bitmask
, H_uefi_Z_pixel_format_S_blt
};
struct H_uefi_Z_protocol_Z_graphics_Z_mode_information_Z_pixel_bitmask
{ N32 red;
  N32 green;
  N32 blue;
  N32 reserved;
};
struct H_uefi_Z_protocol_Z_graphics_Z_mode_information
{ N32 version;
  N32 horizontal_resolution;
  N32 vertical_resolution;
  enum H_uefi_Z_pixel_format pixel_format;
  struct H_uefi_Z_protocol_Z_graphics_Z_mode_information_Z_pixel_bitmask pixel_bitmask;
  N32 pixels_per_scan_line;
};
struct H_uefi_Z_protocol_Z_graphics_I_blt_Z_pixel
{ N32 blue;
  N32 green;
  N32 red;
  N32 reserved;
};
enum H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation
{ H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation_S_fill
, H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation_S_video2buffer
, H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation_S_buffer2video
, H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation_S_video2video
};
struct H_uefi_Z_protocol_Z_graphics_Z_mode
{ N32 max_mode;
  N32 mode;
  struct H_uefi_Z_protocol_Z_graphics_Z_mode_information *info;
  N info_l;
  P framebuffer;
  N framebuffer_l;
};
struct H_uefi_Z_protocol_Z_graphics
{ S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *query )( struct H_uefi_Z_protocol_Z_graphics *this, N32 mode, N *l, struct H_uefi_Z_protocol_Z_graphics_Z_mode_information **info );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_mode )( struct H_uefi_Z_protocol_Z_graphics *this, N32 mode );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *blt )( struct H_uefi_Z_protocol_Z_graphics *this, struct H_uefi_Z_protocol_Z_graphics_I_blt_Z_pixel *buffer, enum H_uefi_Z_protocol_Z_graphics_I_blt_Z_operation operation, N src_x, N src_y, N dest_x, N dest_y, N width, N height, N delta );
  struct H_uefi_Z_protocol_Z_graphics_Z_mode *mode;
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
S H_oux_E_fs_Q_disk_M( struct H_uefi_Z_system_table *, struct H_uefi_Z_protocol_Z_disk_io *, N32 );
S H_oux_E_fs_Q_disk_W( struct H_uefi_Z_system_table * );
N64 H_oux_E_fs_Q_kernel_R_size(void);
S H_oux_E_fs_Q_kernel_I_read( struct H_uefi_Z_protocol_Z_disk_io *, N32, Pc );
//==============================================================================
struct H_oux_Z_pixel_shifts
{ N8 red;
  N8 green;
  N8 blue;
};
struct H_main_Z_framebuffer
{ volatile N32 *p;
  N32 width, height;
  N32 pixels_per_scan_line;
  enum H_uefi_Z_pixel_format pixel_format;
  struct H_oux_Z_pixel_shifts pixel_shifts;
};
struct H_main_Z_uefi_runtime_services
{ S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_time )( struct H_uefi_Z_time *time, struct H_uefi_Z_time_capabilities *capabilities );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_time )( struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_wakeup_time )( B *enabled, B *pending, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_wakeup_time )( B enable, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_variable )( Pc16 name, struct H_uefi_Z_guid *vendor_guid, N32 attrbutes, N *data_l, P *data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_next_variable_name )( N *name_l, Pc16 name, struct H_uefi_Z_guid *vendor_guid );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *P_variable )( Pc16 name, struct H_uefi_Z_guid *guid, N32 attributes, N data_l, P data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_next_high_monotonic_count )( N32 *count );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *reset_system )( enum H_uefi_Z_reset type, N status, N data_l, Pc16 data );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *update_capsule )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 scatter_gather_list );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_capsule_capabilities )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 *maximum_capsule_size, enum H_uefi_Z_reset *reset_type );
  S ( H_uefi_Z_api __attribute__ (( __warn_unused_result__ )) *R_variable_info )( N32 attributes, N64 *maximum_variable_storage_size, N64 *remaining_variable_storage_size, N64 *maximum_variable_size );
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
  P bootloader;
  P kernel;
  P page_table;
  N additional_pages;
  P kernel_stack;
  P pcie_base_address;
  struct H_main_Z_framebuffer framebuffer;
  struct H_main_Z_uefi_runtime_services uefi_runtime_services;
  struct H_main_Z_kernel_Z_acpi acpi;
  P local_apic_address;
  P io_apic_address;
  struct E_interrupt_Z_gsi *gsi;
  P *processor_proc;
  N32 processor_start_page;
  N8 gsi_n;
  N8 processor_n;
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
#define E_vga_S_background_color            0xcacaca
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
N E_font_M(void);
void E_font_W(void);
N E_font_I_draw( N, N, N, U );
void E_font_I_print_nl(void);
void E_font_I_print_u(U);
N E_font_I_print(Pc);
void E_font_I_print_hex(N);
//==============================================================================
Pc E_text_Z_su_R_u( Pc, U *);
//==============================================================================
N32 E_vga_Z_color_M( N8, N8, N8 );
N8 E_vga_Z_color_R_red(N32);
N8 E_vga_Z_color_R_green(N32);
N8 E_vga_Z_color_R_blue(N32);
N32 E_vga_Z_color_M_gray(N8);
N32 E_vga_R_video_color(N32);
N32 E_vga_R_pixel( N32, N32 );
void E_vga_P_pixel( N32, N32, N32 );
void E_vga_I_set_pixel_aa( N32, N32, N32, F, N );
void E_vga_I_draw_rect( N32, N32, N32, N32, N32 );
void E_vga_I_fill_rect( N32, N32, N32, N32, N32 );
/******************************************************************************/
