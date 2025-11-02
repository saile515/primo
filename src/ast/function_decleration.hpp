#pragma once

#include "./statement.hpp"
#include "./block_statement.hpp"

namespace ast {

class FunctionDeclaration : public Statement
{
  private:
    std::string m_name;
    std::vector<std::string> m_parameters;
    std::unique_ptr<BlockStatement> m_body;

  public:
    FunctionDeclaration(std::string name, std::vector<std::string> parameters,
                        std::unique_ptr<BlockStatement> body)
        : m_name(std::move(name)), m_parameters(std::move(parameters)), m_body(std::move(body))
    {
    }

    virtual llvm::Value *codegen(IRContext& context);
};

}
