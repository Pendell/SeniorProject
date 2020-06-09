/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */

#ifndef NODE_H
#define NODE_H

#include <iostream> 
#include <list>   // for statement lists & params, etc...
#include <map>    // for symbol table

/*
    Reference to global symbol table with any symbols that are defined.
    FunctionDeclarationList and VariableDeclarationList, potentially.
    
    Program (the root) only has one child, and that child is only
    declarationList.
    
    lhs side - (symbol decl list) has 3 production on the right hand side;
        %empty
        funcDecl declList
        varDecl declList
        
    This is how a block is used:
    
    if() {
        
        *BLOCK*
        
    };
    
    The *true* portion of an if conditional goes to a statement node which could
    be a block.
    
 */


// Variable Declarations
class VarDeclNode;


/* Define the base 'node' objects we will derive everything else from...
 * Everything is empty right now because all nodes will be some form of
 * derivative of these nodes. I think I'll need the codeGen stuff in here
 * when I get llvm working.
 */
class ASTNode { 
  public:
  
    virtual const char* getNodeType(){ 
        return "ASTNode";
    }
    
    // No data being stored in this node, so just compare types
    // This function will be overrode as the Node has more data
    // to compare.
    virtual bool compareNode(ASTNode* node){  
        return (getNodeType() == node->getNodeType());
    }
    
    

};

class StmtNode : public ASTNode {
  public:
    const char* getNodeType(){
        return "StmtNode";
    }
    
    bool compareNode(StmtNode* node){
        return (getNodeType() == node->getNodeType());
    }
    
};

class ExprNode : public ASTNode {
  public:
    ExprNode* value = NULL;
    
    // return the value of this node, if it has one
    virtual int getVal() {}
    
    const char* getNodeType(){
        return "ExprNode";
    }
    
    bool compareNode(ExprNode* node){
        return (getNodeType() == node->getNodeType());
    }
};

/* A generic Declaration node class so that I can construct the program node
 * list.
 */
class DeclNode : public StmtNode {
  public:
    DeclNode* lhs = NULL;
    DeclNode* rhs = NULL;
    
    const char* getNodeType(){
        return "DeclNode";
    }
    
    bool compareNode(DeclNode* node){
        
        /* CHECK NODE TYPES **********************************/
        if (getNodeType() != node->getNodeType()) {
            return false;
        } 
        
        
        /** CHECK IF ANYTHING IS NULL WHERE IT SHOULDN'T BE **/
        if ((lhs == NULL ) && (node->lhs != NULL)) {
            return false;
        }
        
        if ((lhs != NULL) && (node->lhs == NULL)) {
            return false;
        }
        
        if ((rhs == NULL) && (node->rhs != NULL)) {
            return false;
        }
        
        
        /* CHECK CHILDREN NODES ******************************/
        return (lhs->compareNode(node->lhs))
                && (rhs->compareNode(node->rhs));
        
    }
};



// Typedef the lists and tables, because I'm lazy
typedef std::list<StmtNode*> StmtList;
typedef std::list<ExprNode*> ExprList;

typedef std::list<DeclNode*> DeclList;

typedef std::map<std::string, std::string> SymbolTable;


/****************************** EXPRESSIONS **********************************/

/* Block
 * This is basically a collection of statements; so there's only one entry
 * point and one exit point with no branching in a block. So the statements
 * are just basically executed line by line.
 */
class Block : public ExprNode {
    
  public:
    StmtList statements;
    
    Block() {
        std::cout << std::endl;
        std::cout << "Block created!" << std::endl;
        std::cout << std::endl;
    }
    
    const char* getNodeType(){
        return "Block";
    }
    
};

/* IntegerNode
 * for IntegerConstants
 */
class IntegerNode : public ExprNode {
  public:
    int value;
    
    // Constructor
    IntegerNode(int v) : value(v) { };
    
    // Print out the value for debugging purposes.
    int getVal(){
        return value;
    }
    
    const char* getNodeType(){
        return "IntegerNode";
    }
    
    
    bool compareNode(IntegerNode* node){
        if (getNodeType() != node->getNodeType()) {
            return false;
        } else {
            return getVal() == node->getVal();
        }
    }
};



/* BinaryOpNode
 * A node created for binary operations (+, -, *, /, etc...)
 */
class BinaryOpNode : public ExprNode {
  public:
    int op;        // Operation
    ExprNode& lhs;  // left-hand side
    ExprNode& rhs;  // right-hand side
    
    BinaryOpNode(int op, ExprNode& lh, ExprNode& rh) :
        op(op), lhs(lh), rhs(rh) {
            
      std::cout << std::endl << std::endl;
      std::cout << "Creating a BinaryOpNode!" << std::endl;
      std::cout << "The operation is : " << op;
      std::cout << std::endl << std::endl;
    }
    
    const char* getNodeType(){
        return "BinaryOpNode";
    }
};

