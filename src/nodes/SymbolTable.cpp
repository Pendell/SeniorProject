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

void SymbolTable::dump() {
    // std::map<std::string, VarDeclNode*>::iterator vit = vars.begin();
    // while (vit != vars.end()){
        // printf("Var %s = %d\n", vit->first.c_str(), vit->second->getVal());
        // vit++;
    // }
    
    // std::map<FuncPrototype*, FuncDeclNode*>::iterator fit = funcs.begin();
    // while (fit != funcs.end()){
        // printf("Func %s\n", fit->first->getName());
        // fit++;
    // }
}

/// Global Symbol Table definitions ///////////////////////////////////////////

/* GlobalSymbolTable is the object that keeps track of all symbols at a global
 * level. Since it was agreed that the C-Lite language would not include nested 
 * functions, this is the only symbol table that needs to keep track of names 
 * of functions
 *  
 */ 
// GlobalSymbolTable::GlobalSymbolTable() : SymbolTable(){
    // functions = {};
    // vars = {};
// }

// GlobalSymbolTable::~GlobalSymbolTable(){
    
// }

// int GlobalSymbolTable::size_funcs() {
    // return functions.size();
// }

// int GlobalSymbolTable::size() {
    // return vars.size();
// }

// int GlobalSymbolTable::lookup_func(std::string n) {
    // for(int i = 0; i < functions.size(); i++){
        // if(std::string(functions[i]->getName()) == n)
            // return i;
    // }
    // return -1;
// }

// int GlobalSymbolTable::local_lookup(std::string n) {
    // for(int i = 0; i < size(); i++){
        // if(std::string(this->vars[i]->getName()) == n)
            // return i;
    // }
// }

// int GlobalSymbolTable::global_lookup(std::string n){
    // for(int i = 0; i < size(); i++){
        // if(std::string(vars[i]->getName()) == n)
            // return i;
    // }
    
    // // Didn't find, if we're searching globally, search the next symbol table
    // if (parent)
        // return getParent()->global_lookup(n);
    // else
        // return -1;
// }
// SymbolTable* GlobalSymbolTable::getParent(){
    // return nullptr;
// }

// bool GlobalSymbolTable::add(FuncDeclNode* f) {
    // if(lookup_func(std::string(f->getName())) == -1) {
        // functions.push_back(f);
        // return true;
    // } else {
        // return false;
    // }
// }

// bool GlobalSymbolTable::add(VarDeclNode* v) {
    // printf("Adding %s to GST\n", v->getName());
    // if(local_lookup(std::string(v->getName())) == -1) {
        // this->vars.push_back(v);
        // printf("Size is now: %d\n", vars.size());
        // return true;
    // } else {
        // printf("Error adding.\n");
        // return false;
    // }
// }

// FuncDeclNode* GlobalSymbolTable::get_func(std::string n) {
    // if(int idx = lookup_func(n) != -1)
        // return functions[idx];
    // else 
        // return nullptr;
    
// }

// FuncDeclNode* GlobalSymbolTable::get_func(int idx) {
    // if(idx < size())
        // return functions[idx];
    // else 
        // return nullptr;
    
// }  
// VarDeclNode* GlobalSymbolTable::get(std::string n) {
    // if(int idx = global_lookup(n) != -1)
        // return vars[idx];
    // else 
        // return nullptr;
    
// }

// VarDeclNode* GlobalSymbolTable::get(int idx) {
    // if(idx < size())
        // return vars[idx];
    // else 
        // return nullptr;
    
// }

// bool isEqual(GlobalSymbolTable* o) {
    // // not yet implemented
// }


