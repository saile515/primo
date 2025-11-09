#include "./function_definition.hpp"

#include "../log.hpp"
#include <llvm/IR/Verifier.h>

namespace primo::ast
{

llvm::Value *FunctionDefinition::codegen(IRModuleContext &context)
{
    llvm::Function *function =
        llvm::dyn_cast<llvm::Function>(FunctionDeclaration::codegen(context));

    for (int i = 0; i < m_parameters.size(); i++)
    {
        auto argument = function->args().begin() + i;
        argument->setName(m_parameters[i].name);

        context.named_values.push(m_parameters[i].name, argument);
    }

    llvm::BasicBlock *body_block =
        llvm::BasicBlock::Create(*context.llvm_context, "entry", function);
    context.builder->SetInsertPoint(body_block);

    if (m_body)
    {
        m_body->codegen(context);
    }

    if (!body_block->getTerminator())
    {
        context.builder->CreateRetVoid();
    }

    std::string verify_error;
    llvm::raw_string_ostream output(verify_error);

    bool invalid = llvm::verifyFunction(*function, &output);

    for (auto &parameter_name : m_parameters)
    {
        context.named_values.pop(parameter_name.name);
    }

    if (invalid)
    {
        context.named_values.pop(m_name);

        output.flush();

        error("Function verification failed: " + verify_error);
    }

    return function;
}

} // namespace primo::ast
