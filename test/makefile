CC = clang++
CFLAGS = -g -Wall -std=c++11 -o
LEXERFILES = lexer_test.cpp \
	../src/lexer.cpp \
	../src/token.cpp \
	../src/error.cpp
PARSERFILES = parser_test.cpp \
	../src/lexer.cpp \
	../src/parser.cpp \
	../src/token.cpp \
	../src/symbol_table.cpp \
	../src/symbol.cpp \
	../src/ast.cpp \
	../src/parser_result.cpp \
	../src/error.cpp
IRGENFILES = ir_gen_test.cpp \
	../src/ir_gen.cpp \
	../src/ir_block.cpp \
	../src/instruction.cpp \
	../src/symbol_table.cpp \
	../src/symbol.cpp \
	../src/ast.cpp \
	../src/lexer.cpp \
	../src/parser.cpp \
	../src/parser_result.cpp \
	../src/token.cpp \
	../src/error.cpp
SYMTABFILES = symbol_table_test.cpp \
	../src/ast.cpp \
	../src/symbol.cpp \
	../src/symbol_table.cpp

all: lexer_test parser_test ir_gen_test sym_tab_test

lexer_test: $(LEXERFILES)
	$(CC) $(CFLAGS) lexer.test $(LEXERFILES)

parser_test: $(PARSERFILES)
	$(CC) $(CFLAGS) parser.test $(PARSERFILES)

ir_gen_test: $(IRGENFILES)
	$(CC) $(CFLAGS) irgen.test $(IRGENFILES)

sym_tab_test: $(SYMTABFILES)
	$(CC) $(CFLAGS) symtab.test $(SYMTABFILES)

clean:
	$(RM) lexer.test parser.test irgen.test symtab.test *.o
