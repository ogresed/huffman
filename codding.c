#include "headers.h"
//invert sort order
int comp(const void* i, const void* j)
{
	TreeNode* arg1 = *((TreeNode**)i);
	TreeNode* arg2 = *((TreeNode**)j);

	return arg2->number - arg1->number;
}

void codding(FILE* input, FILE* output) {
	int SIZE_BUF = 512;
	unsigned char buf[512] = "";
	int readed = 0;

	TreeNode** nodes = (TreeNode**)malloc(256 * sizeof(TreeNode*));
	for (int i = 0; i < 256; i++) {
		nodes[i] = (TreeNode*)calloc(1, sizeof(TreeNode));
		nodes[i]->symbol = i;
	}

	while ((readed = fread(buf, sizeof(unsigned char), SIZE_BUF, input)) > 0) {
		for (int i = 0; i < readed; i++) {
			nodes[buf[i]]->number++;
		}
	}
	
	qsort(nodes, 256, sizeof(TreeNode*), comp);

	int i = 0;
	for(i; i < 256; i++) {
		if(!nodes[i]->number)
			break;
	}
	int number_of_used_symbols = i;
	
	printf("number of u s %d\n", i);
	
	build_tree(nodes, number_of_used_symbols);
	
	Code* codes = create_codes(nodes);
	
	long size_of_start_file = ftell(input);
	fseek(input, 0, SEEK_SET);
	
	//writing

		//write number of used symbols
	unsigned char used_symb_char = (unsigned char)(number_of_used_symbols);
	fwrite(&used_symb_char, 1, 1, output);
		//write used symbols
	write_symbols(*nodes, output);
		//write tree
	BitWriter* writer = create_bit_writer(output);
	write_tree(*nodes, writer);
		//write data
	writing(input, writer, codes);	
	//close_bit_writer(writer);
	//print quality info
	long soof = ftell(output);
	printf("compressed: %.3f\n", (soof / (size_of_start_file * 1.0)));
}

void write_symbols(TreeNode* tree, FILE* file) {
	if(tree->left) {
		write_symbols(tree->left, file);
	}
	if(tree->right) {
		write_symbols(tree->right, file);
	}
	if(!tree->right && ! tree->left) {
		fprintf(file, "%c", tree->symbol);
	}
}

void  write_tree(TreeNode* tree, BitWriter* writer) {
	if(tree->left) {
		write_bit(writer, 0ULL);
		write_tree(tree->left, writer);
	}
	if(tree->right) {
		write_bit(writer, 1ULL);
		write_tree(tree->right, writer);
	}
}

void writing(FILE* input, BitWriter* writer, Code* codes) {
	printf("into writting\n");
	int SIZE_BUF = 512;
	unsigned char buf[512] = "";
	int readed = 0;
	while ((readed = fread(buf, sizeof(unsigned char), SIZE_BUF, input)) > 0) {
	//printf("into writting readed: %d\n", readed);
		for (int i = 0; i < readed; i++) {
			Code code = codes[buf[i]];
			unsigned long long int masc = 1ULL << code.lenght - 1;
			for(int j = 0; j < code.lenght; j++) {
				unsigned long long int bit = code.code & masc;
				//printf("%d ", bit);
				write_bit(writer, bit);
				masc >>= 1;
			}
		}
	}
	close_bit_writer(writer);
}

Code* create_codes(TreeNode** nodes) {
	Code* codes = (Code*)calloc(256, sizeof(Code));
	deep_went(*nodes, codes, 0, 0);
		
	return codes;
}

void deep_went(TreeNode* node, Code* codes, unsigned long long int code, int lenght) {
	if(node->left) {
		unsigned long long int new_code = (code << 1);
		deep_went(node->left, codes, new_code, lenght + 1);
	}
	if(node->right) {
		unsigned long long int new_code = (code << 1) | 1ULL;
		deep_went(node->right, codes, new_code, lenght + 1);
	}
	if(!node->right && !node->left) {
		//printf("into writting code lenght: %d\n", lenght);
		codes[node->symbol].lenght = lenght;
		codes[node->symbol].code = code;
		codes[node->symbol].symbol = node->symbol;
	}
}

void build_tree(TreeNode** nodes, int number_of_used_symbols) {
	int mass_size = number_of_used_symbols;
	
	for(int i = 0; i < number_of_used_symbols - 1; i++) {
		TreeNode* new_node = calloc(1, sizeof(TreeNode));
		new_node->number = nodes[mass_size - 1]->number + nodes[mass_size - 2]->number;
		new_node->left = nodes[mass_size - 1];
		new_node->right = nodes[mass_size - 2];
		nodes[mass_size - 2] = new_node;
		mass_size--;
		qsort(nodes, mass_size, sizeof(TreeNode*), comp);
	}
}
