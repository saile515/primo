#pragma once

#include "./expression.hpp"

namespace primo::ast
{

class StringLiteral : public Expression
{
  private:
    std::string m_value;

  public:
    StringLiteral(std::string value) : m_value(value)
    {
    }

    virtual llvm::Value *codegen(IRContext &context);
};

} // namespace primo::ast
