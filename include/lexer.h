#pragma once

#include <fstream>
#include <memory>
#include <unordered_map>
#include "token.h"
#include "error.h"

namespace mimir {
class Lexer {
 public:
  Lexer(const std::string filename);
  ~Lexer();

  // Read the file stream and return the next Token from the file.
  Token Lex();

  // Look at the current char the file iterator is on. Keep in mind this will
  // be the char immediately after the current token. This will also advance
  // the iterator past whitespace, and then rewind it if necessary.
  char PeekCurrChar();

  std::string GetFileName() const { return filename_; }

 private:
  // Create a new token and advance characters, primarly
  // used when lexing single characters. Also moves the
  // char count.
  Token BuildTokenAndAdvance(const TokenType curr_type);

  // Create a new token but do not advance characters.
  Token BuildToken(const TokenType curr_type);

  // Lexes a 64 bit sized number token.
  Token GetNumTkn();

  // Lexes a string token. This method also handles reserved words, and
  // returns the correct token if the string is a reserved word.
  Token GetStrTkn();

  // Move the char pointer ahead one, and if we are at the end, ensure
  // that we will get and EOF token.
  void Advance();

  // Look ahead to the next char and return it. Ensures that we do not
  // consume the current char in the file stream.
  char Peek();

  // Initialize reserved words mapping. Called in the constructor.
  void InitReservedWords();

  std::string filename_;
  std::ifstream ifs_;
  char lastchar_;

  // The lexer stores the current line number and char position within
  // that line, so we can set that in the token returned. This is
  // used later on for error reporting purposes.
  int curr_line_pos_;
  int curr_char_pos_;

  std::unordered_map<std::string, TokenType> reserved_words_;
};
}
