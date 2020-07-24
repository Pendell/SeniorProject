/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler 
 * Spring 2020
 */

#pragma once


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
 
// #ifndef NODE_H
// #define NODE_H

#include <cstring>
#include <string>
#include <iostream>
#include <memory>
#include <map>

#include <list>             // for statement lists & params, etc...
#include <unordered_map>    // for symbol table
#include <vector>

// #include "../visitor/visitor.h"

using namespace llvm;

// Forward Declarations
class DeclNode;
class VarDeclNode;
class ASTNode;

class VarDeclNode;
class FuncDeclNode;

class SymbolTable {
    
    SymbolTable* parent;
    
  public:  
    SymbolTable(SymbolTable* p);
    ~SymbolTable();
    
    // Returns the parent of this symbol table
    virtual SymbolTable* getParent() = 0;
    virtual VarDeclNode* get(std::string n) = 0;
    
};

class GlobalSymbolTable : public SymbolTable {
    
  private:
    std::vector<FuncDeclNode*> functions;
    std::vector<VarDeclNode*> globalvars;
    
  public:
    
    GlobalSymbolTable();
    ~GlobalSymbolTable();
    
    int size_funcs();
    int size_vars();
    

    // Finds any entities with the name 'n' in either funcs or gvars.
    // returns the index if found, -1 if not found.
    int func_lookup(std::string n);
    int lookup(std::string n);
    
    // Tries to add the DeclNode* to the respective symbol table.
    // returns true if added successfully, false otherwise.
    bool add(FuncDeclNode* f);
    bool add(VarDeclNode* v);
    
    
    // Tries to grab the function or variable with the name 'n'
    // returns the DeclNode* if found, nullptr o/w.
    FuncDeclNode* get_func(std::string n);
    VarDeclNode* get(std::string n);
    
    bool isEqual(GlobalSymbolTable* o); // Not yet implemented
    
};

class ScopedSymbolTable : public SymbolTable {
    
  private:
    std::vector<VarDeclNode*> vars;
    
  public:
    
    ScopedSymbolTable(SymbolTable* p);
    ~ScopedSymbolTable();
  
    int size();
    
    int lookup(std::string n, bool lookGlobally);
    
    void add();
    
    VarDeclNode* get(std::string n);
    
    bool isEqual(ScopedSymbolTable* o);
    
}; 

// TheContext = object that accesses much of the core llvm data structures
static LLVMContext TheContext;

// The object that constructs much of the instruction for IR
static IRBuilder<> builder(TheContext);

// Top level container of the program
static Module* TheModule;

// An llvm-implemented symbol table
static std::map<std::string, AllocaInst*> NamedValues;

// Keep a reference of the current basic block.
static BasicBlock* bbreference;

static Function* fref;


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
    //virtual void accept(Visitor* v);
    
    virtual Value* codegen() = 0;
    
};

class ExprNode : public ASTNode {
    
  public:
    
    virtual int getVal();
    virtual const char* getNodeType();
    virtual bool equals(ASTNode* node);
    
    virtual Value* codegen() = 0;
    
};

class StmtNode : public ExprNode {
    
  public:
  
    virtual const char* getNodeType();
    virtual bool equals (ASTNode* node);
    
    virtual Value* codegen() = 0;
    
    
};

/* A generic Declaration node class so that I can construct the program node
 * list.
 */
class DeclNode : public StmtNode {
    
  public:
  
    ExprNode* value = nullptr;
    
    DeclNode* lhs;
    DeclNode* rhs;
    
    DeclNode();
    ~DeclNode();
    
    virtual const char* getName();
    virtual int getVal();
    virtual const char* getNodeType();
    bool isLast();
    bool equals(ASTNode* node);
    //virtual void accept(Visitor* v);
    
    virtual Value* codegen();
    
};



// Typedef the lists and tables, because I'm lazy
typedef std::list<StmtNode*> StmtList;
// typedef std::list<ExprNode*> ExprList;
// typedef std::list<DeclNode*> SymbolTable;


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
    //void accept(Visitor* v);
    
    virtual Value* codegen();
    
};

class CharacterNode : public ExprNode {
  
  public:
    
    // Note: CharacterNode stores the integer representation
    //       of the desired character.
    int value;
    
    CharacterNode(int v);
    ~CharacterNode();
    
    int getVal();
    const char* getNodeType();
    
    virtual Value* codegen();

};

class UnaryOpNode : public ExprNode {
  public:
    const char op;
    const char* inc; // when to increment
    ExprNode* lhs;
    
    UnaryOpNode(const char o, ExprNode* l, const char* i);
    ~UnaryOpNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
};

/* BinaryOpNode
 * A node created for binary operations (+, -, *, /, etc...)
 */
class BinaryOpNode : public ExprNode {
    
  public:
  
    const char op;  // Operation
    
    ExprNode* lhs;  // left-hand side
    ExprNode* rhs;  // right-hand side
    
    BinaryOpNode(const char op, ExprNode* lh, ExprNode* rh);
    ~BinaryOpNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
    
};

class BoolExprNode : public ExprNode {
    
  public:
  
