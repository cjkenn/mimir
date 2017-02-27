#!/bin/bash

make all
./lexer.test
./parser.test
./symtab.test
./irgen.test
./cfggen.test
./localopt.test
