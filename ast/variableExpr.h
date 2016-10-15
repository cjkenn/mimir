#pragma once

#include <string>
#include "expr.h"

class VariableExprAST : public ExprAST {
public:
  VariableExprAST(const std::string &name) : m_name(name) {};

private:
  std::string m_name;
};
