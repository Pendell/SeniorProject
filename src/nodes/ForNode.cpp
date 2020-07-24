#include "node.h"

ForNode::ForNode(ExprNode* sta, BoolExprNode* be,  StmtList* b){
    start = sta;
    bExpr = be;
    step = nullptr;
    body = b;
    generateCheats();
}

ForNode::ForNode(ExprNode* sta, BoolExprNode* be, StmtNode* ste, StmtList* b){
    start = sta;
    step = ste;
    bExpr = be;
    body = b;
    generateCheats();
}

ForNode::~ForNode(){

}

const char* ForNode::getNodeType(){
    return "ForNode";
}

void ForNode::generateCheats(){
    body->push_back(step);
    cheat = new DoWhileNode(body, 'w', bExpr);
}


Value* ForNode::codegen(){
    start->codegen();
    return cheat->codegen();
    
    
}
