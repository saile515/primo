#pragma once

#include "./ast/module.hpp"
#include "token_list.hpp"

namespace primo::ast
{

std::unique_ptr<Module> parse_module(TokenList &tokens, std::string path);

}; // namespace primo::ast
