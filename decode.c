#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"
#define ASCIIMAX 0x7F 


void decode(FILE *input, FILE* output) {
	int i, j;
	size_t elements_read;
	Dict *dct = dctcreate();
	insertAsciiRev(dct);
	char codesubstr[4096], prevsubstr[4096];
	unsigned char bytes[3] = {-1};	
	short int codes[2];
	printf("hi/n");
	while ((elements_read = fread(bytes ,1, 3, input)) == 3) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			sprintf(codesubstr, "%d",  dctget(dct, codes[i]));
			//strcpy(codesubstr, dctget(dct, codes[i]));
			printf("code is: %x; associated string is: %s\n", codes[i], codesubstr);	
			if (codesubstr != NULL) {
				for (j = 0; codesubstr[j] != '\0'; j++) {
					fputc(codesubstr[j], output);
					//maybe handle fail fputs or EOF?
				}
				if (prevsubstr != NULL) {
					strcat(prevsubstr, codesubstr[0]);
					dctinsert(dct, codes[i], (void *)(long)prevsubstr);
					strcpy(prevsubstr, codesubstr);  
				}
			} else {
				strcat(prevsubstr, prevsubstr[0]); //i think this is wrong
				for (j = 0; prevsubstr[j] != '\0'; j++) {
                                	fputc(prevsubstr[j], output);
                                }
				dctinsert(dct, codes[i], (void*)(long)prevsubstr);
				
			}
			
				
		}	
	
	}
	if (elements_read == 2) {
		short int singlecode;
		singlecode = ((codes[0] << 4) | (codes[1] >> 4)) & 0xFFF;		
		strcpy(codesubstr, dctget(dct, singlecode));
		if (codesubstr == NULL) {
			strcat(prevsubstr, prevsubstr[0]); //i think this is wrong
                        strcpy(codesubstr, prevsubstr);
		}
		for (j = 0; codesubstr[j] != '\0'; j++) {
                        fputc(codesubstr[j], output);
                }
	}
	printf("reaching dct destroy\n");
	dctdestroy(dct);
	return;	
}

void bitUnpacking(unsigned char codes[], short int newcodes[]) {
	short int first, second;
	first = ((codes[0] << 4) | (codes[1] >> 4)) & 0xFFF;
	second =  ((codes[1] << 8) | (codes[2])) & 0xFFF;
	newcodes[0] = first;
	newcodes[1] = second;
}


void insertAsciiRev(Dict *dct) {
    	int asciiCode;
	char c;
    	for (asciiCode = 0; asciiCode <= 127; asciiCode++) {
		char c = (char)asciiCode;
        	dctinsert(dct, (char *)&asciiCode, (void*)(long)asciiCode);
    	}
}
