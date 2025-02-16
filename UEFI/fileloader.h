/*******************************************************************************
*   ___   publicplace
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
#define M(l)                                E_mem_Q_blk_M(l)
#define Mt(u,n)                             E_mem_Q_blk_M_tab((u),(n))
#define W(pointer_variable)                 E_mem_Q_blk_W( pointer_variable )
#define M_(pointer_variable)                pointer_variable = M( sizeof( *( pointer_variable )))
#define Mt_(pointer_variable,n)             pointer_variable = Mt( sizeof( *( pointer_variable )), (n) )
#define W_(pointer_variable)                ( W( pointer_variable ), pointer_variable = 0 )
#define W_tab_(pointer_variable)            ( E_mem_Q_tab_W( pointer_variable ), pointer_variable = 0 )
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define _inline                             static __attribute__ ((__always_inline__,__unused__))
#define _internal                           static
//==============================================================================
#define E_memory_S_page_size                0x1000
//==============================================================================
Pc16 E_text_Z_n_N_s( Pc16, N, N, N );
N E_text_Z_n_N_s_G( N, N, N );
//==============================================================================
#define H_uefi_Z_api __attribute__(( ms_abi ))
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
#define H_uefi_Z_error_S_ip_address_confilct H_uefi_S_error(34)
#define H_uefi_Z_error_S_http_error         H_uefi_S_error(35)
//------------------------------------------------------------------------------
#define H_uefi_S_page_size 0x1000
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
{ S ( H_uefi_Z_api *reset )( struct H_uefi_Z_simple_input_interface *this, B extended_verification );
  S ( H_uefi_Z_api *read_key_stroke )( struct H_uefi_Z_simple_input_interface *this, struct H_uefi_Z_input_key *key );
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
{ S ( H_uefi_Z_api *reset )( struct H_uefi_Z_simple_text_output_interface *this, B extended_verification );
  S ( H_uefi_Z_api *output )( struct H_uefi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_uefi_Z_api *test )( struct H_uefi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_uefi_Z_api *R_mode )( struct H_uefi_Z_simple_text_output_interface *this, N mode, N *columns, N *rows );
  S ( H_uefi_Z_api *P_mode )( struct H_uefi_Z_simple_text_output_interface *this, N mode );
  S ( H_uefi_Z_api *P_attribute )( struct H_uefi_Z_simple_text_output_interface *this, N attribute );
  S ( H_uefi_Z_api *clear_screen )( struct H_uefi_Z_simple_text_output_interface *this );
  S ( H_uefi_Z_api *P_cursor_position )( struct H_uefi_Z_simple_text_output_interface *this, N column, N row );
  S ( H_uefi_Z_api *P_cursor )( struct H_uefi_Z_simple_text_output_interface *this, B enable );
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
  S ( H_uefi_Z_api *R_time )( struct H_uefi_Z_time *time, struct H_uefi_Z_time_capabilities *capabilities );
  S ( H_uefi_Z_api *P_time )( struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api *R_wakeup_time )( B *enabled, B *pending, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api *P_wakeup_time )( B enable, struct H_uefi_Z_time *time );
  S ( H_uefi_Z_api *P_virtual_address_map )( N memory_map_l, N descriptor_size, N32 descriptor_version, struct H_uefi_Z_memory_descriptor *memory_map );
  S ( H_uefi_Z_api *convert_pointer )( N debug_disposition, P *address );
  S ( H_uefi_Z_api *R_variable )( Pc16 name, struct H_uefi_Z_guid *vendor_guid, N32 attrbutes, N *data_l, P *data );
  S ( H_uefi_Z_api *R_next_variable_name )( N *name_l, Pc16 name, struct H_uefi_Z_guid *vendor_guid );
  S ( H_uefi_Z_api *P_variable )( Pc16 name, struct H_uefi_Z_guid *guid, N32 attributes, N data_l, P data );
  S ( H_uefi_Z_api *R_next_high_monotonic_count )( N32 *count );
  S ( H_uefi_Z_api *reset_system )( enum H_uefi_Z_reset type, N status, N data_l, Pc16 data );
  S ( H_uefi_Z_api *update_capsule )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 scatter_gather_list );
  S ( H_uefi_Z_api *R_capsule_capabilities )( struct H_uefi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 *maximum_capsule_size, enum H_uefi_Z_reset *reset_type );
  S ( H_uefi_Z_api *R_variable_info )( N32 attributes, N64 *maximum_variable_storage_size, N64 *remaining_variable_storage_size, N64 *maximum_variable_size );
  
};
struct H_uefi_Z_boot_services
{ struct H_uefi_Z_table_header header;
  S ( H_uefi_Z_api *Q_tpl_I_raise )( N tpl );
  S ( H_uefi_Z_api *Q_tpl_I_restore )( N tpl );
  S ( H_uefi_Z_api *M_pages )( enum H_uefi_Z_allocate type, enum H_uefi_Z_memory memory_type, N pages_n, N64 *physical_address );
  S ( H_uefi_Z_api *W_pages )( N64 *physical_address, N pages_n );
  S ( H_uefi_Z_api *R_memory_map )( N *memory_map_l, struct H_uefi_Z_memory_descriptor *memory_map, N *map_key, N *descriptor_l, N32 *descriptor_version );
  S ( H_uefi_Z_api *M_pool )( enum H_uefi_Z_memory memory_type, N l, P *buffer );
  S ( H_uefi_Z_api *W_pool )( P buffer );
  S ( H_uefi_Z_api *M_event )( N32 type, N tpl, H_uefi_Z_notify_I *notify_proc, P notify_context, P *event );
  S ( H_uefi_Z_api *P_timer )( P event, enum H_uefi_Z_timer_delay type, N64 trigger_time );
  S ( H_uefi_Z_api *wait_for_event )( N events_n, P *events, N *index );
  S ( H_uefi_Z_api *signal_event )( P event );
  S ( H_uefi_Z_api *W_event )( P event );
  S ( H_uefi_Z_api *check_event )( P event );
  S ( H_uefi_Z_api *install_protocol_interface )( P *handle, struct H_uefi_Z_guid *protocol, enum H_uefi_Z_inteface typ, P interface );
  S ( H_uefi_Z_api *reinstall_protocol_interface )( P handle, struct H_uefi_Z_guid *protocol, P old, P new );
  S ( H_uefi_Z_api *uninstall_protocol_interface )( P handle, struct H_uefi_Z_guid *protocol, P interface );
  S ( H_uefi_Z_api *handle_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface );
  S ( H_uefi_Z_api *pc_handle_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface );
  S ( H_uefi_Z_api *register_protocol_notify )( struct H_uefi_Z_guid *protocol, P event, P *registration );
  S ( H_uefi_Z_api *locate_handle )( struct H_uefi_Z_guid *protocol, P registration, P *interface );
  S ( H_uefi_Z_api *locate_device_path )( struct H_uefi_Z_guid *protocol, struct H_uefi_Z_device_path **device_path, P *device );
  S ( H_uefi_Z_api *install_configuration_table )( struct H_uefi_Z_guid *protocol, P table );
  S ( H_uefi_Z_api *load_image )( B boot_policy, P parent_image_handle, struct H_uefi_Z_device_path *file_path, P source_buffer, N source_l, P *image_handle );
  S ( H_uefi_Z_api *start_image )( P handle, N *exit_data_l, Pc16 *exit_data );
  S ( H_uefi_Z_api *exit )( P image_handle, S exit_status, N exit_data_l, Pc16 exit_data );
  S ( H_uefi_Z_api *unload_image )( P handle );
  S ( H_uefi_Z_api *exit_boot_services )( P image_handle, N map_key );
  S ( H_uefi_Z_api *R_next_monotonic_count )( N64 *count );
  S ( H_uefi_Z_api *stall )( N microseconds );
  S ( H_uefi_Z_api *P_watchdog_timer )( N timeout, N64 code, N data_l, Pc16 data );
  S ( H_uefi_Z_api *connect_controller )( P handle, P *driver_image_handle, struct H_uefi_Z_device_path *remaining_device_path, B recursive );
  S ( H_uefi_Z_api *disconnect_controller )( P handle, P driver_image_handle, P child_handle );
  S ( H_uefi_Z_api *open_protocol )( P handle, struct H_uefi_Z_guid *protocol, P *interface, P agent_handle, P controller_handle, N32 attributes );
  S ( H_uefi_Z_api *close_protocol )( P handle, struct H_uefi_Z_guid *protocol, P agent_handle, P controller_handle );
  S ( H_uefi_Z_api *open_protocol_information )( P handle, struct H_uefi_Z_guid *protocol, struct H_uefi_Z_open_protocol_information_entry **entry_buffer, N *entry_n );
  S ( H_uefi_Z_api *protocols_per_handle )( P handle, struct H_uefi_Z_guid **protocol_buffer, N *protocol_n );
  S ( H_uefi_Z_api *locate_handle_buffer )( enum H_uefi_Z_locate_search type, struct H_uefi_Z_guid *protocol, P search_key, N *handles_n, P *handles_buffer );
  S ( H_uefi_Z_api *locate_protocol )( struct H_uefi_Z_guid *protocol, P registration, P *interface );
  S ( H_uefi_Z_api *install_multiple_protocol_interfaces )( P *handle, ... );
  S ( H_uefi_Z_api *uninstall_multiple_protocol_interfaces )( P handle, ... );
  S ( H_uefi_Z_api *calculate_crc32 )( P data, N data_l, N32 *crc );
  S ( H_uefi_Z_api *Q_mem_I_copy )( P destination, P source, N l );
  S ( H_uefi_Z_api *Q_mem_I_fill )( P buffer, N l, N8 value );
  S ( H_uefi_Z_api *M_event_ex )( N32 type, N notify_tpl, H_uefi_Z_notify_I *notify_proc, const P notify_context, const struct H_uefi_Z_guid *event_group, P *event );
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
  struct H_uefi_Z_configuration_table_entry *configuration_table;
};
/******************************************************************************/
