#pragma once

#include <vector>
#include "ast.h"
#include "error.h"

class ParserResult {
 public:
  ParserResult() {};
  ~ParserResult() {};

  AstNodePtr GetAst() const { return ast_; }
  void SetAst(AstNodePtr ast) { ast_ = ast; }
  bool HasError();
  void AddError(Error error);
  void ReportErrors();

 private:
  AstNodePtr ast_;
  std::vector<Error> errors_;
};
