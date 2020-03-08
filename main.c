#include "headers.h"

const char* help = "usage: \n" \
	"<name of this app> <mode: -c to codding or -d to> <name of file to codding/decodding(input)> <name of final"\
" file(output)>\n";
const char* wrong_mode = "wrong mode\n";
const char* cod_suf = "-c";
const char* dec_suf = "-d";
const char* impossible_open_file = "impossible open file\n";

void main(int argc, char * argv[]) {
	//tests();
	if(argc < 4) {
		printf(help);
		return;
	}
	if(!strcmp(argv[1], cod_suf)) {
		FILE * input = fopen (argv[2], "rb");
		if(!input) {
			printf("\"%s\": %s", argv[2], impossible_open_file);
			return;
		}
		//===================================================
		/*
		char* nbuf = (char*)calloc(512, 1);
		int readed = 0;
		readed = fread(nbuf, sizeof(char), 511, input);
		printf("GEG\n");
		while (readed > 0) {
			printf("___\n%s\n", nbuf);
			nbuf[0] = nbuf[509];
			nbuf[1] = nbuf[510];
			readed = fread(nbuf + 2, sizeof(unsigned char), 509, input);
		}
		return;
		*/
		//===================================================
		FILE* output = fopen (argv[3], "wb");
		codding(input, output);
		fclose(input);
		fclose(output);
	} 
	else if(!strcmp(argv[1], dec_suf)) {
		//===================================================
		/*
	FILE* file = fopen(argv[2], "rb");
	unsigned char nbuf[512];
	int readed = 0;
	printf("GEG\n");
	
	while ((readed = fread(nbuf, sizeof(unsigned char), 512, file)) > 0) {
		for (int i = 0; i < readed; i++) {
			printf("%d ", nbuf[i]);
		}
	}
	printf("\n");
	return;
	*/
	//===================================================
		FILE * input = fopen (argv[2], "rb");
		if(!input) {
			printf("\"%s\": %s", argv[2], impossible_open_file);
			return;
		}
		FILE* output = fopen (argv[3], "wb");
		decodding(input, output);
		fclose(input);
		fclose(output);
	}
	else {
		printf(wrong_mode);
	}
}
