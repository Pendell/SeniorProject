#ifndef VISITOR_H
#define VISITOR_H

#include <iostream>
#include <vector>
#include <memory>

#include "../nodes/node.h"





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




class CodeGenVisitor : public Visitor {
    
  public:
    CodeGenVisitor();
    ~CodeGenVisitor();
    
    void visit(ASTNode* node);
    void visit(DeclNode* node);
    void visit(IntegerNode* node);
    void visit(ReturnNode* node);
    void visit(VarDeclNode* node);
    void visit(PrototypeNode* node);
    void visit(FuncDeclNode* node);
    void visit(ProgramNode* node);
    

};

#endif
