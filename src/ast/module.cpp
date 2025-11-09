#include "./module.hpp"

#include "../log.hpp"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/TargetParser/Host.h"
#include <filesystem>

namespace primo::ast
{

void add_libc_function(IRModuleContext &context, std::string name)
{
    llvm::FunctionType *type = llvm::FunctionType::get(
        context.builder->getInt32Ty(), {llvm::PointerType::getUnqual(*context.llvm_context)}, true);
    llvm::Function *function =
        llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, context.module.get());

    context.named_values.push(name, function);
}

llvm::Value *Module::codegen(IRModuleContext &context)
{
    llvm::Triple target_triple(llvm::sys::getDefaultTargetTriple());

    context.name = m_path;
    context.builder = std::make_unique<llvm::IRBuilder<>>(*context.llvm_context);
    context.module = std::make_unique<llvm::Module>(m_path, *context.llvm_context);
    context.module->setDataLayout(context.target->createDataLayout());
    context.module->setTargetTriple(target_triple);
    context.named_values = {};

    add_libc_function(context, "printf");

    if (m_block)
    {
        m_block->codegen(context);
    }

    auto filename = "primo_build/" + std::filesystem::path(m_path).stem().string() + ".s";

    std::error_code error_code;
    llvm::raw_fd_ostream destination(filename, error_code, llvm::sys::fs::OF_None);

    if (error_code)
    {
        error(error_code.message());
    }

    llvm::legacy::PassManager pass;
    auto file_type = llvm::CodeGenFileType::AssemblyFile;

    context.target->addPassesToEmitFile(pass, destination, nullptr, file_type);

    pass.run(*context.module);
    destination.flush();

    return nullptr;
}

const std::string &Module::get_path()
{
    return m_path;
}

} // namespace primo::ast
