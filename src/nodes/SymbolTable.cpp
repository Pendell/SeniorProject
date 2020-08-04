#include "node.h"
/// SYMBOL TABLES //////////////////////////////////////////////////////
SymbolTable::SymbolTable(SymbolTable* p){
    parent = p;
    vars = {};
    funcs = {};
}

SymbolTable::~SymbolTable(){
    
}


int SymbolTable::size(){
    return vars.size();
}

int SymbolTable::size_funcs() {
    return funcs.size();
}
FuncDeclNode* SymbolTable::func_lookup(const FuncPrototype& fp) {
    std::map<FuncPrototype*, FuncDeclNode*>::iterator it = funcs.begin();
    while(it != funcs.end()) {
        if(&fp == it->first)
            return it->second;
        it++;
    }
    return nullptr;
    
}
VarDeclNode* SymbolTable::local_lookup(std::string n) {
    for(int i = 0; i < vars.size(); i++) {
        if(strcmp(n.c_str(), vars[i]->getName()) == 0) {
            return vars[i];
        }
    }
    return nullptr;
}

VarDeclNode* SymbolTable::global_lookup(std::string n){
    for(int i = 0; i < vars.size(); i++) {
        if(strcmp(n.c_str(), vars[i]->getName()) == 0) {
            return vars[i];
        }
    }
    
    if(parent) {
        return parent->global_lookup(n);
    } else {
        return nullptr;
    }
}   
    

bool SymbolTable::add(VarDeclNode* v) {
    if(local_lookup(std::string(v->getName()))) {
        printf("Variable %s already defined.", v->getName());
        return false;
    } else {
        vars.push_back(v);
        return true;
    }
}

bool SymbolTable::add(FuncDeclNode* f) {
    FuncDeclNode* tmp;
    FuncPrototype* fp = f->getProto();
    if( tmp = func_lookup(*fp)) {
        return false;
    } else {
        funcs[fp] = f;
        return true;
    }
}

bool SymbolTable::add(FuncPrototype* fp){
    FuncDeclNode* tmp;
    if(tmp = func_lookup(*fp)) {
        return false;
    } else {
        funcs[fp] = nullptr;
        return true;
    }
}

VarDeclNode* SymbolTable::get(std::string n){  
    return global_lookup(n);
}

FuncDeclNode* SymbolTable::get(FuncPrototype* fp){
    return func_lookup(*fp);
}

bool SymbolTable::isEqual(SymbolTable* o){
    // not yet implemented
}

SymbolTable* SymbolTable::getParent() {
    return parent;
}