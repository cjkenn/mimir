#include <ctype.h>
#include <string.h>
#include "lexer.h"

Token lex(char curr_c)
{
  TokenValue val = {"", 0};
  // TODO: get line nums and positions
  Token tkn = { EOF_TKN, val, 0, 0 };

  // Skip whitespace
  while (isspace(curr_c))
  {
    curr_c = get_char();
  }

  // Lex a single character symbol
  switch (curr_c) {
  case '(':
    tkn.type = LEFT_PAREN_TKN;
  case ')':
    tkn.type = RIGHT_PAREN_TKN;
  case '{':
    tkn.type = LEFT_BRACE_TKN;
  case '}':
    tkn.type = RIGHT_BRACE_TKN;
  case '+':
    tkn.type = PLUS_TKN;
  case '-':
    tkn.type = MINUS_TKN;
  case '*':
    tkn.type = MULT_TKN;
  case '/':
    tkn.type = DIV_TKN;
  case '%':
    tkn.type = MOD_TKN;
  }

  // Lex a number
  if (isdigit(curr_c)) {
    return get_num_tkn(curr_c);
  }

  // Lex a string
  if (isalpha(curr_c)) {
    return get_str_tkn(curr_c);
  }

  return tkn;
}

Token get_num_tkn(char curr_c)
{
  int num = 0;
  while (isdigit(curr_c)) {
    num += curr_c;
    curr_c = get_char();
  }

  Token tkn = {NUM_TKN, {"", num}, 0, 0};
  return tkn;
}

Token get_str_tkn(char curr_c)
{
  // TODO: buffer length
  char str[20];
  while (isalpha(curr_c)) {
    strncat(str, &curr_c, 1);
    curr_c = get_char();
  }

  Token tkn = {STR_TKN, {str, 0}, 0, 0};
  return tkn;
}

// TODO: do something
char get_char()
{
  return 'c';
}
