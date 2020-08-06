#include "node.h"

FuncPrototype::FuncPrototype(std::string t, std::string n, std::vector<Parameter*> a = {}) {
    type = t;
    name = n;
    args = std::move(a);
    
}

FuncPrototype::~FuncPrototype() {

}

std::string FuncPrototype::getType() {
    return type;
}

std::string FuncPrototype::getName() {
    return name;
}


const char* FuncPrototype::getNodeType() {
    return "FuncPrototype";
}

Function* FuncPrototype::codegen(){
    
    // Vector for argument types
    std::vector<Type*> argTypes(args.size());
    for(int i = 0; i < args.size(); i++){
        if(args[i]->getType() == std::string("int")) {
            argTypes[i] = Type::getInt32Ty(TheContext);
        }
    }
    
    // Setting LLVM Function type
    FunctionType* ftype;
    if (type == "int") {
        ftype = FunctionType::get(Type::getInt32Ty(TheContext), argTypes, false);
    } else {
        exit(99);
    }
    
    // Building the LLVM Function and setting the function arguments
    Function* f = Function::Create(ftype, Function::ExternalLinkage, std::string(getName()), TheModule);
    unsigned idx = 0;
    for(auto &argument : f->args()) {
        argument.setName(args[idx++]->getName());
    }
    
    return f;
}

bool FuncPrototype::operator==(FuncPrototype& rhs) const {
    if(type == rhs.type && name == rhs.name && args == rhs.args)
        return true;
    else
        return false;
}