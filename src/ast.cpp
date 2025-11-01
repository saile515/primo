#include "ast.hpp"

namespace ast
{

std::unique_ptr<BlockStatement> parse_block(TokenList &tokens);
std::unique_ptr<Expression> parse_expression(TokenList &tokens);

std::unique_ptr<FunctionDeclaration> parse_function_declaration(TokenList &tokens)
{
    std::string name = tokens.at(1)->value.value();

    int parameter_end =
        tokens.find_matching_token(2, TokenType::BracketRoundOpen, TokenType::BracketRoundClose);

    std::vector<TokenList> parameter_tokens =
        TokenList(tokens.begin() + 3, tokens.begin() + parameter_end).split(TokenType::Comma);

    std::vector<std::string> parameters{};

    for (auto &token_list : parameter_tokens)
    {
        parameters.push_back(token_list.at(0)->value.value());
    }

    int body_end = tokens.find_matching_token(parameter_end + 1, TokenType::BracketCurlyOpen,
                                              TokenType::BracketCurlyClose);

    TokenList body_tokens(tokens.begin() + parameter_end + 2, tokens.begin() + body_end);

    return std::make_unique<FunctionDeclaration>(name, parameters, parse_block(body_tokens));
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
    if (tokens.at(0)->type == TokenType::KeywordFunc)
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

std::unique_ptr<Program> parse(TokenList &tokens)
{
    TokenList program_block_tokens = TokenList(tokens.begin(), tokens.end() - 1);
    std::unique_ptr<BlockStatement> program_block = parse_block(program_block_tokens);

    return std::make_unique<Program>(std::move(program_block));
}

}; // namespace ast
