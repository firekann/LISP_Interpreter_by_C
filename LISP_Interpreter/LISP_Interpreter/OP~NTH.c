#define _CRT_SECURE_NO_WARNINGS
#include "OP~NTH.h"
#include "type.h"
#include "lexer.h"
#include "util.h"
#include <stdio.h>

T_OBJ fn_add() {
	if (cur_node->value.type == LEFT_PAREN) {
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR ADD\n");
		return;
	}

	int cnt = 0;
	bool is_ident = false;
	char *key = NULL;
	float sum = 0;
	int type = cur_node->value.type;
	while (cur_node->value.type != RIGHT_PAREN) {
		bool flag = true;
		if (cur_node->value.type == INT) {
			sum += (float)cur_node->value.t_int;
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			sum += (float)cur_node->value.t_float;
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {
					type = INT;
				}
				sum += (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				sum += (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR ADD\n");
				return;
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//add의 첫번째 인자가 IDENT일 경우 그 IDENT의 값을 바꿔줌
			is_ident = true;
			T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);
			if (tmp.type == INT) {
				type = INT;
				sum = (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				sum = (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR ADD\n");
				return;
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR ADD\n");
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR ADD\n");
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR ADD\n");
		return;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	if (type == INT) {
		tmp.t_int = (int)sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%d", (int)sum);
	}
	else {
		tmp.t_float = sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%f", sum);
	}
	if (is_ident) {
		insert_dict_node(dict, key, &tmp);
		free(key);
	}
	printf("ADD : %s, type : %d\n", tmp.t_string, tmp.type);	//디버깅용
	return tmp;
}

T_OBJ fn_sub() {
	if (cur_node->value.type == LEFT_PAREN) {
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR SUB\n");
		return;
	}

	int cnt = 0;
	bool is_ident = false;
	char *key = NULL;
	float sum = 0;
	int type = cur_node->value.type;
	while (cur_node->value.type != RIGHT_PAREN) {
		bool flag = true;
		if (cur_node->value.type == INT) {
			if (cnt == 0) {	//sub의 경우 처음 초기값이 존재해야함
				type = INT;
				sum = (float)cur_node->value.t_int;
			}
			else {
				sum -= (float)cur_node->value.t_int;
			}
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			if (cnt == 0) {	//sub의 경우 처음 초기값이 존재해야함
				sum = (float)cur_node->value.t_float;
			}
			else {
				sum -= (float)cur_node->value.t_float;
			}
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {	//sub의 경우 처음 초기값이 존재해야함
					type = INT;
					sum = (float)tmp.t_int;
				}
				else {
					sum -= (float)tmp.t_int;
				}
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				if (cnt == 0) {	//sub의 경우 처음 초기값이 존재해야함
					sum = (float)tmp.t_float;
				}
				else {
					sum -= (float)tmp.t_float;
				}
			}
			else {
				printf("ERROR : TYPE ERROR FOR SUB\n");
				return;
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//sub의 첫번째 인자가 IDENT일 경우 그 IDENT의 값을 바꿔줌
			is_ident = true;
			T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);
			if (tmp.type == INT) {
				type = INT;
				sum = (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				sum = (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR SUB\n");
				return;
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR SUB\n");
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR SUB\n");
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR SUB\n");
		return;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	if (type == INT) {
		tmp.t_int = (int)sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%d", (int)sum);
	}
	else {
		tmp.t_float = sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%f", sum);
	}
	if (is_ident) {
		insert_dict_node(dict, key, &tmp);
		free(key);
	}
	printf("SUB : %s, type : %d\n", tmp.t_string, tmp.type);	//디버깅용
	return tmp;
}

T_OBJ fn_mul() {
	if (cur_node->value.type == LEFT_PAREN) {
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR MUL\n");
		return;
	}

	int cnt = 0;
	bool is_ident = false;
	char *key = NULL;
	float sum = 1;
	int type = cur_node->value.type;
	while (cur_node->value.type != RIGHT_PAREN) {
		bool flag = true;
		if (cur_node->value.type == INT) {
			sum *= (float)cur_node->value.t_int;
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			sum *= (float)cur_node->value.t_float;
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {
					type = INT;
				}
				sum *= (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				sum *= (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR MUL\n");
				return;
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//mul의 첫번째 인자가 IDENT일 경우 그 IDENT의 값을 바꿔줌
			is_ident = true;
			T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);
			if (tmp.type == INT) {
				type = INT;
				sum = (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				sum = (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR MUL\n");
				return;
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR MUL\n");
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR MUL\n");
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR MUL\n");
		return;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	if (type == INT) {
		tmp.t_int = (int)sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%d", (int)sum);
	}
	else {
		tmp.t_float = sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%f", sum);
	}
	if (is_ident) {
		insert_dict_node(dict, key, &tmp);
		free(key);
	}
	printf("MUL : %s, type : %d\n", tmp.t_string, tmp.type);	//디버깅용
	return tmp;
}

T_OBJ fn_div() {
	if (cur_node->value.type == LEFT_PAREN) {
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR DIV\n");
		return;
	}

	int cnt = 0;
	bool is_ident = false;
	char *key = NULL;
	float sum = 0;
	int type = cur_node->value.type;
	while (cur_node->value.type != RIGHT_PAREN) {
		bool flag = true;
		if (cur_node->value.type == INT) {
			if (cnt == 0) {	//div의 경우 처음 초기값이 존재해야함
				sum = (float)cur_node->value.t_int;
			}
			else {
				sum /= (float)cur_node->value.t_int;
			}
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			if (cnt == 0) {	//div의 경우 처음 초기값이 존재해야함
				sum = (float)cur_node->value.t_float;
			}
			else {
				sum /= (float)cur_node->value.t_float;
			}
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {	//div의 경우 처음 초기값이 존재해야함
					type = INT;
					sum = (float)tmp.t_int;
				}
				else {
					sum /= (float)tmp.t_int;
				}
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				if (cnt == 0) {	//div의 경우 처음 초기값이 존재해야함
					sum = (float)tmp.t_float;
				}
				else {
					sum /= (float)tmp.t_float;
				}
			}
			else {
				printf("ERROR : TYPE ERROR FOR DIV\n");
				return;
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//div의 첫번째 인자가 IDENT일 경우 그 IDENT의 값을 바꿔줌
			is_ident = true;
			T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);
			if (tmp.type == INT) {
				type = INT;
				sum = (float)tmp.t_int;
			}
			else if (tmp.type == FLOAT) {
				type =FLOAT;
				sum = (float)tmp.t_float;
			}
			else {
				printf("ERROR : TYPE ERROR FOR DIV\n");
				return;
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR DIV\n");
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR DIV\n");
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR DIV\n");
		return;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	if (type == INT) {
		tmp.t_int = (int)sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%d", (int)sum);
	}
	else {
		tmp.t_float = sum;
		tmp.t_string = (char*)malloc(sizeof(char) * 20);
		sprintf(tmp.t_string, "%f", sum);
	}
	if (is_ident) {
		insert_dict_node(dict, key, &tmp);
		free(key);
	}
	printf("DIV : %s, type : %d\n", tmp.t_string, tmp.type);	//디버깅용
	return tmp;
}

T_OBJ fn_setq() {
	if (cur_node->value.type == LEFT_PAREN) {
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR SETQ\n");
		return;
	}

	char* symbol;	//변수 값

	if (cur_node->value.type == IDENT) {
		symbol = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
		strcpy(symbol, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO IDENTIFIER FOR SETQ\n");
		return;
	}

	T_OBJ res;
	if (cur_node->value.type == SQUOTE) {	//이 경우는 리스트 입력을 받음

	}
	else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
		T_OBJ tmp = call_fn();	//리턴값이 있는 함수의 경우
		if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == T_LIST || tmp.type == BOOLEAN || tmp.type == NIL) {
			insert_dict_node(dict, symbol, &tmp);
			res = tmp;
		}
		else {
			printf("ERROR : TYPE ERROR FOR SETQ\n");
			return;
		}
	}
	else if (cur_node->value.type == IDENT) {	//a=b의 경우 b의 값을 a에 대입 하는 느낌
		T_OBJ tmp = get_dict_obj(dict, cur_node->value.t_string);	//dict에서 먼저 값을 가져옴
		insert_dict_node(dict, symbol, &tmp);
		res = tmp;
		cur_node = cur_node->next;
	}
	else {
		// 정수 실수 문자열의 경우
		if (cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == STRING) {
			insert_dict_node(dict, symbol, &(cur_node->value));
			res = cur_node->value;
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : VALUE ERROR FOR SETQ\n");
			return;
		}
	}
	free(symbol);
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		return res;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR SETQ\n");
		return;
	}
}

T_OBJ fn_list() {

}

T_OBJ fn_car() {

}

T_OBJ fn_cdr() {

}

T_OBJ fn_nth() {

}