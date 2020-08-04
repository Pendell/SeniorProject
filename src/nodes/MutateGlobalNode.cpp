#include "node.h"

MutateGlobalNode::MutateGlobalNode(VarDeclNode* v, ExprNode* val) : rhs(val) {
    lhs = dynamic_cast<GlobalVarDeclNode*>(v);
}

MutateGlobalNode::~MutateGlobalNode() {

}

const char* MutateGlobalNode::getNodeType() {
    return "MutateGlobalNode";
}

Value* MutateGlobalNode::codegen() {
    // Codegen the right side
    Value* v = rhs->codegen();
    std::string name(lhs->getName());
    return builder.CreateStore(v, Globals[name]);
}