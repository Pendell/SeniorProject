#include "node.h"


/******************************* AST NODE ************************************/
const char* ASTNode::getNodeType(){
    return "ASTNode";
}

bool ASTNode::equals (ASTNode* node){
    printf("Inside ASTNode equals()\n");
    return (getNodeType() == node->getNodeType());
}

void ASTNode::accept(Visitor* v){
    v->visit(this);
}


/******************************* STMT NODE ***********************************/
const char* StmtNode::getNodeType(){
    return "StmtNode";
}

//virtual bool compareNode(ASTNode* node){
bool StmtNode::equals(ASTNode* node){
    printf("Inside StmtNode equals()\n");
    return (getNodeType() == node->getNodeType());
}


/******************************* EXPR NODE ***********************************/
int ExprNode::getVal(){}
const char* ExprNode::getNodeType(){
    return "ExprNode";
}

bool ExprNode::equals(ASTNode* node){
    return (getNodeType() == node->getNodeType());
}

/******************************* DECL NODE ***********************************/
DeclNode::DeclNode() {
    lhs = nullptr;
    rhs = nullptr;
}
DeclNode::~DeclNode() {}

const char* DeclNode::getNodeType(){
    return "DeclNode";
}

bool DeclNode::isLast(){
    return (lhs == nullptr && rhs==nullptr);
}

bool DeclNode::equals(ASTNode* node){
    printf("Inside DeclNode equals()\n");        
    /* CHECK NODE TYPES **********************************/
    if (getNodeType() != node->getNodeType()) {
        return false;
    }
    
    DeclNode* node_casted = dynamic_cast<DeclNode*>(node);  
    
    if(isLast() && node_casted->isLast()) {
        return true;
    } else {
        if(rhs != nullptr) {
            return (lhs->equals(node_casted->lhs) && rhs->equals(node_casted->rhs));
        } else {
            return (lhs->equals(node_casted->lhs));
        }
    }
}

void DeclNode::accept(Visitor* v){
    
    v->visit(this);
    
    if(lhs)
        lhs->accept(v);
    if(rhs)
        rhs->accept(v);
}


/***************************** INTEGER NODE **********************************/
// Constructor
IntegerNode::IntegerNode(int v) : value(v) {}

// Print out the value for debugging purposes.
int IntegerNode::getVal(){
    return value;
}

// Return the type of node this is
const char* IntegerNode::getNodeType(){
    return "IntegerNode";
}


bool IntegerNode::equals(ASTNode* node){
    printf("Inside IntegerNode equals()\n");
    if (getNodeType() != node->getNodeType()) {
        return false;
    } else {
        IntegerNode* node_casted = dynamic_cast<IntegerNode*>(node);
        if (getVal() == node_casted->getVal()) {
            printf("\tIntegerNodes Equal. Returning true\n");
            return true;
        } else {
            printf("\tIntegerNodes not Equal. Returning false");
            return false;
        }
    }
}

void IntegerNode::accept(Visitor* v){
    v->visit(this);
}



/***************************** RETURN NODE ***********************************/
// Constructors
ReturnNode::ReturnNode() : retVal(nullptr) {}
ReturnNode::ReturnNode(ExprNode* rv) : retVal(rv) {}

const char* ReturnNode::getNodeType(){
    return "ReturnNode";
}

int ReturnNode::getVal(){
    return retVal->getVal();
}


bool ReturnNode::equals(ASTNode* node){
    printf("Inside ReturnNode equals()\n");
    if (getNodeType() != node->getNodeType())
        return false;
    
    else {
        ReturnNode* node_casted = dynamic_cast<ReturnNode*>(node);
        // Check Both Null
        if(!retVal && !node_casted->retVal) {
            return true;
            
        // Check Both not null, if so, compare children
        } else if (retVal && node_casted->retVal) {
            
            if (retVal->equals(node_casted->retVal)){
                printf("Return Nodes Equal. Returning true\n");
                return true;
            } else {
                printf("Return Nodes not Equal. Returning false\n");
                return false;
            }
            
        } else {
            printf("Return Nodes not Equal. Returning false\n");
            return false;
        }
    }
}

void ReturnNode::accept(Visitor* v){
    v->visit(this);
    if (retVal)
        retVal->accept(v);
}


/***************************** VARDECL NODE **********************************/

VarDeclNode::VarDeclNode(const char* ty, const char* na) : type(ty), name(na) {
    value = nullptr;
}

VarDeclNode::VarDeclNode(const char* ty, const char* na, ExprNode* va) :
                        type(ty), name(na), value(va) { }

VarDeclNode::~VarDeclNode(){
    
}
// Returns the typing of the data stored -- NOT the type of the node
const char* VarDeclNode::getType(){
    return type;
}

const char* VarDeclNode::getName(){
    return name;
}

int VarDeclNode::getVal(){
    return value->getVal();
}

const char* VarDeclNode::getNodeType(){
    return "VarDeclNode";
}

