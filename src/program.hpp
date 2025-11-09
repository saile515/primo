#pragma once

#include "./ast/module.hpp"

namespace primo
{

class Program
{
  private:
    std::vector<std::unique_ptr<ast::Module>> m_modules;
    IRContext m_context;

  public:
    void initialize();
    void compile_module(std::string path);
};

} // namespace primo
