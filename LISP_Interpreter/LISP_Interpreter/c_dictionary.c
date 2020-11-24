#define _CRT_SECURE_NO_WARNINGS
#include "c_dictionary.h"
#include <stdio.h>
#include <string.h>

c_DICT* initialize_dict() {
	c_DICT* dict = (c_DICT*)malloc(sizeof(c_DICT));
	dict->dict_size = 0;
	dict->head = NULL;
	return dict;
}

void free_dict(c_DICT* dict) {
	DICT_NODE* node = dict->head;
	while (node != NULL)
	{
		DICT_NODE* n_node = node->next;
		free_node(node);	//노드 free
		node = n_node;
	}
	free(dict);
}

void free_node(DICT_NODE* node) {
	free(node->key);	//key 할당 해제
	free(node);	//노드 할당 해제
}

void delete_dict_node(c_DICT* dict, char* key) {
	DICT_NODE* pre_node = dict->head;
	DICT_NODE* cur_node = pre_node;
	while (cur_node != NULL)
	{
		if (!strcmp(key, cur_node->key)) {
			pre_node->next = cur_node->next;	//다음 노드에 이전 노드 포인터 전달
			free_node(cur_node);	//현재 노드 free
			dict->dict_size--;
			break;
		}
		pre_node = cur_node;
		cur_node = cur_node->next;
	}
}

void insert_dict_node(c_DICT* dict, char* key, T_OBJ* obj) {
	if (has_dict_key(dict, key)) {	//이미 존재하는 key일 경우 할당 해제하고 obj교체
		DICT_NODE* node = dict->head;
		while (node != NULL)
		{
			if (!strcmp(key, node->key)) {
				node->value = *obj;
				break;
			}
			node = node->next;
		}
		return;
	}
	DICT_NODE* new_node = (DICT_NODE*)malloc(sizeof(DICT_NODE));
	new_node->key = (char*)malloc(sizeof(char)*(strlen(key) + 1));
	strcpy(new_node->key, key);
	new_node->value = *obj;
	new_node->next = NULL;
	if (!dict->dict_size) {
		dict->head = new_node;
	}
	else {
		DICT_NODE* node = dict->head;
		while (node != NULL)
		{
			node = node->next;
		}
		node->next = new_node;
	}
	dict->dict_size++;
	return;
}

bool has_dict_key(c_DICT* dict, char* key) {
	DICT_NODE* node = dict->head;
	while (node != NULL)
	{
		if (!strcmp(key, node->key)) {
			return true;
		}
		node = node->next;
	}
	return false;
}

T_OBJ get_dict_obj(c_DICT* dict, char* key) {
	DICT_NODE* node = dict->head;
	while (node != NULL)
	{
		if (!strcmp(key, node->key)) {
			return node->value;
		}
		node = node->next;
	}
	printf("key doesn't exist int dictionary\n");
	exit(0);
}