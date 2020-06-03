/* definitions of functions I use in the parser */

#ifndef SRC_H
#define SRC_H


#include <string>

#include "node.h"

void yyerror(const char* e);
bool checkAndAdd(SymbolTable* SymTable, std::string name, std::string type);

#endif
