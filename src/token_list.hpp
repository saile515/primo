#pragma once

#include <memory>
#include <optional>
#include <vector>

enum TokenType
{
    EndOfFile,
    KeywordFunc,
    Identifier,
    LiteralInt,
    LiteralFloat,
    LiteralString,
    BracketRoundOpen,
    BracketRoundClose,
    BracketCurlyOpen,
    BracketCurlyClose,
    BracketSquareOpen,
    BracketSquareClose,
    BracketChevronOpen,
    BracketChevronClose,
    SemiColon,
    Comma,
};

struct Token
{
    TokenType type;
    std::optional<std::string> value;

    bool is_bracket();

    static TokenType get_closing_bracket_type(TokenType open_type);
};

class TokenList : public std::vector<std::shared_ptr<Token>>
{
  public:
    using std::vector<std::shared_ptr<Token>>::push_back;
    using std::vector<std::shared_ptr<Token>>::vector;

    void push_back(Token token);

    int find_matching_token(int start_index, TokenType open_type, TokenType close_type);

    std::vector<TokenList> split(TokenType seperator);

    std::vector<TokenList> split_statements(TokenType seperator);
};
