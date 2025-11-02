#include "./node.hpp"

namespace ast
{

llvm::Value *Node::codegen(IRContext &context)
{
    throw "Error: Node::codegen should only be called through sub-classes.";
}

} // namespace ast
