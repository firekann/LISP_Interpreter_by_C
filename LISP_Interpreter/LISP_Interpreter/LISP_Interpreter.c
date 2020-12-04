/**
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

void print_list(T_OBJ* value) {
	printf("list size : %d\n", value->t_int);
	T_OBJ* tmp = value->t_list_value;
	T_OBJ* node = value->next;
	while (node != NULL) {
		if (tmp->type == T_LIST) print_list(tmp);
		else {
			printf("type : %d , ", tmp->type);
			printf("value : %s\n", tmp->t_string);
		}
		tmp = node->t_list_value;
		node = node->next;
	}
	if (tmp->type == T_LIST) print_list(tmp);
	else {
		printf("type : %d , ", tmp->type);
		printf("value : %s\n", tmp->t_string);
	}
	return;
}

int main()
{
	dict = initialize_dict();
	while (1) {
		left_paren_Count = right_paren_Count = 0;
		obj_list = initialize_list();

		printf("Enter the lisp line: ");
		scanf("%[^\n]s", command);
		getchar();
		commandLen = strlen(command);
		cursor = 0;
		getChar();
		do {
			lex();
		} while (nextToken != EOF);

		cur_node = obj_list->head;
		call_fn();
		
		DICT_NODE* dict_node = dict->head;
		while (dict_node != NULL)
		{
			printf("key : %s , ", dict_node->key);
			printf("type : %d , ", dict_node->value.type);
			if (dict_node->value.type == T_LIST) {
				printf("\n");
				print_list(&(dict_node->value));
			}
			else printf("value : %s\n", dict_node->value.t_string);
			dict_node = dict_node->next;
		}

		/*
		LIST_NODE* list_node = obj_list->head;
		while (list_node != NULL)
		{
			printf("Next token is: %d, Next lexeme is %s\n", list_node->value.type, list_node->value.t_string);
			list_node = list_node->next;
		}
		*/

		ifFlag = 0;
		free_list(obj_list);
	}
	free_dict(dict);
	return 0;
}