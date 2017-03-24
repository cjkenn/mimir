#include <string>
#include "symbol.h"

Symbol::Symbol() {
  name_ = "";
  type_ = "";
  value_ = 0;
  size_ = 4;
  stack_offset_ = 0;
}

Symbol::Symbol(std::string name) {
  name_ = name;
  type_ = "";
  value_ = 0;
  size_ = 4;
  stack_offset_ = 0;
}

Symbol::Symbol(std::string name, int value) {
  name_ = name;
  type_ = "";
  value_ = value;
  size_ = 4;
  stack_offset_ = 0;
}
