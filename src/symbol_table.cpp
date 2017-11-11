#include <string>
#include <utility>
#include <iostream>
#include <memory>
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"

SymbolTable::SymbolTable() {
  curr_level_ = -1;
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
  // TODO: This will eventually need to be getSize based on type of symbol instead of only 8
  s->SetStackOffset((sym_tab_[curr_level_].size() + 1) * 8);
  s->SetScopeLevel(curr_level_);

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

int SymbolTable::GetSizeOfCurrentScope() {
  // TODO: Eventually this will need to be a loop over each var, to check types and sizes.
  // For now, each var created is assumed to be 32 bits
  return sym_tab_[curr_level_].size() * 8;
}

std::vector<SymbolPtr> SymbolTable::GetGlobals() {
  std::vector<SymbolPtr> result;
  auto globals = sym_tab_[0];
  for (auto entry : globals) {
    result.push_back(entry.second);
  }

  return result;
}
