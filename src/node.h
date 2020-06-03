/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */

#ifndef NODE_H
#define NODE_H

#include <iostream> 
#include <list>   // list construction
#include <map>      // map construction (for symbol table)

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
class ASTNode { };
class StmtNode : public ASTNode {
    
};

class ExprNode : public ASTNode {
  public:
    virtual int getVal() { }
};

/* A generic Declaration node class so that I can construct the program node
 * list.
 */
class DeclNode : public StmtNode {
  public:
    DeclNode* lhs = NULL;
    DeclNode* rhs = NULL;
};



// Typedef the lists and tables, because I'm lazy
typedef std::list<StmtNode*> StmtList;
typedef std::list<ExprNode*> ExprList;

typedef std::list<DeclNode*> DeclList;

typedef std::map<std::string, std::string> SymbolTable;


class Program {
  public:
    
};

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
    
    
};

/* IntegerNode
 * for IntegerConstants
 */
class IntegerNode : public ExprNode {
  public:
    int value;
    
    // Constructor
    IntegerNode(int v) : value(v) {
        std::cout << std::endl << std::endl;
        std::cout << "Integer Node Created! Value = " << value << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    // Print out the value for debugging purposes.
    int getVal(){
        return value;
    
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

};



/****************************** STATEMENTS ***********************************/

/* ExpressionStatement
 * I'm... not comfortable enough with the information on why we need
 * expression statements, so I won't comment on it here
 *
 * But I'm pretty sure this how they're constructed.
 */
class ExpressionStatement : public StmtNode {
  public:
    ExprNode& expr;
    
    ExpressionStatement(ExprNode& ex) : expr(ex) {
        std::cout << std::endl << std::endl;
        std::cout << "ExpressionStatement Node created!" << std::endl;
        std::cout << std::endl << std::endl;
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
    //VarList args;            // arguments passed
    Block* statements;         // statements to be executed
    
    SymbolTable* SymTable;
        
    // SymbolTable SymTable
    // Hold a reference to the symbol table here. We need to hold one for 
    // when the codeGen phase comes along.
    
    FuncDeclNode(const std::string* ty, const std::string* na, Block* stl, SymbolTable* st) :
                            type(ty), name(na), statements(stl), SymTable(st){
                                std::cout << std::endl << std::endl;
        std::cout << "FuncDeclNode created!" << std::endl;
        std::cout << "Type: " << *type << std::endl;
        std::cout << "Name: " << *name << std::endl;
        std::cout << std::endl << std::endl;
    }
        
};

/******************************* PROGRAM *************************************/


class ProgramNode : StmtNode {
  public:
    DeclNode* start = NULL;
    
    ProgramNode() { }
};


#endif