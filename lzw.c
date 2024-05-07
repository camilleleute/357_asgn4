#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"

int main(int argc, char* argv[]) {
	FILE *in, *out;
	if (argc != 4 || (strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-x") != 0)) {	
		fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
		exit(EXIT_FAILURE);
	}
	if ((in = fopen(argv[2], "r")) == NULL){
		fprintf(stderr,"./lzw: No such file or directory\n");
		fclose(in);
		exit(EXIT_FAILURE);
	}
		
	if (strcmp(argv[1], "-c") == 0) {
		fclose(in);
		in = fopen(argv[2], "r");
		out = fopen(argv[3], "wb");
		encode(in, out);	
	}

	if (strcmp(argv[1], "-x") == 0) {
		in = fopen(argv[2], "rb");
                out = fopen(argv[3], "w");
                //decode(in, out);
        }
	printf("and closing\n");
	fclose(in);
	fclose(out);
	return EXIT_SUCCESS;
}
