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
    FunctionDefinition(std::string name, std::vector<std::string> parameters,
                       std::unique_ptr<BlockStatement> body)
        : FunctionDeclaration(std::move(name), std::move(parameters)), m_body(std::move(body))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
