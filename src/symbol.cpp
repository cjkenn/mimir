#include <string>
#include "symbol.h"

Symbol::Symbol() {
  name_ = "";
  type_ = "";
  value_ = 0;
}

Symbol::Symbol(std::string name) {
  name_ = name;
  type_ = "";
  value_ = 0;
}

Symbol::Symbol(std::string name, int value) {
  name_ = name;
  type_ = "";
  value_ = value;
}
