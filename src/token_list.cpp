#include "token_list.hpp"

namespace primo
{

bool Token::is_bracket()
{
    return type == TokenType::BracketRoundOpen || type == TokenType::BracketRoundClose ||
           type == TokenType::BracketCurlyOpen || type == TokenType::BracketCurlyClose ||
           type == TokenType::BracketSquareOpen || type == TokenType::BracketSquareClose ||
           type == TokenType::BracketChevronOpen || type == TokenType::BracketChevronClose;
}

TokenType Token::get_closing_bracket_type(TokenType open_type)
{
    if (open_type == TokenType::BracketRoundOpen)
    {
        return TokenType::BracketRoundClose;
    }

    if (open_type == TokenType::BracketCurlyOpen)
    {
        return TokenType::BracketCurlyClose;
    }

    if (open_type == TokenType::BracketSquareOpen)
    {
        return TokenType::BracketSquareClose;
    }

    if (open_type == TokenType::BracketChevronOpen)
    {
        return TokenType::BracketChevronClose;
    }

    throw "Error: 'open_type' is not a bracket type.";
}

void TokenList::push_back(Token token)
{
    push_back(std::make_unique<Token>(token));
}

int TokenList::find_matching_token(int start_index, TokenType open_type, TokenType close_type)
{
    int open_count = 0;
    int close_count = 0;

    int i = start_index;

    do
    {
        TokenType token_type = this->at(i)->type;

        if (token_type == open_type)
        {
            open_count++;
        }
        else if (token_type == close_type)
        {
            close_count++;
        }

        i++;
    } while (open_count != close_count);

    return i - 1;
}

std::vector<TokenList> TokenList::split(TokenType seperator)
{
    std::vector<TokenList> result = {};

    int i = 0;

    while (i < size())
    {
        int start = i;

        while (i < size() && this->at(i)->type != seperator)
        {
            i++;
        }

        result.push_back(TokenList(begin() + start, begin() + i));

        i++;
    }

    return result;
}

std::vector<TokenList> TokenList::split_statements(TokenType seperator)
{
    std::vector<TokenList> result = {};

    int i = 0;

    while (i < size())
    {
        int start = i;

        while (i < size())
        {
            std::shared_ptr<Token> token = this->at(i);

            if (token->is_bracket())
            {
                i = find_matching_token(i, token->type,
                                        Token::get_closing_bracket_type(token->type)) +
                    1;
                continue;
            }

            if (token->type == seperator)
            {
                break;
            }

            i++;
        }

        result.push_back(TokenList(begin() + start, begin() + i));

        i++;
    }

    return result;
}

} // namespace primo
