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
	while ((elements_read = fread(bytes ,1, 3, input)) == 3) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			sprintf(code, "%03x", codes[i]); 
			ptr = dctget(dct, code);
			if (ptr != NULL) {
				length = strlen((char *)ptr);
                        	codesubstr = (char *)malloc(length + 1);
                        	strcpy(codesubstr, (char *)ptr);
				fwrite(codesubstr, sizeof(char), length, output);
				if (prevsubstr[0] !='\0') {
					char * insertstr = (char *)malloc(4096 * (sizeof(char)));
					dumbstr[0] = codesubstr[0];
					dumbstr[1] = '\0';
					sprintf(insertstr, "%s%s", prevsubstr, dumbstr);
					int valinsert = ASCIIMAX + (d++);
					sprintf(key, "%03x", valinsert);
					dctinsert(dct, key, (void *)insertstr);
					strcpy(prevsubstr, codesubstr); 
				}
			} else {
				char * insertstr = (char *)malloc(4096 * (sizeof(char)));
				dumbstr[0] = prevsubstr[0];
                                dumbstr[1] = '\0';
				sprintf(insertstr, "%s%s", prevsubstr, dumbstr);
				for (j = 0; prevsubstr[j] != '\0'; j++) {
                                	fputc(prevsubstr[j], output);
                                }
				dctinsert(dct, code, (void*)insertstr);
				strcpy(prevsubstr, insertstr);
				continue;
			}
		strcpy(prevsubstr, codesubstr);	
		free(codesubstr);		
		}	
	
	}
	if (elements_read != 0) {
		short int singlecode;
		singlecode = ((bytes[0] << 4) | (bytes[1] >> 4)) & 0xFFF;		
		sprintf(code, "%03x", singlecode); // int code now char
                ptr = dctget(dct, code);
		if (ptr == NULL) {
			char * insertstr = (char *)malloc(4096 * (sizeof(char)));
			dumbstr[0] = prevsubstr[0];
                        dumbstr[1] = '\0';
			sprintf(insertstr, "%s%s", prevsubstr, dumbstr);
			dctinsert(dct, code, (void*)insertstr);
                        strcpy(codesubstr, prevsubstr);
		} else {
			length = strlen((char *)ptr);
                	codesubstr = (char *)malloc(length + 1);
                	strcpy(codesubstr, (char *)ptr);
			
			if (prevsubstr[0] !='\0') {
                        	char * insertstr = (char *)malloc(4096 * (sizeof(char)));
                                dumbstr[0] = codesubstr[0];
                                dumbstr[1] = '\0';
                                sprintf(insertstr, "%s%s", prevsubstr, dumbstr);
                                int valinsert = ASCIIMAX + (d++);
                                sprintf(key, "%03x", valinsert);
				free(dctget(dct, key));
                                dctinsert(dct, key, (void *)insertstr);
                           }
					
		}
		for (j = 0; codesubstr[j] != '\0'; j++) {
                        fputc(codesubstr[j], output);
                }
		free(codesubstr);
	}
	
	//printf("before dctkeys\n");
	keys = dctkeys(dct);
	//printf("bafter dctkeys\n");
	for (int i = 0; i < dct ->size; i++) {
		free(dctget(dct, keys[i]));
	}
	free(keys);
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
