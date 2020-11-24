#ifndef _LEXER_
#define _LEXER_

#include <stdio.h>

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Global Variable */
#define MAX_LEXEME_LEN 100	//lexeme ���̸� ����
#define MAX_COMMAND_LEN 100
int nextToken;
int charClass;
char lexeme[MAX_LEXEME_LEN];
char nextChar;
int lexLen;
int token;
int commandLen;
int cursor;
char command[MAX_COMMAND_LEN];

FILE *in_fp;
FILE *out_fp; /* for save code.out */
/* Local Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

#endif