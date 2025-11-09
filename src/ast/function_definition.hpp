#pragma once

#include "./block_statement.hpp"
#include "./function_declaration.hpp"

namespace primo::ast
{

class FunctionDefinition : public FunctionDeclaration
{
  private:
    std::unique_ptr<BlockStatement> m_body;

  public:
    FunctionDefinition(std::string name, std::vector<FunctionParameter> parameters,
                       std::string return_type, std::unique_ptr<BlockStatement> body)
        : FunctionDeclaration(std::move(name), std::move(parameters), std::move(return_type)),
          m_body(std::move(body))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
