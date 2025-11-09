#include "tokenize.hpp"

#include "stream_parser.hpp"
#include <unordered_map>

namespace primo
{

static const std::unordered_map<std::string, TokenType> keyword_lookup = {
    {"import", TokenType::KeywordImport},
    {"export", TokenType::KeywordExport},
    {"func", TokenType::KeywordFunc},
    {"declare", TokenType::KeywordDeclare}};

static const std::unordered_map<char, TokenType> bracket_lookup = {
    {'(', TokenType::BracketRoundOpen},   {')', TokenType::BracketRoundClose},
    {'{', TokenType::BracketCurlyOpen},   {'}', TokenType::BracketCurlyClose},
    {'[', TokenType::BracketSquareOpen},  {']', TokenType::BracketSquareClose},
    {'<', TokenType::BracketChevronOpen}, {'>', TokenType::BracketChevronClose},
};

TokenList tokenize(std::istream &stream)
{
    StreamParser stream_parser(stream);

    TokenList result;

    while (!stream_parser.is_at_end())
    {
        char next = stream_parser.peek();

        Token token;

        if (std::isalpha(next))
        {
            std::string word = stream_parser.get_word();

            if (keyword_lookup.contains(word))
            {
                token.type = keyword_lookup.at(word);
            }
            else
            {
                token.type = TokenType::Identifier;
                token.value = word;
            }

            result.push_back(token);
            continue;
        }

        if (std::isdigit(next))
        {
            std::string number = stream_parser.get_number();

            token.value = number;

            if (number.contains('.'))
            {
                token.type = TokenType::LiteralFloat;
            }
            else
            {
                token.type = TokenType::LiteralInt;
            }

            result.push_back(token);
            continue;
        }

        if (next == '"')
        {
            std::string string = stream_parser.get_string();

            token.type = TokenType::LiteralString;
            token.value = string;

            result.push_back(token);
            continue;
        }

        if (next == ';')
        {
            stream_parser.get();

            token.type = TokenType::SemiColon;

            result.push_back(token);
            continue;
        }

        if (next == ':')
        {
            stream_parser.get();

            token.type = TokenType::Colon;

            result.push_back(token);
            continue;
        }

        if (bracket_lookup.contains(next))
        {
            TokenType type = bracket_lookup.at(stream_parser.get());
            Token token = {type = type};
            result.push_back(token);
            continue;
        }

        if (std::iswspace(next))
        {
            stream_parser.get();
            continue;
        }

        throw std::format("Error: Unexpected character '{}'.", stream_parser.get());
    }

    {
        Token token;
        token.type = TokenType::EndOfFile;

        result.push_back(token);
    }

    return result;
}

} // namespace primo
