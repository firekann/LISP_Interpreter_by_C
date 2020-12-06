#define _CRT_SECURE_NO_WARNINGS
#include "OP~NTH.h"
#include "type.h"
#include "lexer.h"
#include "util.h"
#include <stdio.h>

T_OBJ fn_add() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//������ ����
		printf("ERROR : NO LEFT_PAREN FOR ADD\n");
		return return_false();
	}

	int cnt = 0;	//���� ������ ���� ī��Ʈ �ϱ� ���� ����
	bool is_ident = false;	//ident�� �������� ����ϴ� ����
	char *key = NULL;	//ident�� �������� �� ���� dict�� key�� �ֱ� ���� ����
	float sum = 0;
	int type = cur_node->value.type;	//�Լ� ��ü�� ���� Ÿ���� ���� ����
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		bool flag = true;
		if (cur_node->value.type == INT) {
			sum += (float)cur_node->value.t_int;
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			sum += (float)cur_node->value.t_float;
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// ��ȣ�� ���� ��� ó����
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
				return return_false();
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//add�� ù��° ���ڰ� IDENT�� ��� �� IDENT�� ���� �ٲ���
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
				return return_false();
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR ADD\n");
			free(key);
			return return_false();
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR ADD\n");
			free(key);
			return return_false();
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR ADD\n");
		free(key);
		return return_false();
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR ADD\n");
		return return_false();
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	tmp.t_bool = true;
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
	//printf("ADD : %s, type : %d\n", tmp.t_string, tmp.type);	//������
	return tmp;
}

T_OBJ fn_sub() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//������ ����
		printf("ERROR : NO LEFT_PAREN FOR SUB\n");
		return return_false();
	}

	int cnt = 0;	//���� ������ ���� ī��Ʈ �ϱ� ���� ����
	bool is_ident = false;	//ident�� �������� ����ϴ� ����
	char *key = NULL;	//ident�� �������� �� ���� dict�� key�� �ֱ� ���� ����
	float sum = 0;
	int type = cur_node->value.type;	//�Լ� ��ü�� ���� Ÿ���� ���� ����
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		bool flag = true;
		if (cur_node->value.type == INT) {
			if (cnt == 0) {	//sub�� ��� ó�� �ʱⰪ�� �����ؾ���
				type = INT;
				sum = (float)cur_node->value.t_int;
			}
			else {
				sum -= (float)cur_node->value.t_int;
			}
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			if (cnt == 0) {	//sub�� ��� ó�� �ʱⰪ�� �����ؾ���
				sum = (float)cur_node->value.t_float;
			}
			else {
				sum -= (float)cur_node->value.t_float;
			}
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// ��ȣ�� ���� ��� ó����
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {	//sub�� ��� ó�� �ʱⰪ�� �����ؾ���
					type = INT;
					sum = (float)tmp.t_int;
				}
				else {
					sum -= (float)tmp.t_int;
				}
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				if (cnt == 0) {	//sub�� ��� ó�� �ʱⰪ�� �����ؾ���
					sum = (float)tmp.t_float;
				}
				else {
					sum -= (float)tmp.t_float;
				}
			}
			else {
				printf("ERROR : TYPE ERROR FOR SUB\n");
				free(key);
				return return_false();
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//sub�� ù��° ���ڰ� IDENT�� ��� �� IDENT�� ���� �ٲ���
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
				return return_false();
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR SUB\n");
			free(key);
			return return_false();
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR SUB\n");
			free(key);
			return return_false();
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR SUB\n");
		free(key);
		return return_false();
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR SUB\n");
		return return_false();
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	tmp.t_bool = true;
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
	//printf("SUB : %s, type : %d\n", tmp.t_string, tmp.type);	//������
	return tmp;
}

