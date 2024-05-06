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
	int j = 1, k=0, i = 0;
	void *prevval, *val;
	Dict *dct = dctcreate();
	substr[0] = '\0';
	while ((c = fgetc(input)) != EOF) {
		printf("new char ---------------------------------\n");
		void *num;
		num =  dctget(dct, &c);
		if (num == NULL) {
			num = (void *)(long)c;
			dctinsert(dct, &c, (void*)(long)num);
			printf("inserted char %c with val %p\n", c, num);
		}
		int length = strlen(substr);
		substr[length] = c;
		substr[length+1] = '\0';

		printf("substr is: %s\n", substr);
		val = dctget(dct, substr);
		printf("Value of substr before if: %p\n", val);
   		if (val == NULL){
			//char prevsubstr[4096] = {'\0'};
			val = (void *)(long)(ASCIIMAX + (j++));
			dctinsert(dct, substr, (void*)(long)val);
			printf("inserted %s with val %p\n", substr, val);
			printf("for loop length: %d\n", length);
			printf("prev substr before for loop: %s\n", prevsubstr);
			for (i = 0; i < length; i++) {
				printf("char added to prevsubstr: %c\n", substr[i]);
				prevsubstr[i] = substr[i];
				
			}
			for (i = length; i < 4096; i++) {
				prevsubstr[i] = '\0';
			}


			if (strlen(prevsubstr) == 1) {
				prevval = (void *)(short)(prevsubstr[0]);
			} else {
				prevval = dctget(dct, prevsubstr);
			}
			printf("prevsubstr is in if: %s and value is: %p\n", prevsubstr, prevval);
			codes[k++%2] = (short)prevval;
			if (k%2 == 0) {
				writecodes(codes, output);
				codes[0] = -1;
				codes[1] = -1;
			}
			substr[0] = c;
			substr[1] = '\0';
		}
	}
	printf("out of while loop \n");
	printf("code at codes[1]: %d\n", codes[1]);	
	if (codes[0] != -1) {
		codes[1] =0x00A;
		writecodes(codes, output);
	} else {
		printf("hi\n");
		unsigned char byte = 0x00;
		unsigned char byteB = 0xA0;
		fwrite(&byte, sizeof(unsigned char), 1, output);
		fwrite(&byteB, sizeof(unsigned char), 1, output);
	}
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

