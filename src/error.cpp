#include <iostream>
#include <string>
#include "error.h"

void Error::ReportUnknownToken(std::string filename, int line, int pos, char ch) {
  std::string output;
  output += filename + " -> ";
  output += "At line " + std::to_string(line) + ":" + std::to_string(pos) + " ";
  output += "Unknown character '";
  output += ch;
  output += "' found!";

  std::cout << output << std::endl;
}
