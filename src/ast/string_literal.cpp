#include "./string_literal.hpp"

namespace primo::ast
{

llvm::Value *StringLiteral::codegen(IRModuleContext &context)
{
    return context.builder->CreateGlobalString(m_value);
}

} // namespace primo::ast
