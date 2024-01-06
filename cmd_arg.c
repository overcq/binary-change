/** =========
 qo   |)    | public
    _x/ ,   | library
    |    ng | user interface
_opRWTng-rwt=

Command line parser
---------------------------------------
Do zrobienia: czy przekazywać odczytane indeksy dozwolonych wartości, gdy “value_list” i nie ‟other_value”?
==================================== */
#include "cmd_arg.h"
//==============================================================================
static
const char *long_arg_prefixes_false[] =
{ "disable-"
, "without-"
, "no-"
};
static
const char *long_arg_prefixes_true[] =
{ "enable-"
, "with-"
, "yes-"
};
//==============================================================================
static
void
E_cmd_arg_I_help(
  unsigned avail_args_n
, struct Z_avail_arg avail_args[]
){  for( unsigned i = 0; i < avail_args_n; i++ )
    {   const char *const switchable_text = avail_args[i].switchable ? "[yes-,no-]" : "";
        const char *const param_text = avail_args[i].value_list ? " ... ..." : avail_args[i].other_value || avail_args[i].values_count ? " ..." : "";
        if( *avail_args[i].long_text
        && *avail_args[i].short_text
        )
            fprintf( stderr, "--%s%s, -%s%s", switchable_text, avail_args[i].long_text, avail_args[i].short_text, param_text );
        else if( *avail_args[i].long_text )
            fprintf( stderr, "--%s%s%s", switchable_text, avail_args[i].long_text, param_text );
        else if( *avail_args[i].short_text )
            fprintf( stderr, "-%s%s", avail_args[i].short_text, param_text );
        else
            continue;
        fprintf( stderr, "\t%s\n", avail_args[i].description );
        if( avail_args[i].values_count )
        {   fprintf( stderr, "\t%s", avail_args[i].values[0] );
            for( unsigned j = 1; j < avail_args[i].values_count; j++ )
                fprintf( stderr, ", %s", avail_args[i].values[j] );
            if( avail_args[i].other_value )
                fprintf( stderr, ", ..." );
            fprintf( stderr, "\n" );
        }
    }
    exit(1);
}
static
void
E_cmd_arg_I_ign(
  int argi
, char *argv[]
){  fprintf( stderr, "%s: ignored %d. command line argument: \"%s\"\n", argv[0], argi, argv[argi] );
}
static
void
E_cmd_arg_I_err(
  int argi
, char *argv[]
){  error( true, 0, "invalid %d. command line argument: \"%s\"", argi, argv[argi] );
}
void
E_cmd_arg_I_parse(
  int argc
, char *argv[]
, Z_arg_proc *proc
, unsigned avail_args_n
, struct Z_avail_arg avail_args[]
){  unsigned arg_param_count = 0;
    struct Z_avail_arg *arg_data;
    enum
    { ARG_SWITCH_UNKNOWN = -1
    , ARG_SWITCH_NO
    , ARG_SWITCH_YES
    } arg_switch;
    while( --argc )
    {   char *arg = argv[argc];
        if( !arg[0] )
            E_cmd_arg_I_err( argc, argv );
        else if( arg[0] == '-' )
        {   if( !arg[1] )
                E_cmd_arg_I_err( argc, argv );
            arg++;
            if( arg[0] == '?' )
                E_cmd_arg_I_help( avail_args_n, avail_args );
            arg_data = 0;
            arg_switch = ARG_SWITCH_UNKNOWN;
            if( arg[0] == '-' )
            {   arg++;
                for( unsigned i = 0; i < sizeof( long_arg_prefixes_false ) / sizeof( const char * ); i++ )
                    if( !strncmp( arg, long_arg_prefixes_false[i], strlen( long_arg_prefixes_false[i] )))
                    {   arg_switch = ARG_SWITCH_NO;
                        arg += strlen( long_arg_prefixes_false[i] );
                        break;
                    }
                if( arg_switch == ARG_SWITCH_UNKNOWN )
                    for( unsigned i = 0; i < sizeof( long_arg_prefixes_true ) / sizeof( const char * ); i++ )
                        if( !strncmp( arg, long_arg_prefixes_true[i], strlen( long_arg_prefixes_true[i] )))
                        {   arg_switch = ARG_SWITCH_YES;
                            arg += strlen( long_arg_prefixes_true[i] );
                            break;
                        }
                for( unsigned i = 0; i < avail_args_n; i++ )
                    if( !strcmp( arg, avail_args[i].long_text ))
                    {   arg_data = &avail_args[i];
                        break;
                    }
                if( !arg_data )
                    E_cmd_arg_I_err( argc, argv );
                if( arg_switch != ARG_SWITCH_UNKNOWN
                && !arg_data->switchable
                )
                    E_cmd_arg_I_err( argc, argv );
            }else
            {   for( unsigned i = 0; i < avail_args_n; i++ )
                    if( !strncmp( arg, avail_args[i].short_text, strlen( avail_args[i].short_text )))
                    {   arg_data = &avail_args[i];
                        break;
                    }
                if( !arg_data )
                    E_cmd_arg_I_err( argc, argv );
            }
            if( arg_data->value_list )
            {   if( arg_param_count < 1 )
                    E_cmd_arg_I_err( argc, argv );
            }else if( arg_data->other_value
            || arg_data->values_count
            )
            {   if( arg_param_count != 1 )
                    E_cmd_arg_I_err( argc + ( arg_param_count ? 2 : 0 ), argv );
            }else
                if( arg_param_count )
                    E_cmd_arg_I_err( argc + 1, argv );
            if( !arg_data->other_value ) // When strict values then check if there are the only in the set.
                for( unsigned i = 0; i < arg_param_count; i++ )
                {   unsigned j = 0;
                    for( ; j < arg_data->values_count; j++ )
                        if( !strcmp( argv[ argc + 1 + i ], arg_data->values[j] ))
                            break;
                    if( j == arg_data->values_count )
                        E_cmd_arg_I_err( argc + 1 + i, argv );
                }
            switch( proc( arg_switch, arg_data - avail_args, arg_param_count, argv + argc + 1 ))
            {   case Z_arg_proc_ret_Z_accept:
                    break;
                case Z_arg_proc_ret_Z_reject:
                    E_cmd_arg_I_err( argc + ( arg_param_count == 1 ? 1 : 0 ), argv );
                case Z_arg_proc_ret_Z_ignore:
                    E_cmd_arg_I_ign( argc, argv );
            }
            arg_param_count = 0;
        }else
            arg_param_count++;
    }
    if( arg_param_count )
        E_cmd_arg_I_err( argc, argv );
}
/******************************************************************************/
