/*******************************************************************************
*   ___   public
*  ¦OUX¦  C
*  ¦/C+¦  commandline utility
*   ---   binary change
*         binary write
* ©overcq                on ‟Gentoo Linux 17.1” “x86_64”              2024‒1‒6 W
*******************************************************************************/
#include <errno.h>
#include <error.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "cmd_arg.h"
//==============================================================================
static bool verbose;
static
struct Z_file
{ char *pathname;
  unsigned data_n;
  struct Z_file_data
  { size_t position;
    unsigned short little_endian;
    unsigned short bytes;
    unsigned long value;
  } *data;
} *files;
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
static unsigned files_n;
//==============================================================================
static
void
mem_alloc_failed( unsigned line_i
){  error( true, 0, "memory allocation failed in line %u", line_i );
}
static
void
Z_file_data_M( void
){  void *p = realloc( files[ files_n - 1 ].data, ( files[ files_n - 1 ].data_n + 1 ) * sizeof( struct Z_file_data ));
    if( !p )
        mem_alloc_failed( __LINE__ );
    files[ files_n - 1 ].data = p;
    files[ files_n - 1 ].data_n++;
    files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].position = ~0;
    files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian = ~0;
    files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].bytes = ~0;
    files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].value = ~0;
}
static
void
Z_file_M( void
){  void *p = realloc( files, ( files_n + 1 ) * sizeof( struct Z_file ));
    if( !p )
        mem_alloc_failed( __LINE__ );
    files = p;
    files_n++;
    files[ files_n - 1 ].pathname = 0;
    files[ files_n - 1 ].data_n = 0;
    Z_file_data_M();
}
static
bool
Z_file_data_I_check( void
){  bool ret = ~files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].position
    && ~(short)files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].bytes
    && ~files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].value;
    if( ret
    && !~(short)files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian
    && files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].bytes == 1
    )
        files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian = true;
    else
        ret = ret && ~(short)files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian;
    return ret;
}
static
bool
Z_file_data_I_check_init( void
){  return ~files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].position
    || ~(short)files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian
    || ~(short)files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].bytes
    || ~files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].value;
}
static
enum Z_arg_proc_ret
E_cmd_arg_I_proc(
  int switch_
, unsigned avail_arg_i
, int argc
, char *argv[]
){  switch( avail_arg_i )
    { case 0:
        {   if( files[ files_n - 1 ].data_n == 1
            || Z_file_data_I_check_init()
            )
                return Z_arg_proc_ret_Z_reject;
            size_t l = strlen( argv[0] );
            files[ files_n - 1 ].pathname = ( char * )malloc( l + 1 );
            if( !files[ files_n - 1 ].pathname )
                mem_alloc_failed( __LINE__ );
            strcpy( files[ files_n - 1 ].pathname, argv[0] );
            void *p = realloc( files[ files_n - 1 ].data, --files[ files_n - 1 ].data_n * sizeof( struct Z_file_data ));
            if( !p )
                mem_alloc_failed( __LINE__ );
            files[ files_n - 1 ].data = p;
            Z_file_M();
            break;
        }
      case 1:
        {   char *ret;
            files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].position = strtoul( argv[0], &ret, 0 );
            if( *ret
            || !Z_file_data_I_check()
            )
                return Z_arg_proc_ret_Z_reject;
            Z_file_data_M();
            break;
        }
      case 2:
            files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian = true;
            break;
      case 3:
            files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].little_endian = false;
            break;
      case 4:
            files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].bytes = strtoul( argv[0], 0, 0 );
            break;
      case 5:
        {   char *ret;
            files[ files_n - 1 ].data[ files[ files_n - 1 ].data_n - 1 ].value = strtoul( argv[0], &ret, 0 );
            if( *ret )
                return Z_arg_proc_ret_Z_reject;
            break;
        }
      case 6:
            verbose = switch_;
            break;
    }
    return Z_arg_proc_ret_Z_accept;
}
//==============================================================================
int
main( int argc
, char *argv[]
){  Z_file_M();
    struct Z_avail_arg avail_args[] =
    { "file", "f", false, false, true, 0, 0, "file to change"
    , "position", "p", false, false, true, 0, 0, "next data position"
    , "little-endian", "l", false, false, false, 0, 0, "next data as little-endian"
    , "big-endian", "b", false, false, false, 0, 0, "next data as big-endian"
    , "data-size", "s", false, false, false, 3, ( const char *const [] ){ "1", "2", "4", "8" }, "next data size in bytes"
    , "data", "d", false, false, true, 0, 0, "data value"
    , "verbose", "v", true, false, false, 0, 0, "verbose operation"
    };
    E_cmd_arg_I_parse( argc, argv, E_cmd_arg_I_proc, sizeof( avail_args ) / sizeof( avail_args[0] ), avail_args );
    if( Z_file_data_I_check_init() )
        error( true, 0, "missed file in argument list" );
    if( --files_n )
    {   void *p = realloc( files, files_n * sizeof( struct Z_file ));
        if( !p )
            mem_alloc_failed( __LINE__ );
        files = p;
    }else
        free(files);
    for( unsigned files_i = 0; files_i != files_n; files_i++ )
    {   if(verbose)
            printf( "\"%s\"\n", files[ files_i ].pathname );
        int fd = open( files[ files_i ].pathname, O_WRONLY );
        if( !~fd )
            error( true, errno, "unable to open file" );
        for( unsigned data_i = 0; data_i != files[ files_i ].data_n; data_i++ )
        {   if(verbose)
                printf( "\tposition: %lu\n\tlittle_endian: %u\n\tbytes: %u\n\tvalue: %lu\n", files[ files_i ].data[ data_i ].position, files[ files_i ].data[ data_i ].little_endian, files[ files_i ].data[ data_i ].bytes, files[ files_i ].data[ data_i ].value );
            if( !~lseek( fd, files[ files_i ].data[ data_i ].position, SEEK_SET ))
                error( true, errno, "unable to seek in file" );
            unsigned short bits = files[ files_i ].data[ data_i ].bytes * 8;
            unsigned long value = files[ files_i ].data[ data_i ].value;
            if( files[ files_i ].data[ data_i ].little_endian )
                while(bits)
                {   unsigned char c = value & 0xff;
                    if( write( fd, &c, 1 ) != 1 )
                        error( true, errno, "unable to write to file" );
                    value >>= 8;
                    bits -= 8;
                }
            else
                while(bits)
                {   bits -= 8;
                    unsigned char c = ( value >> bits ) & 0xff;
                    if( write( fd, &c, 1 ) != 1 )
                        error( true, errno, "unable to write to file" );
                    value >>= 8;
                }
        }
        if( !~close(fd) )
            error( true, errno, "unable to close file" );
    }
    return 0;
}
/******************************************************************************/
