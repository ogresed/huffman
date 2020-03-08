typedef enum Type {
	Int, LLong, Float, Double, Ref
}
Type;

typedef struct Value
{
	union
	{
		int intValue;
		long long llValue;
		float floatValue;
		double doubleValue;
		void* refValue;
	} value;
	Type type;
} Value;

typedef struct Node {
	struct Node* next;
	Value* value;
} Node;

void add(Node** head, Value* value);
Value* create_value(void* value, Type type);
Value* pop(Node **head);
