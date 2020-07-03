/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler 
 * Spring 2020
 */
 
 
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

// LLVM goodies 
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
 
#ifndef NODE_H
#define NODE_H

#include <cstring>
#include <string>
#include <iostream>
#include <memory>

#include <list>   // for statement lists & params, etc...
#include <map>    // for symbol table
#include <vector>

#include "../visitor/visitor.h"


using namespace llvm;

// Forward Declarations
class VarDeclNode;
class Visitor;
class ASTNode;

// TheContext = object that accesses much of the core llvm data structures
static LLVMContext TheContext;

// The object that constructs much of the instruction for IR
static IRBuilder<> builder(TheContext);

// Top level container of the program
static Module* TheModule;

// An llvm-implemented symbol table
static std::map<std::string, Value *> NamedValues;

// ASTNode* LogError(const char* str){
    // fprintf(stderr, "Error: %s\n", str);
    // return nullptr;
// }

// Value* LogErrorV(const char* str){
    // LogError(str);
    // return nullptr;
// }


/* Define the base 'node' objects we will derive everything else from...
 * Everything is empty right now because all nodes will be some form of
 * derivative of these nodes. I think I'll need the codeGen stuff in here
 * when I get llvm working.
 */
class ASTNode {

  public:
  
    virtual const char* getNodeType();
    virtual bool equals(ASTNode* node);
    virtual void accept(Visitor* v);
    
    virtual void codegen() = 0;
    
};

class StmtNode : public ASTNode {
    
  public:
  
    virtual const char* getNodeType();
    virtual bool equals (ASTNode* node);
    
    virtual void codegen() = 0;
    
    
};

class ExprNode : public ASTNode {
    
  public:
    
    virtual int getVal();
    virtual const char* getNodeType();
    virtual bool equals(ASTNode* node);
    
    virtual void codegen() = 0;
    
};

/* A generic Declaration node class so that I can construct the program node
 * list.
 */
class DeclNode : public StmtNode {
    
  public:
  
    DeclNode* lhs;
    DeclNode* rhs;
    
    DeclNode();
    ~DeclNode();
    
    virtual const char* getNodeType();
    bool isLast();
    bool equals(ASTNode* node);
    virtual void accept(Visitor* v);
    
    virtual void codegen();
    
};



// Typedef the lists and tables, because I'm lazy
typedef std::list<StmtNode*> StmtList;
typedef std::list<ExprNode*> ExprList;
typedef std::list<DeclNode*> SymbolTable;


/****************************** EXPRESSIONS **********************************/
/* IntegerNode
 * for IntegerConstants
 */
class IntegerNode : public ExprNode {
  public:
    int value;
    
    IntegerNode(int v);
    
    int getVal();
    const char* getNodeType();
    bool equals(ASTNode* node);
    
    // Visitor Functionality
    void accept(Visitor* v);
    
    virtual void codegen();
    
};

/****************************** STATEMENTS ***********************************/
/* ReturnNode
 * for returning expressions
 */
class ReturnNode : public StmtNode {
    
  public:
  
    ExprNode* retVal;
    
    ReturnNode();
    ReturnNode(ExprNode* rv);
    
    const char* getNodeType();
    int getVal();
    bool equals(ASTNode* node);
    void accept(Visitor* v);
    
    virtual void codegen();
    
};


/* VariableDeclarationNode
 * for creating nodes containing variable definitions
 */
class VarDeclNode : public DeclNode {
  public:
    
    const char* type; 
    const char* name;
    ExprNode* value;
    
    VarDeclNode(const char* ty, const char* na);
    VarDeclNode(const char* ty, const char* na, ExprNode* va);
    
    ~VarDeclNode();
    
    // Returns the typing of the data stored -- NOT the type of the node
    const char* getType();
    const char* getName();
    
    int getVal();
    
    const char* getNodeType();
    bool equals(ASTNode* node);
    
    void accept(Visitor* v);
    
    virtual void codegen();
    
};

// I think I'm going to use this for arguments, not sure yet.
typedef std::list<VarDeclNode*> VarList;

// Keep basic info about functions
class PrototypeNode : public ASTNode {
  public:
  
    const char* name;
    const char* type;
    
    std::vector<char*> args;
    
    PrototypeNode(const char* ty, const char* na);
    ~PrototypeNode();

    void accept(Visitor* v);
    const char* getName();
    const char* getType();
    
    virtual void codegen();
    
};

/* FunctionDeclarationNode
 * for creating nodes containing functions
 */
class FuncDeclNode : public DeclNode {
    
  public:
    
    PrototypeNode* prototype;
    
    const char* type;           // return type
    const char* name;           // name of function
    
    StmtList* statements;       // statements to be executed
    SymbolTable* SymTable;
    //VarList args;             // arguments passed * NOT YET IMPLEMENTED *
        
    // SymbolTable SymTable
    // Hold a reference to the symbol table here. We need to hold one for 
    // when the codeGen phase comes along.
    
    FuncDeclNode(const char* ty, const char* na, StmtList* stl, SymbolTable* st);
    ~FuncDeclNode();
    
    const char* getType();
    const char* getName();
    const char* getNodeType();
    bool equals(ASTNode* node);
    void accept(Visitor* v);
    
    virtual void codegen();
    
    
};

/******************************* PROGRAM *************************************/
class ProgramNode : public StmtNode {
    
    const char* srcName;
    
  public:
  
    DeclNode* start;
    ProgramNode();
    ProgramNode(char* src);
    
    const char* getNodeType();
    const char* getName();
    
    bool equals(ASTNode* node);
    void accept(Visitor* v);
    
    void compile();
    
    virtual void codegen();
};


/*****************************Not yet Implemented*****************************/
// Or broken... needs to get implemented later

/* BinaryOpNode
 * A node created for binary operations (+, -, *, /, etc...)
 *
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
    
    //void accept(Visitor* visitor){ }
};


/* FunctionCallNode
 * This node is created when a function is called.
 *
 
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

/* ExpressionStatement
 * I'm... not comfortable enough with the information on why we need
 * expression statements, so I won't comment on it here
 *
 * But I'm pretty sure this how they're constructed.
 *
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

/* Block
 * This is basically a collection of statements; so there's only one entry
 * point and one exit point with no branching in a block. So the statements
 * are just basically executed line by line.
 *
class Block : public ExprNode {
    
  public:
    StmtList* statements;
    
    Block() {
        std::cout << std::endl;
        std::cout << "Block created!" << std::endl;
        std::cout << std::endl;
    }
    
    virtual const char* getNodeType(){
        return "Block";
    }
    
};
*/






#endif
