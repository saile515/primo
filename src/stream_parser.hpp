#pragma once

#include <istream>
#include <string>

class StreamParser
{
  private:
    std::istream &_stream;

  public:
    StreamParser(std::istream &stream);

    char get();

    std::string get_word();

    std::string get_number();

    std::string get_string();

    char peek(int offset = 0);

    bool is_at_end();
};
