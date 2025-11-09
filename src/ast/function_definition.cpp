#include "./function_definition.hpp"

#include "../log.hpp"
#include <llvm/IR/Verifier.h>

namespace primo::ast
{

llvm::Value *FunctionDefinition::codegen(IRModuleContext &context)
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

    for (int i = 0; i < m_parameters.size(); i++)
    {
        auto argument = function->args().begin() + i;
        argument->setName(m_parameters[i]);

        context.named_values.push(m_parameters[i], argument);
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
        context.named_values.pop(parameter_name);
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