/* FunctionCallNode
 * This node is created when a function is called.
 */
class FunctionCallNode : public ExprNode {
  public:
  
    const std::string* name;  // Name of function being called
    ExprList args;  // Parameters passed to the function
    
    FunctionCallNode(const std::string* na, ExprList& ar) : name(na), args(ar) {
        std::cout << std::endl << std::endl;
        std::cout << "Creating a MethodCallNode!" << std::endl;
        std::cout << "Though implementation not quite finished" << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    const char* getNodeType(){
        return "FunctionCallNode";
    }

};



/****************************** STATEMENTS ***********************************/

/* ExpressionStatement
 * I'm... not comfortable enough with the information on why we need
 * expression statements, so I won't comment on it here
 *
 * But I'm pretty sure this how they're constructed.
 */
class ExpressionStatementNode : public StmtNode {
  public:
    ExprNode& expr;
    
    ExpressionStatementNode(ExprNode& ex) : expr(ex) {
        std::cout << std::endl << std::endl;
        std::cout << "ExpressionStatement Node created!" << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    const char* getNodeType(){
        return "ExpressionStatmentNode";
    }
    
};

/* ReturnNode
 * for returning expressions
 */
class ReturnNode : public StmtNode {
  public:
    ExprNode* retVal;
    
    ReturnNode(ExprNode* rv) : retVal(rv) {
        std::cout << std::endl << std::endl;
        std::cout << "ReturnNode created!" << std::endl;
        std::cout << "Return Value: " << retVal->getVal() << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    const char* getNodeType(){
        return "ReturnNode";
    }
    
    bool compareNode(ReturnNode* node){
        if (getNodeType() != node->getNodeType())
            return false;
        else {
            return retVal->compareNode(node->retVal);
            
        }
    }
};


/* VariableDeclarationNode
 * for creating nodes containing variable definitions
 */
class VarDeclNode : public DeclNode {
  public:
    
    const std::string* type; 
    const std::string* name;
    
    ExprNode* value;
    
    VarDeclNode(const std::string* ty, const std::string* na) : type(ty), name(na) {
        std::cout << std::endl << std::endl;
        std::cout << "VarDeclNode created(No init)!" << std::endl;
        std::cout << "Type: " << *type << std::endl;
        std::cout << "Name: " << *name << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    VarDeclNode(const std::string* ty, const std::string* na, ExprNode* va) :
                            type(ty), name(na), value(va) {
        std::cout << std::endl << std::endl;
        std::cout << "VarDeclNode created(Init)!" << std::endl;
        std::cout << "Type: " << *type << std::endl;
        std::cout << "Name: " << *name << std::endl;
        std::cout << "Value: " << value->getVal() << std::endl;
        std::cout << std::endl << std::endl;
                               
    }
    
    const std::string getType(){
        return std::string(*type);
    }
    
    const std::string getName(){
        return std::string(*name);
    }
    
    const char* getNodeType(){
        return "VarDeclNode";
    }
    
    bool compareNode(VarDeclNode* node){
        // compare types and names
        if (getType() != node->getType())
            return false;
        else if (getName() != node->getName())
            return false;
        else{ // Compare values
            return value->compareNode(node->value);
        }
    }

};

// I think I'm going to use this for arguments, not sure yet.
typedef std::list<VarDeclNode*> VarList;



/* FunctionDeclarationNode
 * for creating nodes containing functions
 */
class FuncDeclNode : public DeclNode {
  public:
    const std::string* type;   // return type
    const std::string* name;   // name of function
    StmtList* statements;      // statements to be executed
    
    //VarList args;            // arguments passed * NOT YET IMPLEMENTED *
    
    SymbolTable* SymTable;
        
    // SymbolTable SymTable
    // Hold a reference to the symbol table here. We need to hold one for 
    // when the codeGen phase comes along.
    
    FuncDeclNode(const std::string* ty, const std::string* na, StmtList* stl, SymbolTable* st) :
                            type(ty), name(na), statements(stl), SymTable(st){
                                std::cout << std::endl << std::endl;
        std::cout << "FuncDeclNode created!" << std::endl;
        std::cout << "Type: " << *type << std::endl;
        std::cout << "Name: " << *name << std::endl;
        std::cout << std::endl << std::endl;
    }
        
    const char* getNodeType(){
        return "FuncDeclNode";
    }
};

/******************************* PROGRAM *************************************/


class ProgramNode : StmtNode {
  public:
    DeclNode* start = NULL;
    
    ProgramNode() { }
    
    const char* getNodeType(){
        return "ProgramNode";
    }
};

/*****************************Other functions*********************************/

/*
 */
bool equals(ProgramNode* p1, ProgramNode* p2) {
    if(p1->start == NULL || p2->start == NULL) {
        printf("TEST: One or both start symbols are NULL\n");
        return false;
    }

}



#endif