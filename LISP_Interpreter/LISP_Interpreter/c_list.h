#ifndef C_LIST
#define C_LIST

#include "type.h"

typedef struct LIST_NODE* NODE_P;

typedef struct {
	T_OBJ value;
	NODE_P next;
}LIST_NODE;

typedef struct {
	int list_size;
	LIST_NODE* head;
}c_LIST;

c_LIST* initialize_list();
void free_list(c_LIST*);
void insert_list_node(c_LIST*, T_OBJ*);
void delete_list_node(c_LIST*, T_OBJ*);
T_OBJ get_list_obj(c_LIST*, T_OBJ*);

#endif