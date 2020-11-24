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

/******************************************/
/* main driver                            */
/******************************************/
int main()
{
	dict = initialize_dict();
	while (1) {
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
		LIST_NODE* node = obj_list->head;
		while (node != NULL)
		{
			printf("Next token is: %d, Next lexeme is %s\n", node->value.type, node->value.t_string);
			node = node->next;
		}
		free_list(obj_list);
	}
	free_dict(dict);
	return 0;
}