CC=clang
CFLAGS=-g -Wall -std=c++11 -o

ir: ir_block.cpp ir_gen.cpp instruction.cpp ../ast.cpp
	$(CC) $(CFLAGS) ir ir_block.cpp ir_gen.cpp instruction.cpp ../ast.cpp

clean:
	$(RM) ir *.o
