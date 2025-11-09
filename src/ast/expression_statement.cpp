#include "./expression_statement.hpp"

namespace primo::ast
{

llvm::Value *ExpressionStatement::codegen(IRModuleContext &context)
{
    return m_expression->codegen(context);
}

} // namespace primo::ast
