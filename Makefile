all: code 

code: lzw.o encode.o dict.o decode.o
	gcc -o lzw -g lzw.o encode.o decode.o dict.o 

lzw.o: lzw.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c -g lzw.c 

encode.o: encode.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c encode.c

decode.o: decode.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c decode.c

dict.o: dict.c dict.h
	gcc  -Wall -Wextra -std=gnu11 -pedantic -c dict.c

debug:
	gcc -Wall -Wextra -ansi -pedantic -g -o lzw lzw.c decode.c encode.c dict.c

clean:
	rm -f *.o
	rm -f *.out
