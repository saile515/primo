#include "./function_declaration.hpp"

#include "../log.hpp"
#include <llvm/IR/DerivedTypes.h>

namespace primo::ast
{

llvm::Type *get_type(IRModuleContext &context, const std::string &name)
{
    llvm::Type *type;

    if (name == "String")
    {
        type = llvm::PointerType::getUnqual(*context.llvm_context);
    }
    else if (name == "Void")
    {
        type = llvm::Type::getVoidTy(*context.llvm_context);
    }
    else
    {
        error("Unknown type '{}'.", name);
    }

    return type;
}

llvm::Value *FunctionDeclaration::codegen(IRModuleContext &context)
{
    std::vector<llvm::Type *> parameter_types;

    parameter_types.reserve(m_parameters.size());

    for (auto &parameter : m_parameters)
    {
        parameter_types.push_back(get_type(context, parameter.type));
    }

    llvm::FunctionType *function_type =
        llvm::FunctionType::get(get_type(context, m_return_type), parameter_types, false);
    llvm::Function *function = llvm::Function::Create(
        function_type, llvm::Function::ExternalLinkage, m_name, context.module.get());

    context.named_values.push(m_name, function);

    return function;
}

} // namespace primo::ast
