#ifndef CONS_SUBST
#define CONS_SUBST

#include "type.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"

T_OBJ fn_cons();
T_OBJ fn_reverse();
T_OBJ fn_length();
T_OBJ fn_member();
T_OBJ fn_remove();
T_OBJ fn_assoc();
T_OBJ fn_append();
T_OBJ fn_subst();

#endif