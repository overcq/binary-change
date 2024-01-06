/** =========
 qo   |)    | public
    _x/ ,   | library
    |    ng | user interface
_opRWTng-rwt=

Command line parser
==================================== */
#include <errno.h>
#include <error.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//==============================================================================
enum Z_arg_proc_ret
{ Z_arg_proc_ret_Z_accept
, Z_arg_proc_ret_Z_reject
, Z_arg_proc_ret_Z_ignore
};
typedef
enum Z_arg_proc_ret
Z_arg_proc(
  int switch_
, unsigned avail_arg_i
, int argc
, char *argv[]
);
struct Z_avail_arg
{ const char *const long_text;
  const char *const short_text;
  const unsigned switchable : 1;
  const unsigned value_list : 1;
  const unsigned other_value : 1;
  const unsigned values_count;
  const char *const *const values;
  const char *const description;
};
//==============================================================================
void
E_cmd_arg_I_parse(
  int argc
, char *argv[]
, Z_arg_proc *proc
, unsigned avail_args_n
, struct Z_avail_arg avail_args[]
);
/******************************************************************************/
