/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */
 
#include "parser.cpp"
#include "parser.hpp"
#include "tokens.cpp"

extern FILE* yyin;
extern FILE* yyout;
extern int yyparse(void);


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
}

void yyerror(const char* s) {
    printf("Help! Error! Help! --> %s\n", s);
}

bool CheckAndAdd(SymbolTable* SymTable, std::string name, std::string type){
    
    // Check to see if the symbol already exists in the symbol table 
    if(SymTable->count(name) != 0){
        
        // The symbol already exists
        return false;
        
    } else {
        
        std::cout << std::endl;
        std::cout << "Inserting... " << name << std::endl;
        std::cout << "Of type... " << type << std::endl;
        printf("\nBefore insertion, count = %d\n", SymTable->count(name));
        SymTable->insert({name, type});
        printf("\nAfter insertion, count = %d\n", SymTable->count(name));
        printf("SymbolTable Size: %d\n", SymTable->size());
        printf("Added some stuff to the symbol table\n\n");
        return true;
        
    }
};