T_OBJ fn_mul() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//������ ����
		printf("ERROR : NO LEFT_PAREN FOR MUL\n");
		return return_false();
	}

	int cnt = 0;	//���� ������ ���� ī��Ʈ �ϱ� ���� ����
	bool is_ident = false;	//ident�� �������� ����ϴ� ����
	char *key = NULL;	//ident�� �������� �� ���� dict�� key�� �ֱ� ���� ����
	float sum = 1;
	int type = cur_node->value.type;	//�Լ� ��ü�� ���� Ÿ���� ���� ����
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		bool flag = true;
		if (cur_node->value.type == INT) {
			sum *= (float)cur_node->value.t_int;
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			sum *= (float)cur_node->value.t_float;
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// ��ȣ�� ���� ��� ó����
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
				return return_false();
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//mul�� ù��° ���ڰ� IDENT�� ��� �� IDENT�� ���� �ٲ���
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
				return return_false();
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR MUL\n");
			free(key);
			return return_false();
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR MUL\n");
			free(key);
			return return_false();
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR MUL\n");
		free(key);
		return return_false();
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR MUL\n");
		return return_false();
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	tmp.t_bool = true;
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
	//printf("MUL : %s, type : %d\n", tmp.t_string, tmp.type);	//������
	return tmp;
}

T_OBJ fn_div() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {	//������ ����
		printf("ERROR : NO LEFT_PAREN FOR DIV\n");
		return return_false();
	}

	int cnt = 0;	//���� ������ ���� ī��Ʈ �ϱ� ���� ����
	bool is_ident = false;	//ident�� �������� ����ϴ� ����
	char *key = NULL;	//ident�� �������� �� ���� dict�� key�� �ֱ� ���� ����
	float sum = 0;
	int type = cur_node->value.type;	//�Լ� ��ü�� ���� Ÿ���� ���� ����
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		bool flag = true;
		if (cur_node->value.type == INT) {
			if (cnt == 0) {	//div�� ��� ó�� �ʱⰪ�� �����ؾ���
				sum = (float)cur_node->value.t_int;
			}
			else {
				sum /= (float)cur_node->value.t_int;
			}
		}
		else if (cur_node->value.type == FLOAT) {
			type = FLOAT;
			if (cnt == 0) {	//div�� ��� ó�� �ʱⰪ�� �����ؾ���
				sum = (float)cur_node->value.t_float;
			}
			else {
				sum /= (float)cur_node->value.t_float;
			}
		}
		else if (cur_node->value.type == LEFT_PAREN) {	// ��ȣ�� ���� ��� ó����
			flag = false;
			T_OBJ tmp = call_fn();
			if (tmp.type == INT) {
				if (cnt == 0) {	//div�� ��� ó�� �ʱⰪ�� �����ؾ���
					type = INT;
					sum = (float)tmp.t_int;
				}
				else {
					sum /= (float)tmp.t_int;
				}
			}
			else if (tmp.type == FLOAT) {
				type = FLOAT;
				if (cnt == 0) {	//div�� ��� ó�� �ʱⰪ�� �����ؾ���
					sum = (float)tmp.t_float;
				}
				else {
					sum /= (float)tmp.t_float;
				}
			}
			else {
				printf("ERROR : TYPE ERROR FOR DIV\n");
				free(key);
				return return_false();
			}
		}
		else if (cnt == 0 && cur_node->value.type == IDENT) {	//div�� ù��° ���ڰ� IDENT�� ��� �� IDENT�� ���� �ٲ���
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
				return return_false();
			}
			key = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
			strcpy(key, cur_node->value.t_string);
		}
		else {
			printf("ERROR : TYPE ERROR FOR DIV\n");
			free(key);
			return return_false();
		}
		if (cur_node->next == NULL) {
			printf("ERROR : SYNTAX ERROR FOR DIV\n");
			free(key);
			return return_false();
		}
		if (flag) {
			cur_node = cur_node->next;
		}
		cnt++;
	}
	if (cnt == 0) {
		printf("ERROR : SYNTAX ERROR FOR DIV\n");
		free(key);
		return return_false();
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR DIV\n");
		return return_false();
	}
	T_OBJ tmp;
	tmp.type = type;
	tmp.next = NULL;
	tmp.t_bool = true;
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
	//printf("DIV : %s, type : %d\n", tmp.t_string, tmp.type);	//������
	return tmp;
}

