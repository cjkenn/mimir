#pragma once

#include <string>
#include <memory>

class Symbol;
typedef std::shared_ptr<Symbol> SymbolPtr;

class Symbol {
 public:
  Symbol();
  Symbol(std::string name);
  Symbol(std::string name, int value);
  ~Symbol() {};

  std::string GetName() const { return name_; }
  std::string GetType() const { return type_; }
  int GetValue() const { return value_; }

  void SetName(std::string name) { name_ = name; }
  void SetValue(int value) { value_ = value; }

 private:
  std::string name_;
  std::string type_;
  int value_;
};
