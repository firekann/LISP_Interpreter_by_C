#include "c_stack.h"
#include <stdio.h>
#include <string.h>

c_STACK* initialize_stack() {
	c_STACK* stack = (c_STACK*)malloc(sizeof(c_STACK));
	stack->stack_size = 0;
	stack->stack = initialize_list();
	return stack;
}

T_OBJ pop(c_STACK* stack) {
	if (stack->stack_size == 0) {
		printf("stack_size is \'0\'\n");
		exit(0);
	}
	stack->stack_size--;
	LIST_NODE *node = stack->stack->head;
	T_OBJ tmp_obj = node->value;
	stack->stack->head = node->next;
	free(node);
	return tmp_obj;
}

void push(c_STACK* stack, T_OBJ* obj) {
	LIST_NODE* new_node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
	new_node->value = *obj;
	new_node->next = NULL;
	if (stack->stack_size == 0) {	//크기가 0일 경우
		stack->stack->head = new_node;
	}
	else {
		new_node->next = stack->stack->head;
		stack->stack->head = new_node;
	}
	stack->stack_size++;
	return;
}