    ExprNode* lhs;
    ExprNode* rhs;
    char op;
    
    BoolExprNode(ExprNode* l, ExprNode* r, const char o);
    ~BoolExprNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
    
};


/****************************** LOOPS ***********************************/
class IfNode : public StmtNode {
  public:
    
    
    BoolExprNode* bExpr;
    char op;
    StmtList* then;
    
    IfNode(BoolExprNode* b, StmtList* t);
    ~IfNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
    
};

class DoWhileNode : public StmtNode {
    
  public:
    
    BoolExprNode* exit_cond;
    StmtList* body;
    
    // either a 'd' or 'w' to determine this a 'do-while' or just 'while'
    char loop; 
    
    DoWhileNode(StmtList* b, char l, BoolExprNode* e);
    ~DoWhileNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
    virtual Value* whilecodegen();
    virtual Value* dowhilecodegen();
};

class ForNode : public StmtNode {
  public:
  
    ExprNode* start;
    BoolExprNode* bExpr;
    StmtNode* step;
    StmtList* body;
    
    DoWhileNode* cheat; 
    
    ForNode(ExprNode* sta, BoolExprNode* be,  StmtList* b);
    ForNode(ExprNode* sta, BoolExprNode* be, StmtNode* ste, StmtList* b);
   
    ~ForNode();
    
    const char* getNodeType();
    void generateCheats();
    virtual Value* codegen();
    
};

/****************************** STATEMENTS ***********************************/
class MutateVarNode : public StmtNode {
    
  public:
  
    VarDeclNode* lhs;
    ExprNode* rhs;
    
    MutateVarNode(VarDeclNode* v, ExprNode* val);
    ~MutateVarNode();
    
    
    const char* getNodeType();
    
    virtual Value* codegen();
};


/* ReturnNode
 * for returning expressions
 */
class ReturnNode : public StmtNode {
    
  public:
  
    ExprNode* value;
    
    ReturnNode();
    ReturnNode(ExprNode* rv);
    
    const char* getNodeType();
    int getVal();
    bool equals(ASTNode* node);
    //void accept(Visitor* v);
    
    virtual Value* codegen();
    
};

class LoadVarNode : public ExprNode {
  public:
    
    const char* name;
    
    LoadVarNode(const char* na);
    ~LoadVarNode();
    
    const char* getNodeType();
    const char* getName();
    virtual Value* codegen();


};


/* VariableDeclarationNode
 * for creating nodes containing variable declarations
 */
class VarDeclNode : public DeclNode {
    
    bool init = false;
    bool isGlobal = false;
    
  public:
    
    const char* type; 
    const char* name;
    
    ExprNode* value = nullptr;
    
    VarDeclNode(const char* ty, const char* na, ExprNode* va, bool global);
    
    ~VarDeclNode();
    
    // Returns the typing of the data stored -- NOT the type of the node
    const char* getType();
    const char* getName();
    
    int getVal();
    
    const char* getNodeType();
    bool equals(ASTNode* node);
    
    //void accept(Visitor* v);
    
    virtual Value* codegen();
    virtual Value* codegenGlobalVar();
    
    Type* getLLVMType();
    
};

// Function Calls
class FCallNode : public ExprNode {
  
  public:
    std::string callee;
    std::vector<ExprNode*> args;  // We'll see how this turns out
    
    FCallNode(const char* c, std::vector<ExprNode*>& a);
    ~FCallNode();
    
    const char* getNodeType();
    
    virtual Value* codegen();
};

class Parameter {
  public:
    std::string type;
    std::string name;
    
    Parameter(const char* ty, const char* na);
    ~Parameter();
    
    const char* getType();
    const char* getName();
    
    virtual Value* codegen();
    
};


class FuncPrototype {
  public:  
    std::string type;
    std::string name;
    std::vector<std::pair<std::string, std::string>*> args;
    
    FuncPrototype(std::string& t, std::string& n, std::vector<std::pair<std::string, std::string>*> a);
    ~FuncPrototype();
    
    const char* getType();
    const char* getName();
    
    const char* getNodeType();
    
    Function* codegen();
    
};


/* FunctionDeclarationNode
 * for creating nodes containing functions
 */
class FuncDeclNode : public DeclNode {
    
  public:
    
    FuncPrototype* proto;
    StmtList* statements;       // statements to be executed
    
    SymbolTable* SymTable;
    //VarList args;             // arguments passed * NOT YET IMPLEMENTED *
        
    // SymbolTable SymTable
    // Hold a reference to the symbol table here. We need to hold one for 
    // when the codeGen phase comes along.
    
    FuncDeclNode(FuncPrototype* p, StmtList* stl, SymbolTable* st);
    ~FuncDeclNode();
    
    const char* getType();
    const char* getName();
    const char* getNodeType();
    bool equals(ASTNode* node);
    //void accept(Visitor* v);
    
    virtual Value* codegen();
    
    AllocaInst* createEntryBlockAlloca(Function* f, const std::string& name);
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
    //void accept(Visitor* v);
    
    void compile();
    
    virtual Value* codegen();
};

/*****************************Not yet Implemented*****************************/

// Or broken... needs to get implemented later

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

//#endif
