#pragma once

#include "../ir_context.hpp"

namespace primo::ast
{

class Node
{
  public:
    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
