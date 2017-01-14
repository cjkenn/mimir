CC=g++

analysis: cfg_node.cpp cfg.cpp cfg_builder.cpp driver.cpp ../ast.cpp
	$(CC) -g -Wall -std=c++11 -o analysis cfg_node.cpp cfg.cpp cfg_builder.cpp driver.cpp ../ast.cpp

clean:
	$(RM) analysis *.o
