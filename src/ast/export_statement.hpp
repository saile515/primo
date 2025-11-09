#pragma once

#include "./statement.hpp"

namespace primo::ast
{

class ExportStatement : public Statement
{
  private:
    std::string m_name;
    std::unique_ptr<Statement> m_statement;

  public:
    ExportStatement(std::string name, std::unique_ptr<Statement> statement)
        : m_name(std::move(name)), m_statement(std::move(statement))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
