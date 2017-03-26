#include <string>
#include <utility>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"

SymbolTable::SymbolTable() {
  curr_level_ = -1;
  sym_count_ = 0;
  // Create a top level scope for the entire program (we need at least one scope).
  InitScope();
}

void SymbolTable::Insert(AstNodePtr var_ast) {
  if (curr_level_ < 0) {
    std::cout << "Insert: No scope initialized!" << std::endl;
    return;
  }

  std::string key = var_ast->GetText();
  SymbolPtr s = std::make_shared<Symbol>(key);
  // TODO: Set the value of the symbol here too
  sym_count_++;
  s->SetStackOffset(sym_count_ * 4);

  std::pair<std::string, SymbolPtr> entry(key, s);

  sym_tab_[curr_level_].insert(entry);
}

SymbolPtr SymbolTable::Find(std::string key) {
  if (curr_level_ < 0) {
    std::cout << "Find: No scope initialized!" << std::endl;
    return nullptr;
  }

  int level = curr_level_;

  while (level >= 0) {
    auto curr_table = sym_tab_[level];
    auto entry = curr_table.find(key);

    if (entry != curr_table.end()) {
      return entry->second;
    }

    level--;
  }

  return nullptr;
}

bool SymbolTable::ExistsAtCurrentScope(std::string key) {
  if (curr_level_ < 0) {
    std::cout << "Exists: No scope initialized!" << std::endl;
    return false;
  }

  auto curr_table = sym_tab_[curr_level_];
  auto entry = curr_table.find(key);

  return (entry != curr_table.end());
}

void SymbolTable::InitScope() {
  curr_level_++;
  std::unordered_map<std::string, SymbolPtr> level_map;
  sym_tab_.push_back(level_map);
}

void SymbolTable::ExitScope() {
  curr_level_--;
}
