#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"
#define ASCIIMAX 0x7F 


void decode(FILE *input, FILE* output) {
	char code[4];
	int i, j;
	size_t elements_read;
	Dict *dct = revdctcreate();
	insertAsciiRev(dct);
	void *ptr;
	char dumbstr[2];
	char codesubstr[4096], prevsubstr[4096];
	unsigned char bytes[3] = {-1};	
	short int codes[2];
	while ((elements_read = fread(bytes ,1, 3, input)) == 3) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			sprintf(code, "%03x", codes[i]); // int code now char
		//	codesubstr = dctget(dct, code);
			
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
				dctinsert(dct, code, (void*)prevsubstr);
				
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
    	char key[4];
	char assostr[2];
	char *strcopy;
	for (unsigned int asciiCode = 0; asciiCode <= 127; asciiCode++) {
        	sprintf(key, "%03x", asciiCode);
		sprintf(assostr, "%c", (char)asciiCode);
		strcopy = (char *)malloc((strlen(assostr) + 1) * sizeof(char));
		strcpy(strcopy, assostr);
		dctinsert(dct, key, (void *)strcopy);
    	}
}
