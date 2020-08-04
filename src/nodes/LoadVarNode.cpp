#include "node.h"


LoadVarNode::LoadVarNode(const char* na) {
    name = na;
}

const char* LoadVarNode::getName(){
    return name;
}

const char* LoadVarNode::getNodeType(){
    return "LoadVarNode";
}

Value* LoadVarNode::codegen() {
    
    std::string name(getName());
    
    return builder.CreateLoad(NamedValues[name]);
    
}