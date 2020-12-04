#ifndef _HOMIN_
#define _HOMIN_

#include "type.h"
#include "c_dictionary.h"
#include "c_list.h"

T_OBJ fn_add();
T_OBJ fn_sub();
T_OBJ fn_mul();
T_OBJ fn_div();
T_OBJ fn_setq();
T_OBJ fn_list();
T_OBJ fn_car(bool);	//기본적으로 true로 인자를 주고 호출하면 됨
T_OBJ fn_cdr(bool);	//기본적으로 true로 인자를 주고 호출하면 됨
T_OBJ fn_nth();
T_OBJ fn_make_list();

#endif