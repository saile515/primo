#include "./expression_statement.hpp"

namespace ast
{

llvm::Value *ExpressionStatement::codegen(IRContext &context)
{
    return m_expression->codegen(context);
}

} // namespace ast
