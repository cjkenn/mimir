#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include "ast.h"
#include "error.h"

class ParserResult {
 public:
  ParserResult() {};
  ~ParserResult() {};

  AstNodePtr GetAst() const { return ast_; }
  std::unordered_map<std::string, std::string> GetOptions() const { return options_; }

  void SetAst(AstNodePtr ast) { ast_ = ast; }
  void SetOptions(std::unordered_map<std::string, std::string> options) { options_ = options; }
  bool HasError();
  void AddError(Error error);
  void ReportErrors();

 private:
  std::unordered_map<std::string, std::string> options_;
  AstNodePtr ast_;
  std::vector<Error> errors_;
};
