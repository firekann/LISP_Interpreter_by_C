#ifndef C_STACK
#define C_STACK

#include "type.h"
#include "c_list.h"

typedef struct {
	int stack_size;
	c_LIST *stack;
}c_STACK;

c_STACK* initialize_stack();
T_OBJ pop(c_STACK*);
void push(c_STACK*, T_OBJ*);

#endif