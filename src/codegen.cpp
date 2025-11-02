#include "ast.hpp"

#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/TargetParser/Host.h"
#include <map>
#include <stdexcept>

namespace ast
{

static std::unique_ptr<llvm::LLVMContext> context;
static std::unique_ptr<llvm::IRBuilder<>> builder;
static std::unique_ptr<llvm::Module> module;
static std::map<std::string, llvm::Value *> named_values;

llvm::Value *Node::codegen()
{
    throw "Error: Node::codegen should only be called through sub-classes.";
}

llvm::Value *Identifier::codegen()
{
    auto it = named_values.find(m_name);

    if (it == named_values.end())
    {
        std::string msg = "Unknown identifier: " + m_name;
        throw std::runtime_error(msg);
    }

    return it->second;
}

llvm::Value *StringLiteral::codegen()
{
    return builder->CreateGlobalString(m_value);
}

llvm::Value *CallExpression::codegen()
{
    llvm::Value *callee_val = m_callee->codegen();
    if (!callee_val)
        throw std::runtime_error("CallExpression: callee codegen returned null.");

    llvm::Function *callee_func = llvm::dyn_cast<llvm::Function>(callee_val);
    if (!callee_func)
        throw std::runtime_error("CallExpression: callee is not a function.");

    std::vector<llvm::Value *> args_v;
    args_v.reserve(m_arguments.size());
    for (auto &arg : m_arguments)
    {
        llvm::Value *v = arg->codegen();
        if (!v)
            throw std::runtime_error("CallExpression: argument codegen returned null.");
        args_v.push_back(v);
    }

    return builder->CreateCall(callee_func, llvm::ArrayRef<llvm::Value *>(args_v));
}

llvm::Value *ExpressionStatement::codegen()
{
    return m_expression->codegen();
}

llvm::Value *BlockStatement::codegen()
{
    llvm::Value *last = nullptr;
    for (auto &stmt : m_statements)
    {
        last = stmt->codegen();
    }
    return last;
}

llvm::Value *FunctionDeclaration::codegen()
{
    if (!context || !builder || !module)
        throw std::runtime_error(
            "FunctionDeclaration::codegen called before Program::codegen initialised LLVM.");

    std::vector<llvm::Type *> param_types;
    param_types.reserve(m_parameters.size());
    for (size_t i = 0; i < m_parameters.size(); ++i)
    {
        param_types.push_back(llvm::PointerType::getUnqual(*context));
    }

    llvm::FunctionType *ft =
        llvm::FunctionType::get(llvm::Type::getVoidTy(*context), param_types, false);
    llvm::Function *function =
        llvm::Function::Create(ft, llvm::Function::ExternalLinkage, m_name, module.get());

    if (named_values.find(m_name) != named_values.end())
    {
        throw "Error: Function defined twice.";
    }

    named_values[m_name] = function;

    {
        unsigned idx = 0;
        for (auto &arg : function->args())
        {
            if (idx < m_parameters.size())
            {
                arg.setName(m_parameters[idx]);
                auto pit = named_values.find(m_parameters[idx]);
                llvm::Value *old_param_binding = nullptr;
                if (pit != named_values.end())
                    old_param_binding = pit->second;

                named_values[m_parameters[idx]] = &arg;
            }
            ++idx;
        }
    }

    llvm::BasicBlock *bb = llvm::BasicBlock::Create(*context, "entry", function);
    builder->SetInsertPoint(bb);

    if (m_body)
    {
        m_body->codegen();
    }

    if (!bb->getTerminator())
    {
        builder->CreateRetVoid();
    }

    std::string verify_err;
    llvm::raw_string_ostream rso(verify_err);
    if (llvm::verifyFunction(*function, &rso))
    {
        for (auto &pname : m_parameters)
        {
            named_values.erase(pname);
        }
        named_values.erase(m_name);

        rso.flush();
        throw std::runtime_error("Function verification failed: " + verify_err);
    }

    for (auto &pname : m_parameters)
    {
        named_values.erase(pname);
    }

    return function;
}

llvm::Value *Program::codegen()
{
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::make_unique<llvm::IRBuilder<>>(*context);
    module = std::make_unique<llvm::Module>("ast_module", *context);

    named_values.clear();

	llvm::FunctionType *printfTy = llvm::FunctionType::get(builder->getInt32Ty(),
                                               { llvm::PointerType::getUnqual(*context) }, true);
	llvm::Function* printfFunc = 
        llvm::Function::Create(printfTy, llvm::Function::ExternalLinkage, "printf", module.get());

	named_values["printf"] = printfFunc;

    if (m_block)
    {
        m_block->codegen();
    }

	llvm::Triple targetTriple(llvm::sys::getDefaultTargetTriple());

	llvm::InitializeAllTargetInfos();
	llvm::InitializeAllTargets();
	llvm::InitializeAllTargetMCs();
	llvm::InitializeAllAsmParsers();
	llvm::InitializeAllAsmPrinters();

	std::string error;
	auto target = llvm::TargetRegistry::lookupTarget(targetTriple, error);

	auto cpu = "generic";
	auto features = "";
	
	llvm::TargetOptions opt;
	auto targetMachine = target->createTargetMachine(targetTriple, cpu, features, opt, llvm::Reloc::PIC_);

	module->setDataLayout(targetMachine->createDataLayout());
	module->setTargetTriple(targetTriple);

	auto Filename = "output.s";
	std::error_code EC;
	llvm::raw_fd_ostream dest(Filename, EC, llvm::sys::fs::OF_None);

	llvm::legacy::PassManager pass;
	auto FileType = llvm::CodeGenFileType::AssemblyFile;

	targetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType);

	pass.run(*module);
	dest.flush();

    return nullptr;
}

} // namespace ast
