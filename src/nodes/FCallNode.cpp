#include "node.h"

FCallNode::FCallNode(const char* c, std::vector<ExprNode*>& a){
    std::string name(c);
    args = a;
    callee = name;
}

FCallNode::~FCallNode(){
    
}

const char* FCallNode::getNodeType(){
    return "FCallNode";
}

Value* FCallNode::codegen() {
    Function* fcallee = TheModule->getFunction(callee);


    if(!fcallee) {
        printf("Bad Function Call: Name %s not found.\n", callee.c_str());
        exit(98);
        return nullptr;
    }

    if(fcallee->arg_size() != args.size()){
        printf("Bad Function Call: Expected %d arguments, %d given.\n", fcallee->arg_size(), args.size());
        exit(99);
        return nullptr;
    }
    
    std::vector<Value* > valvec = {};
    for(int i = 0; i < args.size(); i++){
        valvec.push_back(args.at(i)->codegen());
    }
    
    return builder.CreateCall(fcallee, valvec, callee.c_str());
    
}
