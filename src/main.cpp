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
extern int yyparse(void);


void constructTestAST(Block* programBlock){
    
    printf("\n++++++++++TEST CONSTRUCTION++++++++++\n");
    printf("\nConstructing the test AST...\n");
    printf("Constructing the test StatementList...\n");
    StmtList* stl = new StmtList();
    
    printf("Constructing the test SymbolTable...\n");
    SymbolTable* SymTable = new SymbolTable();
    
    printf("Constructing the test IntegerNode for the Return...\n");
    IntegerNode* intNode = new IntegerNode(0);
    
    printf("Constructing the test ReturnNode for the function...\n");
    ReturnNode* ret = new ReturnNode(intNode);
    
    printf("Pushing the test Return Node into StatementList...\n");
    stl->push_back(ret);
    
    //FuncDeclNode mainDecl = new FuncDeclNode(new std::string("main"), new std::string("int"), stl);

}    

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

void yyerror(const char* s) {
    printf("Help! Error! Help! --> %s\n", s);
}

int main(int argc, char** argv){
    
    yydebug = 1;
    
    // Open the file, read if good
    FILE* f = fopen(argv[1], "r");
    if(!f) {
        
        printf("Error: Bad Input\n");
    
    } else { // We're lexing/parsing the file now
    
        yyin = f;
        CurrSymTable = new SymbolTable();
        SymList = new std::list<SymbolTable*>();
        yyparse();
        fclose(yyin);
        
    }
    
    Block* testBlock;
    constructTestAST(testBlock);
}

