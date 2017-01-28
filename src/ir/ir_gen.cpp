#include <memory>
#include "../ast.h"
#include "basic_block.h"
#include "ir_gen.h"

IrGen::IrGen() {
  reg_count_ = 0;
}

BasicBlockPtr IrGen::Gen(AstNodePtr ast) {
  // Confirm the ast structure
  if (ast->GetType() != AstType::PROG_AST) {
    // TODO: error handling maybe?
    return nullptr;
  }

  AstNodePtr start = ast->GetChildAtIndex(0);
  if (start->GetType() == AstType::EXPR_AST) {
    start = start->GetChildAtIndex(0);
  }

  auto root = GetBlock(start);

  return root;
}

BasicBlockPtr IrGen::GetBlock(AstNodePtr ast) {
  return nullptr;
}
