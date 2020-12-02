#ifndef _LEXER_
#define _LEXER_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "c_list.h"
#include "c_dictionary.h"

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
int minusFlag; // to distinguish minus and negative num
int ifFlag; // to notice whether it is if function
int parenCount; // to count left paren and right paren
char command[MAX_COMMAND_LEN];
c_LIST* obj_list;
LIST_NODE* cur_node;
c_DICT* dict;

FILE *in_fp;
FILE *out_fp; /* for save code.out */
/* Local Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();
T_OBJ create_obj();

#endif