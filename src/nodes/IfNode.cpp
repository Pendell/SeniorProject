#include "node.h"

IfNode::IfNode(BoolExprNode* b, StmtList* t){
    bExpr = b;
    then = t;
    
}

IfNode::~IfNode(){

}

const char* IfNode::getNodeType(){
    return "IfNode";
}

Value* IfNode::codegen(){
    
    Value* v = bExpr->codegen();
    
    if(!v){
        printf("Bad Expression Codegen\n");
        return nullptr;
    }
    
    
    Function* f = builder.GetInsertBlock()->getParent();
    
    BasicBlock* thenblock = BasicBlock::Create(TheContext, "then", f);
    BasicBlock* elseblock = BasicBlock::Create(TheContext, "else");
    
    Value* br = builder.CreateCondBr(v, thenblock, elseblock);
    
    // Generate the then block
    builder.SetInsertPoint(thenblock);
    
    std::list<StmtNode*>::iterator it = then->begin();
    int count = 0;
    while(it != then->end()){
        (*it)->codegen();
        
        ++count;
        ++it;
    }
    
    builder.CreateBr(elseblock);
    thenblock = builder.GetInsertBlock();
    f->getBasicBlockList().push_back(elseblock);
    
    builder.SetInsertPoint(elseblock);
    
       
}