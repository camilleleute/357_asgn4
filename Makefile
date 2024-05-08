all: code 

code: lzw.o encode.o dict.o decode.o revdict.o
	gcc -o lzw -g lzw.o revdict.c encode.o decode.o dict.o 

lzw.o: lzw.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c -g lzw.c 

encode.o: encode.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c encode.c

decode.o: decode.c
	gcc -Wall -Wextra -std=gnu11 -pedantic -c decode.c


revdict.o: revdict.c revdict.h
	gcc  -Wall -Wextra -std=gnu11 -pedantic -c revdict.c


dict.o: dict.c dict.h
	gcc  -Wall -Wextra -std=gnu11 -pedantic -c dict.c

debug:
	gcc -Wall -Wextra -ansi -pedantic -g -o lzw lzw.c decode.c encode.c dict.c revdict.c

clean:
	rm -f *.o
	rm -f *.out
