#pragma once

#include "./expression.hpp"
#include "./statement.hpp"

namespace primo::ast
{

class ExpressionStatement : public Statement
{
  private:
    std::unique_ptr<Expression> m_expression;

  public:
    ExpressionStatement(std::unique_ptr<Expression> expression)
        : m_expression(std::move(expression)) {};

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
