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
	insertAscii(dct);
	substr[0] = '\0';
	while ((c = fgetc(input)) != EOF) {
		printf("new char ---------------------------------\n");
		int length = strlen(substr);
		strcpy(prevsubstr, substr);
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
					
			prevval = dctget(dct, prevsubstr);
			
			printf("prevsubstr is in if: %s and value is: %p\n", prevsubstr, prevval);
			codes[k++%2] = (short)prevval;
			if (k%2 == 0) {
				writecodes(codes, output);
				codes[0] = -1;
				codes[1] = -1;
			}
			substr[0] = c;
			substr[1] = '\0';
			printf("substr is: %s\n", substr);
		}
	}
	printf("exited while loop\n");
	printf("in codes[0]: %d; in codes[1]: %d\n", codes[0], codes[1]);
	printf("substr: %s\n", substr);
	val = dctget(dct,substr);
        
        codes[k++%2] = (short)val;
	 printf("in codes[0]: %x; in codes[1]: %d\n", codes[0], codes[1]);
	if (codes[1] == -1) {
		unsigned char byteA, byteB;
		byteA = (codes[0] >> 4) & 0xFF;
		byteB = (codes[0] << 4) & 0xFF;
		fwrite(&byteA, sizeof(unsigned char), 1, output);
		fwrite(&byteB, sizeof(unsigned char), 1, output);
	} else {
		writecodes(codes, output);
	}
	dctdestroy(dct);
	return;
}

void insertAscii(Dict *dict) {
    	int i;
	for (i = 0; i < 128; i++) {
   		char key[2];
		sprintf(key, "%c", i); 
		dctinsert(dict, key, (void*)(long)i);
        }
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

