#define _CRT_SECURE_NO_WARNINGS
#include "ATOM~COND.h"
#include "lexer.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "c_dictionary.h"

bool fn_atom(c_DICT *dict, LIST_NODE *node){ // ATOM 함수
	LIST_NODE *temp = node;

	if (temp->value.type == T_CODE){ // 명령어가 ATOM 인지 확인
		if (!strcmp(temp->value.t_string, "ATOM"))
			temp = temp->next;
		else{
			return false;
		}
	}

	int currentType = temp->value.type; // 현재 type을 저장해둠
	if (currentType != T_LIST && temp->next == NULL){ // 현재 타입이 LIST가 아니고 마지막 노드일 경우 true
		return true;
	}
	else{
		return false;
	}
	return false;
}

bool fn_null(c_DICT *dict, LIST_NODE *node){ // NULL 함수
	LIST_NODE *temp = node;

	if (temp->value.type == T_CODE){ // 명령어가 NULL 인지 확인
		if (!strcmp(temp->value.t_string, "NULL"))
			temp = temp->next;
		else{
			return false;
		}
	}

	int currentType = temp->value.type;
	if (currentType == T_SYMBOL){ // 다음 값이 symbol인지 확인
		if (!strcmp(temp->value.t_string, "NIL")) // 입력 값이 NIL 인 경우
			return true;
		else if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (!strcmp(data.t_string, "NIL")){ // 입력 받은 변수의 값이 NIL인 경우
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}

bool fn_numberp(c_DICT *dict, LIST_NODE *node){
	LIST_NODE *temp = node;

	if (temp->value.type == T_CODE){ // 명령어가 NUMBERP 인지 확인
		if (!strcmp(temp->value.t_string, "NUMBERP"))
			temp = temp->next;
		else{
			return false;
		}
	}

	int currentType = temp->value.type;
	if (currentType == T_FLOAT || currentType == T_INT){ // 다음 값이 숫자인지 확인
		return true;
	}
	else if (currentType == T_SYMBOL){ // 변수가 주어진 경우
		if (has_dict_key(dict, &(temp->value.t_string))){
			T_OBJ data = get_dict_obj(dict, &(temp->value.t_string));
			if (data.type == T_FLOAT || data.type == T_INT){ // 입력 받은 변수의 값이 숫자인 경우
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}
	return false;
}