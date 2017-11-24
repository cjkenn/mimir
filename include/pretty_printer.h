#pragma once

#include <vector>
#include <string>
#include "ir_instr.h"
#include "ast.h"
#include "token.h"

namespace mimir {
class PrettyPrinter {
 public:
  const static std::string tkn_map[];
  const static std::string ast_map[];
  const static std::string ir_map[];

  static void PrintAst(const AstNodePtr ast, unsigned int level);
  static void PrintSingleAst(const AstNodePtr ast);

  static void PrintIr(const std::vector<IrInstrPtr> ir);
  static void PrintSingleIr(const IrInstrPtr ir);

  static void PrintTkn(const Token tkn);
};
}
