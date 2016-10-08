#pragma once

typedef enum
{
  LEFT_PAREN_TKN,
  RIGHT_PAREN_TKN,
  LEFT_BRACE_TKN,
  RIGHT_BRACE_TKN,
  PLUS_TKN,
  MINUS_TKN,
  MULT_TKN,
  DIV_TKN,
  MOD_TKN,
  ID_TKN,
  STR_TKN,
  EOF_TKN
} TokenType;

typedef struct
{
  TokenType type;
  int line;
  int pos;

} Token;


Token lex(char curr_c);
