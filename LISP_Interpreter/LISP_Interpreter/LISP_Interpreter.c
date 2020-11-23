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
	/* Open the input data file and process its contents */
	if ((in_fp = fopen("code.in", "r")) == NULL) {
		printf("ERROR - cannot open code.in \n");
	}
	else if ((out_fp = fopen("code.out", "w")) == NULL) {  /* Open the output data file */
		printf("ERROR - cannot open code.out \n");
	}
	else {
		getChar();
		do {
			lex();
		} while (nextToken != EOF);
	}

	return 0;
}
