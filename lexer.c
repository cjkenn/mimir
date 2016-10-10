#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "lexer.h"

#define MAX_ID_SIZE 128

Token lex()
{
  // TODO: get line nums and positions
  Token tkn = { EOF_TKN, NULL};
  int lastchar = ' ';

  // Skip whitespace
  while (isspace(lastchar))
  {
    lastchar = getchar();
  }

  // Lex a single character symbol
  switch (lastchar) {
  case '(':
    tkn.type = LEFT_PAREN_TKN;
    break;
  case ')':
    tkn.type = RIGHT_PAREN_TKN;
    break;
  case '{':
    tkn.type = LEFT_BRACE_TKN;
    break;
  case '}':
    tkn.type = RIGHT_BRACE_TKN;
    break;
  case '+':
    tkn.type = PLUS_TKN;
    break;
  case '-':
    tkn.type = MINUS_TKN;
    break;
  case '*':
    tkn.type = MULT_TKN;
    break;
  case '/':
    tkn.type = DIV_TKN;
    break;
  case '%':
    tkn.type = MOD_TKN;
    break;
  }

  // Lex a number
  if (isdigit(lastchar)) {
    return get_num_tkn(lastchar);
  }

  // Lex a string
  if (isalpha(lastchar)) {
    return get_str_tkn(lastchar);
  }

  return tkn;
}

Token get_num_tkn(char lastchar)
{
  char *numstr = "";

  while (isdigit(lastchar)) {
    numstr = append(numstr, lastchar);
    lastchar = getchar();
  }

  Token tkn = { NUM_TKN, numstr };
  return tkn;
}

Token get_str_tkn(char lastchar)
{
  char *str = "";

  while (isalnum(lastchar)) {
    str = append(str, lastchar);
    lastchar = getchar();
  }

  Token tkn = { STR_TKN, str };
  return tkn;
}

char *append(char *str, char c)
{
  size_t len = strlen(str);
  char *newstr = malloc(len + 2);

  strcpy(newstr, str);
  newstr[len] = c;
  newstr[len+1] = '\0';

  return strdup(newstr);
}
