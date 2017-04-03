CC = clang++
CFLAGS = -g -Wall -std=c++11 -I../include -o
SRCS = compiler.cpp \
	lexer.cpp \
	parser.cpp \
	parser_result.cpp \
	ast.cpp \
	token.cpp \
	error.cpp \
	symbol_table.cpp \
	symbol.cpp \
	ir_gen.cpp \
	ir_instr.cpp \
	cfg_gen.cpp \
	cfg.cpp \
	cfg_node.cpp \
	x86_instr.cpp \
	local_optimizer.cpp \
	x86_instr_sel.cpp \
	x86_writer.cpp \
	x86_section.cpp \
	command_line_parser.cpp
OUT = ../build/mimir

mimir: $(SRCS)
	$(CC) $(CFLAGS) $(OUT) $(SRCS)

clean:
	$(RM) $(OUT) *.o
