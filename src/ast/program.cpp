#include "./program.hpp"

#include "../log.hpp"
#include "llvm/ADT/StringRef.h"
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
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"

namespace ast
{

void add_libc_function(IRContext &context, std::string name)
{
    llvm::FunctionType *type = llvm::FunctionType::get(
        context.builder->getInt32Ty(), {llvm::PointerType::getUnqual(*context.llvm_context)}, true);
    llvm::Function *function =
        llvm::Function::Create(type, llvm::Function::ExternalLinkage, name, context.module.get());

    context.named_values.push(name, function);
}

llvm::Value *Program::codegen(IRContext &context)
{
    add_libc_function(context, "printf");

    if (m_block)
    {
        m_block->codegen(context);
    }

    llvm::Triple target_triple(llvm::sys::getDefaultTargetTriple());

    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargets();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllAsmParsers();
    llvm::InitializeAllAsmPrinters();

    std::string target_lookup_error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, target_lookup_error);

    if (!target_lookup_error.empty())
    {
        error(target_lookup_error);
    }

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions options;
    auto targetMachine =
        target->createTargetMachine(target_triple, cpu, features, options, llvm::Reloc::PIC_);

    context.module->setDataLayout(targetMachine->createDataLayout());
    context.module->setTargetTriple(target_triple);

    auto filename = "output.s";
    std::error_code error_code;
    llvm::raw_fd_ostream destination(filename, error_code, llvm::sys::fs::OF_None);

    if (error_code)
    {
        error(error_code.message());
    }

    llvm::legacy::PassManager pass;
    auto file_type = llvm::CodeGenFileType::AssemblyFile;

    targetMachine->addPassesToEmitFile(pass, destination, nullptr, file_type);

    pass.run(*context.module);
    destination.flush();

    return nullptr;
}

void Program::codegen()
{
    IRContext context{};

    context.llvm_context = std::make_unique<llvm::LLVMContext>();
    context.builder = std::make_unique<llvm::IRBuilder<>>(*context.llvm_context);
    context.module = std::make_unique<llvm::Module>("program", *context.llvm_context);
    context.named_values = {};

    codegen(context);
}

} // namespace ast
