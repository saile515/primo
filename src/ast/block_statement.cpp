#include "./block_statement.hpp"

namespace primo::ast
{

llvm::Value *BlockStatement::codegen(IRModuleContext &context)
{
    llvm::Value *last = nullptr;

    for (auto &statement : m_statements)
    {
        last = statement->codegen(context);
    }

    return last;
}

} // namespace primo::ast
