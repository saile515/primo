#pragma once

#include "./block_statement.hpp"
#include "./node.hpp"

namespace primo::ast
{

class Module : public Node
{
  private:
    std::string m_path;
    std::unique_ptr<BlockStatement> m_block;

  public:
    Module(std::string path, std::unique_ptr<BlockStatement> block)
        : m_path(std::move(path)), m_block(std::move(block))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);

    const std::string &get_path();
};

} // namespace primo::ast
