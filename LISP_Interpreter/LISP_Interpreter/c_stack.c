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
	insert_list_node(stack->stack, obj);
}