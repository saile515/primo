#pragma once

#include "./statement.hpp"

namespace primo::ast
{

class ImportStatement : public Statement
{
  private:
    std::string m_source;
    std::vector<std::string> m_imported_identifiers;

  public:
    ImportStatement(std::string source, std::vector<std::string> imported_identifiers)
        : m_source(std::move(source)), m_imported_identifiers(std::move(imported_identifiers))
    {
    }

    virtual llvm::Value *codegen(IRModuleContext &context);
};

} // namespace primo::ast
