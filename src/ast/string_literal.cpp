#include "./string_literal.hpp"

namespace primo::ast
{

llvm::Value *StringLiteral::codegen(IRContext &context)
{
    return context.builder->CreateGlobalString(m_value);
}

} // namespace primo::ast
