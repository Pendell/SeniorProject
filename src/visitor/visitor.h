#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>
#include <vector>
#include <memory>

#include "../nodes/node.h"

#include "llvm/IR/Value.h"
#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"



// Forward Declarations
class ASTNode;
class DeclNode;
class IntegerNode;
class ReturnNode;
class VarDeclNode;
class FuncDeclNode;
class ProgramNode;
class PrototypeNode;

class Visitor {
    
  public:
  
    Visitor();
    ~Visitor();
    
    virtual void visit(ASTNode* node);
    virtual void visit(DeclNode* node);
    virtual void visit(IntegerNode* node);
    virtual void visit(ReturnNode* node);
    virtual void visit(VarDeclNode* node);
    virtual void visit(PrototypeNode* node);
    virtual void visit(FuncDeclNode* node);
    virtual void visit(ProgramNode* node);
    
};


class PrintVisitor : public Visitor {
    
  public:
  
    PrintVisitor();
    ~PrintVisitor();
     
    void visit(ASTNode* node);
    void visit(DeclNode* node);
    void visit(IntegerNode* node);
    void visit(ReturnNode* node);
    void visit(VarDeclNode* node);
    void visit(PrototypeNode* node);
    void visit(FuncDeclNode* node);
    void visit(ProgramNode* node);
    
};

// Some LLVM stuff found in chapter 3 of the Kaleidoscope tutorial.
// An opaque object that owns a lof of core LLVM data structures -
// such as type and constant value tables
static llvm::LLVMContext TheContext; 

// Helper object that makes it easy to generate llvm instructions
static llvm::IRBuilder<> Builder(TheContext);

// LLVM construct that contains functions and global variables.
// Basically top-level structure that LLVMIR uses to contain code.
// Owns the memory for all of the IR generated.
static std::unique_ptr<llvm::Module> TheModule;

// Map that keeps track of which values are defined in the current scope
// and what their LLVM representation is.
static std::map<std::string, llvm::Value*> NamedValues;

class CodeGenVisitor : public Visitor {
    
  public:
    CodeGenVisitor();
    ~CodeGenVisitor();
    
    virtual llvm::Value* visit(ASTNode* node);
    virtual llvm::Value* visit(DeclNode* node);
    virtual llvm::Value* visit(IntegerNode* node);
    virtual llvm::Value* visit(ReturnNode* node);
    virtual llvm::Value* visit(VarDeclNode* node);
    virtual llvm::Function* visit(FuncDeclNode* node);
    virtual llvm::Function* visit(PrototypeNode* node);
    virtual llvm::Value* visit(ProgramNode* node);
    

};

#endif
