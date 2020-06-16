/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */
#include "parser.cpp"
#include "parser.hpp"
#include "tokens.cpp"
#include "node.h"

extern FILE* yyin;
extern FILE* yyout;

// Global variables I need to track constantly
ProgramNode* program;       // The root of the AST that bison builds
ProgramNode* testProgram;   // The AST we're handbuilding to test bison against

SymbolTable* globalST;
SymbolTable* currSymTable;

extern int yyparse(void);

bool checkAndAdd(SymbolTable* SymTable, std::string name, std::string type){
    
    // Check to see if the symbol already exists in the symbol table 
    if(SymTable->count(name) != 0){
        
        // The symbol already exists
        return false;
        
    } else {
        printf("\nInserting... %s\n", name.c_str());
        printf("Of type... %s\n", type.c_str());
        printf("\nBefore insertion, count = %d\n", SymTable->count(name));
        SymTable->insert({name, type});
        printf("\nAfter insertion, count = %d\n", SymTable->count(name));
        printf("SymbolTable Size: %d\n", SymTable->size());
        printf("Added some stuff to the symbol table\n\n");
        return true;
        
    }
}

/* buildAndTestProgram()
 * Constructs a quick AST that should be identical to the one  that Bison
 * constructs when parsing the file ./tests/prog.c.
 * When the function is complete, testProgram will be the root of the 
 * test tree. This will allow us to compare the two trees against each other
 * and verify that Bison is creating the tree we want it to create.
 */
void buildAndTestProgram(){
    
    testProgram = new ProgramNode();
    
    DeclNode* d1 = new DeclNode();
    DeclNode* d2 = new DeclNode();
    
    testProgram->start = d1;
    
    StmtList* testStmtList = new StmtList();
    
    ReturnNode* r1 = new ReturnNode(new IntegerNode(0));
    VarDeclNode* x = new VarDeclNode("int", "x", new IntegerNode(10));
    
    testStmtList->push_back(r1);
    testStmtList->push_back(x);
    
    const char* type = "int";
    const char* name = "main";
    
    FuncDeclNode* f1 = new FuncDeclNode(type, name, testStmtList, currSymTable);
    d1->lhs = f1;
    // d1->rhs = d2;
    
    /*
    
    if(program->equals(testProgram)) {
        printf("We've succeeded! Bison is constructing the proper AST!\n");
    } else {
        printf("Comparison failed; Program != testProgram\n");
    }
    
    */
    
    if(testProgram->equals(program)){
        printf("We've succeeded! Bison is constructing the proper AST!\n");
    } else {
        printf("Comparison failed; Program != testProgram\n");
    }
    
}

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
        
        program = new ProgramNode();
        currSymTable = new SymbolTable();
        
        yyparse();
        fclose(yyin);
        
    }
    
    buildAndTestProgram();
    
    
}

