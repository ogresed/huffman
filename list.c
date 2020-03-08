#include "headers.h" 

unsigned Start_List_Size = 10;

void add(Node **head, Value* value) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = value;
    tmp->next = (*head);
    (*head) = tmp;
}

Value* pop(Node **head) {
    Node* prev = NULL;
    Value* val;
    if (head == NULL) {
        exit(-1);
    }
    prev = (*head);
    val = prev->value;
    (*head) = (*head)->next;
    free(prev);
    return val;
}

Value* create_value(void* value, Type type) {
	Value* ret_val = calloc(1, sizeof(Value));
	ret_val->type = type;
	switch(type) {
		case Int: {			
			ret_val->value.intValue = *((int*)value);
			break;
		}
		case Float: {
			ret_val->value.floatValue = *((float*)(value));
			break;
		}
		case Double: {
			ret_val->value.doubleValue = *((double*)(value));
			break;
		}
		case Ref: {
			ret_val->value.refValue = (value);
			break;
		}
		default: {
			ret_val->value.intValue = *((int*)value);
			break;
		}
	}
	return ret_val;
}