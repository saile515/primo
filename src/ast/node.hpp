#pragma once

#include "../hash_stack.hpp"
#include "llvm/IR/DerivedTypes.h"
#include <llvm/IR/IRBuilder.h>

namespace ast
{

struct IRContext
{
    std::unique_ptr<llvm::LLVMContext> llvm_context;
    std::unique_ptr<llvm::IRBuilder<>> builder;
    std::unique_ptr<llvm::Module> module;
    HashStack<std::string, llvm::Value *> named_values;
};

class Node
{
  public:
    virtual llvm::Value *codegen(IRContext &context);
};

} // namespace ast
