CC=gcc

mimir: compiler.c lexer.c
	$(CC) -g -Wall -o mimir compiler.c lexer.c

clean:
	$(RM) mimir *.o
