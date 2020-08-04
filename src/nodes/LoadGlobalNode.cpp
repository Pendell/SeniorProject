#include "node.h"

LoadGlobalNode::LoadGlobalNode(VarDeclNode* g) {
    global = dynamic_cast<GlobalVarDeclNode*>(g);
}

LoadGlobalNode::~LoadGlobalNode() {

}

const char* LoadGlobalNode::getNodeType() {
    return "LoadGlobalNode";
}

const char* LoadGlobalNode::getName() {
    return global->getName();
}

Value* LoadGlobalNode::codegen() {
    return builder.CreateLoad(global->get());
}