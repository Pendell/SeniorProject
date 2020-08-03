#include "node.h"

GlobalVarDeclNode::GlobalVarDeclNode(const char* ty, const char* na, ExprNode* va = nullptr) : VarDeclNode(ty, na, va){
    type = ty;
    name = na;
    value = va;
}

GlobalVarDeclNode::~GlobalVarDeclNode() {

}

const char* GlobalVarDeclNode::getType() {
    return type;
}

const char* GlobalVarDeclNode::getName() {
    return name;
}

int GlobalVarDeclNode::getVal() {
    return 1;
}


const char* GlobalVarDeclNode::getNodeType() {
    return "GlobalVarDeclNode";
}

bool GlobalVarDeclNode::equals(ASTNode* node) {
    printf("GlobalVarDeclNode equals not yet implemented; returning false\n");
    return false;
}
 

Value* GlobalVarDeclNode::codegen() {
    GlobalVariable* gv = new GlobalVariable(*TheModule, getLLVMType(), false, GlobalVariable::CommonLinkage, 0, Twine("globalvar"));
    gv->setInitializer((Constant*)value->codegen());
}

Type* GlobalVarDeclNode::getLLVMType() {
    if (strcmp(getType(), "int") == 0) {
        return Type::getInt32Ty(TheContext);
    }
}