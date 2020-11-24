#ifndef C_DICTIONARY
#define C_DICTIONARY

#include "type.h"
#include <stdbool.h>

typedef struct DICT_NODE* NODE_PTR;
typedef struct {
	char* key;
	T_OBJ value;
	NODE_PTR next;
}DICT_NODE;

typedef struct {
	int dict_size;
	DICT_NODE* head;
}c_DICT;

c_DICT* initialize_dict();
void free_dict(c_DICT*);
void free_node(DICT_NODE*);
void insert_dict_node(c_DICT*, char*, T_OBJ*);
void delete_dict_node(c_DICT*, char*);
bool has_dict_key(c_DICT*, char*);
T_OBJ get_dict_obj(c_DICT*, char*);

#endif