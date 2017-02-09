## mimir

Very small language based on tinyc grammar. 

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

#### Grammar
```
<program> ::= <statement>
<statement> ::= "if" <paren_expr> <statement> |
                "if" <paren_expr> <statement> "else" <statement> |
                "while" <paren_expr> <statement> |
                "{" { <statement> } "}" |
                <expr> ";" |
                ";"
 <paren_expr> ::= "(" <expr> ")"
 <expr> ::= <test> | <id> "=" <expr>
 <test> ::= <sum> | <sum> "<" <sum>
 <sum> ::= <term> | <sum> "+" <term> | <sum> "-" <term>
 <term> ::= <id> | <int> | <paren_expr>
 <id> ::= "a" | "b" | "c" | "d" | ... | "z"
 <int> ::= <an_unsigned_decimal_integer>
```
