#pragma once

#include "token_list.hpp"
#include "llvm/IR/DerivedTypes.h"
#include <memory>
#include <string>
#include <vector>

namespace ast
{

class Node
{
  public:
    virtual llvm::Value *codegen();
};

class Expression : public Node
{
};

class Statement : public Node
{
};

class Identifier : public Expression
{
  private:
    std::string m_name;

  public:
    Identifier(std::string name) : m_name(name)
    {
    }

    virtual llvm::Value *codegen();
};

class StringLiteral : public Expression
{
  private:
    std::string m_value;

  public:
    StringLiteral(std::string value) : m_value(value)
    {
    }

    virtual llvm::Value *codegen();
};

class CallExpression : public Expression
{
  private:
    std::unique_ptr<Expression> m_callee;
    std::vector<std::unique_ptr<Expression>> m_arguments;

  public:
    CallExpression(std::unique_ptr<Expression> callee,
                   std::vector<std::unique_ptr<Expression>> arguments)
        : m_callee(std::move(callee)), m_arguments(std::move(arguments)) {};

    virtual llvm::Value *codegen();
};

class ExpressionStatement : public Statement
{
  private:
    std::unique_ptr<Expression> m_expression;

  public:
    ExpressionStatement(std::unique_ptr<Expression> expression)
        : m_expression(std::move(expression)) {};

    virtual llvm::Value *codegen();
};

class BlockStatement : public Statement
{
  private:
    std::vector<std::unique_ptr<Statement>> m_statements;

  public:
    BlockStatement(std::vector<std::unique_ptr<Statement>> statements)
        : m_statements(std::move(statements))
    {
    }

    virtual llvm::Value *codegen();
};

class FunctionDeclaration : public Statement
{
  private:
    std::string m_name;
    std::vector<std::string> m_parameters;
    std::unique_ptr<BlockStatement> m_body;

  public:
    FunctionDeclaration(std::string name, std::vector<std::string> parameters,
                        std::unique_ptr<BlockStatement> body)
        : m_name(std::move(name)), m_parameters(std::move(parameters)), m_body(std::move(body))
    {
    }

    virtual llvm::Value *codegen();
};

class Program : public Node
{
  private:
    std::unique_ptr<BlockStatement> m_block;

  public:
    Program(std::unique_ptr<BlockStatement> block) : m_block(std::move(block))
    {
    }

    virtual llvm::Value *codegen();
};

std::unique_ptr<Program> parse(TokenList &tokens);

}; // namespace ast
