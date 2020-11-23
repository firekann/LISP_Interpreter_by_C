#include "c_list.h"
#include <stdio.h>
#include <string.h>

c_LIST* initialize_list() {
	c_LIST* list = (c_LIST*)malloc(sizeof(c_LIST));
	list->list_size = 0;
	list->head = NULL;
	return list;
}

void free_list(c_LIST* list) {
	LIST_NODE* node = list->head;
	while (node)
	{
		LIST_NODE* n_node = node->next;
		free(node);	//노드 free
		node = n_node;
	}
	free(list);
}

void insert_list_node(c_LIST* list, T_OBJ* obj) {
	LIST_NODE* new_node = (LIST_NODE*)malloc(sizeof(LIST_NODE));
	new_node->value = *obj;
	new_node->next = list->head;
	list->head = new_node;
	list->list_size++;
}


void delete_list_node(c_LIST* list, T_OBJ* obj) {
	LIST_NODE* pre_node = list->head;
	LIST_NODE* cur_node = pre_node;
	while (cur_node != NULL)
	{
		if (obj->type == cur_node->value.type && !strcmp(obj->t_string, cur_node->value.t_string)) {
			pre_node->next = cur_node->next;	//다음 노드에 이전 노드 포인터 전달
			free(cur_node);	//현재 노드 free
			list->list_size--;
			break;
		}
		pre_node = cur_node;
		cur_node = cur_node->next;
	}
}

T_OBJ get_list_obj(c_LIST* list, T_OBJ* obj) {
	LIST_NODE* node = list->head;
	while (node)
	{
		if (obj->type == node->value.type && !strcmp(obj->t_string, node->value.t_string)) {
			return node->value;
		}
		node = node->next;
	}
}