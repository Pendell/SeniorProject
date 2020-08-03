/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */

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

#include "parser.cpp"
#include "parser.hpp"

#include "tokens.cpp"

#include "./nodes/node.h"
#include "./visitor/visitor.h"


using namespace llvm;

extern FILE* yyin;
extern FILE* yyout;

// LLVMContext TheContext;
// Module* TheModule;

// Global variables I need to track constantly
ProgramNode* program;       // The root of the AST that bison builds
ProgramNode* testProgram;   // The AST we're handbuilding to test bison against


void yyerror(const char* e);
//bool checkAndAdd(SymbolTable* SymTable, std::string name, std::string type);

extern int yyparse(void);

// bool checkAndAdd(SymbolTable* symTable, VarDeclNode* node){
    
    // // Check to see if the symbol already exists in the symbol table 
    // SymbolTable::iterator it = symTable->begin();
    // while(it != symTable->end()){
        
        // VarDeclNode* node_casted = dynamic_cast<VarDeclNode*>(*it);
        
        // if(strcmp(node_casted->getName(), node->getName()))
            // return false;
        // ++it;
    // }
    
    // symTable->push_back(node);
    // return true;
        
    
// }

/* buildAndTestProgram()
 * Constructs a quick AST that should be identical to the one  that Bison
 * constructs when parsing the file ./tests/prog.c.
 * When the function is complete, testProgram will be the root of the 
 * test tree. This will allow us to compare the two trees against each other
 * and verify that Bison is creating the tree we want it to create.
 */
// void buildAndTestProgram(){
    
    // printf("\n\u001b[36mTESTING BISON-AUTOMATED AST AGAINST HANDMADE AST\n\u001b[0m");
    
    // testProgram = new ProgramNode();
    
    // DeclNode* d1 = new DeclNode();
    // DeclNode* d2 = new DeclNode();
    
    // testProgram->start = d1;
    
    // StmtList* testStmtList = new StmtList;
    
    // ReturnNode* r1 = new ReturnNode(new IntegerNode(0));
    // VarDeclNode* x = new VarDeclNode("int", "x", new IntegerNode(10));
    
    // testStmtList->push_front(r1);
    // testStmtList->push_front(x);
    
    // const char* type = "int";
    // const char* name = "main";
    
    // FuncDeclNode* f1 = new FuncDeclNode(type, name, testStmtList, ST);
    // d1->lhs = f1;
    // // d1->rhs = d2;
    
    // if(testProgram->equals(program)){
        // printf("We've succeeded! Bison is constructing the correct AST!\n");
    // } else {
        // printf("Comparison failed; Program != testProgram\n");
    // }
    
// }

void yyerror(const char* s) {
    printf("Help! Error! Help! --> %s\n", s);
}

int main(int argc, char** argv){
    
    yydebug = 1;
    
    // Open the file, read if good
    FILE* f = fopen(argv[1], "r");
    
    if (!f) {
        
        printf("Error: Bad Input\n");
    
    } else { // We're lexing/parsing the file now
        yyin = f;
        
        // TheModule = std::make_unique<Module>(std::string(argv[1]), TheContext);
        
        program = new ProgramNode(argv[1]);
        printf("Program name is: ");
        printf("%s\n", program->getName());
        
        if(yyparse() == 0) {
            
            // buildAndTestProgram();
            program->codegen();
            
        }
        
        fclose(yyin);
        
        
        
    }
}

