#include "node.h"

// Constructor
MutateVarNode::MutateVarNode(VarDeclNode* v, ExprNode* val) {
    lhs = v;
    rhs = val;
    
}

// Destructor
MutateVarNode::~MutateVarNode() {
    // Do nothing
}

const char* MutateVarNode::getNodeType(){
    return "MutateVarNode";
}

Value* MutateVarNode::codegen() {
    
    // Grab whats in the rhs.
    Value* v = rhs->codegen();
    std::string name(lhs->getName());
    return builder.CreateStore(v, symref->allocations[name]);
    
}