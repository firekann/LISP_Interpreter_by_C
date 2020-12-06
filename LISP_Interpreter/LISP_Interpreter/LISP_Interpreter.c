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
				printf("\n");
			}
			else {
				printf("NO IDENT\n");
			}
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == LEFT_PAREN) {
			T_OBJ decision = call_fn();
			if (left_paren_Count != right_paren_Count) {
				printf("NIL\n");
			}
			else if (decision.t_bool == true) {
				print_obj(&decision);
				printf("\n");
			}
			else if (decision.t_bool == false) {
				print_obj(&decision);
				printf("\n");
			}
			else if (nextToken == EOF) {
				printf("EOF\n");
				return 0;
			}
			else {
				printf("NIL\n");
			}
		}
		else {
			cur_node = cur_node->next;
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
		printf("%d", value->t_int);
	}
	else if (value->type == FLOAT) {
		printf("%f", value->t_float);
	}
	else if (value->type == STRING) {
		printf("%s", value->t_string);
	}
	else if (value->type == SQUOTE) {
		printf("\'");
	}
	else if (value->type == BOOLEAN) {
		if (value->t_bool == false) {
			printf("NIL");
		}
		else {
			printf("T");
		}
	}
	else if (value->type == T_LIST) {
		print_list(value);
		printf(" ");
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
		print_obj(list);
		if (i + 1 != value->t_int)
			printf(" ");
	}
	printf(")");
	return;
}