#include "./program.hpp"

#include "./ast.hpp"
#include "./log.hpp"
#include "./tokenize.hpp"
#include "ir_context.hpp"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <filesystem>
#include <fstream>
#include <functional>
#include <unordered_map>

namespace primo
{

void Program::initialize()
{
    m_context.llvm_context = std::make_shared<llvm::LLVMContext>();
    m_context.compile_module = std::bind(&Program::compile_module, this, std::placeholders::_1);
    m_context.exported_values = std::make_shared<
        std::unordered_map<std::string, std::unordered_map<std::string, llvm::Type *>>>();

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
    m_context.target = std::unique_ptr<llvm::TargetMachine>(
        target->createTargetMachine(target_triple, cpu, features, options, llvm::Reloc::PIC_));
}

void Program::compile_module(std::string path)
{
    if (!std::filesystem::exists(path) || std::filesystem::is_directory(path))
    {
        error("File '{}' doesn't exist.", path);
    }

    path = std::filesystem::canonical(path);

    for (auto &module : m_modules)
    {
        if (module->get_path() == path)
        {
            return;
        }
    }

    std::ifstream file_stream(path);

    TokenList tokens = tokenize(file_stream);

    std::unique_ptr<ast::Module> module = ast::parse_module(tokens, path);

    IRModuleContext module_context = m_context;

    module->codegen(module_context);

    m_modules.push_back(std::move(module));
}

} // namespace primo
