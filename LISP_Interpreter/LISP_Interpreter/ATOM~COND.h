#ifndef __ATOM_COND__
#define __ATOM_COND__

#include "type.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"
#include "util.h"

T_OBJ fn_atom();
T_OBJ fn_null();
T_OBJ fn_numberp();
T_OBJ fn_zerop();
T_OBJ fn_minusp();
T_OBJ fn_equal();
T_OBJ fn_left_inequal();
T_OBJ fn_right_inequal();
T_OBJ fn_left_inequal_same();
T_OBJ fn_right_inequal_same();
T_OBJ fn_stringp();
T_OBJ fn_if();
T_OBJ fn_cond();

#endif