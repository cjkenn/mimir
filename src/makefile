CC = clang++
CFLAGS = -g -Wall -std=c++11 -o
SRCS = compiler.cpp \
	lexer.cpp \
	parser.cpp \
	parser_result.cpp \
	ast.cpp \
	token.cpp \
	error.cpp \
	symbol_table.cpp \
	symbol.cpp \
	ir_block.cpp \
	ir_gen.cpp \
	instruction.cpp \
	cfg_builder.cpp \
	cfg.cpp \
	cfg_node.cpp
OUT = ../build/mimir

mimir: $(SRCS)
	$(CC) $(CFLAGS) $(OUT) $(SRCS)

clean:
	$(RM) $(OUT) *.o