T_OBJ fn_setq() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR SETQ\n");
		return return_false();
	}

	char* symbol;	//���� ��

	if (cur_node->value.type == IDENT) {
		symbol = (char*)malloc(sizeof(char)*cur_node->value.lexed_len + 1);
		strcpy(symbol, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO IDENTIFIER FOR SETQ\n");
		return return_false();
	}

	T_OBJ res;
	if (cur_node->value.type == SQUOTE) {	//�� ���� ����Ʈ �Է��� ����
		cur_node = cur_node->next;
		if (cur_node->value.type == LEFT_PAREN) {
			res = fn_make_list();
		}
		else if (cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == STRING || cur_node->value.type == BOOLEAN) {
			res = cur_node->value;
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == IDENT) {
			res = cur_node->value;
			res.type = STRING;
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR SETQ\n");
			return return_false();
		}
	}
	else if (cur_node->value.type == LEFT_PAREN) {	// ��ȣ�� ���� ��� ó����
		res = call_fn();	//���ϰ��� �ִ� �Լ��� ���
		if(!(res.type == INT || res.type == FLOAT || res.type == STRING || res.type == T_LIST || res.type == BOOLEAN)) {
			printf("ERROR : TYPE ERROR FOR SETQ\n");
			free(symbol);
			return return_false();
		}
	}
	else if (cur_node->value.type == IDENT) {	//a=b�� ��� b�� ���� a�� ���� �ϴ� ����
		res = get_dict_obj(dict, cur_node->value.t_string);	//dict���� ���� ���� ������
		cur_node = cur_node->next;
	}
	else {
		// ���� �Ǽ� ���ڿ��� ���
		if (cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == STRING || cur_node->value.type == BOOLEAN) {
			res = cur_node->value;
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : VALUE ERROR FOR SETQ\n");
			free(symbol);
			return return_false();
		}
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		res.t_bool = true;
		insert_dict_node(dict, symbol, &res);
		free(symbol);
		return res;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR SETQ\n");
		return return_false();
	}
}

T_OBJ fn_list() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR LIST\n");
		return return_false();
	}

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//����Ʈ�� �����ϱ� ���� �ӽ� ����Ʈ
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		T_OBJ tmp;
		if (cur_node->value.type == SQUOTE) {
			cur_node = cur_node->next;
			if (cur_node->value.type == LEFT_PAREN) {	//��ȣ�� �� ��� ó����
				tmp = fn_make_list();
			}
			else {	//��ȣ�� ���� ��� ���� ����� value�� tmp�� �Ҵ�
				tmp = cur_node->value;
				cur_node = cur_node->next;
				if (!(tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST)) {
					tmp.type = STRING;
				}
			}
		}
		else if (cur_node->value.type == LEFT_PAREN) {	//�� ��� �Լ��� ȣ���ؼ� ó����.
			tmp = call_fn();
		}
		else if (cur_node->value.type == INT || cur_node->value.type == FLOAT || cur_node->value.type == STRING || cur_node->value.type == BOOLEAN) {
			tmp = cur_node->value;
			cur_node = cur_node->next;
		}
		else if (cur_node->value.type == IDENT) {
			tmp = get_dict_obj(dict, cur_node->value.t_string);
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		if (tmp.type == IDENT) {	//IDENT�� ��� STRING���� ����ؼ� ó��
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST) {	//�� ���� �׳� �Ҵ�
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}
	//printf("list size : %d , %d\n", tmp_list->list_size, cnt);	//������
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	head.t_bool = true;
	if (cnt == 0) {	//���ڰ� 0���� ���̰� 0�� ����Ʈ�� ��ȯ�Ѵ�.
		return head;
	}
	//�ƴ϶�� �ӽ÷� ���� ����Ʈ�� ���� �̿��ؼ� ����Ʈ�� �����Ѵ�.
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
			tmp->t_bool = true;
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}

