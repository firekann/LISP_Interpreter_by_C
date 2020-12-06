#define _CRT_SECURE_NO_WARNINGS
#include "ATOM~COND.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include "c_dictionary.h"

/* ATOM, NULL, NUMBERP, ZEROP, MINUSP, EQUAL, <, >, <=, >=, STRINGP, IF, COND function */

T_OBJ fn_atom(){ // ATOM function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN) { // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == ATOM){ // check instruction ATOM
		if (!strcmp(cur_node->value.t_string, "ATOM"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION ATOM\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION ATOM\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){ // meet left paren
		T_OBJ retValue = call_fn(); // if next type is not ident, it means a instruction. Call function
		if (retValue.type == T_LIST){ // if next type is list, should return NIL
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
		else if (retValue.t_bool == false){ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == INT || currentType == FLOAT || currentType == STRING || currentType == NIL){ // check whether it is symbol
		cur_node = cur_node->next;
	}
	else if (currentType == SQUOTE){ // if current type is single quote
		cur_node = cur_node->next;
		currentType = cur_node->value.type;
		if (currentType == INT || currentType == FLOAT || currentType == STRING || currentType == NIL){ // check whether it is symbol
			cur_node = cur_node->next;
		}
		else if (currentType == IDENT){ // if current type is double quote
			if (has_dict_key(dict, cur_node->value.t_string)){
				T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
				if (data.type != T_LIST){ // check the identifier's type. shouldn't be list
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else
				cur_node = cur_node->next;
		}
		else if (currentType == LEFT_PAREN){
			left_paren_Count++;
			int pCount = 1;
			cur_node = cur_node->next;
			while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
					left_paren_Count++;
					pCount++;
				}
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					pCount--;
				}
			}
			right_paren_Count++;
			return result;
		}
		else{
			printf("ERROR : Syntax Error\n");
			return result;
		}
	}
	else if (currentType == IDENT){ // if current type is double quote
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type != T_LIST){ // check the identifier's type. shouldn't be list
				cur_node = cur_node->next;
			}
			else
				return result;
		}
		else
			cur_node = cur_node->next;
	}
	else{
		printf("ERROR : Syntax Error\n");
		return result;
	}

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}

	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_null(){ // NULL function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN) { // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == L_NULL){ // check instruction NULL
		if (!strcmp(cur_node->value.t_string, "NULL"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION NULL\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION NULL\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){
		LIST_NODE *check = cur_node->next;
		if (check->value.type > 500){ // next value is not a function
			left_paren_Count++;
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count += 2;
					break;
				}
			}
			return result;
		}

		T_OBJ retValue = call_fn(); // Call function

		if (retValue.t_bool != false){ // if return value is true, return false
			cur_node = cur_node->next;
			return result;
		}			
		else if (retValue.t_bool == false){ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == NIL) // currentType is NIL
		cur_node = cur_node->next;
	else if (currentType == IDENT){ // next token should be identifier or NIL
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type == NIL){ // current identifier's value is NIL
				cur_node = cur_node->next;
			}
			else
				return result; // not NIL
		}
		else
			return result; // not NIL
	}
	else if (currentType == INT || currentType == FLOAT){
		cur_node = cur_node->next;
		currentType = cur_node->value.type;
		if (currentType == RIGHT_PAREN){ // end of instruction
			right_paren_Count++;
			cur_node = cur_node->next;
			if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
				result.t_bool = false;
				return result;
			}
			right_paren_Count++;
			printf("ERROR : Syntax Error\n");
		}
	}
	else{
		while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){ // move cur_node to end of corresponding instruction
			cur_node = cur_node->next;
			if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
				cur_node = cur_node->next;
				right_paren_Count++;
				break;
			}
		}
		return result; // not NIL
	}

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_numberp(){ // NUMBERP function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == NUMBERP){ // check instruction NUMBERP
		if (!strcmp(cur_node->value.t_string, "NUMBERP"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION NUBMERP\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION NUMBERP\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT || retValue.type == FLOAT){ // if return value is INT or FLOAT, continue
		}
		else{
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			right_paren_Count++;
			return result;			
		}
	}
	else if (currentType == FLOAT || currentType == INT){ // if type is FLOAT or INT
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is identifier
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type == FLOAT || data.type == INT){ // if type is FLOAT or INT
				cur_node = cur_node->next;
			}
			else
				return result; // not number
		}
		else
			return result; // not number
	}
	else
		return result; // not number

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_zerop(){ // ZEROP function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == ZEROP){ // check instruction ZEROP
		if (!strcmp(cur_node->value.t_string, "ZEROP"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION ZEROP\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION ZEROP\n");
		return result;
	}

	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT, check whether it is zero
			if (retValue.t_int != 0)
				return result;
		} 
		else if (retValue.type == FLOAT){ // if return type is FLOAT, check whether it is zero
			if (retValue.t_float != 0)
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				if (data.type == INT && data.t_int == 0){ // if type is INT and value is 0
				}
				else if (data.type == FLOAT && data.t_float == 0){ // if type is FLOAT and value is 0
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == INT){ // if type is INT
		if (cur_node->value.t_int == 0) // value is 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (cur_node->value.t_float == 0)
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == IDENT){ // if type is identifier
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type == INT){ // if type is INT
				if (data.t_int == 0) // value is 0
					cur_node = cur_node->next;
				else
					return result;
			}
			else if (data.type == FLOAT){ // if type is FLOAT
				if (data.t_float == 0) // value is 0
					cur_node = cur_node->next;
				else
					return result;
			}
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_minusp(){ // MINUSP function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN) { // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == MINUSP){ // check instruction MINUSP
		if (!strcmp(cur_node->value.t_string, "MINUSP"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION NULL\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION NULL\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT, check whether value is bigger than 0
			if (retValue.t_int >= 0)
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT, check whether value is bigger than 0
			if (retValue.t_float >= 0)
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				if (data.type == INT && data.t_int == 0){ // if type is INT and value is 0
				}
				else if (data.type == FLOAT && data.t_float == 0){ // if type is FLOAT and value is 0
				}
				else
					return result;
			}
			else
				return result;
		}
		else { // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == INT){ // if type is INT
		if (cur_node->value.t_int < 0) // value is smaller than 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (cur_node->value.t_float < 0) // value is smaller than 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == IDENT){ // if type is FLOAT		
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type == INT){ // if type is INT
				if (data.t_int < 0) // value is smaller than 0
					cur_node = cur_node->next;
				else
					return result;
			}
			else if (data.type == FLOAT){ // if type is FLOAT
				if (data.t_float < 0) // value is smaller than 0
					cur_node = cur_node->next;
				else
					return result;
			}
			else
				return result; // NOT INT or FLOAT
		}
		else
			return result; // NOT INT or FLOAT
	}
	else
		return result; // NOT INT or FLOAT

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_equal(){ // EQUAL function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}


	if (cur_node->value.type == EQUAL){ // check instruction EQUAL
		if (!strcmp(cur_node->value.t_string, "EQUAL"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION EQUAL\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION EQUAL\n");
		return result;
	}


	int beforeType; // to save before node's type
	int intValue; // to save before node's value
	float floatValue; // to save before node's value
	bool boolValue; // to save before node's value
	char stringValue[MAX_COMMAND_LEN]; // to save before node's value
	stringValue[0] = '\0';
	char listValue[MAX_COMMAND_LEN]; // to save before node's value
	listValue[0] = '\0';

	int currentType = cur_node->value.type; // save the value's type

	// first attribute, save to beforeType for comparision
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT 
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == NIL){ // if return type is NIL
			beforeType = retValue.type;
		}
		else if (retValue.type == STRING){ // if return type is STRING
			beforeType = retValue.type;
			strcpy(stringValue, cur_node->value.t_string);
		}
		else if (retValue.type == BOOLEAN){ // if return type is BOOLEAN
			beforeType = retValue.type;
			boolValue = retValue.t_bool;
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				beforeType = NIL;
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else if (currentType == NIL){ // if currentType is NIL
					beforeType = currentType;
				}
				else if (currentType == STRING){ // if currentType is STRING
					beforeType = currentType;
					strcpy(stringValue, data.t_string);
				}
				else if (currentType == BOOLEAN){ // if currentType is BOOLEAN
					beforeType = currentType;
					boolValue = data.t_bool;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == INT){ // if currentType is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == NIL){ // if currentType is NIL
		beforeType = currentType;
		cur_node = cur_node->next;
	}
	else if (currentType == STRING){ // if currentType is STRING
		beforeType = currentType;
		strcpy(stringValue, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if currentType is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
			else if (currentType == NIL){ // if currentType is NIL
				beforeType = currentType;
				cur_node = cur_node->next;
			}
			else if (currentType == STRING){ // if currentType is STRING
				beforeType = currentType;
				strcpy(stringValue, data.t_string);
				cur_node = cur_node->next;
			}
			else
				return result;
		}
		else
			return result;
	}
	else if (currentType == SQUOTE){ // if it meets list		
		cur_node = cur_node->next;
		if (cur_node->value.type == IDENT){
			beforeType = IDENT;
			strcat(stringValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == INT){
			beforeType = IDENT;
			strcat(stringValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == FLOAT){
			beforeType = IDENT;
			strcat(stringValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == STRING){
			beforeType = IDENT;
			strcat(stringValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == NIL){
			beforeType = IDENT;
			strcat(stringValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == LEFT_PAREN){
			beforeType = T_LIST;
			left_paren_Count++;
			int pCount = 1;
			while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
					LIST_NODE *check = cur_node->next;
					if (check->value.type == IDENT || check->value.type == INT || check->value.type == FLOAT || check->value.type == NIL || check->value.type == STRING){
						left_paren_Count++;
						cur_node = cur_node->next;
						strcat(stringValue, cur_node->value.t_string);
					}
					else{
						T_OBJ decision = call_fn();
						while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && decision.t_bool == false){
							cur_node = cur_node->next;
							if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
								cur_node = cur_node->next;
								right_paren_Count++;
								break;
							}
						}
						if (cur_node == NULL){
							printf("ERROR : Syntax Error\n");
							return result;
						}
						if (decision.type == IDENT || decision.type == INT || decision.type == FLOAT || decision.type == NIL || decision.type == STRING){
							strcat(stringValue, decision.t_string);
						}
						else if (decision.type == BOOLEAN){
							if (decision.t_bool == false)
								strcat(stringValue, "0");
							else
								strcat(stringValue, "1");
						}
					}
					pCount++;
				}
				else{
					if (cur_node->value.type == IDENT || cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == NIL || cur_node->value.type == STRING){
						strcat(stringValue, cur_node->value.t_string);
					}
				}
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					pCount--;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute, compare with beforeType and value
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			if (beforeType == retValue.type && intValue == retValue.t_int){
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			if (beforeType == retValue.type && floatValue == retValue.t_float){
			}
			else
				return result;
		}
		else if (retValue.type == NIL){ // if return type is NIL
			if (beforeType == retValue.type){
			}
			else
				return result;
		}
		else if (retValue.type == STRING){ // if return type is STRING
			if (beforeType == retValue.type && !strcmp(stringValue, retValue.t_string)){
			}
			else
				return result;
		}
		else if (retValue.type == BOOLEAN){ // if return type is STRING
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL)
				return result;
			if (beforeType == retValue.type && boolValue == retValue.t_bool){
			}
			else if (beforeType == NIL && retValue.t_bool == false){
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					if (beforeType == currentType && intValue == data.t_int){
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					if (beforeType == retValue.type && floatValue == retValue.t_float){
					}
					else
						return result;
				}
				else if (currentType == NIL){ // if type is NIL
					if (beforeType == retValue.type){
					}
					else
						return result;
				}
				else if (currentType == STRING){ // if type is STRING
					if (beforeType == retValue.type && !strcmp(stringValue, retValue.t_string)){
					}
					else
						return result;
				}
				else if (currentType == BOOLEAN){ // if type is BOOLEAN
					if (beforeType == retValue.type && boolValue == retValue.t_bool){
					}
					else
						return result;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}
	else if (currentType == INT){ // if currentType is INT
		if (beforeType == currentType && intValue == cur_node->value.t_int){
			cur_node = cur_node->next;
		}
		else
			return result;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		if (beforeType == currentType && floatValue == cur_node->value.t_float){
			cur_node = cur_node->next;
		}
		else
			return result;
	}
	else if (currentType == NIL){ // if currentType is NIL
		if (beforeType == currentType)
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == STRING){ // if currentType is STRING		
		if (beforeType == currentType && !strcmp(stringValue, cur_node->value.t_string))
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == IDENT){ // if currentType is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				if (beforeType == currentType && intValue == data.t_int){
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				if (beforeType == currentType && floatValue == data.t_float){
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else if (currentType == NIL){ // if currentType is NIL
				if (beforeType == currentType)
					cur_node = cur_node->next;
				else
					return result;
			}
			else if (currentType == STRING){ // if currentType is STRING		
				if (beforeType == currentType && !strcmp(stringValue, cur_node->value.t_string))
					cur_node = cur_node->next;
				else
					return result;
			}
			else
				return result;
		}
		else
			return result;
	}
	else if (currentType == SQUOTE){ // if it meets list
		
		cur_node = cur_node->next;
		if (cur_node->value.type == IDENT){
			currentType = IDENT;
			strcat(listValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == INT){
			currentType = IDENT;
			strcat(listValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == FLOAT){
			currentType = IDENT;
			strcat(listValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == STRING){
			currentType = IDENT;
			strcat(listValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == NIL){
			currentType = IDENT;
			strcat(listValue, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == LEFT_PAREN){
			currentType = T_LIST;
			left_paren_Count++;
			int pCount = 1;
			while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
					LIST_NODE *check = cur_node->next;
					if (check->value.type == IDENT || check->value.type == INT || check->value.type == FLOAT || check->value.type == NIL || check->value.type == STRING){
						left_paren_Count++;
						cur_node = cur_node->next;
						strcat(listValue, cur_node->value.t_string);
					}
					else{
						T_OBJ decision = call_fn();
						while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && decision.t_bool == false){
							cur_node = cur_node->next;
							if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
								cur_node = cur_node->next;
								right_paren_Count++;
								break;
							}
						}
						if (cur_node == NULL){
							printf("ERROR : Syntax Error\n");
							return result;
						}
						if (decision.type == IDENT || decision.type == INT || decision.type == FLOAT || decision.type == NIL || decision.type == STRING){
							strcat(listValue, decision.t_string);
						}
						else if (decision.type == BOOLEAN){
							if (decision.t_bool == false)
								strcat(listValue, "0");
							else
								strcat(listValue, "1");
						}
					}
					pCount++;
				}
				else{
					if (cur_node->value.type == IDENT || cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == NIL || cur_node->value.type == STRING){
						strcat(listValue, cur_node->value.t_string);
					}
				}
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					pCount--;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
		if (beforeType == currentType && !strcmp(stringValue, listValue)){
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_left_inequal(){ // > function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == LEFT_INEQUAL_SIGN){ // check instruction LEFT_INEQUAL_SIGN
		if (!strcmp(cur_node->value.t_string, ">"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION >\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION >\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type
	int beforeType; // to save before node's type
	int intValue; // to save before node's value
	float floatValue; // to save before node's value

	// first attribute, save node to beforeType for comparision
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if currentType is IDENT
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute, compare with beforeType
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			if (beforeType == INT){ // if beforeType is INT and before value is bigger than retValue
				if (intValue > retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is bigger than retValue
				if (floatValue > retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			if (beforeType == INT){ // if beforeType is INT and before value is bigger than retValue
				if (intValue > retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is bigger than retValue
				if (floatValue > retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					if (beforeType == INT){ // if beforeType is INT and before value is bigger than retValue
						if (intValue > data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is bigger than retValue
						if (floatValue > data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					if (beforeType == INT){ // if beforeType is INT and before value is bigger than retValue
						if (intValue > data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is bigger than retValue
						if (floatValue > data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue > cur_node->value.t_int) // check whether before value is bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue > cur_node->value.t_int) // check whether before value is bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue > cur_node->value.t_float) // check whether before value is bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue > cur_node->value.t_float) // check whether before value is bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if currentType is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue > data.t_int) // check whether before value is bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue > data.t_int) // check whether before value is bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue > data.t_float) // check whether before value is bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue > data.t_float) // check whether before value is bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else
				return result;
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_right_inequal(){ // < function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == RIGHT_INEQUAL_SIGN){ // check instruction RIGHT_INEQUAL_SIGN
		if (!strcmp(cur_node->value.t_string, "<"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION <\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION <\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type
	int beforeType; // to save before node's type
	int intValue; // to save before node's value
	float floatValue; // to save before node's value

	// first attribute, save node to beforeType for comparision
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if currentType is IDENT
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute, compare with beforeType
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			if (beforeType == INT){ // if beforeType is INT and before value is smaller than retValue
				if (intValue < retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is smaller than retValue
				if (floatValue < retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			if (beforeType == INT){ // if beforeType is INT and before value is smaller than retValue
				if (intValue < retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is smaller than retValue
				if (floatValue < retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					if (beforeType == INT){ // if beforeType is INT and before value is smaller than retValue
						if (intValue < data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is smaller than retValue
						if (floatValue < data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					if (beforeType == INT){ // if beforeType is INT and before value is smaller than retValue
						if (intValue < data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is smaller than retValue
						if (floatValue < data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue < cur_node->value.t_int) // check whether before value is smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue < cur_node->value.t_int) // check whether before value is smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue < cur_node->value.t_float) // check whether before value is smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue < cur_node->value.t_float) // check whether before value is smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if currentType is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue < data.t_int) // check whether before value is smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue < data.t_int) // check whether before value is smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if beforeType is FLOAT
				if (beforeType == INT){// if beforeType is INT
					if (intValue < data.t_float) // check whether before value is smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue < data.t_float) // check whether before value is smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else
				return result;
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_left_inequal_same(){ // >= function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == LEFT_INEQUAL_SAME_SIGN){ // check instruction LEFT_INEQUAL_SAME_SIGN
		if (!strcmp(cur_node->value.t_string, ">="))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION >=\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION >=\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type
	int beforeType; // to save before node's type
	int intValue; // to save before node's value
	float floatValue; // to save before node's value

	// first attribute, save node to beforeType for comparision
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if currentType is IDENT
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute, compare with beforeType
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			if (beforeType == INT){ // if beforeType is INT and before value is same or bigger than retValue
				if (intValue >= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or bigger than retValue
				if (floatValue >= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			if (beforeType == INT){ // if beforeType is INT and before value is same or bigger than retValue
				if (intValue >= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or bigger than retValue
				if (floatValue >= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					if (beforeType == INT){ // if beforeType is INT and before value is same or bigger than retValue
						if (intValue >= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or bigger than retValue
						if (floatValue >= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					if (beforeType == INT){ // if beforeType is INT and before value is same or bigger than retValue
						if (intValue >= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or bigger than retValue
						if (floatValue >= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue >= cur_node->value.t_int) // check whether before value is same or bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue >= cur_node->value.t_int) // check whether before value is same or bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue >= cur_node->value.t_float) // check whether before value is same or bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue >= cur_node->value.t_float) // check whether before value is same or bigger than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if beforeType is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue >= data.t_int) // check whether before value is same or bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue >= data.t_int) // check whether before value is same or bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue >= data.t_float) // check whether before value is same or bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue >= data.t_float) // check whether before value is same or bigger than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else
				return result;
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_right_inequal_same(){ // <= function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == RIGHT_INEQUAL_SAME_SIGN){ // check instruction RIGHT_INEQUAL_SAME_SIGN
		if (!strcmp(cur_node->value.t_string, "<="))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION <=\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION <=\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type
	int beforeType; // to save before node's type
	int intValue; // to save before node's value
	float floatValue; // to save before node's value

	// first attribute, save node to beforeType for comparision
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if currentType is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if currentType is IDENT
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute, compare with beforeType
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn(); // Call function
		if (retValue.type == INT){ // if return type is INT
			if (beforeType == INT){ // if beforeType is INT and before value is same or smaller than retValue
				if (intValue <= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or smaller than retValue
				if (floatValue <= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){ // if return type is FLOAT
			if (beforeType == INT){ // if beforeType is INT and before value is same or smaller than retValue
				if (intValue <= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or smaller than retValue
				if (floatValue <= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){ // if return type is IDENT
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if currentType is INT
					if (beforeType == INT){ // if beforeType is INT and before value is same or smaller than retValue
						if (intValue <= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or smaller than retValue
						if (floatValue <= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if currentType is FLOAT
					if (beforeType == INT){ // if beforeType is INT and before value is same or smaller than retValue
						if (intValue <= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){ // if beforeType is FLOAT and before value is same or smaller than retValue
						if (floatValue <= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{ // move cur_node to end of corresponding instruction
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
	}
	else if (currentType == INT){ // if currentType is INT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue <= cur_node->value.t_int) // check whether before value is same or smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue <= cur_node->value.t_int) // check whether before value is same or smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){// if currentType is FLOAT
		if (beforeType == INT){ // if beforeType is INT
			if (intValue <= cur_node->value.t_float) // check whether before value is same or smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){ // if beforeType is FLOAT
			if (floatValue <= cur_node->value.t_float) // check whether before value is same or smaller than current value
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if beforeType is IDENT
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if currentType is INT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue <= data.t_int) // check whether before value is same or smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue <= data.t_int) // check whether before value is same or smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if currentType is FLOAT
				if (beforeType == INT){ // if beforeType is INT
					if (intValue <= data.t_float) // check whether before value is same or smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){ // if beforeType is FLOAT
					if (floatValue <= data.t_float) // check whether before value is same or smaller than current value
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else
				return result;
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_stringp(){ // STRINGP function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == STRINGP){ // check instruction STRINGP
		if (!strcmp(cur_node->value.t_string, "STRINGP"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION STRINGP\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION STRINGP\n");
		return result;
	}
	
	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){ // if currentType is left_paren
		T_OBJ retValue = call_fn(); // Call function
		while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
			cur_node = cur_node->next;
			if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
				cur_node = cur_node->next;
				right_paren_Count++;
				break;
			}
		}
		return result;
	}
	else if (currentType == STRING){ // if currentType is STRING
		cur_node = cur_node->next;
	}
	else if (currentType == SQUOTE){ // if currentType is single quote
		cur_node = cur_node->next;
		currentType = cur_node->value.type;
		if (currentType == LEFT_PAREN){
			T_OBJ retValue = call_fn(); // Call function
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
		else if (currentType == STRING){ // if currentType is STRING
			cur_node = cur_node->next;
		}
		else if (currentType == IDENT){ // if currentType is identifier
			if (has_dict_key(dict, cur_node->value.t_string)){
				T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
				currentType = data.type;
				if (currentType == STRING){ // if currentType is STRING
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else
				return result;
		}
		else
			return result;
	}
	else if (currentType == IDENT){ // if currentType is identifier
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == STRING){ // if currentType is STRING
				cur_node = cur_node->next;
			}
			else
				return result;
		}
		else
			return result;
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			result.t_bool = true;
			return result;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_if(){ // IF function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == IF){ // check instruction IF
		if (!strcmp(cur_node->value.t_string, "IF"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION IF\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION IF\n");
		return result;
	}

	int currentType = cur_node->value.type; // save the value's type
	int condition = false; // check whether the condition part is ture
	T_OBJ retValue; // to save the executive part

	// condition part
	if (currentType == LEFT_PAREN){
		retValue = call_fn();
		while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && cur_node->value.type != LEFT_PAREN){ // move cur_node to end of corresponding instruction
			cur_node = cur_node->next;
			if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
				cur_node = cur_node->next;
				right_paren_Count++;
				break;
			}
		}
		if (cur_node == NULL){
			printf("ERROR : Syntax Error\n");
			return result;
		}
	}
	else {
		return result;
	}

	// check the retValue.type for set the condition
	if (retValue.type == INT){
		if (retValue.t_int == 0)
			condition = false;
		else
			condition = true;
	}
	else if (retValue.type == FLOAT){
		if (retValue.t_float == 0)
			condition = false;
		else
			condition = true;
	}
	else if (retValue.t_bool == 0){
		condition = false;
	}
	else if (retValue.t_bool == 1){
		condition = true;
	}

	// executive part with true condition
	if (condition == true){
		// practical part of execution
		if (currentType == LEFT_PAREN){
			retValue = call_fn(); // Call function
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
			result.t_bool = retValue.t_bool;
			if (cur_node->value.type == RIGHT_PAREN)
				currentType = retValue.type;
		}
		else {
			return result;
		}

		// ignore the part of execution for false condition 
		if (currentType == LEFT_PAREN){
			left_paren_Count++;
			int pCount = 1;
			cur_node = cur_node->next;
			while (cur_node != NULL && cur_node->next != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
					left_paren_Count++;
					pCount++;
				}
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					right_paren_Count++;
					pCount--;
				}
			}
			cur_node = cur_node->next;
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
		else {
			if (cur_node->value.type != RIGHT_PAREN)
				return result;
		}
	}
	// executive part with false condition
	else if (condition == false){
		// ignore the part of execution for true condition 
		if (currentType == LEFT_PAREN){
			left_paren_Count++;
			int pCount = 1;
			cur_node = cur_node->next;
			while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
					left_paren_Count++;
					pCount++;
				}
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					right_paren_Count++;
					pCount--;
				}
			}
			cur_node = cur_node->next;
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
		currentType = cur_node->value.type;

		// practical part of execution
		if (currentType == LEFT_PAREN){
			retValue = call_fn(); // Call function
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
		else if (currentType == RIGHT_PAREN){
		}
		else {
			return result;
		}
	}
	else
		return result;

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			return retValue;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}

T_OBJ fn_cond(){ // COND function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else{
		printf("ERROR : NO LEFT_PAREN\n");
		return result;
	}

	if (cur_node->value.type == COND){ // check instruction COND
		if (!strcmp(cur_node->value.t_string, "COND"))
			cur_node = cur_node->next;
		else{
			printf("ERROR : NO INSTRUCTION COND\n");
			return result;
		}
	}
	else{
		printf("ERROR : NO INSTRUCTION COND\n");
		return result;
	}

	int currentType; 
	int condition = false;
	T_OBJ retValue;

	while (condition == false){ // if condition is false, it will continue the loop
		currentType = cur_node->value.type; // save the value's type
		if (currentType == LEFT_PAREN){ // inside of current instruction 
			left_paren_Count++;
			cur_node = cur_node->next;
			currentType = cur_node->value.type;
		}
		else {
			return result;
		}

		// condition part
		if (currentType == LEFT_PAREN){
			retValue = call_fn(); // Call function
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			if (cur_node == NULL)
				return result;
		}
		else {
			return result;
		}

		currentType = cur_node->value.type;
		// executive part
		if (retValue.t_bool == true){ // if condition is true, it will be executed
			condition = true;
			if (currentType == LEFT_PAREN){
				retValue = call_fn();
				while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){ // move cur_node to end of corresponding instruction
					cur_node = cur_node->next;
					if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
						cur_node = cur_node->next;
						right_paren_Count++;
						break;
					}
				}
				if (cur_node == NULL){
					printf("ERROR : Syntax Error\n");
					return result;
				}
			}
			else {
				return result;
			}
		}
		else if (retValue.t_bool == false){ // if condition is false, it will be ignored
			if (currentType == LEFT_PAREN){
				left_paren_Count++;
				int pCount = 1;
				cur_node = cur_node->next;
				while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
					cur_node = cur_node->next;
					if (cur_node->value.type == LEFT_PAREN){
						left_paren_Count++;
						pCount++;
					}
					if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
						cur_node = cur_node->next;
						right_paren_Count++;
						pCount--;
					}
				}
				if (cur_node == NULL){
					printf("ERROR : Syntax Error\n");
					return result;
				}
			}
			else {
				return result;
			}
		}

		currentType = cur_node->value.type;

		if (currentType == RIGHT_PAREN){ // break out the current instruction 
			right_paren_Count++;
			cur_node = cur_node->next;			
			if (cur_node->value.type == SEMI_COLON){
				cur_node = cur_node->next;
			}
		}
		else {
			return result;
		}
	}
	// if condition is true, it will exit the loop

	LIST_NODE *checkLast;

	if (condition == true){ // if condition is true, it will ignore other instructions
		while (1){		
			checkLast = cur_node->next;
			if (cur_node->value.type == RIGHT_PAREN && checkLast->value.type == EOF){ // for check the end of cond instruction
				break;
			}
			else if (cur_node->value.type == RIGHT_PAREN && checkLast->value.type == SEMI_COLON){ // for check the end of cond instruction
				break;
			}

			currentType = cur_node->value.type;
			if (currentType == LEFT_PAREN){ // inside of current instruction 
				left_paren_Count++;
				cur_node = cur_node->next;
				currentType = cur_node->value.type;
			}
			else {
				return result;
			}

			// ignore condition part
			if (currentType == LEFT_PAREN){
				left_paren_Count++;
				int pCount = 1;
				cur_node = cur_node->next;
				while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
					cur_node = cur_node->next;
					if (cur_node->value.type == LEFT_PAREN){
						left_paren_Count++;
						pCount++;
					}
					if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
						cur_node = cur_node->next;
						right_paren_Count++;
						pCount--;
					}
				}
				if (cur_node == NULL)
					return result;
			}

			// ignore executive part
			if (currentType == LEFT_PAREN){
				left_paren_Count++;
				int pCount = 1;
				cur_node = cur_node->next;
				while (cur_node != NULL && pCount != 0){ // move cur_node to end of corresponding instruction
					cur_node = cur_node->next;
					if (cur_node->value.type == LEFT_PAREN){
						left_paren_Count++;
						pCount++;
					}
					if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
						cur_node = cur_node->next;
						right_paren_Count++;
						pCount--;
					}
				}
				if (cur_node == NULL)
					return result;
			}

			currentType = cur_node->value.type;
			if (currentType == RIGHT_PAREN){ // break out the current instruction 
				right_paren_Count++;
				cur_node = cur_node->next;
				if (cur_node->value.type == SEMI_COLON){
					cur_node = cur_node->next;
				}
			}
			else {
				return result;
			}

			if (cur_node == NULL){
				printf("ERROR : Syntax Error\n");
				return result;
			}
		}
	}	

	currentType = cur_node->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		right_paren_Count++;
		cur_node = cur_node->next;
		if (cur_node->value.type == EOF || cur_node->value.type == SEMI_COLON || cur_node->value.type == RIGHT_PAREN || cur_node->value.type == LEFT_PAREN){
			return retValue;
		}
		right_paren_Count++;
		printf("ERROR : Syntax Error\n");
	}
	printf("ERROR : Syntax Error\n");
	return result;
}
