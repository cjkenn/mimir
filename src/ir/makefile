CC=clang
CFLAGS=-g -Wall -std=c++11 -o

ir: basic_block.cpp ir_gen.cpp ../ast.cpp
	$(CC) $(CFLAGS) ir basic_block.cpp ir_gen.cpp ../ast.cpp

clean:
	$(RM) ir *.o
