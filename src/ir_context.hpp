#pragma once

#include "./hash_stack.hpp"
#include "llvm/IR/DerivedTypes.h"
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Module.h>
#include <llvm/Target/TargetMachine.h>
#include <memory>

namespace primo
{

struct IRContext
{
    std::shared_ptr<llvm::LLVMContext> llvm_context;
    std::shared_ptr<llvm::TargetMachine> target;
    std::shared_ptr<std::unordered_map<std::string, std::unordered_map<std::string, llvm::Type *>>>
        exported_values;
    std::function<void(std::string path)> compile_module;
};

struct IRModuleContext : public IRContext
{
    std::string name;
    HashStack<std::string, llvm::Value *> named_values;
    std::shared_ptr<llvm::IRBuilder<>> builder;
    std::shared_ptr<llvm::Module> module;

    IRModuleContext(const IRContext &context) : IRContext(context)
    {
    }

    std::unordered_map<std::string, llvm::Type *> &get_export_map()
    {
        return (*exported_values)[name];
    }
};

} // namespace primo
