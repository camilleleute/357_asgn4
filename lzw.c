#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"

int main(int argc, char* argv[]) {
	FILE *in, *out;
	if (argc != 4 || ((strcmp(argv[1], "-c") != 0) && (strcmp(argv[1], "-x") != 0))) {	
		fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(argv[1], "-c") == 0) {
		in = fopen(argv[2], "r");
		if (in == NULL){
                	perror("./lzw");
                	exit(EXIT_FAILURE);
        	}
		out = fopen(argv[3], "wb");
		encode(in, out);	
	}

	if (strcmp(argv[1], "-x") == 0) {
		in = fopen(argv[2], "rb");
                if (in == NULL){
                	perror("./lzw");
                	exit(EXIT_FAILURE);
        	}
		out = fopen(argv[3], "w");
                decode(in, out);
        }
	fclose(in);
	fclose(out);
	return EXIT_SUCCESS;
}
