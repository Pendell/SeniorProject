#include "node.h"

// Constructor
MutateVarNode::MutateVarNode(VarDeclNode* v, ExprNode* val) {
    lhs = v;
    rhs = val;
    printf("LHS type: %s\n", lhs->getNodeType());
    printf("RHS type: %s\n", rhs->getNodeType());
    
    printf("Mutating the variable: %s\n", lhs->getName());
    
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
    return builder.CreateStore(v, NamedValues[name]);
    
}