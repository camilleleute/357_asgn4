#include <stdio.h>
#include <stdlib.h>
#include "dict.h"
#include <string.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char* argv[]) {
	FILE *in, *out;
	if (argc != 4 || (strcmp(argv[1], "-c") != 0 && strcmp(argv[1], "-x") != 0)) {	
		fprintf(stderr, "usage: ./lzw (-c | -x) SOURCE DESTINATION\n");
		exit(EXIT_FAILURE);
	}
	iif (in = fopen(argv[2], "r") == NULL){
		printf("in: %s; out: %s\n", argv[2], argv[3]);
		fprintf(stderr,"./lzw: No such file or directory\n");
		exit(EXIT_FAILURE);
	}
		
	if (strcmp(argv[1], "-c") == 0) {
		in = fopen(argv[2], "r");
		out = fopen(argv[3], "wb");
		encode(in, out);	
	}

	if (strcmp(argv[1], "-x") == 0) {
		in = fopen(argv[2], "rb");
                out = fopen(argv[3], "w");
                decode(in, out);
        }

	return EXIT_SUCCESS;
}