bool VarDeclNode::equals(ASTNode* node) {
    printf("Inside VarDeclNode equals()\n");
    if(getNodeType() != node->getNodeType()) {
        return false;
    }
    
    VarDeclNode* node_casted = dynamic_cast<VarDeclNode*>(node);
    printf("\tVarDeclNode -- THIS node & THAT node info:\n");
    printf("\tTHIS node type and name: %s %s\n", getType(), getName());
    printf("\tTHAT node type and name: %s %s\n", node_casted->getType(), node_casted->getName());
    
   
    
    // compare types, names, then values
    if (strcmp(getType(), node_casted->getType()) != 0) {
        printf("\t\u001b[35mVarDeclNode Type mismatch\u001b[0m: THIS type %s != THAT type %s\n", getType(), node_casted->getType());
        return false;
    } else if (strcmp(getName(), node_casted->getName()) != 0) {
        printf("\t\u001b[35mVarDeclNode Name mismatch\u001b[0m: THIS name %s != THAT name %s\n", getName(), node_casted->getName());
        return false;
    } else { // Compare values
    
        if (value == nullptr && node_casted->value == nullptr) {
            return true;
            
        } else if (value == nullptr ^ node_casted->value == nullptr) {
            
            if(value == nullptr) {
                printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS nullptr != THAT %s", node_casted->getVal());
            } else if (node_casted == nullptr){
                printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS %s != THAT nullptr",getVal());
            }
            
        return false;
        } else if (!value->equals(node_casted->value)) {
            printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS %s != THAT %s",getVal(), node_casted->getVal());
        } else {
            return true;
        }
    }
}

void VarDeclNode::accept(Visitor* v){
    v->visit(this);
    if (value)
        value->accept(v);
}


/***************************** FUNCDECL NODE *********************************/
FuncDeclNode::FuncDeclNode(const char* ty, const char* na, StmtList* stl, SymbolTable* st) :
                            type(ty), name(na), statements(stl), SymTable(st){ }
    
const char* FuncDeclNode::getType(){
    return type;
}

const char* FuncDeclNode::getName(){
    return name;
}
    
const char* FuncDeclNode::getNodeType(){
    return "FuncDeclNode";
}

bool FuncDeclNode::equals(ASTNode* node) {
    printf("Inside FuncDeclNode equals()\n");
    if (getNodeType() != node->getNodeType()) {
        return false;
    } else {
        
        FuncDeclNode* node_casted = dynamic_cast<FuncDeclNode*>(node);
        
        // Simple Comparisons -- Types and names
        if (strcmp(getType(), node_casted->getType()) != 0) {      
            printf("\t\u001b[35mType mismatch\u001b[0m: THIS type (%s) != THAT type (%s)\n", getType(), node_casted->getType());
            return false;
        } else if (strcmp(getName(), node_casted->getName()) != 0) { 
            printf("\t\u001b[35mName mismatch\u001b[0m: THIS name (%s) != THAT name (%s)\n", getName(), node_casted->getName());
            return false;
        }
        
        // Checking statement lists
        if (statements->empty() ^ node_casted->statements->empty()){
            printf("\t\u001b[35mStatement List mismatch\u001b[0m: ");
            if(statements->empty())
                printf("This statement list empty\n");
            else
                printf("Node_casted statement list empty\n");
            return false;
        }
        
        // Parameter Checking ...
        // Parameters themselves aren't implemented so ...
        // NOT YET IMPLEMENTED
        
        // Symbol Table Checking ...
        // NOT YET IMPLEMENTED
        
        
        
        // Now we compare statement lists, which is actually
        // just a list of nodes to run equals() on.
        StmtList::iterator it1 = statements->begin();
        StmtList::iterator it2 = node_casted->statements->begin();
        /* Notes:
         * Iterators point at the objects that make up the thing we're iterating over.
         * So in this case, I think they should all point at StmtNodes of some kind.
         */
         
        int count = 0; 
        
        if (it1 == statements->end()  && it2 == node_casted->statements->end()) {
            return true;
        } else if (it1 == statements->end() ^ it2 == node_casted->statements->end()) {
            return false;
        }
        
        // Check Statement lists now.
        printf("Navigating and comparing Statement Lists...\n");
        while(it1 != statements->end() && it2 != node_casted->statements->end()){
            
            printf("\nNodes at position %d in StatementLists:\n", count);
            
            printf("\t\tTHIS type: %s", (*it1)->getNodeType());
            printf("\tTHAT type: %s\n\n", (*it2)->getNodeType());
            if (!((*it1)->equals(*it2))){
                printf("\t\u001b[35mASTNode Type mismatch\u001b[0m: THIS %s != THAT %s\n", (*it1)->getNodeType(), (*it2)->getNodeType());
                return false;
            } else {
                ++it1;
                ++it2;
                ++count;
            }
        }
        return true;
    }
}

void FuncDeclNode::accept(Visitor* v){
    printf("FuncDeclNode Accepting visitor\n");
    v->visit(this);
    
    // Navigate through statement list
    if (!statements->empty()){
        
        StmtList::iterator it = statements->begin();
        
        while(it != statements->end()) {
            (*it)->accept(v);
            ++it;
            
        }
    }
}

/***************************** PROGRAM NODE *********************************/
ProgramNode::ProgramNode() { 
    start = nullptr;
}
    
const char* ProgramNode::getNodeType() {
    return "ProgramNode";
}

bool ProgramNode::equals(ASTNode* node) {
    printf("Inside ProgramNode equals()\n");
    if (getNodeType() != node->getNodeType())
        return false;
    else {
        ProgramNode* node_casted = dynamic_cast<ProgramNode*>(node);
        
        if (start == nullptr ^ node_casted->start == nullptr)
            return false;
        else
            return start->equals(node_casted->start);
    }
}

void ProgramNode::accept(Visitor* v){
    v->visit(this);
    if (start)
        start->accept(v);
}



















