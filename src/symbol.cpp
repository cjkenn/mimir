#include <string>
#include "symbol.h"

Symbol::Symbol() {
  name_ = "";
  type_ = "";
  size_ = 4;
  stack_offset_ = 0;
}

Symbol::Symbol(std::string name) {
  name_ = name;
  type_ = "";
  size_ = 4;
  stack_offset_ = 0;
}
