#pragma once

#include "./block_statement.hpp"
#include "./node.hpp"

namespace primo::ast
{

class Program : public Node
{
  private:
    std::unique_ptr<BlockStatement> m_block;

  public:
    Program(std::unique_ptr<BlockStatement> block) : m_block(std::move(block))
    {
    }

    virtual llvm::Value *codegen(IRContext &context);
    void codegen();
};

} // namespace primo::ast
