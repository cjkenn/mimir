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
	../src/ir_instr.cpp \
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
CFGGENFILES = cfg_gen_test.cpp \
	../src/ir_instr.cpp \
	../src/cfg_gen.cpp \
	../src/cfg.cpp \
	../src/cfg_node.cpp

all: lexer_test parser_test ir_gen_test sym_tab_test cfg_gen_test

lexer_test: $(LEXERFILES)
	$(CC) $(CFLAGS) lexer.test $(LEXERFILES)

parser_test: $(PARSERFILES)
	$(CC) $(CFLAGS) parser.test $(PARSERFILES)

ir_gen_test: $(IRGENFILES)
	$(CC) $(CFLAGS) irgen.test $(IRGENFILES)

sym_tab_test: $(SYMTABFILES)
	$(CC) $(CFLAGS) symtab.test $(SYMTABFILES)

cfg_gen_test: $(CFGGENFILES)
	$(CC) $(CFLAGS) cfggen.test $(CFGGENFILES)

clean:
	$(RM) lexer.test parser.test irgen.test symtab.test cfggen.test *.o
