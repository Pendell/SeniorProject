#include "node.h"
/// Base Symbol Table definitions /////////////////////////////////////////////
SymbolTable::SymbolTable(SymbolTable* p = nullptr) : parent(p) {}
SymbolTable::~SymbolTable() {}

SymbolTable* SymbolTable::getParent(){
    return parent;
}


/// Global Symbol Table definitions ///////////////////////////////////////////
GlobalSymbolTable::GlobalSymbolTable(){
    // There can be no parent to the global sym table
    parent = nullptr;
    functions = new std::vector<FuncDeclNode*>;
    globalvars = new std::vector<VarDeclNode*>;
}

GlobalSymbolTable::~GlobalSymbolTable(){
    delete[] functions, globalvars;
}

int GlobalSymbolTable::size_funcs() {
    return functions.size();
}

int GlobalSymbolTable::size_vars() {
    return globalvars.size();
}

int GlobalSymbolTable::func_lookup(std::string n) {
    for(int i = 0; i < functions.size(); i++){
        if(std::string(functions[i].getName()) == n)
            return i;
    }
    return -1;
}

int GlobalSymbolTable::var_lookup(std::string n) {
    for(int i = 0; i < globalvars.size(); i++){
        if(std::string(globalvars[i].getName()) == n)
            return i;
    }
    return -1;
}

bool GlobalSymbolTable::add(FuncDeclNode* f) {
    if(func_lookup(std::string(f->getName())) == -1) {
        functions.push_back(f);
        return true;
    } else {
        return false;
    }
}

bool GlobalSymbolTable::add(VarDeclNode* v) {
    if(var_lookup(std::string(v->getName())) == -1) {
        globalvars.push_back(v);
        return true;
    } else {
        return false;
    }
}
FuncDeclNode* GlobalSymbolTable::get_function(std::string n) {
    if(int idx = func_lookup(n) != -1)
        return functions[idx];
    else 
        return nullptr;
    
}

VarDeclNode* GlobalSymbolTable::get_globalvariable(std::string n) {
    if(int idx = var_lookup(n) != -1)
        return globalvars[idx];
    else 
        return nullptr;
    
}

bool isEqual(GlobalSymbolTable* o) {
    // not yet implemented
}


/// SCOPED SYMBOL TABLES //////////////////////////////////////////////////////
ScopedSymbolTable::ScopedSymbolTable(){
    vars = new std::vector<VarDeclNode*>;
}

ScopedSymbolTable::~ScopedSymbolTable(){
    delete[] vars;
}

int ScopedSymbolTable::size(){
    return vars.size();
}

int lookup(std::string n) {
    for(int i = 0; i < size(); i++){
        if(std::string(vars[i]->getName()) == n)
            return i
    }
    return -1;
}

bool ScopedSymbolTable::add(VarDeclNode* v) {
    if(lookup(std::string(v->getName())) != -1){
        vars.push_back(v);
        return true;
    } else {
        return false;
    }
}

VarDeclNode* ScopedSymbolTable::get(std::string n){  
    if(int i = lookup(n) != -1)
        return vars[i];
    else
        return nullptr;
}

bool ScopedSymbolTable::isEqual(ScopedSymbolTable* o){
    // not yet implemented
}