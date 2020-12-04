#define _CRT_SECURE_NO_WARNINGS
#include "OP~NTH.h"
#include "type.h"
#include "lexer.h"
#include "util.h"
#include <stdio.h>

T_OBJ fn_add() {
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//없을시 에러
		printf("ERROR : NO LEFT_PAREN FOR ADD\n");
		return;
	}

	int cnt = 0;	//나온 원소의 수를 카운트 하기 위한 변수
	bool is_ident = false;	//ident가 나오는지 기록하는 변수
	char *key = NULL;	//ident가 나왔을때 그 것을 dict의 key로 넣기 위한 변수
	float sum = 0;
	int type = cur_node->value.type;	//함수 전체의 리턴 타입을 위한 변수
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
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
				free(key);
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
			free(key);
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR ADD\n");
			free(key);
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR ADD\n");
		free(key);
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
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//없을시 에러
		printf("ERROR : NO LEFT_PAREN FOR SUB\n");
		return;
	}

	int cnt = 0;	//나온 원소의 수를 카운트 하기 위한 변수
	bool is_ident = false;	//ident가 나오는지 기록하는 변수
	char *key = NULL;	//ident가 나왔을때 그 것을 dict의 key로 넣기 위한 변수
	float sum = 0;
	int type = cur_node->value.type;	//함수 전체의 리턴 타입을 위한 변수
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
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
				free(key);
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
			free(key);
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR SUB\n");
			free(key);
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR SUB\n");
		free(key);
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
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//없을시 에러
		printf("ERROR : NO LEFT_PAREN FOR MUL\n");
		return;
	}

	int cnt = 0;	//나온 원소의 수를 카운트 하기 위한 변수
	bool is_ident = false;	//ident가 나오는지 기록하는 변수
	char *key = NULL;	//ident가 나왔을때 그 것을 dict의 key로 넣기 위한 변수
	float sum = 1;
	int type = cur_node->value.type;	//함수 전체의 리턴 타입을 위한 변수
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
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
				free(key);
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
			free(key);
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR MUL\n");
			free(key);
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR MUL\n");
		free(key);
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
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//없을시 에러
		printf("ERROR : NO LEFT_PAREN FOR DIV\n");
		return;
	}

	int cnt = 0;	//나온 원소의 수를 카운트 하기 위한 변수
	bool is_ident = false;	//ident가 나오는지 기록하는 변수
	char *key = NULL;	//ident가 나왔을때 그 것을 dict의 key로 넣기 위한 변수
	float sum = 0;
	int type = cur_node->value.type;	//함수 전체의 리턴 타입을 위한 변수
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
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
				free(key);
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
			free(key);
			return;
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR DIV\n");
			free(key);
			return;
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR DIV\n");
		free(key);
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
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
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
		cur_node = cur_node->next;
		res = fn_make_list();
		insert_dict_node(dict, symbol, &res);
	}
	else if (cur_node->value.type == LEFT_PAREN) {	// 괄호가 나올 경우 처리함
		res = call_fn();	//리턴값이 있는 함수의 경우
		if (res.type == INT || res.type == FLOAT || res.type == STRING || res.type == T_LIST || res.type == BOOLEAN || res.type == NIL) {
			insert_dict_node(dict, symbol, &res);
		}
		else {
			printf("ERROR : TYPE ERROR FOR SETQ\n");
			free(symbol);
			return;
		}
	}
	else if (cur_node->value.type == IDENT) {	//a=b의 경우 b의 값을 a에 대입 하는 느낌
		res = get_dict_obj(dict, cur_node->value.t_string);	//dict에서 먼저 값을 가져옴
		insert_dict_node(dict, symbol, &res);
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
			free(symbol);
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
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return;
	}

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
				LIST_NODE* tmp_node = cur_node->next;
				if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
					//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
					tmp = fn_make_list();
				}
				else {
					//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
					tmp = call_fn();
				}
			}
			else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
				tmp = cur_node->value;
				cur_node = cur_node->next;
			}
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return;
		}
		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return;
		}
		cnt++;
	}
	printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	free_list(tmp_list);
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		return;
	}
}

T_OBJ fn_car() {

}

T_OBJ fn_cdr() {

}

T_OBJ fn_nth() {

}

T_OBJ fn_make_list() {
	if (cur_node->value.type == LEFT_PAREN) {	//왼쪽 괄호 확인
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR MAKE_LIST\n");
		return;
	}

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//리스트를 생성하기 위한 임시 리스트
	while (cur_node->value.type != RIGHT_PAREN) {	//오른쪽 괄호가 나오기 전까지 반복
		T_OBJ tmp;
		if (cur_node->value.type == LEFT_PAREN) {	//괄호가 올 경우 처리함
			LIST_NODE* tmp_node = cur_node->next;
			if (tmp_node->value.type == INT || tmp_node->value.type == FLOAT || tmp_node->value.type == STRING || tmp_node->value.type == BOOLEAN) {
				//괄호 뒤의 토큰이 함수가 아니면 make_list를 호출해준다.
				tmp = fn_make_list();
			}
			else {
				//괄호 뒤의 토큰이 함수라면 call_fn을 통해서 함수를 호출해준다.
				tmp = call_fn();
			}
		}
		else {	//괄호가 없을 경우 현재 노드의 value를 tmp에 할당
			tmp = cur_node->value;
			cur_node = cur_node->next;
		}

		if (tmp.type == IDENT) {	//IDENT의 경우 STRING으로 취급해서 처리
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//이 경우는 그냥 할당
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR MAKE_LIST\n");
			free_list(tmp_list);
			return;
		}
		cnt++;
	}
	printf("make_list size : %d , %d\n", tmp_list->list_size, cnt);	//디버깅용
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	if (cnt == 0) {	//인자가 0개면 길이가 0인 리스트를 반환한다.
		return head;
	}
	//아니라면 임시로 만든 리스트의 값을 이용해서 리스트를 생성한다.
	LIST_NODE* tmp_node = tmp_list->head;
	while (tmp_node != NULL) {
		if (head.t_list_value == NULL) {
			head.t_list_value = &(tmp_node->value);
		}
		else {
			T_OBJ* tmp = malloc(sizeof(T_OBJ));
			tmp->type = T_LIST;
			tmp->t_int = cnt;
			tmp->t_list_value = &(tmp_node->value);
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	free_list(tmp_list);
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR MAKE_LIST\n");
		return;
	}
}