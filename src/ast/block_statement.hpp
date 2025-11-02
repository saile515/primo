#pragma once

#include "./statement.hpp"
#include <vector>
#include <memory>

namespace ast {

class BlockStatement : public Statement
{
  private:
    std::vector<std::unique_ptr<Statement>> m_statements;

  public:
    BlockStatement(std::vector<std::unique_ptr<Statement>> statements)
        : m_statements(std::move(statements))
    {
    }

    virtual llvm::Value *codegen(IRContext& context);
};

}
