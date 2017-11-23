#pragma once

#include <vector>
#include <string>
#include "ir_instr.h"
#include "ast.h"
#include "token.h"

const unsigned int NUM_AST_TYPES = 26;
const unsigned int NUM_IR_TYPES = 21;
const unsigned int NUM_TKN_TYPES = 27;

namespace mimir {
class PrettyPrinter {
 public:
  PrettyPrinter();
  ~PrettyPrinter() {};

  void PrintAst(const AstNodePtr ast);
  void PrintSingleAst(const AstNodePtr ast);

  void PrintIr(const std::vector<IrInstrPtr> ir);
  void PrintSingleIr(const IrInstrPtr ir);

  void PrintTkn(const Token tkn);

 private:
  void InitTknMap();
  void InitAstMap();
  void InitIrMap();

  std::string tkn_map_[NUM_TKN_TYPES];
  std::string ast_map_[NUM_AST_TYPES];
  std::string ir_map_[NUM_IR_TYPES];
};
}