T_OBJ fn_car(bool is_first) {
	if (is_first) {	//ó�� ������ �� ���� ��ȣ�� �ִ� car�� ���
		if (cur_node->value.type == LEFT_PAREN) {
			left_paren_Count++;
			cur_node = cur_node->next;
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : NO LEFT_PAREN FOR CAR\n");
			return return_false();
		}
	}
	else {
		if (cur_node->value.type == LEFT_PAREN) {
			left_paren_Count++;
			cur_node = cur_node->next;
			cur_node = cur_node->next;
		}
		else {
			cur_node = cur_node->next;
		}
	}

	T_OBJ tmp;
	if (cur_node->value.type == SQUOTE) {
		cur_node = cur_node->next;
		if (cur_node->value.type == LEFT_PAREN) {	//��ȣ�� �� ��� ó����
			tmp = fn_make_list();
		}
		else {	//��ȣ�� ���� ��� ����Ʈ�� �ƴϹǷ� ù��° ���Ұ� ����
			printf("ERROR : TYPE ERROR FOR CAR\n");
			return return_false();
		}
	}
	else if (cur_node->value.type == LEFT_PAREN) {	//�� ��� �Լ��� ȣ���ؼ� ó����.
		tmp = call_fn();
	}
	else if (cur_node->value.type == IDENT) {
		tmp = get_dict_obj(dict, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else if (cur_node->value.type == CAR) {
		tmp = fn_car(false);
	}
	else if (cur_node->value.type == CDR) {
		tmp = fn_cdr(false);
	}
	else {
		printf("ERROR : TYPE ERROR FOR CAR\n");
		return return_false();
	}

	if (tmp.type == T_LIST) {
		if (tmp.t_int == 0) {	//����Ʈ�� ���Ұ� 0���� ��� ����
			printf("ERROR : NO ITEM IN THE LIST ERROR FOR CAR\n");
			return return_false();
		}
		else {	//����Ʈ�� ���Ұ� ���� ��� �� ó�� �Ϳ��� ���Ҹ� ������
			if (!is_first) {
				T_OBJ t = *((T_OBJ*)(tmp.t_list_value));
				t.t_bool = true;
				return t;
			}
			else if (cur_node->value.type == RIGHT_PAREN) {
				right_paren_Count++;
				cur_node = cur_node->next;
				T_OBJ t = *((T_OBJ*)(tmp.t_list_value));
				t.t_bool = true;
				return t;
			}
			else {
				printf("ERROR : NO RIGHT_PAREN FOR CAR\n");
				return return_false();
			}
		}
	}
	else {
		printf("ERROR : TYPE ERROR FOR CAR\n");
		return return_false();
	}
}

T_OBJ fn_cdr(bool is_first) {
	if (is_first) {	//ó�� ������ �� ���� ��ȣ�� �ִ� cdr�� ���
		if (cur_node->value.type == LEFT_PAREN) {
			left_paren_Count++;
			cur_node = cur_node->next;
			cur_node = cur_node->next;
		}
		else {
			printf("ERROR : NO LEFT_PAREN FOR CDR\n");
			return return_false();
		}
	}
	else {
		if (cur_node->value.type == LEFT_PAREN) {
			left_paren_Count++;
			cur_node = cur_node->next;
			cur_node = cur_node->next;
		}
		else {
			cur_node = cur_node->next;
		}
	}

	T_OBJ tmp;
	if (cur_node->value.type == SQUOTE) {
		cur_node = cur_node->next;
		if (cur_node->value.type == LEFT_PAREN) {	//��ȣ�� �� ��� ó����
			tmp = fn_make_list();
		}
		else {	//��ȣ�� ���� ��� ����Ʈ�� �ƴϹǷ� ù��° ���Ұ� ����
			printf("ERROR : TYPE ERROR FOR CDR\n");
			return return_false();
		}
	}
	else if (cur_node->value.type == LEFT_PAREN) {	//�� ��� �Լ��� ȣ���ؼ� ó����.
		tmp = call_fn();
	}
	else if (cur_node->value.type == IDENT) {
		tmp = get_dict_obj(dict, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else if (cur_node->value.type == CAR) {
		tmp = fn_car(false);
	}
	else if (cur_node->value.type == CDR) {
		tmp = fn_cdr(false);
	}
	else {
		printf("ERROR : TYPE ERROR FOR CDR\n");
		return return_false();
	}

	if (tmp.type == T_LIST) {
		if (tmp.t_int < 2) {	//����Ʈ�� ���Ұ� 0���� ��� ����
			printf("ERROR : NO ITEM IN THE LIST ERROR FOR CDR\n");
			return return_false();
		}
		else {	//����Ʈ�� ���Ұ� 2�� �̻��� ��� ù ���Ҹ� ������ ������ ���Ҹ� ��ȯ�Ѵ�.
			c_LIST* tmp_list = initialize_list();
			int list_size = tmp.t_int - 1;
			T_OBJ* node = tmp.next;
			while (node != NULL) {	//������ ���Ҹ� ����Ʈ�� �ִ´�.
				T_OBJ tmp_node = *node;
				tmp_node.t_int = list_size;
				insert_list_node(tmp_list, &tmp_node);
				node = node->next;
			}
			T_OBJ head;
			head.t_bool = true;
			T_OBJ* pre_obj = &head;
			head.next = head.t_list_value = NULL;
			LIST_NODE* tmp_node = tmp_list->head;
			while (tmp_node != NULL) {	//����Ʈ�� ���ҵ�� obj����Ʈ�� �����Ѵ�.
				tmp_node->value.t_bool = true;
				if (head.t_list_value == NULL) {
					head = tmp_node->value;
				}
				else {
					pre_obj->next = &(tmp_node->value);
					pre_obj = &(tmp_node->value);
				}
				tmp_node = tmp_node->next;
			}
			free(tmp_list);
			if (!is_first) {
				return head;
			}
			else if (cur_node->value.type == RIGHT_PAREN) {
				right_paren_Count++;
				cur_node = cur_node->next;
				return head;
			}
			else {
				printf("ERROR : NO RIGHT_PAREN FOR CDR\n");
				return return_false();
			}
		}
	}
	else {
		printf("ERROR : TYPE ERROR FOR CDR\n");
		return return_false();
	}
}

T_OBJ fn_nth() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR NTH\n");
		return return_false();
	}

	T_OBJ tmp;
	int nth;
	if (cur_node->value.type == INT) {
		nth = cur_node->value.t_int;
		cur_node = cur_node->next;
	}
	else if (cur_node->value.type == LEFT_PAREN) {	//�� ��� �Լ��� ȣ���ؼ� ó����.
		tmp = call_fn();
		if (tmp.type != INT) {
			printf("ERROR : TYPE ERROR FOR NTH\n");
			return return_false();
		}
		nth = tmp.t_int;
	}
	else {
		printf("ERROR : TYPE ERROR FOR NTH\n");
		return return_false();
	}

	if (cur_node->value.type == SQUOTE) {
		cur_node = cur_node->next;
		if (cur_node->value.type == LEFT_PAREN) {	//��ȣ�� �� ��� ó����
			tmp = fn_make_list();
		}
		else {	//��ȣ�� ���� ��� ����Ʈ�� �ƴϹǷ� N��° ���Ұ� ����
			printf("ERROR : TYPE ERROR FOR NTH\n");
			return return_false();
		}
	}
	else if (cur_node->value.type == LEFT_PAREN) {	//�� ��� �Լ��� ȣ���ؼ� ó����.
		tmp = call_fn();
	}
	else if (cur_node->value.type == IDENT) {
		tmp = get_dict_obj(dict, cur_node->value.t_string);
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : TYPE ERROR FOR NTH\n");
		return return_false();
	}

	if (tmp.type == T_LIST) {
		if (tmp.t_int == 0) {	//����Ʈ�� ���Ұ� 0���� ��� ����
			printf("ERROR : NO ITEM IN THE LIST ERROR FOR NTH\n");
			return return_false();
		}
		else {	//����Ʈ�� ���Ұ� ���� ��� �� ó�� �Ϳ��� ���Ҹ� ������
			if (cur_node->value.type == RIGHT_PAREN) {
				for (; nth--;) {
					tmp = cdr(tmp);
				}
				return car(tmp);
			}
			else {
				printf("ERROR : NO RIGHT_PAREN FOR NTH\n");
				return return_false();
			}
		}
	}
	else {
		printf("ERROR : TYPE ERROR FOR NTH\n");
		return return_false();
	}
}

T_OBJ fn_make_list() {
	if (cur_node->value.type == LEFT_PAREN) {	//���� ��ȣ Ȯ��
		left_paren_Count++;
		cur_node = cur_node->next;
	}
	else {
		printf("ERROR : NO LEFT_PAREN FOR MAKE_LIST\n");
		return return_false();
	}

	int cnt = 0;
	c_LIST* tmp_list = initialize_list();	//����Ʈ�� �����ϱ� ���� �ӽ� ����Ʈ
	while (cur_node->value.type != RIGHT_PAREN) {	//������ ��ȣ�� ������ ������ �ݺ�
		T_OBJ tmp;
		if (cur_node->value.type == LEFT_PAREN) {	//��ȣ�� �� ��� ó����
			tmp = fn_make_list();
		}
		else {	//��ȣ�� ���� ��� ���� ����� value�� tmp�� �Ҵ�
			tmp = cur_node->value;
			cur_node = cur_node->next;
		}

		if (tmp.type == IDENT) {	//IDENT�� ��� STRING���� ����ؼ� ó��
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type == INT || tmp.type == FLOAT || tmp.type == STRING || tmp.type == BOOLEAN || tmp.type == T_LIST || tmp.type == SQUOTE) {	//�� ���� �׳� �Ҵ�
			insert_list_node(tmp_list, &tmp);
		}
		else if (tmp.type != LEFT_PAREN && tmp.type != RIGHT_PAREN) {
			tmp.type = STRING;
			insert_list_node(tmp_list, &tmp);
		}
		else {
			printf("ERROR : TYPE ERROR FOR MAKE_LIST\n");
			free_list(tmp_list);
			return return_false();
		}
		cnt++;
	}
	//printf("make_list size : %d , %d\n", tmp_list->list_size, cnt);	//������
	T_OBJ head;
	T_OBJ* pre_obj = &head;
	head.type = T_LIST;
	head.t_int = cnt;
	head.next = head.t_list_value = NULL;
	head.t_bool = true;
	if (cnt == 0) {	//���ڰ� 0���� ���̰� 0�� ����Ʈ�� ��ȯ�Ѵ�.
		free_list(tmp_list);
		return head;
	}
	//�ƴ϶�� �ӽ÷� ���� ����Ʈ�� ���� �̿��ؼ� ����Ʈ�� �����Ѵ�.
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
			tmp->next = NULL;
			tmp->t_bool = true;
			pre_obj->next = tmp;
			pre_obj = tmp;
		}
		tmp_node = tmp_node->next;
	}
	if (cur_node->value.type == RIGHT_PAREN) {
		right_paren_Count++;
		cur_node = cur_node->next;
		free(tmp_list);
		return head;
	}
	else {
		printf("ERROR : NO RIGHT_PAREN FOR MAKE_LIST\n");
		free_list(tmp_list);
		return return_false();
	}
}

