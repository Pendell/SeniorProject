#include "node.h"

UnaryOpNode::UnaryOpNode(const char o, ExprNode* l, const char* i) : op(o), lhs(l), inc(i){
}

UnaryOpNode::~UnaryOpNode(){}


const char* UnaryOpNode::getNodeType(){
    return "UnaryOpNode";
}

Value* UnaryOpNode::codegen(){
    
    // Create a new integer constant that's equal to 1
    // This will be our add-erand or sub-erand.
    Value* one = ConstantInt::get(TheContext, APInt(32, 1, true));
    
    // load whatever is in lhs
    Value* ldptr;
    
    switch(op){
        case '+':
            return builder.CreateAdd(lhs->codegen(), one);
        case '-':
            return builder.CreateSub(lhs->codegen(), one);
    }
    
    
}