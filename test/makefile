CC = clang++
CFLAGS = -g -Wall -std=c++11 -I../include -o

X86GENFILES = x86_gen.cpp \
	../src/lexer.cpp \
	../src/parser.cpp \
	../src/token.cpp \
	../src/parser_result.cpp \
	../src/error.cpp \
	../src/ast.cpp \
	../src/ir_gen.cpp \
	../src/ir_instr.cpp \
	../src/cfg_node.cpp \
	../src/symbol.cpp \
	../src/symbol_table.cpp \
	../src/cfg_gen.cpp \
	../src/cfg.cpp \
	../src/x86_instr.cpp \
	../src/x86_section.cpp \
	../src/x86_instr_sel.cpp \
	../src/x86_writer.cpp \
	../src/x86_reg.cpp \
	../src/x86_reg_alloc.cpp

all: x86_gen

x86_gen: $(X86GENFILES)
	$(CC) $(CFLAGS) x86_gen $(X86GENFILES)

clean:
	$(RM) x86_gen *.o
