CC=g++
CFLAGS=-g -Wall -std=c++11 -o

analysis: cfg_node.cpp cfg.cpp cfg_builder.cpp driver.cpp ../ast.cpp
	$(CC) $(CFLAGS) analysis cfg_node.cpp cfg.cpp cfg_builder.cpp driver.cpp ../ast.cpp

clean:
	$(RM) analysis *.o
