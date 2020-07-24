BoolExprNode::BoolExprNode(ExprNode* l, ExprNode* r, char o) {
    lhs = l;
    rhs = r;
    op = o;
}
BoolExprNode::~BoolExprNode() {} 

const char* BoolExprNode::getNodeType() {
    return "BoolExprNode";
}

Value* BoolExprNode::codegen() {
    switch(op){
        case '=':
            return builder.CreateICmpEQ(lhs->codegen(), rhs->codegen(), "cond");
        case '!':
            return builder.CreateICmpNE(lhs->codegen(), rhs->codegen(), "cond");
    }
    
}