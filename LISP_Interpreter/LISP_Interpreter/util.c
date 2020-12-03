#define _CRT_SECURE_NO_WARNINGS
#include "util.h"
#include "OP~NTH.h"
#include "ATOM~COND.h"
#include "CONS~SUBST.h"

T_OBJ call_fn() {
	LIST_NODE* tmp_node = cur_node->next;
	if (tmp_node->value.type == ADD_OP) return fn_add();
	if (tmp_node->value.type == SUB_OP) return fn_sub();
	if (tmp_node->value.type == MUL_OP) return fn_mul();
	if (tmp_node->value.type == DIV_OP) return fn_div();
	if (tmp_node->value.type == SETQ) return fn_setq();
	if (tmp_node->value.type == LIST) return;
	if (tmp_node->value.type == CAR) return;
	if (tmp_node->value.type == CDR) return;
	if (tmp_node->value.type == NTH) return;
	if (tmp_node->value.type == CONS) return;
	if (tmp_node->value.type == REVERSE) return;
	if (tmp_node->value.type == APPEND) return;
	if (tmp_node->value.type == LENGTH) return;
	if (tmp_node->value.type == MEMBER) return;
	if (tmp_node->value.type == ASSOC) return;
	if (tmp_node->value.type == REMOVE) return;
	if (tmp_node->value.type == SUBST) return;
}