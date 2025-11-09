#include "./import_statement.hpp"

#include "../log.hpp"
#include "llvm/Transforms/Utils/Cloning.h"
#include <filesystem>
#include <llvm/Support/Casting.h>
#include <llvm/Transforms/Utils/ValueMapper.h>

namespace primo::ast
{

llvm::Value *ImportStatement::codegen(IRModuleContext &context)
{
    std::filesystem::path path =
        std::filesystem::path(context.module->getName().str()).parent_path() / m_source;

    context.compile_module(path);

    std::string normalized_path = std::filesystem::canonical(path);

    for (auto &identifier : m_imported_identifiers)
    {
        llvm::Type *type = (*context.exported_values)[normalized_path][identifier];

        llvm::FunctionType *function_type = llvm::dyn_cast<llvm::FunctionType>(type);

        if (function_type == nullptr)
        {
            error("Export '{}' from module '{}' is not a function.", identifier, normalized_path);
        }

        llvm::Function *function = llvm::Function::Create(
            function_type, llvm::Function::ExternalLinkage, identifier, context.module.get());

        if (function == nullptr)
        {
            error("Module '{}' does not have an export named '{}'.", normalized_path, identifier);
        }

        context.named_values.push(identifier, function);
    }

    return nullptr;
}

} // namespace primo::ast
