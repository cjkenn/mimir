CC=g++

mimir: compiler.cpp lexer.cpp parser.cpp ast.cpp
	$(CC) -g -Wall -std=c++11 -o mimir compiler.cpp lexer.cpp parser.cpp ast.cpp

clean:
	$(RM) mimir *.o
