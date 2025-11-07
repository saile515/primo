#pragma once

#include "./ast/program.hpp"
#include "token_list.hpp"

namespace primo::ast
{

std::unique_ptr<Program> parse(TokenList &tokens);

}; // namespace primo::ast
