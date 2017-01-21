CC=g++
CFLAGS=-g -Wall -std=c++11 -o

lexer_test: lexer_test.cpp ../src/lexer.cpp ../src/token.cpp
	$(CC) $(CFLAGS) lexer_test lexer_test.cpp ../src/lexer.cpp ../src/token.cpp

clean:
	$(RM) lexer_test *.o
