#include "node.h"

DoWhileNode::DoWhileNode(StmtList* b, char l, BoolExprNode* e = nullptr){
    
    exit_cond = e;
    body = b;
    loop = l;
    
}

DoWhileNode::~DoWhileNode(){
    
}

const char* DoWhileNode::getNodeType(){
    return "DoWhileNode";
}

Value* DoWhileNode::codegen(){
    if(loop == 'w'){
        return whilecodegen();
    }
    if(loop == 'd') {
        return dowhilecodegen();
    }
}

Value* DoWhileNode::whilecodegen(){
    
    BasicBlock* cond = BasicBlock::Create(TheContext, "cond", fref);
    
    builder.CreateBr(cond);
    builder.SetInsertPoint(cond);
    
    // Branched to condition
    // inserting in condition
    Value* exit_val = exit_cond->codegen();
    
    BasicBlock* on_true = BasicBlock::Create(TheContext, "true", fref);
    BasicBlock* on_false = BasicBlock::Create(TheContext, "false", fref);
    
    builder.CreateCondBr(exit_val, on_true, on_false);
    
    // Branch to true
    // Insert in true
    builder.SetInsertPoint(on_true);
    
    StmtList::iterator it = body->begin();
    while(it != body->end()){
        (*it)->codegen();
        it++;
    } // codegen the body of the loop
    // Branch back to the conditional
    builder.CreateBr(cond);
    
    builder.SetInsertPoint(on_false);

}

Value* DoWhileNode::dowhilecodegen(){
    
    BasicBlock* on_true = BasicBlock::Create(TheContext, "true", fref);
    builder.CreateBr(on_true);
    
    // Branch to true
    // Insert in true
    
    builder.SetInsertPoint(on_true);
    
    StmtList::iterator it = body->begin();
    while(it != body->end()){
        (*it)->codegen();
        it++;
    } // codegen the body of the loop
    
    // Branch to the conditional
    BasicBlock* cond = BasicBlock::Create(TheContext, "cond", fref);
    builder.CreateBr(cond);
    
    builder.SetInsertPoint(cond);
    
    
    Value* exit_val = exit_cond->codegen();
    
    
    BasicBlock* on_false = BasicBlock::Create(TheContext, "false", fref);
    builder.CreateCondBr(exit_val, on_true, on_false);
    
    builder.SetInsertPoint(on_false);
    
}