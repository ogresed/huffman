#include "headers.h" 

void decodding(FILE* input, FILE* output) {
	//get number of used symbols
	int number_s = 0;
	fread(&number_s, 1, 1, input);
	number_s = number_s == 0 ? 256 : number_s;	
	
	//get used symbols
	char* symbols = calloc(number_s, sizeof(char));
	fread(symbols, 1,  number_s, input);
	
	//get current position
	long current = ftell(input);
	
	//get size of file
	fseek(input, 0, SEEK_END);
	long size_input = ftell(input);
	fseek(input, current, SEEK_SET);
	
	//create service structures
	BitReader* reader = create_bit_reader(input);
	TreeNode * tree = create_tree(reader, number_s, symbols);
	//read file and create new file	
	TreeNode* head = tree;
	//printf("\n___\n");
	while(reader->state != EMPTY) {
		int bit = read_bit(reader);
		//printf("%d ", bit);
		if(bit) {
			head = head->right;
		}
		else {
			head = head->left;
		}
		if(!head->left && !head->right) {
			fwrite(&(head->symbol), 1, 1, output);
			//fprintf(output,"%c", head->symbol);
			head = tree;
		}
	}	
}

TreeNode * create_tree(BitReader* reader, int number_s, char* symbols) {
	int counter = 0;
	Node * list = 0;
	TreeNode* head = calloc(1, sizeof(TreeNode));
	TreeNode* ret = head;
	for(int i = 0; i < (number_s - 1) * 2; i++) {
		int bit = read_bit(reader);
		
		TreeNode* tmp = calloc(1, sizeof(TreeNode));
		if(bit) {
			head->symbol = symbols[counter++];
			Value* val = pop(&list); 	
			head = (TreeNode*)(val->value.refValue);
			head->right = tmp;
		} else {
			Value* tmp_val = create_value(head, Ref);
			add(&list, tmp_val);
			head->left = tmp;
		}
		head = tmp;
	}
	head->symbol = symbols[number_s -1];
	return ret;
}
