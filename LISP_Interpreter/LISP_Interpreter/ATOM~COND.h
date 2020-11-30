#ifndef __ATOM_COND__
#define __ATOM_COND__

#include "type.h"
#include "lexer.h"
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"
#define ABS(X) ((X) < 0 ? -(X) : (X))
#define epsilon 0.000001

int fn_atom(c_DICT *dict, c_LIST *list);
int fn_null(c_DICT *dict, c_LIST *list);
int fn_numberp(c_DICT *dict, c_LIST *list);
int fn_zerop(c_DICT *dict, c_LIST *list);
int fn_minusp(c_DICT *dict, c_LIST *list);
int fn_equal(c_DICT *dict, c_LIST *list);
int fn_right_inequal(c_DICT *dict, c_LIST *list);

#endif