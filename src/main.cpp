#include <fstream>

#include "ast.hpp"
#include "tokenize.hpp"

int main(int argc, char **argv)
{
    std::ifstream file_stream(argv[1]);

    TokenList tokens = tokenize(file_stream);

    std::unique_ptr<ast::Program> program = ast::parse(tokens);

    llvm::Value *program_ir = program->codegen();
}
