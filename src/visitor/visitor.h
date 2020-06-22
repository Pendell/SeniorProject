#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>

#include "../nodes/node.h"

#include <llvm/IR/Value.h>
#include "llvm/Module.h"
#include "llvm/Function.h"
#include "llvm/PassManager.h"
#include "llvm/CallingConv.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Assembly/PrintModulePass.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"

// Forward Declarations
class ASTNode;
class DeclNode;
class IntegerNode;
class ReturnNode;
class VarDeclNode;
class FuncDeclNode;
class ProgramNode;

class Visitor {
    
  public:
  
    Visitor();
    ~Visitor();
  
    
    virtual void visit(ASTNode* node);
    virtual void visit(DeclNode* node);
    virtual void visit(IntegerNode* node);
    virtual void visit(ReturnNode* node);
    virtual void visit(VarDeclNode* node);
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
    void visit(FuncDeclNode* node);
    void visit(ProgramNode* node);
    
};


class CodeGenVisitor : public Visitor {
    
  public:
    CodeGenVisitor();
    ~CodeGenVisitor();
    
    // Start with Integer Node
    void visit(IntegerNode* node);
    

};

#endif
