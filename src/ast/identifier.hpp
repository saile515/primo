#pragma once

#include "./expression.hpp"
#include <string>

namespace primo::ast
{

class Identifier : public Expression
{
  private:
    std::string m_name;

  public:
    Identifier(std::string name) : m_name(name)
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
