#define _CRT_SECURE_NO_WARNINGS
#include "util.h"

T_OBJ call_fn() {
	LIST_NODE* tmp_node = cur_node->next;
	if (tmp_node->value.type == ADD_OP) return fn_add();
	else if (tmp_node->value.type == SUB_OP) return fn_sub();
	else if (tmp_node->value.type == MUL_OP) return fn_mul();
	else if (tmp_node->value.type == DIV_OP) return fn_div();
	else if (tmp_node->value.type == SETQ) return fn_setq();
	else if (tmp_node->value.type == LIST) return fn_list();
	else if (tmp_node->value.type == CAR) return fn_car(true);
	else if (tmp_node->value.type == CDR) return fn_cdr(true);
	else if (tmp_node->value.type == NTH) return fn_nth();
	else if (tmp_node->value.type == CONS) return;
	else if (tmp_node->value.type == REVERSE) return;
	else if (tmp_node->value.type == APPEND) return;
	else if (tmp_node->value.type == LENGTH) return;
	else if (tmp_node->value.type == MEMBER) return;
	else if (tmp_node->value.type == ASSOC) return;
	else if (tmp_node->value.type == REMOVE) return;
	else if (tmp_node->value.type == SUBST) return;
	else if (tmp_node->value.type == ATOM) return fn_atom();
	else if (tmp_node->value.type == L_NULL) return fn_null();
	else if (tmp_node->value.type == NUMBERP) return fn_numberp();
	else if (tmp_node->value.type == ZEROP) return fn_zerop();
	else if (tmp_node->value.type == MINUSP) return fn_minusp();
	else if (tmp_node->value.type == EQUAL) return fn_equal();
	else if (tmp_node->value.type == LEFT_INEQUAL_SIGN) return fn_left_inequal();
	else if (tmp_node->value.type == RIGHT_INEQUAL_SIGN) return fn_right_inequal();
	else if (tmp_node->value.type == LEFT_INEQUAL_SAME_SIGN) return fn_left_inequal_same();
	else if (tmp_node->value.type == RIGHT_INEQUAL_SAME_SIGN) return fn_right_inequal_same();
	else if (tmp_node->value.type == STRINGP) return fn_stringp();
	else if (tmp_node->value.type == IF) return fn_if();
	else return;
}

T_OBJ return_false() {
	T_OBJ error_result;
	error_result.type = BOOLEAN;
	error_result.t_bool = false;
	return error_result;
}