#pragma once

#include <memory>
#include "expr.h"

class BinaryExprAST : public ExprAST {
public:
  BinaryExprAST(char op, std::unique_ptr<ExprAST> lhs, std::unique_ptr<ExprAST> rhs) :
  m_op(op), m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {};
private:
  char m_op;
  std::unique_ptr<ExprAST> m_lhs, m_rhs;
};
