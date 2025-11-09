#pragma once

#include "./statement.hpp"

namespace primo::ast
{

class FunctionDeclaration : public Statement
{
  protected:
    std::string m_name;
    std::vector<std::string> m_parameters;

  public:
    FunctionDeclaration(std::string name, std::vector<std::string> parameters)
        : m_name(std::move(name)), m_parameters(std::move(parameters))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
