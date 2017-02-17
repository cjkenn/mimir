#pragma once

#include <vector>
#include <unordered_map>
#include "ast.h"
#include "symbol.h"

class SymbolTable {
 public:
  SymbolTable();
  ~SymbolTable() {};

  void Insert(AstNodePtr var_ast);
  SymbolPtr Find(std::string key);
  void InitScope();
  void ExitScope();

 private:
  int curr_level_;
  std::vector<std::unordered_map<std::string, SymbolPtr>> sym_tab_;
};
