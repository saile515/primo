#include "./node.hpp"

#include "../log.hpp"

namespace ast
{

llvm::Value *Node::codegen(IRContext &context)
{
    error("Node::codegen should only be called through sub-classes.");
    return nullptr;
}

} // namespace ast
