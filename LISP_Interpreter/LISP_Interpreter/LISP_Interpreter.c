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
	while(1){
		printf("Enter the lisp line: ");
		scanf("%[^\n]s", command);
		getchar();
		commandLen = strlen(command);
		cursor = 0;
		
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	}

	return 0;
}
