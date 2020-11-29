#ifndef __ATOM_COND__
#define __ATOM_COND__

#include "type.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"

int fn_atom(c_DICT *dict, c_LIST *list);
int fn_null(c_DICT *dict, c_LIST *list);
int fn_numberp(c_DICT *dict, c_LIST *list);


#endif