#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "dict.h"
#define ASCIIMAX 0x7F 


void decode(FILE *input, FILE* output) {
	char code[4], key[4];
	int i, j;
	int d = 1;
	size_t elements_read, length;
	Dict *dct = dctcreate();
	insertAsciiRev(dct);
	char ** keys = NULL;
	void *ptr;
	char *codesubstr;
	char dumbstr[2];
	unsigned char bytes[3] = {-1};	
	short int codes[2];
	char prevsubstr[4096] = {'\0'};
	//char** mallocedpointers[4096] = {NULL};
	//size_t mallocindex = 0;
	while ((elements_read = fread(bytes ,1, 3, input)) == 3) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			sprintf(code, "%03x", codes[i]); // int code now char
			ptr = dctget(dct, code);
			//printf("direct: %s\n", (char *)ptr);
			length = strlen((char *)ptr);
			codesubstr = (char *)malloc(length + 1);
			strcpy(codesubstr, (char *)ptr);
			//printf("code read: %x; asso substr: %s\n", codes[i], codesubstr);
			if (codesubstr != NULL) {
				fwrite(codesubstr, sizeof(char), length, output);
			
				if (prevsubstr[0] !='\0') {
					char * insertstr = (char *)malloc(4096 * (sizeof(char)));
					//mallocedpointers[mallocindex] = insertstr;
					//mallocindex++;
					dumbstr[0] = codesubstr[0];
					dumbstr[1] = '\0';
					sprintf(insertstr, "%s%s", prevsubstr, dumbstr);
					//strcat(insertstr, dumbstr);
					int valinsert = ASCIIMAX + (d++);
					sprintf(key, "%03x", valinsert);
					//printf("insert %s with code %s\n", insertstr, key);
					dctinsert(dct, key, (void *)insertstr);
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
		strcpy(prevsubstr, codesubstr);	
		free(codesubstr);		
		}	
	
	}
	if (elements_read == 2) {
		short int singlecode;
		singlecode = ((codes[0] << 4) | (codes[1] >> 4)) & 0xFFF;		
		sprintf(code, "%03x", singlecode); // int code now char
                ptr = dctget(dct, code);
                length = strlen((char *)ptr);
                codesubstr = (char *)malloc(length + 1);
                strcpy(codesubstr, (char *)ptr);
		if (codesubstr == NULL) {
			dumbstr[0] = prevsubstr[0];
                        dumbstr[1] = '\0';
			strcat(prevsubstr, dumbstr);
                        strcpy(codesubstr, prevsubstr);
		}
		for (j = 0; codesubstr[j] != '\0'; j++) {
                        fputc(codesubstr[j], output);
                }
	}
	keys = dctkeys(dct);
	for (int i = 0; i < dct ->size; i++) {
		free(dctget(dct, &key[i]));
	}
	
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
