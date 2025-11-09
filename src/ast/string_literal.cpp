#include "./string_literal.hpp"

namespace primo::ast
{

std::string unescape_string(const std::string &input)
{
    std::string result;
    result.reserve(input.size());

    for (size_t i = 0; i < input.size(); i++)
    {
        char character = input[i];
        if (character == '\\' && i + 1 < input.size())
        {
            char next = input[++i];

            switch (next)
            {
            case 'n':
                result += '\n';
                break;
            case 'r':
                result += '\r';
                break;
            case 't':
                result += '\t';
                break;
            case 'b':
                result += '\b';
                break;
            case 'f':
                result += '\f';
                break;
            case 'a':
                result += '\a';
                break;
            case 'v':
                result += '\v';
                break;
            case '\\':
                result += '\\';
                break;
            case '\'':
                result += '\'';
                break;
            case '\"':
                result += '\"';
                break;
            case '0':
            {
                int value = 0;

                size_t j = i;

                while (j < input.size() && j < i + 3 && isdigit(input[j]) && input[j] < '8')
                {
                    value = value * 8 + (input[j] - '0');

                    j++;
                }

                result += static_cast<char>(value);

                i = j - 1;

                break;
            }
            case 'x':
            {
                int value = 0;

                size_t j = i + 1;

                while (j < input.size() && isxdigit(input[j]))
                {
                    value *= 16;

                    if (isdigit(input[j]))
                    {
                        value += input[j] - '0';
                    }
                    else
                    {
                        value += std::tolower(input[j]) - 'a' + 10;
                    }

                    j++;
                }

                result += static_cast<char>(value);

                i = j - 1;

                break;
            }
            default:
                result += next;
                break;
            }
        }
        else
        {
            result += character;
        }
    }

    return result;
}

llvm::Value *StringLiteral::codegen(IRModuleContext &context)
{
    return context.builder->CreateGlobalString(unescape_string(m_value));
}

} // namespace primo::ast
