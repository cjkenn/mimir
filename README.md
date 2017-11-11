## mimir

Small language with c-style syntax. Currently, the language generates NASM valid x86_64 assembly, on linux platforms. Local optimizations are also performed (by default, this will be a compiler flag once I get around to it).

#### Building
Building is done using make. The directory structure is a WIP right now, so the builds are far from perfect. To compile a file with mimir, run:

```
mkdir build # until I get around to changing the makefile
cd src
make
cd ../build
./mimir {filename}
```

#### Tests
mimir has unit tests and integration tests.
To run unit tests:

```
cd unittest
./runUnitTests.sh
```

To run integration tests:

```
cd test
./runIntegrationTests.sh
```

#### mimIR

mimIR (mimir IR) is a simple and standard IR used for analysis. mimIR is a two address, infinte register, infinte
label linear IR. It has assembly-like syntax, as follows:
```
mv 5, r0
mv 5, r1
add r0, r1 ;result is in r1
```
Supported operations can be found in src/ir_instr.h


#### Grammar
```
<program> ::= <statement>
<statement> ::= "if" <paren_expr> <statement> |
                "if" <paren_expr> <statement> "else" <statement> |
                "while" <paren_expr> <statement> |
                <brace_statement> |
                <expr> ";" |
		<func> |
                ";"
 <brace_statement> ::= "{" <statement> "}"
 <paren_expr> ::= "(" <expr> ")"
 <expr> ::= <test> | <id> "=" <expr>
 <test> ::= <sum> | <sum> <testop> <sum>
 <testop> ::= "<" | "<=" | ">" | ">=" | "!=" | "=="
 <sum> ::= <term> | <sum> <binop> <term>
 <binop> ::= "+" | "-" | "*" | "/" | "%"
 <term> ::= <id> | <int> | <paren_expr>
 <id> ::= "a" | "b" | "c" | "d" | ... | "z"
 <int> ::= <an_unsigned_decimal_integer>
 <func> ::= "function" <id> "(" <params> ")" <brace_statement>
 <params> ::= <sum> | "," <params>
```