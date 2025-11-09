#pragma once

#include "./statement.hpp"

struct FunctionParameter
{
    std::string name;
    std::string type;
};

namespace primo::ast
{

class FunctionDeclaration : public Statement
{
  protected:
    std::string m_name;
    std::vector<FunctionParameter> m_parameters;
    std::string m_return_type;

  public:
    FunctionDeclaration(std::string name, std::vector<FunctionParameter> parameters,
                        std::string return_type)
        : m_name(std::move(name)), m_parameters(std::move(parameters)),
          m_return_type(std::move(return_type))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
