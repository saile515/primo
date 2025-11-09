#include "./export_statement.hpp"

namespace primo::ast
{

llvm::Value *ExportStatement::codegen(IRModuleContext &context)
{
    llvm::Value *value = m_statement->codegen(context);

    llvm::Function *function = llvm::dyn_cast<llvm::Function>(value);

    if (function != nullptr)
    {
        context.get_export_map()[m_name] = function->getFunctionType();

        return nullptr;
    }

    context.get_export_map()[m_name] = value->getType();

    return nullptr;
}

} // namespace primo::ast
