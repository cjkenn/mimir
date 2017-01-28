#include <memory>
#include "../ast.h"
#include "basic_block.h"
#include "ir_gen.h"

IrGen::IrGen() {

}

std::shared_ptr<BasicBlock> IrGen::Gen(AstNodePtr ast) {
  // 1. Walk ast, switch on ast type to build blocks

}
