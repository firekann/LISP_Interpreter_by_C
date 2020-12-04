#define _CRT_SECURE_NO_WARNINGS
#include "ATOM~COND.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include "c_dictionary.h"

T_OBJ fn_atom(){ // ATOM function
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN) { // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR ATOM\n");
		return result;
	}

	if (cur_node->value.type == ATOM){ // check instruction ATOM
		if (!strcmp(cur_node->value.t_string, "ATOM"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){
		LIST_NODE *check = cur_node->next;
		if (check->value.type == IDENT){
			left_paren_Count++;
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count += 2;
					break;
				}
			}
			result.type = T_LIST;
			return result;
		}
		T_OBJ retValue = call_fn();
		if (retValue.type == T_LIST){
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){
				cur_node = cur_node->next;
				if (cur_node != NULL && cur_node->value.type == RIGHT_PAREN){
					cur_node = cur_node->next;
					right_paren_Count++;
					break;
				}
			}
			return result;
		}
		else if (retValue.t_bool == false){
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN){
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
		else
			return result;
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

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
		printf("ERROR : NO LEFT_PAREN FOR NULL\n");
		return result;
	}

	if (cur_node->value.type == L_NULL){ // check instruction NULL
		if (!strcmp(cur_node->value.t_string, "NULL"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type; // save the value's type

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();

		if (retValue.t_bool != false){
			cur_node = cur_node->next;
			return result;
		}			
		else if (retValue.t_bool == false){
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_numberp(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == NUMBERP){ // check instruction NULL
		if (!strcmp(cur_node->value.t_string, "NUMBERP"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT || retValue.type == FLOAT){
		}
		else{
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_zerop(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == ZEROP){ // check instruction ZEROP
		if (!strcmp(cur_node->value.t_string, "ZEROP"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (retValue.t_int != 0)
				return result;
		} 
		else if (retValue.type == FLOAT){
			if (retValue.t_float != 0)
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				if (data.type == INT && data.t_int == 0){ // if type is FLOAT or INT
				}
				else if (data.type == FLOAT && data.t_float == 0){
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
	}
	else if (currentType == INT){ // if type is INT
		if (cur_node->value.t_int == 0) // value is 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (cur_node->value.t_float == 0)
			//if (ABS(0 - atof(cur_node->value.t_string) < epsilon)) // value is 0
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_minusp(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN) { // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR ATOM\n");
		return result;
	}

	if (cur_node->value.type == MINUSP){ // check instruction MINUSP
		if (!strcmp(cur_node->value.t_string, "MINUSP"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (retValue.t_int >= 0)
				return result;
		}
		else if (retValue.type == FLOAT){
			if (retValue.t_float >= 0)
				return result;
		}
		else {
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
	}
	else if (currentType == INT){ // if type is INT
		if (cur_node->value.t_int < 0) // value is 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (cur_node->value.t_float < 0)
			//if (ABS(0 - atof(cur_node->value.t_string) < epsilon)) // value is 0
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == IDENT){ // if type is FLOAT		
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			if (data.type == INT){ // if type is INT
				if (data.t_int < 0) // value is 0
					cur_node = cur_node->next;
				else
					return result;
			}
			else if (data.type == FLOAT){ // if type is FLOAT
				if (data.t_float < 0) // value is 0
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_equal(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == EQUAL){ // check instruction EQUAL
		if (!strcmp(cur_node->value.t_string, "EQUAL"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int beforeType;
	int intValue;
	float floatValue;
	bool boolValue;
	char stringValue[MAX_COMMAND_LEN];

	int currentType = cur_node->value.type;

	// first attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == NIL){ // if type is NIL
			beforeType = retValue.type;
		}
		else if (retValue.type == STRING){ // if type is STRING
			beforeType = retValue.type;
			strcpy(stringValue, cur_node->value.t_string);
		}
		else if (retValue.type == BOOLEAN){
			beforeType = retValue.type;
			boolValue = retValue.t_bool;
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
				beforeType = NIL;
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
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else if (currentType == NIL){ // if type is NIL
					beforeType = currentType;
				}
				else if (currentType == STRING){ // if type is STRING
					beforeType = currentType;
					strcpy(stringValue, data.t_string);
				}
				else if (currentType == BOOLEAN){
					beforeType = currentType;
					boolValue = data.t_bool;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
	}
	else if (currentType == INT){ // if type is INT
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == NIL){ // if type is NIL
		beforeType = currentType;
		cur_node = cur_node->next;
	}
	else if (currentType == STRING){ // if type is STRING
		beforeType = currentType;
		strcpy(stringValue, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				cur_node = cur_node->next;
			}
			else if (currentType == NIL){ // if type is NIL
				beforeType = currentType;
				cur_node = cur_node->next;
			}
			else if (currentType == STRING){ // if type is STRING
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
	else
		return result;

	currentType = cur_node->value.type;

	// second attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (beforeType == retValue.type && intValue == retValue.t_int){
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){
			if (beforeType == retValue.type && floatValue == retValue.t_float){
			}
			else
				return result;
		}
		else if (retValue.type == NIL){ // if type is NIL
			if (beforeType == retValue.type){
			}
			else
				return result;
		}
		else if (retValue.type == STRING){ // if type is STRING
			if (beforeType == retValue.type && !strcmp(stringValue, retValue.t_string)){
			}
			else
				return result;
		}
		else if (retValue.type == BOOLEAN){
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
		else if (retValue.type == IDENT){
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
				else if (retValue.type == BOOLEAN){
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
		else{
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
	}
	else if (currentType == INT){ // if type is INT
		if (beforeType == currentType && intValue == cur_node->value.t_int){
			cur_node = cur_node->next;
		}
		else
			return result;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == currentType && floatValue == cur_node->value.t_float){
			cur_node = cur_node->next;
		}
		else
			return result;
	}
	else if (currentType == NIL){ // if type is NIL
		if (beforeType == currentType)
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == STRING){ // if type is STRING		
		if (beforeType == currentType && !strcmp(stringValue, cur_node->value.t_string))
			cur_node = cur_node->next;
		else
			return result;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				if (beforeType == currentType && intValue == data.t_int){
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == currentType && floatValue == data.t_float){
					cur_node = cur_node->next;
				}
				else
					return result;
			}
			else if (currentType == NIL){ // if type is NIL
				if (beforeType == currentType)
					cur_node = cur_node->next;
				else
					return result;
			}
			else if (currentType == STRING){ // if type is STRING		
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_left_inequal(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == LEFT_INEQUAL_SIGN){ // check instruction LEFT_INEQUAL_SIGN
		if (!strcmp(cur_node->value.t_string, ">"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
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
	else if (currentType == INT){
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
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

	// second attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (beforeType == INT){
				if (intValue > retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue > retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){
			if (beforeType == INT){
				if (intValue > retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue > retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					if (beforeType == INT){
						if (intValue > data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
						if (floatValue > data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					if (beforeType == INT){
						if (intValue > data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
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
		else{
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
	else if (currentType == INT){
		if (beforeType == INT){
			if (intValue > cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue > cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue > cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue > cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue > data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue > data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue > data.t_float)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue > data.t_float)
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_right_inequal(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == RIGHT_INEQUAL_SIGN){ // check instruction RIGHT_INEQUAL_SIGN
		if (!strcmp(cur_node->value.t_string, "<"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
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
	else if (currentType == INT){
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
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

	// second attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (beforeType == INT){
				if (intValue < retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue < retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){
			if (beforeType == INT){
				if (intValue < retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue < retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					if (beforeType == INT){
						if (intValue < data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
						if (floatValue < data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					if (beforeType == INT){
						if (intValue < data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
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
		else{
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
	else if (currentType == INT){
		if (beforeType == INT){
			if (intValue < cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue < cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue < cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue < cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue < data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue < data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue < data.t_float)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue < data.t_float)
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_left_inequal_same(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == LEFT_INEQUAL_SAME_SIGN){ // check instruction LEFT_INEQUAL_SAME_SIGN
		if (!strcmp(cur_node->value.t_string, ">="))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
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
	else if (currentType == INT){
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
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

	// second attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (beforeType == INT){
				if (intValue >= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue >= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){
			if (beforeType == INT){
				if (intValue >= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue >= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					if (beforeType == INT){
						if (intValue >= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
						if (floatValue >= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					if (beforeType == INT){
						if (intValue >= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
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
		else{
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
	else if (currentType == INT){
		if (beforeType == INT){
			if (intValue >= cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue >= cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue >= cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue >= cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue >= data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue >= data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue >= data.t_float)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue >= data.t_float)
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_right_inequal_same(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == RIGHT_INEQUAL_SAME_SIGN){ // check instruction RIGHT_INEQUAL_SAME_SIGN
		if (!strcmp(cur_node->value.t_string, "<="))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			beforeType = retValue.type;
			intValue = retValue.t_int;
		}
		else if (retValue.type == FLOAT){
			beforeType = retValue.type;
			floatValue = retValue.t_float;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					beforeType = currentType;
					intValue = data.t_int;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					beforeType = currentType;
					floatValue = data.t_float;
				}
				else
					return result;
			}
			else
				return result;
		}
		else{
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
	else if (currentType == INT){
		beforeType = currentType;
		intValue = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = cur_node->value.t_float;
		cur_node = cur_node->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				cur_node = cur_node->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
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

	// second attribute
	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
		if (retValue.type == INT){
			if (beforeType == INT){
				if (intValue <= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue <= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == FLOAT){
			if (beforeType == INT){
				if (intValue <= retValue.t_int){
				}
				else
					return result;
			}
			else if (beforeType == FLOAT){
				if (floatValue <= retValue.t_int){
				}
				else
					return result;
			}
			else
				return result;
		}
		else if (retValue.type == IDENT){
			if (has_dict_key(dict, retValue.t_string)){
				T_OBJ data = get_dict_obj(dict, retValue.t_string);
				currentType = data.type;
				if (currentType == INT){ // if type is INT
					if (beforeType == INT){
						if (intValue <= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
						if (floatValue <= data.t_int){
						}
						else
							return result;
					}
					else
						return result;
				}
				else if (currentType == FLOAT){ // if type is FLOAT
					if (beforeType == INT){
						if (intValue <= data.t_int){
						}
						else
							return result;
					}
					else if (beforeType == FLOAT){
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
		else{
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
	else if (currentType == INT){
		if (beforeType == INT){
			if (intValue <= cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue <= cur_node->value.t_int)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue <= cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
		else if (beforeType == FLOAT){
			if (floatValue <= cur_node->value.t_float)
				cur_node = cur_node->next;
			else
				return result;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue <= data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue <= data.t_int)
						cur_node = cur_node->next;
					else
						return result;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue <= data.t_float)
						cur_node = cur_node->next;
					else
						return result;
				}
				else if (beforeType == FLOAT){
					if (floatValue <= data.t_float)
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_stringp(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == STRINGP){ // check instruction STRINGP
		if (!strcmp(cur_node->value.t_string, "STRINGP"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;
	
	int currentType = cur_node->value.type;

	if (currentType == LEFT_PAREN){
		T_OBJ retValue = call_fn();
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
	else if (currentType == STRING){
		cur_node = cur_node->next;
	}
	else if (currentType == SQUOTE){
		cur_node = cur_node->next;
		currentType = cur_node->value.type;
		if (currentType == LEFT_PAREN){
			T_OBJ retValue = call_fn();
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
		else if (currentType == STRING){
			cur_node = cur_node->next;
		}
		else if (currentType == IDENT){
			if (has_dict_key(dict, cur_node->value.t_string)){
				T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
				currentType = data.type;
				if (currentType == STRING){
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
	else if (currentType == IDENT){
		if (has_dict_key(dict, cur_node->value.t_string)){
			T_OBJ data = get_dict_obj(dict, cur_node->value.t_string);
			currentType = data.type;
			if (currentType == STRING){
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
		if (ifFlag){
			result.t_bool = true;
			return result;
		}
	}

	return result;
}

T_OBJ fn_if(){
	T_OBJ result;
	result.type = BOOLEAN;
	result.t_bool = false;

	if (cur_node->value.type == LEFT_PAREN){ // check LEFT_PAREN existence
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else
		return result;

	if (cur_node->value.type == IF){ // check instruction STRINGP
		if (!strcmp(cur_node->value.t_string, "IF"))
			cur_node = cur_node->next;
		else
			return result;
	}
	else
		return result;

	int currentType = cur_node->value.type;
	int condition = false;
	T_OBJ retValue;

	if (currentType == LEFT_PAREN){
		retValue = call_fn();
		while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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

	if (condition == true){
		if (currentType == LEFT_PAREN){
			retValue = call_fn();
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
		if (currentType == LEFT_PAREN){
			left_paren_Count++;
			int pCount = 1;
			cur_node = cur_node->next;
			while (cur_node != NULL && pCount != 0){
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
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
	}
	else if (condition == false){
		if (currentType == LEFT_PAREN){
			left_paren_Count++;
			int pCount = 1;
			cur_node = cur_node->next;
			while (cur_node != NULL && pCount != 0){
				cur_node = cur_node->next;
				if (cur_node->value.type == LEFT_PAREN){
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

		if (currentType == LEFT_PAREN){
			retValue = call_fn();
			while (cur_node != NULL && cur_node->value.type != RIGHT_PAREN && retValue.t_bool == false){
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
		if (ifFlag){
			return retValue;
		}
	}

	return result;
}
