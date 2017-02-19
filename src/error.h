#pragma once

#include <string>

class Error {
 public:
  Error() {};
  ~Error() {};

  void ReportUnknownToken(std::string filename, int line, int pos, char ch);
 private:

};
