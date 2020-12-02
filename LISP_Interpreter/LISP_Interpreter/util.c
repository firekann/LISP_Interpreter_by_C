#define _CRT_SECURE_NO_WARNINGS
#include "util.h"
#include "OP~NTH.h"
#include "ATOM~COND.h"
#include "CONS~SUBST.h"

T_OBJ call_fn() {
	if (cur_node->value.type == ADD_OP) return fn_add();
	if (cur_node->value.type == SUB_OP) return fn_sub();
	if (cur_node->value.type == MUL_OP) return fn_mul();
	if (cur_node->value.type == DIV_OP) return fn_div();
	if (cur_node->value.type == SETQ) return;
	if (cur_node->value.type == LIST) return;
	if (cur_node->value.type == CAR) return;
	if (cur_node->value.type == CDR) return;
	if (cur_node->value.type == NTH) return;
	if (cur_node->value.type == CONS) return;
	if (cur_node->value.type == REVERSE) return;
	if (cur_node->value.type == APPEND) return;
	if (cur_node->value.type == LENGTH) return;
	if (cur_node->value.type == MEMBER) return;
	if (cur_node->value.type == ASSOC) return;
	if (cur_node->value.type == REMOVE) return;
	if (cur_node->value.type == SUBST) return;
}