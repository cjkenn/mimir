CC = clang++
CFLAGS = -g -Wall -std=c++11 -o
LEXERFILES = lexer_test.cpp \
	../src/lexer.cpp \
	../src/token.cpp
PARSERFILES = parser_test.cpp \
	../src/lexer.cpp \
	../src/parser.cpp \
	../src/token.cpp \
	../src/ast.cpp
IRGENFILES = ir_gen_test.cpp \
	../src/ir_gen.cpp \
	../src/ir_block.cpp \
	../src/instruction.cpp \
	../src/ast.cpp \
	../src/lexer.cpp \
	../src/parser.cpp \
	../src/token.cpp

all: lexer_test parser_test ir_gen_test

lexer_test: $(LEXERFILES)
	$(CC) $(CFLAGS) lexer.test $(LEXERFILES)

parser_test: $(PARSERFILES)
	$(CC) $(CFLAGS) parser.test $(PARSERFILES)

ir_gen_test: $(IRGENFILES)
	$(CC) $(CFLAGS) irgen.test $(IRGENFILES)

clean:
	$(RM) lexer.test parser.test irgen.test *.o
