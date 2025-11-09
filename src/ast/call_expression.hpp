#pragma once

#include "./expression.hpp"

namespace primo::ast
{

class CallExpression : public Expression
{
  private:
    std::unique_ptr<Expression> m_callee;
    std::vector<std::unique_ptr<Expression>> m_arguments;

  public:
    CallExpression(std::unique_ptr<Expression> callee,
                   std::vector<std::unique_ptr<Expression>> arguments)
        : m_callee(std::move(callee)), m_arguments(std::move(arguments)) {};

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
