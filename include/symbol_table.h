#pragma once

#include <vector>
#include <unordered_map>
#include "ast.h"
#include "symbol.h"

namespace mimir {
// The symbol table is represented by cascading unordered maps, one for each
// recognized lexical scope. It is up to the user to initialize and exit these
// scopes, by calling InitScope() and ExitScope() where appropriate.
//
// It is necessary to call InitScope() when this symbol table is created, in order
// to initialize the top level program scope. This step is done in the constructor,
// so make sure not to perform any extra calls to InitScope(), unless you really intend
// to make a new scope! Technically, ExitScope() could be called in the destructor,
// but of course there's really no point in doing that.
class SymbolTable {
 public:
  SymbolTable();
  ~SymbolTable() {};

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

  // Returns the "size" of the current scope. Size should be determined
  // by the number of unique vars in this scope, multipled by their type size.
  int GetSizeOfCurrentScope();

  // Walks the symbol table at level 0 and returns all globally defined symbols.
  std::vector<SymbolPtr> GetGlobals();

 private:
  int curr_level_;
  std::vector<std::unordered_map<std::string, SymbolPtr>> sym_tab_;
};
}
