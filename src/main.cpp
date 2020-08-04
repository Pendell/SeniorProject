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

extern int yyparse(void);

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
        
        
        program = new ProgramNode(argv[1]);
        
        if(yyparse() == 0) {
            program->codegen();
        }
        fclose(yyin);
    }
}

