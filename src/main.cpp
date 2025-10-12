#include <fstream>
#include <print>

#include "tokenize.hpp"

int main() {
	std::ifstream file_stream("./input.code");

	std::vector<Token> tokens = tokenize(file_stream);

	for (Token token : tokens) {
		std::print("{}", (int)token.type);
	}
}
