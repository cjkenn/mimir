#include <string>
#include <utility>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"

SymbolTable::SymbolTable() {
  curr_level_ = -1;
}

void SymbolTable::Insert(AstNodePtr var_ast) {
  if (curr_level_ < 0) {
    std::cout << "No scope initialized!" << std::endl;
    return;
  }

  auto curr_table = sym_tab_[curr_level_];
  std::string key = var_ast->GetText();
  SymbolPtr s = std::make_shared<Symbol>(key);

  std::pair<std::string, SymbolPtr> entry(key, s);

  curr_table.insert(entry);
}

SymbolPtr SymbolTable::Find(std::string key) {
  if (curr_level_ < 0) {
    std::cout << "No scope initialized!" << std::endl;
    return nullptr;
  }

  int level = curr_level_;

  while (level > 0) {
    auto curr_table = sym_tab_[curr_level_];
    auto entry = curr_table.find(key);

    if (entry != curr_table.end()) {
      return entry->second;
    }

    level--;
  }

  return nullptr;
}

void SymbolTable::InitScope() {
  curr_level_++;
  std::unordered_map<std::string, SymbolPtr> level_map;
  sym_tab_[curr_level_] = level_map;
}

void SymbolTable::ExitScope() {
  curr_level_--;
}
