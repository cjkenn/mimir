#include "../ast.h"
#include "cfg_builder.h"
#include "cfg_node.h"

CfgBuilder::CfgBuilder() {
  block_num_ = 0;
}

CfgNodePtr CfgBuilder::BuildCfgFromAst(AstNodePtr root) {
  CfgNodePtr entry = MakeCfgNode(root);

  // 2. Create a block (cfgnode) for the entry ast node. Continue adding
  // ast nodes to this block until we reach one where control splits
  // (an if ast node, or a while ast node)
  for (AstNodePtr node : root->GetChildren()) {
    CfgNodeType cfg_type = GetCfgTypeFromAst(node);
    if (cfg_type == CfgNodeType::BASIC_CFG) {
      entry->AddAstNode(node);
    } else if (cfg_type == CfgNodeType::IF_CFG) {
      // 1. Add the if block to the entry node.
      entry->AddAstNode(node);

      // 2. Create a new block for the node following the if (child 1 in the ast)
      // TODO: Is child 1 correct here?
      AstNodePtr if_ast_node = node->GetChildAtIndex(0);
      CfgNodePtr if_block = MakeCfgNode(if_ast_node);

      // 3. Add that new block as adjacent to the entry.
      entry->AddAdjChild(if_block);

      // 4. Add all the statements in that node (within the if) to the block
      for (auto child_node : if_ast_node->GetChildren()) {
	if_block->AddAstNode(child_node);
      }

      // 5. Create a new block for the statement outside of the if
      // TODO: Is this the right child?
      CfgNodePtr block_after_if = MakeCfgNode(node->GetChildAtIndex(1));

      // 6. Add this block as a child of the entry block, and of the if_block
      entry->AddAdjChild(block_after_if);
      if_block->AddAdjChild(block_after_if);

      // 7. Continue adding statements to that block...

    } else if (cfg_type == CfgNodeType::WHILE_CFG) {

    }
  }

  // 3. For if-ast: Initial basic block has two successors: one for
  // when the if condition is satisfied, and the other for when it is not.

  // 4. For a while-ast: Initial basic block has two successors: one
  // is itself, and the other is the statement immediately following
  // the termination of the loop.

  // 5. Mark the last cfg node as an exit node.


  return entry;
}

CfgNodePtr CfgBuilder::MakeCfgNode(AstNodePtr node) {
  std::string name = GetNextBlockName();
  CfgNodeType type = GetCfgTypeFromAst(node);
  block_num_++;

  return std::make_shared<CfgNode>(name, type);
}

CfgNodeType CfgBuilder::GetCfgTypeFromAst(AstNodePtr node) {
  AstType node_type = node->GetType();

  switch(node_type) {
  case AstType::IF_AST:
    return CfgNodeType::IF_CFG;
  case AstType::WHILE_AST:
    return CfgNodeType::WHILE_CFG;
  default:
    return CfgNodeType::BASIC_CFG;
  }

}

std::string CfgBuilder::GetNextBlockName() {
  return "b" + block_num_;
}
