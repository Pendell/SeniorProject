#include "node.h"

FuncPrototype::FuncPrototype(std::string& t, std::string& n, std::vector<std::pair<std::string, std::string>*> a) {
    type = t;
    name = n;
    args = std::move(a);
    printf("\n\n\n");
    for(int i = 0; i < args.size(); i++){
        printf("The time is: %d\n", i);
        printf("Parameter #%d: %s %s\n", i, args[i]->first.c_str(), args[i]->second.c_str());
    }
    printf("\n\n\n");
    
}

FuncPrototype::~FuncPrototype() {

}

const char* FuncPrototype::getType() {
    return type.c_str();
}

const char* FuncPrototype::getName() {
    return name.c_str();
}


const char* FuncPrototype::getNodeType() {
    return "FuncPrototype";
}

Function* FuncPrototype::codegen(){
    
    std::vector<Type*> argTypes(args.size());
    
    
    for(int i = 0; i < args.size(); i++){
        
        if(args[i]->first == std::string("int")) {
            argTypes[i] = Type::getInt32Ty(TheContext);
            
        }
        
    }
    
    FunctionType* ftype;
    
    if (type == "int") {
        ftype = FunctionType::get(Type::getInt32Ty(TheContext), argTypes, false);
    } else {
        exit(99);
    }
    
    Function* f = Function::Create(ftype, Function::ExternalLinkage, name, TheModule);
    unsigned idx = 0;
    
    for(auto &argument : f->args()) {
        printf("Setting name: %s\n", args[idx]->second.c_str());
        argument.setName(args[idx++]->second);
    }
    
    
    return f;
}