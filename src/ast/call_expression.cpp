#include "./call_expression.hpp"

#include "../log.hpp"
#include <llvm/IR/Function.h>

namespace primo::ast
{

llvm::Value *CallExpression::codegen(IRContext &context)
{
    llvm::Value *callee_value = m_callee->codegen(context);

    if (!callee_value)
    {
        error("CallExpression: callee codegen returned null.");
    }

    llvm::Function *callee_function = llvm::dyn_cast<llvm::Function>(callee_value);

    if (!callee_function)
    {
        error("CallExpression: callee is not a function.");
    }

    std::vector<llvm::Value *> arguments;
    arguments.reserve(m_arguments.size());

    for (auto &argument : m_arguments)
    {
        llvm::Value *argument_value = argument->codegen(context);

        if (!argument_value)
        {
            error("CallExpression: argument codegen returned null.");
        }

        arguments.push_back(argument_value);
    }

    return context.builder->CreateCall(callee_function, llvm::ArrayRef<llvm::Value *>(arguments));
}

} // namespace primo::ast
