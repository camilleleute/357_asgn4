



void encode(FILE *input, FILE *output) {
	char * substr[4096];
	substr[0] = '\0';
	while ((c = fgetc(input)) != EOF) {
		int length = strlen(substr);
		substr[length] = c;
		substr[length+1] = '\0';
		if (length + 1 == 1) {
			int val;
			val = (long)c
		}

	}

}
