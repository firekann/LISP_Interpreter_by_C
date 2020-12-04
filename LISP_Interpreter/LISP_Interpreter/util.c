#define _CRT_SECURE_NO_WARNINGS
#include "util.h"
#include "OP~NTH.h"
#include "ATOM~COND.h"
#include "CONS~SUBST.h"

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
	else return;
}