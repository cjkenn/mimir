#pragma once

#include <vector>
#include <unordered_map>
#include "ast.h"
#include "symbol.h"

// The symbol table is represented by cascading unordered maps, one for each
// recognized lexical scope. It is up to the user to initialize and exit these
// scopes, by calling InitScope() and ExitScope() where appropriate.
//
// It is assumed during parsing that the scope level will end up at 0 again
// when the entire input file has been parsed.
class SymbolTable {
 public:
  SymbolTable();
  ~SymbolTable() {};

  int GetSymCount() const { return sym_count_; }

  // Insert a value into the current scope level table.
  void Insert(AstNodePtr var_ast);

  // Find a key. If the key does not exist at the current
  // scope level, traverses up the scope level until a key
  // is found. Returns nullptr if none exists.
  SymbolPtr Find(std::string key);

  // Checks only the current scope for the key.
  bool ExistsAtCurrentScope(std::string key);

  // Increment the scope level and create a new map for that
  // scope. The new map is inserted into the sym_tab_ vector.
  void InitScope();

  // Decrement the scope level as we exit the scope.
  void ExitScope();

 private:
  int curr_level_;
  int sym_count_;
  std::vector<std::unordered_map<std::string, SymbolPtr>> sym_tab_;
};
