#define _CRT_SECURE_NO_WARNINGS
#include "ATOM~COND.h"
#include "lexer.h"
#include <stdio.h>
#include <string.h>
#include "c_dictionary.h"

int fn_atom(c_DICT *dict, LIST_NODE *temp){ // ATOM function
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
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

	if (currentType == INT || currentType == FLOAT || currentType == IDENT || currentType == NIL){ // check whether it is symbol
		temp = temp->next;
	}
	else if (currentType == SQUOTE){ // if current type is single quote
		temp = temp->next;
		currentType = temp->value.type;
		if (currentType == INT || currentType == FLOAT || currentType == IDENT || currentType == NIL){ // check whether it is symbol
			temp = temp->next;
		}
	}
	else if (currentType == DQUOTE){ // if current type is double quote
		temp = temp->next;
		currentType = temp->value.type;
		//while (currentType == INT || currentType == FLOAT || currentType == IDENT || currentType == NIL){ // check whether it is symbol
		while (currentType != DQUOTE){ // loop until meet the next double quote
			temp = temp->next;
			currentType = temp->value.type;
			if (currentType == EOF)
				return false;
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
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_null(c_DICT *dict, LIST_NODE *temp){ // NULL function
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
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

	if (currentType == NIL) // currentType is NIL
		temp = temp->next;
	else if (currentType == IDENT){ // next token should be identifier or NIL
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (data.type == NIL){ // current identifier's value is NIL
				temp = temp->next;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_numberp(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
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
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_zerop(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == ZEROP){ // check instruction ZEROP
		if (!strcmp(temp->value.t_string, "ZEROP"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	if (currentType == INT){ // if type is INT
		if (temp->value.t_int == 0) // value is 0
			temp = temp->next;
		else
			return false;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (temp->value.t_float == 0)
		//if (ABS(0 - atof(temp->value.t_string) < epsilon)) // value is 0
			temp = temp->next;
		else
			return false;
	}
	else if (currentType == IDENT){ // if type is identifier
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (data.type == INT){ // if type is INT
				if (data.t_int == 0) // value is 0
					temp = temp->next;
				else
					return false;
			}
			else if (data.type == FLOAT){ // if type is FLOAT
				if (data.t_float == 0) // value is 0
					temp = temp->next;
				else
					return false;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_minusp(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == MINUSP){ // check instruction MINUSP
		if (!strcmp(temp->value.t_string, "MINUSP"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;	
	/*if (currentType == SUB_OP){ // if type is SUB_OP
		temp = temp->next;
		currentType = temp->value.type;
		if (currentType == INT){ // if type is INT
			if (temp->value.t_int != 0) // value is 0
				temp = temp->next;
			else
				return false;
		}
		else if (currentType == FLOAT){ // if type is FLOAT		
			if (temp->value.t_float != 0)
				//if (ABS(0 - atof(temp->value.t_string) < epsilon)) // value is 0
				temp = temp->next;
			else
				return false;
		}
	}*/
	
	if (currentType == INT){ // if type is INT
		if (temp->value.t_int < 0) // value is 0
			temp = temp->next;
		else
			return false;
	}
	else if (currentType == FLOAT){ // if type is FLOAT		
		if (temp->value.t_float < 0)
			//if (ABS(0 - atof(temp->value.t_string) < epsilon)) // value is 0
			temp = temp->next;
		else
			return false;
	}
	else if (currentType == IDENT){ // if type is FLOAT		
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (data.type == INT){ // if type is INT
				if (data.t_int < 0) // value is 0
					temp = temp->next;
				else
					return false;
			}
			else if (data.type == FLOAT){ // if type is FLOAT
				if (data.t_float < 0) // value is 0
					temp = temp->next;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else 
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_equal(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == EQUAL){ // check instruction EQUAL
		if (!strcmp(temp->value.t_string, "EQUAL"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int beforeType;
	int intValue;
	float floatValue;
	
	int currentType = temp->value.type;

	// first attribute
	if (currentType == INT){ // if type is INT
		beforeType = currentType;
		intValue = temp->value.t_int;
		temp = temp->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = temp->value.t_float;
		temp = temp->next;
	}
	else if (currentType == NIL){ // if type is NIL
		beforeType = currentType;
		temp = temp->next;
	}	
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				temp = temp->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				temp = temp->next;
			}
			else if (currentType == NIL){ // if type is NIL
				beforeType = currentType;
				temp = temp->next;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;

	// second attribute
	if (currentType == INT){ // if type is INT
		if (beforeType == currentType && intValue == temp->value.t_int){
			temp = temp->next;
		}
		else
			return false;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == currentType && floatValue == temp->value.t_float){
			temp = temp->next;
		}
		else
			return false;
	}
	else if (currentType == NIL){ // if type is NIL
		if (beforeType == currentType)
			temp = temp->next;
		else
			return false;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				if (beforeType == currentType && intValue == data.t_int){
					temp = temp->next;
				}
				else
					return false;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == currentType && floatValue == data.t_float){
					temp = temp->next;
				}
				else
					return false;
			}
			else if (currentType == NIL){ // if type is NIL
				if (beforeType == currentType)
					temp = temp->next;
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_left_inequal(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == LEFT_INEQUAL_SIGN){ // check instruction LEFT_INEQUAL_SIGN
		if (!strcmp(temp->value.t_string, ">"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == INT){
		beforeType = currentType;
		intValue = temp->value.t_int;
		temp = temp->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = temp->value.t_float;
		temp = temp->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				temp = temp->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				temp = temp->next;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;

	// second attribute
	if (currentType == INT){
		if (beforeType == INT){
			if (intValue > temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue > temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue > temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue > temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue > data.t_int)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue > data.t_int)
						temp = temp->next;
					else
						return false;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue > data.t_float)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue > data.t_float)
						temp = temp->next;
					else
						return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_right_inequal(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == RIGHT_INEQUAL_SIGN){ // check instruction RIGHT_INEQUAL_SIGN
		if (!strcmp(temp->value.t_string, "<"))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == INT){
		beforeType = currentType;
		intValue = temp->value.t_int;
		temp = temp->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = temp->value.t_float;
		temp = temp->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				temp = temp->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				temp = temp->next;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;

	// second attribute
	if (currentType == INT){
		if (beforeType == INT){
			if (intValue < temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue < temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue < temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue < temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue < data.t_int)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue < data.t_int)
						temp = temp->next;
					else
						return false;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue < data.t_float)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue < data.t_float)
						temp = temp->next;
					else
						return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_left_inequal_same(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == LEFT_INEQUAL_SAME_SIGN){ // check instruction LEFT_INEQUAL_SAME_SIGN
		if (!strcmp(temp->value.t_string, ">="))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == INT){
		beforeType = currentType;
		intValue = temp->value.t_int;
		temp = temp->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = temp->value.t_float;
		temp = temp->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType  == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				temp = temp->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				temp = temp->next;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;

	// second attribute
	if (currentType == INT){
		if (beforeType == INT){
			if (intValue >= temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue >= temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue >= temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue >= temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue >= data.t_int)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue >= data.t_int)
						temp = temp->next;
					else
						return false;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue >= data.t_float)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue >= data.t_float)
						temp = temp->next;
					else
						return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_right_inequal_same(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == RIGHT_INEQUAL_SAME_SIGN){ // check instruction RIGHT_INEQUAL_SAME_SIGN
		if (!strcmp(temp->value.t_string, "<="))
			temp = temp->next;
		else
			return false;
	}
	else
		return false;

	int currentType = temp->value.type;
	int beforeType;
	int intValue;
	float floatValue;

	// first attribute
	if (currentType == INT){
		beforeType = currentType;
		intValue = temp->value.t_int;
		temp = temp->next;
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		beforeType = currentType;
		floatValue = temp->value.t_float;
		temp = temp->next;
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){ // if type is INT
				beforeType = currentType;
				intValue = data.t_int;
				temp = temp->next;
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				beforeType = currentType;
				floatValue = data.t_float;
				temp = temp->next;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;

	// second attribute
	if (currentType == INT){
		if (beforeType == INT){
			if (intValue <= temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue <= temp->value.t_int)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == FLOAT){ // if type is FLOAT
		if (beforeType == INT){
			if (intValue <= temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
		else if (beforeType == FLOAT){
			if (floatValue <= temp->value.t_float)
				temp = temp->next;
			else
				return false;
		}
	}
	else if (currentType == IDENT){ // if type is IDENT	
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == INT){
				if (beforeType == INT){
					if (intValue <= data.t_int)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue <= data.t_int)
						temp = temp->next;
					else
						return false;
				}
			}
			else if (currentType == FLOAT){ // if type is FLOAT
				if (beforeType == INT){
					if (intValue <= data.t_float)
						temp = temp->next;
					else
						return false;
				}
				else if (beforeType == FLOAT){
					if (floatValue <= data.t_float)
						temp = temp->next;
					else
						return false;
				}
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_stringp(c_DICT *dict, LIST_NODE *temp){
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == STRINGP){ // check instruction STRINGP
		temp = temp->next;
	}
	else
		return false;

	int currentType = temp->value.type;

	if (currentType == DQUOTE){ // if current type is double quote
		temp = temp->next;
		currentType = temp->value.type;
		while (currentType != DQUOTE){ // loop until meet the next double quote
			temp = temp->next;
			currentType = temp->value.type;
			if (currentType == EOF)
				return false;
		}

		if (currentType == DQUOTE){ // check whether it is double quote
			temp = temp->next;
		}
		else
			return false;
	}
	else if (currentType == IDENT){
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			currentType = data.type;
			if (currentType == DQUOTE){ // if current type is double quote
				temp = temp->next;
				currentType = temp->value.type;
				while (currentType != DQUOTE){ // loop until meet the next double quote
					temp = temp->next;
					currentType = temp->value.type;
					if (currentType == EOF)
						return false;
				}

				if (currentType == DQUOTE){ // check whether it is double quote
					temp = temp->next;
				}
				else
					return false;
			}
		}
		else
			return false;
	}
	else
		return false;

	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}

int fn_if(c_DICT *dict, LIST_NODE *temp){
	ifFlag = 1;
	if (temp->value.type == LEFT_PAREN){ // check left paren existence
		parenCount++;
		temp = temp->next;
	}
	else
		return false;

	if (temp->value.type == IF){ // check instruction IF
		temp = temp->next;
	}
	else
		return false;

	int currentType = temp->value.type;
	LIST_NODE *nextNode = temp->next;
	int condition = 0;

	if (nextNode->value.type == ATOM){ // check left paren existence
		condition = fn_atom(dict, temp);
		if (currentType == LEFT_PAREN){
			parenCount++;
		}
		while (parenCount != 1){
			temp = temp->next;
			currentType = temp->value.type;
			if (currentType == RIGHT_PAREN)
				parenCount--;
		}
		if (!condition){
			return false;
		}
	}
	else if (currentType == IDENT){}
	//////////////// other option's can be done after function merge
	else
		return false;

	temp = temp->next;
	currentType = temp->value.type;
	if (condition == true){
		nextNode = temp->next;
		condition = 0;

		if (nextNode->value.type == ATOM){ // check left paren existence
			condition = fn_atom(dict, temp);
			if (currentType == LEFT_PAREN){
				parenCount++;
			}
			while (parenCount != 1){
				temp = temp->next;
				currentType = temp->value.type;
				if (currentType == RIGHT_PAREN)
					parenCount--;
			}
			if (!condition){
				return false;
			}
		}
		else if (currentType == IDENT){}
		//////////////// other option's can be done after function merge
		else
			return false;
	}

	else if (condition == false){

	}


	currentType = temp->value.type;
	if (currentType == RIGHT_PAREN){ // end of instruction
		parenCount--;
		temp = temp->next;
		if (temp->value.type == EOF || temp->value.type == SEMI_COLON)
			return true;
		if (ifFlag)
			return true;
	}

	return false;
}