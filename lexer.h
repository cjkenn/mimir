#pragma once

typedef enum
{
  LEFT_PAREN_TKN = 1,
  RIGHT_PAREN_TKN = 2,
  LEFT_BRACE_TKN = 3,
  RIGHT_BRACE_TKN = 4,
  PLUS_TKN = 5,
  MINUS_TKN = 6,
  MULT_TKN = 7,
  DIV_TKN = 8,
  MOD_TKN = 9,
  ID_TKN = 10,
  NUM_TKN = 11,
  STR_TKN = 12,
  EOF_TKN = 13
} TokenType;

typedef struct
{
  TokenType type;
  char *value;
} Token;

Token lex();
Token get_num_tkn(char curr_c);
Token get_str_tkn(char curr_c);
char *append(char *str, char c);
