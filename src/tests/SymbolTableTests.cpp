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
    SymbolTable* st = new SymbolTable(nullptr);
    SymbolTable scopedst = new SymbolTable(st);
    
    std::string* type = new std::string("int");
    std::string* name = new std::string("main");
    
    
    FuncPrototype* fp = new FuncPrototype(*type, *name, {});
}