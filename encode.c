#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"

#define ASCIIMAX 0x07F



void encode(FILE *input, FILE *output) {
	char substr[4096], prevsubstr[4096], c;
	int short codes[2];
	int j = 1, k=0;
	void *prevval, *val;
	Dict *dct = dctcreate();
	substr[0] = '\0';
	prevsubstr[0] = '\0';
	while ((c = fgetc(input)) != EOF) {
		printf("new char ---------------------------------\n");
		void *num;
		num = (void *)(long)c;
		dctinsert(dct, &c, (void*)(long)num);
		printf("inserted %c with val %p\n", c, num);
		int length = strlen(substr);
		substr[length] = c;
		substr[length+1] = '\0';



		printf("substr is: %s\n", substr);
		val = dctget(dct, substr);
		 printf("Value of substr before if: %p\n", val);
                if (val == NULL){
			val = (void *)(long)(ASCIIMAX + (j++));
			dctinsert(dct, substr, (void*)(long)val);
			printf("inserted %s with val %p\n", substr, val);
			prevval =  dctget(dct, prevsubstr);
			codes[k++%2] = (short)prevval;
			if (k%2 == 0) {
			//	printf("going to write to file\n");
				writecodes(codes, output);
				codes[0] = -1;
				codes[1] = -1;
			}
			substr[0] = c;
			substr[1] = '\0';
		 strcpy(prevsubstr, substr);
		}
	}
	
	//codes[k%2] = 0x00A;
	//if (codes[1] == -1) {
	//	codes[1] = 0x000;
	//}
	//writecodes(codes, output);
	dctdestroy(dct);
	return;
}

void writecodes(int short codes[], FILE *output){
	unsigned char byte1, byte2, byte3;
	byte1 = (codes[0] >> 4) & 0xFF;
	byte2 = ((codes[0] << 4) | (codes[1] >> 8)) & 0xFF;
	byte3 = (codes[1] & 0xFF);

	fwrite(&byte1, sizeof(unsigned char), 1, output);
	fwrite(&byte2, sizeof(unsigned char), 1, output);
	fwrite(&byte3, sizeof(unsigned char), 1, output);

}	

