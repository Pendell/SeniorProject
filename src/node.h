/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */

#ifndef NODE_H
#define NODE_H

#include <cstring>
#include <string>
#include <iostream> 
#include <list>   // for statement lists & params, etc...
#include <map>    // for symbol table

#include "./visitorPattern/visitor.h"

/*  NOTES FROM LADD TALK
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
    
    virtual bool equals (ASTNode* node){
        printf("Inside ASTNode equals()\n");
        return (getNodeType() == node->getNodeType());
    }
    
    //virtual void accept(Visitor* visitor){ }

};

class StmtNode : public ASTNode {
  public:
    virtual const char* getNodeType(){
        return "StmtNode";
    }
    
    //virtual bool compareNode(ASTNode* node){
    virtual bool equals (ASTNode* node){
        printf("Inside StmtNode equals()\n");
        return (getNodeType() == node->getNodeType());
    }
    
};

class ExprNode : public ASTNode {
  public:
    // ExprNode* retVal = NULL;
    
    // return the value of this node, if it has one
    virtual int getVal() {}
    
    virtual const char* getNodeType(){
        return "ExprNode";
    }
    
    //virtual bool compareNode(ASTNode* node){
    virtual bool equals(ASTNode* node){
        // printf("This should not be printed... But I think it will be.\n");
        return (getNodeType() == node->getNodeType());
    }
};

/* A generic Declaration node class so that I can construct the program node
 * list.
 */
class DeclNode : public StmtNode {
  public:
    DeclNode* lhs = nullptr;
    DeclNode* rhs = nullptr;
    
    DeclNode(){
        printf("\nCreating new DeclNode()\n\n");
    }
    
    virtual const char* getNodeType(){
        return "DeclNode";
    }
    
    bool isLast(){
        return (lhs == nullptr && rhs==nullptr);
    }
    
    //bool compareNode(ASTNode* node){ 
    bool equals(ASTNode* node){
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
    
    virtual const char* getNodeType(){
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
    IntegerNode(int v) : value(v) { printf("\nCreating new IntegerNode()\n\n"); };
    
    // Print out the value for debugging purposes.
    int getVal(){
        return value;
    }
    
    // Return the type of node this is
    const char* getNodeType(){
        return "IntegerNode";
    }
    
    
    //bool compareNode(ASTNode* node){
    bool equals(ASTNode* node){
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
};



/* BinaryOpNode
 * A node created for binary operations (+, -, *, /, etc...)
 */
class BinaryOpNode : public ExprNode {
  public:
    int op;         // Operation
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
    
    ReturnNode() : retVal(nullptr) { printf("\nCreating new ReturnNode()\n\n"); }
    
    ReturnNode(ExprNode* rv) : retVal(rv) {printf("Creating new ReturnNode()\n"); }
    
    const char* getNodeType(){
        return "ReturnNode";
    }
    
    int getVal(){
        return retVal->getVal();
    }
    
    
    bool equals(ASTNode* node){
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
};


/* VariableDeclarationNode
 * for creating nodes containing variable definitions
 */
class VarDeclNode : public DeclNode {
  public:
    
    const char* type; 
    const char* name;
    
    ExprNode* value = nullptr;
    
    VarDeclNode(const char* ty, const char* na) : type(ty), name(na) {
        std::cout << "VarDeclNode created(No init)!" << std::endl;
        std::cout << "Type: " << type << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << std::endl;
    }
    
    VarDeclNode(const char* ty, const char* na, ExprNode* va) :
                            type(ty), name(na), value(va) {
        std::cout << "VarDeclNode created(Init)!" << std::endl;
        std::cout << "Type: " << type << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << "Value: " << value->getVal() << std::endl;
        std::cout << std::endl;
                               
    }
    
    // Returns the typing of the data stored -- NOT the type of the node
    const char* getType(){
        return type;
    }
    
    const char* getName(){
        return name;
    }
    
    int getVal(){
        return value->getVal();
    }
    
    const char* getNodeType(){
        return "VarDeclNode";
    }
    
    bool equals(ASTNode* node) {
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
};

// I think I'm going to use this for arguments, not sure yet.
typedef std::list<VarDeclNode*> VarList;



/* FunctionDeclarationNode
 * for creating nodes containing functions
 */
class FuncDeclNode : public DeclNode {
  public:
  
    const char* type;           // return type
    const char* name;           // name of function
    StmtList* statements = new StmtList();       // statements to be executed
    
    //VarList args;             // arguments passed * NOT YET IMPLEMENTED *
    
    SymbolTable* SymTable;
        
    // SymbolTable SymTable
    // Hold a reference to the symbol table here. We need to hold one for 
    // when the codeGen phase comes along.
    
    FuncDeclNode(const char* ty, const char* na, StmtList* stl, SymbolTable* st) :
                            type(ty), name(na), statements(stl), SymTable(st){
       printf("\nCreating new FuncDeclNode()\n\n");
    }
    
    const char* getType(){
        return type;
    }
    
    const char* getName(){
        return name;
    }
        
    const char* getNodeType(){
        return "FuncDeclNode";
    }
    
    bool equals(ASTNode* node) {
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
            while(it1 != statements->end() && it2 != node_casted->statements->end()){
                
                printf("\tNodes at position %d in StatementLists:\n", count);
                
                printf("\t\tNode1 type: %s\n", (*it1)->getNodeType());
                printf("\t\tNode2 type: %s\n\n", (*it2)->getNodeType());
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
    
    
    
    
};

/******************************* PROGRAM *************************************/


class ProgramNode : public StmtNode {
  public:
    DeclNode* start = nullptr;
    
    ProgramNode() { printf("\nCreating new ProgramNode()\n\n"); }
    
    const char* getNodeType(){
        return "ProgramNode";
    }
    
    bool equals(ASTNode* node){
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
};

/*****************************Other functions*********************************/

// I don't need any yet.

#endif
