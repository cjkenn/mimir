#pragma once

#include "expr.h"

class NumberExprAST : public ExprAST {
public:
 NumberExprAST(int val) : m_val(val) {};

private:
  int m_val;
};
