#include "stream_parser.hpp"

#include <sstream>

namespace primo
{

StreamParser::StreamParser(std::istream &stream) : _stream(stream)
{
}

char StreamParser::get()
{
    return _stream.get();
}

std::string StreamParser::get_word()
{
    std::stringstream result_stream;

    while (std::isalnum(peek()) || peek() == '_')
    {
        result_stream << get();
    }

    return result_stream.str();
}

std::string StreamParser::get_number()
{
    std::stringstream result_stream;

    while (std::isdigit(peek()) || peek() == '.')
    {
        result_stream << get();
    }

    return result_stream.str();
}

std::string StreamParser::get_string()
{
    // Open '"'
    get();

    std::stringstream result_stream;

extract:
    while (peek() != '"')
    {
        result_stream << get();
    }

    int escape_count = 0;

    while (peek(-(escape_count + 1)) == '\\')
    {
        escape_count++;
    }

    if (escape_count % 2 == 1)
    {
        goto extract;
    }

    // Close '"'
    get();

    return result_stream.str();
}

char StreamParser::peek(int offset)
{
    if (offset == 0)
    {
        return _stream.peek();
    }

    int position = _stream.tellg();

    _stream.seekg(position + offset);

    char result = _stream.peek();

    _stream.seekg(position);

    return result;
}

bool StreamParser::is_at_end()
{
    return peek() == EOF;
}

} // namespace primo
