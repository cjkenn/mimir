CC=g++
CFLAGS=-g -Wall -std=c++11 -o

all: lexer_test parser_test

lexer_test: lexer_test.cpp ../src/lexer.cpp ../src/token.cpp
	$(CC) $(CFLAGS) lexer.test lexer_test.cpp ../src/lexer.cpp ../src/token.cpp

parser_test: parser_test.cpp ../src/lexer.cpp ../src/parser.cpp ../src/token.cpp ../src/ast.cpp
	$(CC) $(CFLAGS) parser.test parser_test.cpp ../src/lexer.cpp ../src/parser.cpp ../src/token.cpp ../src/ast.cpp

clean:
	$(RM) lexer.test parser.test *.o
