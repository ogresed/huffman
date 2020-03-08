//codding

typedef struct TreeNode {
	unsigned char symbol;
	int number;
	struct TreeNode* left;
	struct TreeNode* right;
} TreeNode;

typedef struct Code {
	unsigned long long int code;
	int lenght;
	unsigned char symbol;
} Code;

void codding(FILE* input, FILE* output);
int comp (const void* i, const void* j);
void build_tree(TreeNode** nodes, int number_of_used_symbols);
void deep_went(TreeNode* node, Code* codes, unsigned long long int code, int lenght);
Code* create_codes(TreeNode** nodes);
void writing(FILE* input, BitWriter* writer, Code* codes);
void  write_tree(TreeNode* tree, BitWriter* writer);
void write_symbols(TreeNode* tree, FILE* file);
//decodding
void decodding(FILE* input, FILE* output);
TreeNode* create_tree(BitReader* reader, int number_s, char* symbols);