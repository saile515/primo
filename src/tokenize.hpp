#pragma once

#include "token_list.hpp"
#include <istream>

namespace primo
{

TokenList tokenize(std::istream &stream);

}
