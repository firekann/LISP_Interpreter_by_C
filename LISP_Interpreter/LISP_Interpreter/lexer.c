#include "lexer.h"
#include "type.h"
#include <stdbool.h>

/******************************************
* lookup - a function to lookup operators
* and parentheses and return the token
******************************************/
int lookup(char ch) {
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		nextToken = SUB_OP;
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;

		/* Additional token case */
	case '\'':
		addChar();
		nextToken = SQUOTE;
		break;
	case ';':
		addChar();
		nextToken = SEMI_COLON;
		break;
	case '>':
		addChar();
		getChar();
		if (nextChar == '=') {
			addChar();
			nextToken = LEFT_INEQUAL_SAME_SIGN;
		}
		else {
			nextToken = LEFT_INEQUAL_SIGN;
		}
		break;
	case '<':
		addChar();
		getChar();
		if (nextChar == '=') {
			addChar();
			nextToken = RIGHT_INEQUAL_SAME_SIGN;
		}
		else {
			nextToken = RIGHT_INEQUAL_SIGN;
		}
		break;
	case '#':
		addChar();
		nextToken = SHARP;
		break;
	default:
		addChar();
		nextToken = EOF;
		break;
	}
	return nextToken;
}

/**************************************************/
/* addChar - a function to add nextChar to lexeme */
/**************************************************/
void addChar() {
	if (lexLen < MAX_LEXEME_LEN - 1) {  // max length of Lexime is 99
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = 0; // '\0'
	}
	else {
		printf("Error - lexeme is too long \n");
	}
}

/*****************************************************/
/* getChar - a function to get the next character
of input and determine its character class */
/*****************************************************/
void getChar() {
	if ((nextChar = getc(in_fp)) != EOF) {
		nextChar = toupper(nextChar);	//대소문자 구분안하는 것에대한 처리 : 모든 문자를 uppercase로 바꿔줌
		if (isalpha(nextChar))
			charClass = LETTER;
		else if (isdigit(nextChar))
			charClass = DIGIT;
		else
			charClass = UNKNOWN;
	}
	else {
		charClass = EOF;
	}
}

/*****************************************************/
/* getNonBlank - a function to call getChar until it
returns a non-whitespace character        */
/*****************************************************/
void getNonBlank() {
	while (isspace(nextChar))
		getChar();
}

/*****************************************************/
/* lex - a simple lexical analyzer for arithmetic
expressions                                 */
/*****************************************************/
int lex() {
	lexLen = 0;
	getNonBlank();

	switch (charClass) {
		/* Parse identifiers */
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}

		/* edit for reserved words */
		/* �⺻ �Լ�*/
		if (!strcmp("SETQ", lexeme))
			nextToken = SETQ;
		else if (!strcmp("LIST", lexeme))
			nextToken = LIST;
		else if (!strcmp("CAR", lexeme))
			nextToken = CAR;
		else if (!strcmp("CDR", lexeme))
			nextToken = CDR;
		else if (!strcmp("NTH", lexeme))
			nextToken = NTH;
		else if (!strcmp("CONS", lexeme))
			nextToken = CONS;
		else if (!strcmp("REVERSE", lexeme))
			nextToken = REVERSE;
		else if (!strcmp("APPEND", lexeme))
			nextToken = APPEND;
		else if (!strcmp("LENGTH", lexeme))
			nextToken = LENGTH;
		else if (!strcmp("MEMBER", lexeme))
			nextToken = MEMBER;
		else if (!strcmp("ASSOC", lexeme))
			nextToken = ASSOC;
		else if (!strcmp("REMOVE", lexeme))
			nextToken = REMOVE;
		else if (!strcmp("SUBST", lexeme))
			nextToken = SUBST;
		/* Predicate �Լ� */
		else if (!strcmp("ATOM", lexeme))
			nextToken = ATOM;
		else if (!strcmp("NULL", lexeme))
			nextToken = L_NULL;
		else if (!strcmp("NUMBERP", lexeme))
			nextToken = NUMBERP;
		else if (!strcmp("ZEROP", lexeme))
			nextToken = ZEROP;
		else if (!strcmp("MINUSP", lexeme))
			nextToken = MINUSP;
		else if (!strcmp("EQUAL", lexeme))
			nextToken = EQUAL;
		else if (!strcmp("STRINGP", lexeme))
			nextToken = STRINGP;
		/* ���ǹ� */
		else if (!strcmp("IF", lexeme))
			nextToken = IF;
		else if (!strcmp("COND", lexeme))
			nextToken = COND;
		/*혼합형 처리*/
		else if (lexeme[0] == 'C') {
			int *tmp_arr = (int*)malloc(sizeof(int) * (lexLen - 2));
			bool flag = true;
			if (lexeme[lexLen - 1] == 'R') {
				for (int i = 1; i < lexLen - 1; i++) {
					if (lexeme[i] == 'A') {
						tmp_arr[i - 1] = CAR;
					}
					else if (lexeme[i] = 'D') {
						tmp_arr[i - 1] = CDR;
					}
					else {
						flag = false;
						break;
					}
				}
				if (flag) {
					for (int i = 0; i < lexLen - 3; i++) {
						if (tmp_arr[i] == CAR) {
							printf("Next token is: %d, Next lexeme is %s\n", CAR, "CAR");
						}
						else {
							printf("Next token is: %d, Next lexeme is %s\n", CDR, "CDR");
						}
					}
					if (tmp_arr[lexLen - 3] == CAR) {
						nextToken = CAR;
						strcpy(lexeme, "CAR");
					}
					else {
						nextToken = CDR;
						strcpy(lexeme, "CDR");
					}
				}
			}
		}
		else
			nextToken = IDENT;
		break;

		/* Parse integer literals */
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		if (charClass == UNKNOWN && nextChar == '.') {
			addChar();
			getChar();
			while (charClass == DIGIT) {
				addChar();
				getChar();
			}
			nextToken = FLOAT;
		}
		else {
			nextToken = INT;
		}
		break;

		/* Parentheses and operators */
	case UNKNOWN:
		lookup(nextChar);
		getChar();
		break;

		/* EOF */
	case EOF:
		nextToken = EOF;
		lexeme[0] = 'E';
		lexeme[1] = 'O';
		lexeme[2] = 'F';
		lexeme[3] = 0;
		break;
	} /* End of switch */

	printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);

	/* print to code.out file */
	fprintf(out_fp, "Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);

	return nextToken;
} /* End of function lex */
