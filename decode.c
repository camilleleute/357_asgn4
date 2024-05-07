
#define ASCIIMAX 0x7F 


void decode(FILE *input, FILE* output) {
	int i;
	Dict *dct = dctcreate();
	insertAsciiRev(dct);
	char substr[4096], prevsubstr[4096], currsubstr[4096];
	unsigned char bytes[3] = {-1};	
	short int codes[2];
	while (fread(bytes ,1, 3, input)  != EOF) {
		bitUnpacking(bytes, codes);
		for (i = 0; i < 2; i++) {
			strcpy(prevsubstr, substr);
			strcpy(currsubstr, prevsubstr);
			substr = (char)dctget(codes[i]);
			strcat(currsubstr, substr);
			char code = (char)(ASCIIMAX + (j++));
			
			dctinsert(dct, code, (void*)(long)currsubstr);

			if (substr == NULL) {
				
			}	
				
		}

	}
	
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
        char c = (char)asciiCode;
        dctinsert(dct, &ascii_code, (void*)(long)c);
    }
}
