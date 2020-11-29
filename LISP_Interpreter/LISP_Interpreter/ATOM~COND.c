#define _CRT_SECURE_NO_WARNINGS
#include "ATOM~COND.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include "c_dictionary.h"


int fn_atom(c_DICT *dict, c_LIST *list){ // ATOM function
	LIST_NODE *temp = list->head;

	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		temp = temp->next;
	}
	else
		return false;
	
	if (temp->value.type == ATOM){ // check instruction ATOM
		if (!strcmp(temp->value.t_string, "ATOM"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type; // save the value's type

	if (currentType == INT || currentType == FLOAT || currentType == IDENT){ // check whether it is symbol
		temp = temp->next;
	}
	else if (currentType == SQUOTE){ // if current type is single quote
		temp = temp->next;
		currentType = temp->value.type;
		if (currentType == INT || currentType == FLOAT || currentType == IDENT){ // check whether it is symbol
			temp = temp->next;
		}
	}
	else if (currentType == DQUOTE){ // if current type is double quote
		temp = temp->next;
		currentType = temp->value.type;
		while (currentType == INT || currentType == FLOAT || currentType == IDENT){ // check whether it is symbol
			temp = temp->next;
			currentType = temp->value.type;
		}
		if (currentType == DQUOTE){ // check whether it is dquote
			temp = temp->next;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
	}

	return false;
}

int fn_null(c_DICT *dict, c_LIST *list){ // NULL function
	LIST_NODE *temp = list->head;

	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == L_NULL){ // check instruction NULL
		if (!strcmp(temp->value.t_string, "NULL"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type; // save the value's type

	if (currentType == IDENT){ // next token should be identifier or NIL
		if (!strcmp(temp->value.t_string, "NIL")) // currentType is NIL
			temp = temp->next;
		else if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (!strcmp(data.t_string, "NIL")){ // current identifier's value is NIL
				temp = temp->next;
			}
			else
				return false;
		}
		else
			return false;
	}

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
	}

	return false;
}

int fn_numberp(c_DICT *dict, c_LIST *list){
	LIST_NODE *temp = list->head;

	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == NUMBERP){ // check instruction NULL
		if (!strcmp(temp->value.t_string, "NUMBERP"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	if (currentType == FLOAT || currentType == INT){ // if type is FLOAT or INT
		temp = temp->next;
	}
	else if (currentType == IDENT){ // if type is identifier
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (data.type == FLOAT || data.type == INT){ // if type is FLOAT or INT
				temp = temp->next;
			}
			else
				return false;
		}
		else
			return false;
	}

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
	}

	return false;
}

int fn_zerop(c_DICT *dict, c_LIST *list){

}