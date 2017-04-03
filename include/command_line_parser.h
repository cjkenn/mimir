#pragma once

#include "parser_result.h"

class CommandLineParser {
 public:
  CommandLineParser() {};
  ~CommandLineParser() {};

  ParserResult Parse(const int argc, char **argv);
};
