#pragma once

#include <fstream>
#include <memory>
#include "token.h"
#include "error.h"

class Lexer {
 public:
  Lexer(std::string filename);
  ~Lexer();

  // Read the file stream and return the next Token from the file.
  Token Lex();

  std::string GetFileName() const { return filename_; }

 private:
  // Create a new token and advance characters, primarly
  // used when lexing single characters. Also moves the
  // char count.
  Token BuildTokenAndAdvance(TokenType curr_type);
  Token BuildToken(TokenType curr_type);

  Token GetNumTkn();
  Token GetStrTkn();

  // Move the char pointer ahead one, and if we are at the end, ensure
  // that we will get and EOF token.
  void Advance();

  // Look ahead to the next char and return it. Ensures that we do not
  // consume the current char in the file stream.
  char Peek();

  std::string filename_;
  std::ifstream ifs_;
  char lastchar_;

  // The lexer stores the current line number and char position within
  // that line, so we can set that in the token returned. This is
  // used later on for error reporting purposes.
  int curr_line_pos_;
  int curr_char_pos_;
};
