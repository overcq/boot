/*******************************************************************************
*   ___   publicplace
*  ¦OUX¦  C
*  ¦/C+¦  OUX/C+ OS
*   ---   UEFI boot loader
*         include file
* ©overcq                on ‟Gentoo Linux 23.0” “x86_64”              2025‒2‒1 e
*******************************************************************************/
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
#define H_efi_Z_api __attribute__(( ms_abi ))
struct H_efi_Z_guid
{ N32 data_1;
  N16 data_2;
  N16 data_3;
  N8 data_4[8];
};
struct H_efi_Z_table_header
{ N64 signature;
  N32 revision;
  N32 header_size;
  N32 CRC32;
  N32 reserved;
};
struct H_efi_Z_memory_descriptor
{ N32 type;
  N32 pad;
  N64 physical_start;
  N64 virtual_start;
  N64 pages_n;
  N64 attribute;
};
struct H_efi_Z_time
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
struct H_efi_Z_time_capabilities
{ N32 resolution;
  N32 accuracy;
  B sets_to_zero;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_efi_Z_input_key
{ N16 scan_code;
  C16 unicode_char;
};
struct H_efi_Z_simple_input_interface
{ S ( H_efi_Z_api *reset )( struct H_efi_Z_simple_input_interface *this, B extended_verification );
  S ( H_efi_Z_api *read_key_stroke )( struct H_efi_Z_simple_input_interface *this, struct H_efi_Z_input_key *key );
  P *wait_for_key;
};
struct H_efi_Z_simple_text_output_mode
{ S32 max_mode;
  S32 mode;
  S32 attribute;
  S32 cursor_column, cursor_row;
  B cursor_visible;
};
struct H_efi_Z_simple_text_output_interface
{ S ( H_efi_Z_api *reset )( struct H_efi_Z_simple_text_output_interface *this, B extended_verification );
  S ( H_efi_Z_api *output )( struct H_efi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_efi_Z_api *test )( struct H_efi_Z_simple_text_output_interface *this, Pc16 s );
  S ( H_efi_Z_api *R_mode )( struct H_efi_Z_simple_text_output_interface *this, N mode, N *columns, N *rows );
  S ( H_efi_Z_api *P_mode )( struct H_efi_Z_simple_text_output_interface *this, N mode );
  S ( H_efi_Z_api *P_attribute )( struct H_efi_Z_simple_text_output_interface *this, N attribute );
  S ( H_efi_Z_api *clear_screen )( struct H_efi_Z_simple_text_output_interface *this );
  S ( H_efi_Z_api *P_cursor_position )( struct H_efi_Z_simple_text_output_interface *this, N column, N row );
  S ( H_efi_Z_api *P_cursor )( struct H_efi_Z_simple_text_output_interface *this, B enable );
  struct H_efi_Z_simple_text_output_mode *mode;
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_efi_Z_capsule_header
{ struct H_efi_Z_guid guid;
  N32 header_size;
  N32 flags;
  N32 image_size;
};
enum H_efi_Z_reset_type
{ H_efi_Z_reset_type_Z_cold
, H_efi_Z_reset_type_Z_warm
, H_efi_Z_reset_type_Z_shutdown
};
struct H_efi_Z_runtime_services
{ struct H_efi_Z_table_header header;
  S ( H_efi_Z_api *R_time )( struct H_efi_Z_time *time, struct H_efi_Z_time_capabilities *capabilities );
  S ( H_efi_Z_api *P_time )( struct H_efi_Z_time *time );
  S ( H_efi_Z_api *R_wakeup_time )( B *enabled, B *pending, struct H_efi_Z_time *time );
  S ( H_efi_Z_api *P_wakeup_time )( B enable, struct H_efi_Z_time *time );
  S ( H_efi_Z_api *P_virtual_address_map )( N memory_map_size, N descriptor_size, N32 descriptor_version, struct H_efi_Z_memory_descriptor *virtual_map );
  S ( H_efi_Z_api *convert_pointer )( N debug_disposition, P *address );
  S ( H_efi_Z_api *R_variable )( Pc16 name, struct H_efi_Z_guid *vendor_guid, N32 attrbutes, N *data_size, P *data );
  S ( H_efi_Z_api *R_next_variable_name )( N *name_size, Pc16 name, struct H_efi_Z_guid *vendor_guid );
  S ( H_efi_Z_api *P_variable )( Pc16 name, struct H_efi_Z_guid *guid, N32 attributes, N data_size, P data );
  S ( H_efi_Z_api *R_next_high_monotonic_count )( N32 *count );
  S ( H_efi_Z_api *reset_system )( enum H_efi_Z_reset_type type, N status, N data_size, Pc16 data );
  S ( H_efi_Z_api *update_capsule )( struct H_efi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 scatter_gather_list );
  S ( H_efi_Z_api *R_capsule_capabilities )( struct H_efi_Z_capsule_header **capsule_headers, N capsule_headers_n, N64 *maximum_capsule_size, enum H_efi_Z_reset_type *reset_type );
  S ( H_efi_Z_api *R_variable_info )( N32 attributes, N64 *maximum_variable_storage_size, N64 *remaining_variable_storage_size, N64 *maximum_variable_size );
  
};
struct H_efi_Z_boot_services
{ struct H_efi_Z_table_header header;
  S ( H_efi_Z_api *raise_tpl )(  );
  S ( H_efi_Z_api *restore_tpl )(  );
  S ( H_efi_Z_api *M_pages )(  );
  S ( H_efi_Z_api *W_pages )(  );
  S ( H_efi_Z_api *R_memory_map )( N *memory_map_n, struct H_efi_Z_memory_descriptor *memory_map, N *map_key, N *descriptor_size, N32 *descriptor_version );
  S ( H_efi_Z_api *M_pool )(  );
  S ( H_efi_Z_api *W_pool )(  );
  S ( H_efi_Z_api *M_event )(  );
  S ( H_efi_Z_api *P_timer )(  );
  S ( H_efi_Z_api *wait_for_event )(  );
  S ( H_efi_Z_api *signal_event )(  );
  S ( H_efi_Z_api *W_event )(  );
  S ( H_efi_Z_api *check_event )(  );
  S ( H_efi_Z_api *install_protocol_interface )(  );
  S ( H_efi_Z_api *reinstall_protocol_interface )(  );
  S ( H_efi_Z_api *uninstall_protocol_interface )(  );
  S ( H_efi_Z_api *handle_protocol )(  );
  S ( H_efi_Z_api *pc_handle_protocol )(  );
  S ( H_efi_Z_api *register_protocol_notify )(  );
  S ( H_efi_Z_api *locate_handle )(  );
  S ( H_efi_Z_api *locate_device_path )(  );
  S ( H_efi_Z_api *install_configuration_table )(  );
  S ( H_efi_Z_api *load_image )(  );
  S ( H_efi_Z_api *start_image )(  );
  S ( H_efi_Z_api *exit )(  );
  S ( H_efi_Z_api *unload_image )(  );
  S ( H_efi_Z_api *exit_boot_services )( P image_handle, N map_key );
  S ( H_efi_Z_api *R_next_monotonic_count )(  );
  S ( H_efi_Z_api *stall )(  );
  S ( H_efi_Z_api *P_watchdog_timer )(  );
  S ( H_efi_Z_api *connect_controller )(  );
  S ( H_efi_Z_api *disconnect_controller )(  );
  S ( H_efi_Z_api *open_protocol )(  );
  S ( H_efi_Z_api *close_protocol )(  );
  S ( H_efi_Z_api *open_protocol_information )(  );
  S ( H_efi_Z_api *protocols_per_handle )(  );
  S ( H_efi_Z_api *locate_handle_buffer )(  );
  S ( H_efi_Z_api *locate_protocol )(  );
  S ( H_efi_Z_api *install_multiple_protocol_interfaces )(  );
  S ( H_efi_Z_api *uninstall_multiple_protocol_interfaces )(  );
  S ( H_efi_Z_api *calculate_crc32 )(  );
  S ( H_efi_Z_api *copy_mem )(  );
  S ( H_efi_Z_api *fill_mem )(  );
  S ( H_efi_Z_api *M_event_ex )(  );
};
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
struct H_efi_Z_system_table
{ struct H_efi_Z_table_header header;
  Pc16 firmware_vendor;
  N32 firmware_revision;
  P console_in_handle;
  struct H_efi_Z_simple_input_interface *input;
  P console_out_handle;
  struct H_efi_Z_simple_text_output_interface *output;
  P standar_error_handle;
  struct H_efi_Z_simple_text_output_interface *stderr;
  struct H_efi_Z_runtime_services *runtime_services;
  struct H_efi_Z_boot_services *boot_services;
  N configuration_table_n;
  struct H_efi_Z_configuration_table_entry *configuration_table;
};
/******************************************************************************/
