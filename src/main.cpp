#include "./program.hpp"
#include <filesystem>

int main(int argc, char **argv)
{
    std::string entry_path = std::filesystem::canonical(std::filesystem::absolute(argv[1]));

    primo::Program program{};
    program.initialize();

    program.compile_module(entry_path);
}
