#include "visitor.h"

// Visitor
Visitor::Visitor(){
    printf("New Visitor Created\n");
}

Visitor::~Visitor() { }

void Visitor::visit(ASTNode* node) {}
void Visitor::visit(DeclNode* node) {}
void Visitor::visit(IntegerNode* node) {}
void Visitor::visit(ReturnNode* node) {}
void Visitor::visit(VarDeclNode* node) {}
void Visitor::visit(FuncDeclNode* node) {}
void Visitor::visit(ProgramNode* node) {}


PrintVisitor::PrintVisitor() {
    printf("New Print Visitor Created\n");
}

PrintVisitor::~PrintVisitor() { }


void PrintVisitor::visit(ASTNode* node) {
    printf("Visiting an AST Node!\n");
}

void PrintVisitor::visit(DeclNode* node){
    printf("Visiting a DeclNode!\n");
}

void PrintVisitor::visit(IntegerNode* node){
    printf("Visiting an Integer Node!\n");
}

void PrintVisitor::visit(ReturnNode* node){
    printf("Visiting a Return Node!\n");
}

void PrintVisitor::visit(VarDeclNode* node){
    printf("Visiting a VarDeclNode!\n");
}

void PrintVisitor::visit(FuncDeclNode* node){
    printf("Visiting a FuncDecl Node!\n");
}

void PrintVisitor::visit(ProgramNode* node){
    printf("Visiting a Program Node!\n");
}