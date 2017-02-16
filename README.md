## mimir

Small language with c-language syntax. 

#### Building
Building is done using make. The directory structure is a WIP right now, so the builds are far from perfect. To compile a file with mimir, run:

```
mkdir build # until I get around to changing the makefile
make
cd src
./mimir {filename}
```

#### Tests
To run tests, execute the following commands:
```
cd tests
./runAllTests.sh
```

#### mimIR

mimIR (mimir IR) is a simple and standard IR used for analysis. mimIR is a two address, infinte register, infinte
label linear IR. It has assembly-like syntax, as follows:
```
mv 5, r0
mv 5, r1
add r0, r1 ;result is in r1
```
Supported operations can be found in src/instruction.h


#### Grammar
```
<program> ::= <statement>
<statement> ::= "if" <paren_expr> <statement> |
                "if" <paren_expr> <statement> "else" <statement> |
                "while" <paren_expr> <statement> |
                "{" <statement> "}" |
                <expr> ";" |
                ";"
 <paren_expr> ::= "(" <expr> ")"
 <expr> ::= <test> | <id> "=" <expr>
 <test> ::= <sum> | <sum> <testop> <sum>
 <testop> ::= "<" | "<=" | ">" | ">=" | "!=" | "=="
 <sum> ::= <term> | <sum> <binop> <term>
 <binop> ::= "+" | "-" | "*" | "/" | "%"
 <term> ::= <id> | <int> | <paren_expr>
 <id> ::= "a" | "b" | "c" | "d" | ... | "z"
 <int> ::= <an_unsigned_decimal_integer>
```
