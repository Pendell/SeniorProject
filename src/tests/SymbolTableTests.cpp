#include "../nodes/node.h"

/*

Primo artwork:
The example I'm trying to build:

*-----------------------------*
|GLOBALSYMBOLTABLE            |
|                             |
| int x = 1;                  |
| int main () {               |
| *-------------------------* |
| | SCOPEDSYMBOLTABLE       | |
| |   int y = 2;            | |
| |                         | |
| |   for(...) {            | |
| |   *-------------------* | |
| |   | EVENSCOPIERTABLE1 | | |
| |   |int z = 3;         | | |
| |   |                   | | |
| |   |                   | | |
| |   *-------------------* | |
| |   }                     | |
| |                         | |
| |   for(...) {            | |
| |   *-------------------* | |
| |   | EVENSCOPIERTABLE2 | | |
| |   |int z = 4;         | | |
| |   |                   | | |
| |   |                   | | |
| |   *-------------------* | |
| |   }                     | |
| *-------------------------* |
| }                           |
*-----------------------------*

*/

int main() {
    
    // Make symbol tables
    GlobalSymbolTable* globalSymbolTable = new GlobalSymbolTable();
    ScopedSymbolTable* scopedSymbolTable = new ScopedSymbolTable(globalSymbolTable);
    ScopedSymbolTable* evenScopierTable1 = new ScopedSymbolTable(scopedSymbolTable);
    ScopedSymbolTable* evenScopierTable2 = new ScopedSymbolTable(scopedSymbolTable);
    
    // Make variables
    VarDeclNode* x = new VarDeclNode("int", "x", new IntegerNode(1));
    VarDeclNode* y = new VarDeclNode("int", "y", new IntegerNode(2));
    VarDeclNode* z = new VarDeclNode("int", "z", new IntegerNode(3));
    VarDeclNode* z2 = new VarDeclNode("int", "z", new IntegerNode(4));
    
    // Make function
    FuncPrototype* fp = new FuncPrototype(std::string("int"), std::string("main"));
    FuncDeclNode* f = new FuncDeclNode(fp, nullptr, nullptr);
}