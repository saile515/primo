#include "./string_literal.hpp"

namespace ast {

llvm::Value *StringLiteral::codegen(IRContext& context)
{
    return context.builder->CreateGlobalString(m_value);
}

}
