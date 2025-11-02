#include "./program.hpp"

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

llvm::Value *Program::codegen(IRContext &context)
{
    llvm::FunctionType *printf_type = llvm::FunctionType::get(
        context.builder->getInt32Ty(), {llvm::PointerType::getUnqual(*context.llvm_context)}, true);
    llvm::Function *printf_function = llvm::Function::Create(
        printf_type, llvm::Function::ExternalLinkage, "printf", context.module.get());

    context.named_values.push("printf", printf_function);

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

    std::string error;
    auto target = llvm::TargetRegistry::lookupTarget(target_triple, error);

    auto cpu = "generic";
    auto features = "";

    llvm::TargetOptions opt;
    auto targetMachine =
        target->createTargetMachine(target_triple, cpu, features, opt, llvm::Reloc::PIC_);

    context.module->setDataLayout(targetMachine->createDataLayout());
    context.module->setTargetTriple(target_triple);

    auto filename = "output.s";
    std::error_code error_code;
    llvm::raw_fd_ostream destination(filename, error_code, llvm::sys::fs::OF_None);

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
