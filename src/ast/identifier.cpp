#include "./identifier.hpp"

#include "../log.hpp"

namespace primo::ast
{

llvm::Value *Identifier::codegen(IRModuleContext &context)
{
    auto result = context.named_values.top(m_name);

    if (!result.has_value())
    {
        error("Unknown identifier: '{}'", m_name);
    }

    return result.value();
}

} // namespace primo::ast
