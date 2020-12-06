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
		T_OBJ decision = call_fn();
		if (left_paren_Count != right_paren_Count) {
			printf("NIL\n");
		}
		else if (decision.t_bool == true) {
			if (decision.type == INT) {
				printf("%d\n", decision.t_int);
			}
			else if (decision.type == FLOAT) {
				printf("%f\n", decision.t_float);
			}
			else if (decision.type == STRING) {
				printf("%s\n", decision.t_string);
			}
			else if (decision.type == BOOLEAN) {
				printf("T\n");
			}
			else if (decision.type == T_LIST) {
				print_list(&decision);
				printf("\n");
			}
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
		printf("\n");
	}
	free_list(obj_list);
	free_dict(dict);
	fclose(in_fp);
	return 0;
}