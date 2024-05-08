#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "revdict.h"
#define ASCIIMAX 0x7F 


void decode(FILE *input, FILE* output) {
	int i, j;
	size_t elements_read;
	Dict *dct = revdctcreate();
	insertAsciiRev(dct);
	void *ptr;
	char dumbstr[2];
	char codesubstr[4096], prevsubstr[4096];
	unsigned char bytes[3] = {-1};	
	short int codes[2];
	printf("hi/n");
	while ((elements_read = fread(bytes ,1, 3, input)) == 3) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			ptr = revdctget(dct, codes[i]);
			strcpy(codesubstr, (char *)ptr);
			//strcpy(codesubstr, dctget(dct, codes[i]));
			printf("code is: %x; associated string is: %s\n", codes[i], codesubstr);	
			if (codesubstr != NULL) {
				for (j = 0; codesubstr[j] != '\0'; j++) {
					fputc(codesubstr[j], output);
					//maybe handle fail fputs or EOF?
				}
				if (prevsubstr != NULL) {
					dumbstr[0] = codesubstr[0];
					dumbstr[1] = '\0';
					strcat(prevsubstr, dumbstr);
					revdctinsert(dct, codes[i], (void *)prevsubstr);
					strcpy(prevsubstr, codesubstr);  
				}
			} else {
				dumbstr[0] = prevsubstr[0];
                                        dumbstr[1] = '\0';
				strcat(prevsubstr, dumbstr); //i think this is wrong
				for (j = 0; prevsubstr[j] != '\0'; j++) {
                                	fputc(prevsubstr[j], output);
                                }
				revdctinsert(dct, codes[i], (void*)prevsubstr);
				
			}
			
				
		}	
	
	}
	if (elements_read == 2) {
		short int singlecode;
		singlecode = ((codes[0] << 4) | (codes[1] >> 4)) & 0xFFF;		
		ptr = revdctget(dct, singlecode);
		strcpy(codesubstr, (char *)ptr);
		if (codesubstr == NULL) {
			dumbstr[0] = prevsubstr[0];
                        dumbstr[1] = '\0';
			strcat(prevsubstr, dumbstr);//i think this is wrong
                        strcpy(codesubstr, prevsubstr);
		}
		for (j = 0; codesubstr[j] != '\0'; j++) {
                        fputc(codesubstr[j], output);
                }
	}
	printf("reaching dct destroy\n");
	revdctdestroy(dct);
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
    	for (asciiCode = 0; asciiCode <= 127; asciiCode++) {
        	revdctinsert(dct, asciiCode, (void*)asciiCode);
    	}
}
