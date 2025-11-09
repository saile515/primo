#include "ast.hpp"

#include "./ast/block_statement.hpp"
#include "./ast/call_expression.hpp"
#include "./ast/export_statement.hpp"
#include "./ast/expression.hpp"
#include "./ast/expression_statement.hpp"
#include "./ast/function_declaration.hpp"
#include "./ast/function_definition.hpp"
#include "./ast/identifier.hpp"
#include "./ast/import_statement.hpp"
#include "./ast/string_literal.hpp"
#include <memory>

namespace primo::ast
{

std::unique_ptr<BlockStatement> parse_block(TokenList &tokens);
std::unique_ptr<Expression> parse_expression(TokenList &tokens);
std::unique_ptr<Statement> parse_statement(TokenList &tokens);

std::unique_ptr<ImportStatement> parse_import_statement(TokenList &tokens)
{
    int identifiers_end =
        tokens.find_matching_token(1, TokenType::BracketCurlyOpen, TokenType::BracketCurlyClose);

    std::vector<TokenList> identifier_lists =
        TokenList(tokens.begin() + 2, tokens.begin() + identifiers_end).split(TokenType::Comma);

    std::vector<std::string> identifiers{};

    for (auto &list : identifier_lists)
    {
        if (list.size() == 0)
        {
            continue;
        }

        identifiers.push_back(list.at(0)->value.value());
    }

    std::string source = tokens.at(identifiers_end + 2)->value.value();

    return std::make_unique<ImportStatement>(source, identifiers);
}

std::unique_ptr<ExportStatement> parse_export_statement(TokenList &tokens)
{
    TokenList statement_tokens(tokens.begin() + 1, tokens.end());
    std::unique_ptr<Statement> statement = parse_statement(statement_tokens);

    std::string name = statement_tokens.at(1)->value.value();

    return std::make_unique<ExportStatement>(name, std::move(statement));
}

std::vector<FunctionParameter> parse_function_parameters(TokenList &tokens)
{
    std::vector<FunctionParameter> result{};

    for (auto &token_list : tokens.split(TokenType::Comma))
    {
        std::string name = token_list.at(0)->value.value();
        std::string type = token_list.at(2)->value.value();

        result.push_back(FunctionParameter{name = name, type = type});
    }

    return result;
}

std::unique_ptr<FunctionDefinition> parse_function_definition(TokenList &tokens)
{
    std::string name = tokens.at(1)->value.value();

    int parameter_end =
        tokens.find_matching_token(2, TokenType::BracketRoundOpen, TokenType::BracketRoundClose);

    TokenList parameter_tokens = TokenList(tokens.begin() + 3, tokens.begin() + parameter_end);

    std::vector<FunctionParameter> parameters = parse_function_parameters(parameter_tokens);

    std::string return_type = tokens.at(parameter_end + 2)->value.value();

    int body_end = tokens.find_matching_token(parameter_end + 3, TokenType::BracketCurlyOpen,
                                              TokenType::BracketCurlyClose);

    TokenList body_tokens(tokens.begin() + parameter_end + 4, tokens.begin() + body_end);

    return std::make_unique<FunctionDefinition>(name, parameters, return_type,
                                                parse_block(body_tokens));
}

std::unique_ptr<FunctionDeclaration> parse_function_declaration(TokenList &tokens)
{
    std::string name = tokens.at(2)->value.value();

    int parameter_end =
        tokens.find_matching_token(3, TokenType::BracketRoundOpen, TokenType::BracketRoundClose);

    TokenList parameter_tokens = TokenList(tokens.begin() + 4, tokens.begin() + parameter_end);

    std::vector<FunctionParameter> parameters = parse_function_parameters(parameter_tokens);

    std::string return_type = tokens.at(parameter_end + 2)->value.value();

    return std::make_unique<FunctionDeclaration>(name, parameters, return_type);
}

std::unique_ptr<CallExpression> parse_call_expression(TokenList &tokens)
{
    std::string name = tokens.at(0)->value.value();

    int arguments_end =
        tokens.find_matching_token(1, TokenType::BracketRoundOpen, TokenType::BracketRoundClose);

    std::vector<TokenList> argument_token_lists =
        TokenList(tokens.begin() + 2, tokens.begin() + arguments_end)
            .split_statements(TokenType::Comma);

    std::vector<std::unique_ptr<Expression>> arguments{};

    for (auto argument_tokens : argument_token_lists)
    {
        arguments.push_back(parse_expression(argument_tokens));
    }

    return std::make_unique<CallExpression>(std::make_unique<Identifier>(name),
                                            std::move(arguments));
}

std::unique_ptr<StringLiteral> parse_string_literal(TokenList &tokens)
{
    return std::make_unique<StringLiteral>(tokens.at(0)->value.value());
}

std::unique_ptr<Expression> parse_expression(TokenList &tokens)
{
    if (tokens.at(0)->type == TokenType::Identifier)
    {
        return parse_call_expression(tokens);
    }

    if (tokens.at(0)->type == TokenType::LiteralString)
    {
        return parse_string_literal(tokens);
    }

    return nullptr;
}

std::unique_ptr<Statement> parse_statement(TokenList &tokens)
{
    if (tokens.at(0)->type == TokenType::KeywordImport)
    {
        return std::unique_ptr<Statement>(parse_import_statement(tokens));
    }

    if (tokens.at(0)->type == TokenType::KeywordExport)
    {
        return std::unique_ptr<Statement>(parse_export_statement(tokens));
    }

    if (tokens.at(0)->type == TokenType::KeywordFunc)
    {
        return std::unique_ptr<Statement>(parse_function_definition(tokens));
    }

    if (tokens.at(0)->type == TokenType::KeywordDeclare)
    {
        return std::unique_ptr<Statement>(parse_function_declaration(tokens));
    }

    return std::unique_ptr<Statement>(
        std::make_unique<ExpressionStatement>(parse_expression(tokens)));
}

std::unique_ptr<BlockStatement> parse_block(TokenList &tokens)
{
    std::vector<std::unique_ptr<Statement>> statements = {};

    for (TokenList list : tokens.split_statements(TokenType::SemiColon))
    {
        std::unique_ptr<Statement> statement = parse_statement(list);
        statements.push_back(std::move(statement));
    }

    return std::make_unique<BlockStatement>(std::move(statements));
}

std::unique_ptr<Module> parse_module(TokenList &tokens, std::string path)
{
    TokenList module_block_tokens = TokenList(tokens.begin(), tokens.end() - 1);
    std::unique_ptr<BlockStatement> module_block = parse_block(module_block_tokens);

    return std::make_unique<Module>(path, std::move(module_block));
}

}; // namespace primo::ast
