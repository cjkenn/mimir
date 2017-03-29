#include <assert.h>
#include <memory>
#include <iostream>
#include "ast.h"
#include "symbol.h"
#include "symbol_table.h"

void test_insert_find_single_scope() {
  SymbolTable st;

  st.InitScope();
  auto ast = std::make_shared<AstNode>(AstType::VAR_AST);
  ast->SetText("x");

  st.Insert(ast);

  SymbolPtr result = st.Find("x");
  assert(result != nullptr);
  assert(result->GetName() == "x");
}

void test_insert_find_multiple_scopes() {
  SymbolTable st;

  st.InitScope();
  auto ast = std::make_shared<AstNode>(AstType::VAR_AST);
  ast->SetText("x");

  st.Insert(ast);

  // Initialize a second scope
  st.InitScope();

  auto inner_ast = std::make_shared<AstNode>(AstType::VAR_AST);
  inner_ast->SetText("y");

  st.Insert(inner_ast);

  // Both results should be available in this scope.
  SymbolPtr inner_result = st.Find("y");
  assert(inner_result != nullptr);
  assert(inner_result->GetName() == "y");

  SymbolPtr outer_result = st.Find("x");
  assert(outer_result != nullptr);
  assert(outer_result->GetName() == "x");

  // Now when we close one scope, only the outer result
  // should be available.
  st.ExitScope();

  SymbolPtr new_inner_result = st.Find("y");
  assert(new_inner_result == nullptr);

  SymbolPtr new_outer_result = st.Find("x");
  assert(new_outer_result != nullptr);
  assert(new_outer_result->GetName() == "x");
}

int main(int argc, char **argv) {
  test_insert_find_single_scope();
  test_insert_find_multiple_scopes();

  std::cout << "Symbol Table tests passed!" << std::endl;

  return 0;
}
