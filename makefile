CC=g++

mimir: compiler.cpp lexer.cpp token.cpp
	$(CC) -g -Wall -std=c++11 -o mimir compiler.cpp lexer.cpp token.cpp

clean:
	$(RM) mimir *.o
