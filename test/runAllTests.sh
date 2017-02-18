#!/bin/bash

make all
./lexer.test
./parser.test
./irgen.test
./symtab.test
