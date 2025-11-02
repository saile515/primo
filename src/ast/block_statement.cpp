#include "./block_statement.hpp"

namespace ast
{

llvm::Value *BlockStatement::codegen(IRContext &context)
{
    llvm::Value *last = nullptr;

    for (auto &statement : m_statements)
    {
        last = statement->codegen(context);
    }

    return last;
}

} // namespace ast
