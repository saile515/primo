#pragma once

#include <istream>
#include <sstream>
#include <string>

class StreamParser {
	private:
		std::istream& _stream;

	public:
		StreamParser(std::istream& stream) : _stream(stream) {}

		char get() {
			return _stream.get();
		}

		std::string get_word() {
			std::stringstream result_stream;

			while (std::isalnum(peek()) || peek() == '_'){
				result_stream << get();
			}

			return result_stream.str();
		}

		std::string get_number() {
			std::stringstream result_stream;

			while (std::isdigit(peek()) || peek() == '.'){
				result_stream << get();
			}

			return result_stream.str();
		}

		std::string get_string() {
			// Open '"'
			get();

			std::stringstream result_stream;

extract:
			while (peek() != '"'){
				result_stream << get();
			}

			int escape_count = 0;

			while (peek(-(escape_count+1)) == '\\') {
				escape_count++;
			}

			if (escape_count % 2 == 1) {
				goto extract;
			}

			// Close '"'
			get();

			return result_stream.str();
		}

		char peek(int offset = 0) {
			if (offset == 0) {
				return _stream.peek();
			}

			int position = _stream.tellg();

			_stream.seekg(position + offset);

			char result = _stream.peek();

			_stream.seekg(position);

			return result;
		}

		bool is_at_end() {
			return peek() == EOF;
		}
};
