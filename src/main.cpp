#include <fstream>

#include "ast.hpp"
#include "tokenize.hpp"

int main(int argc, char **argv)
{
    std::ifstream file_stream(argv[1]);

    primo::TokenList tokens = primo::tokenize(file_stream);

    std::unique_ptr<primo::ast::Program> program = primo::ast::parse(tokens);

    program->codegen();
}
