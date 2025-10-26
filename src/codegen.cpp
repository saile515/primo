#include "ast.hpp"

#include <sstream>

namespace ast
{

std::string Node::codegen()
{
    return "";
}

std::string Identifier::codegen()
{
    return m_name;
}

std::string StringLiteral::codegen()
{
    return '"' + m_value + '"';
}

std::string CallExpression::codegen()
{
    std::stringstream result_stream;

    result_stream << m_callee->codegen();

    result_stream << '(';

    for (auto &argument : m_arguments)
    {
        result_stream << argument->codegen();
    }

    result_stream << ')';

    return result_stream.str();
}

std::string ExpressionStatement::codegen()
{
    return m_expression->codegen();
}

std::string BlockStatement::codegen()
{
    std::stringstream result_stream;

    for (auto &statement : m_statements)
    {
        result_stream << statement->codegen() << ';';
    }

    return result_stream.str();
}

std::string FunctionDeclaration::codegen()
{
    std::stringstream result_stream;

    result_stream << "func " << m_name << '(';

    for (auto &parameter : m_parameters)
    {
        result_stream << parameter;

        if (parameter != *m_parameters.end())
        {
            result_stream << ',';
        }
    }

    result_stream << ") {" << m_body->codegen() << '}';

    return result_stream.str();
}

std::string Program::codegen()
{
    return m_block->codegen();
}
} // namespace ast
