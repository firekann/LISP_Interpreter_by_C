﻿/**
* This the example lexical analyzer code in pages 173 - 177 of the
* textbook,
*
* Sebesta, R. W. (2012). Concepts of Programming Languages.
* Pearson, 10th edition.
*
*/

/* front.c - a lexical analyzer system for simple arithmetic expressions */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "util.h"

/******************************************/
/* main driver                            */
/******************************************/

void print_obj(T_OBJ*);
void print_list(T_OBJ*);

int main(int argc, char* argv[])
{
	if ((in_fp = fopen("code.in", "r")) == NULL) {
		printf("ERROR - cannot open code.in \n");
		return 0;
	}

	dict = initialize_dict();
	obj_list = initialize_list();

	getChar();
	do {
		lex();
	} while (nextToken != EOF);

	/*
	LIST_NODE* node = obj_list->head;
	while (node != NULL) {
	printf("Next token is: %d, Next lexeme is %s\n", node->value.type, node->value.t_string);
	node = node->next;
	}
	*/

	cur_node = obj_list->head;
	while (cur_node->value.type != EOF) {
		if (cur_node->value.type == IDENT) {
			if (has_dict_key(dict, cur_node->value.t_string)) {
				T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);
				print_obj(&tmp);
			}
			else {
				printf("NO IDENT\n");
			}
			cur_node = cur_node->next;
			continue;
		}
		T_OBJ decision = call_fn();
		if (left_paren_Count != right_paren_Count) {
			printf("NIL\n");
		}
		else if (decision.t_bool == true) {
			print_obj(&decision);
		}
		else if (decision.t_bool == false){
			print_obj(&decision);
		}
		else if (nextToken == EOF) {
			printf("EOF\n");
			return 0;
		}
		else {
			printf("NIL\n");
		}

		/*
		DICT_NODE* dict_node = dict->head;
		while (dict_node != NULL)
		{
		printf("key : %s , ", dict_node->key);
		printf("type : %d , ", dict_node->value.type);
		if (dict_node->value.type == T_LIST) {
		printf("\n");
		print_list(&(dict_node->value));
		printf("\n");
		}
		else printf("value : %s\n", dict_node->value.t_string);
		dict_node = dict_node->next;
		}
		*/

		left_paren_Count = 0;
		right_paren_Count = 0;
		system("pause");
	}

	free_list(obj_list);
	free_dict(dict);
	fclose(in_fp);
	return 0;
}

void print_obj(T_OBJ* value) {
	if (value->type == INT) {
		printf("%d\n", value->t_int);
	}
	else if (value->type == FLOAT) {
		printf("%f\n", value->t_float);
	}
	else if (value->type == STRING) {
		printf("%s\n", value->t_string);
	}
	else if (value->type == BOOLEAN) {
		if (value->t_bool == false){
			printf("NIL\n");
		}
		else{
			printf("T\n");
		}
	}
	else if (value->type == T_LIST) {
		print_list(value);
		printf("\n");
	}
}

void print_list(T_OBJ* value) {
	//printf("list size : %d : ", value->t_int);
	T_OBJ *list;
	T_OBJ *listNext = value;
	printf("(");
	for (int i = 0; i < value->t_int; i++) {
		list = listNext;
		listNext = listNext->next;
		list = list->t_list_value;
		if (list->type == INT) {
			printf("%d", list->t_int);
		}
		else if (list->type == FLOAT) {
			printf("%f", list->t_float);
		}
		else if (list->type == STRING) {
			printf("%s", list->t_string);
		}
		else if (list->type == T_LIST) {
			print_list(list);
		}
		if (i + 1 != value->t_int)
			printf(" ");
	}
	printf(")");
	return;
}