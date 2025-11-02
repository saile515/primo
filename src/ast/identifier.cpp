#include "./identifier.hpp"

namespace ast
{

llvm::Value *Identifier::codegen(IRContext &context)
{
    auto result = context.named_values.top(m_name);

    if (!result.has_value())
    {
        throw std::runtime_error("Unknown identifier: " + m_name);
    }

    return result.value();
}

} // namespace ast