T_OBJ car(T_OBJ tmp) {
	if (tmp.type == T_LIST) {
		if (tmp.t_int == 0) {	//����Ʈ�� ���Ұ� 0���� ��� ����
			printf("ERROR : NO ITEM IN THE LIST ERROR FOR CAR(�Լ�)\n");
			return return_false();
		}
		else {	//����Ʈ�� ���Ұ� ���� ��� �� ó�� �Ϳ��� ���Ҹ� ������
			T_OBJ t = *((T_OBJ*)(tmp.t_list_value));
			t.t_bool = true;
			return t;
		}
	}
	else {
		printf("ERROR : TYPE ERROR FOR CAR(�Լ�)\n");
		return return_false();
	}
}

T_OBJ cdr(T_OBJ tmp) {
	if (tmp.type == T_LIST) {
		if (tmp.t_int < 2) {	//����Ʈ�� ���Ұ� 0���� ��� ����
			printf("ERROR : NO ITEM IN THE LIST ERROR FOR CDR(�Լ�)\n");
			return return_false();
		}
		else {	//����Ʈ�� ���Ұ� 2�� �̻��� ��� ù ���Ҹ� ������ ������ ���Ҹ� ��ȯ�Ѵ�.
			c_LIST* tmp_list = initialize_list();
			int list_size = tmp.t_int - 1;
			T_OBJ* node = tmp.next;
			while (node != NULL) {	//������ ���Ҹ� ����Ʈ�� �ִ´�.
				T_OBJ tmp_node = *node;
				tmp_node.t_int = list_size;
				insert_list_node(tmp_list, &tmp_node);
				node = node->next;
			}
			T_OBJ head;
			head.t_bool = true;
			T_OBJ* pre_obj = &head;
			head.next = head.t_list_value = NULL;
			LIST_NODE* tmp_node = tmp_list->head;
			while (tmp_node != NULL) {	//����Ʈ�� ���ҵ�� obj����Ʈ�� �����Ѵ�.
				tmp_node->value.t_bool = true;
				if (head.t_list_value == NULL) {
					head = tmp_node->value;
				}
				else {
					pre_obj->next = &(tmp_node->value);
					pre_obj = &(tmp_node->value);
				}
				tmp_node = tmp_node->next;
			}
			free(tmp_list);
			return head;
		}
	}
	else {
		printf("ERROR : TYPE ERROR FOR CDR(�Լ�)\n");
		return return_false();
	}
}