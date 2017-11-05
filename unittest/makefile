CC = g++
CFLAGS = -g -Wall -std=c++11 -I../include -o
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
LOCALOPTFILES = local_optimizer_test.cpp \
	helper/local_optimizer.cpp \
	../src/local_optimizer.cpp \
	../src/cfg_node.cpp \
	../src/ir_instr.cpp
X86SELFILES = x86_instr_sel_test.cpp \
	../src/x86_instr_sel.cpp \
	../src/cfg_node.cpp \
	../src/symbol_table.cpp \
	../src/symbol.cpp \
	../src/ir_instr.cpp \
	../src/ast.cpp \
	../src/x86_instr.cpp \
	../src/x86_instr_arg.cpp \
	../src/x86_reg.cpp

all: lexer_test parser_test ir_gen_test sym_tab_test cfg_gen_test local_opt_test x86_instr_sel_test

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

local_opt_test: $(LOCALOPTFILES)
	$(CC) $(CFLAGS) localopt.test $(LOCALOPTFILES)

x86_instr_sel_test: $(X86SELFILES)
	$(CC) $(CFLAGS) x86instrsel.test $(X86SELFILES)

clean:
	$(RM) lexer.test parser.test irgen.test symtab.test cfggen.test \
	localopt.test x86instrsel.test *.o
