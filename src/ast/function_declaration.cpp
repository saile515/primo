#include "./function_declaration.hpp"

namespace primo::ast
{

llvm::Value *FunctionDeclaration::codegen(IRModuleContext &context)
{
    std::vector<llvm::Type *> parameter_types;

    parameter_types.reserve(m_parameters.size());

    for (size_t i = 0; i < m_parameters.size(); i++)
    {
        parameter_types.push_back(llvm::PointerType::getUnqual(*context.llvm_context));
    }

    llvm::FunctionType *function_type = llvm::FunctionType::get(
        llvm::Type::getVoidTy(*context.llvm_context), parameter_types, false);
    llvm::Function *function = llvm::Function::Create(
        function_type, llvm::Function::ExternalLinkage, m_name, context.module.get());

    context.named_values.push(m_name, function);

    return function;
}

} // namespace primo::ast
