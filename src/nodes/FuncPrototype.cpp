#include "node.h"

FuncPrototype::FuncPrototype(std::string t, std::string n, std::vector<std::pair<std::string, std::string>*> a = {}) {
    type = t;
    name = n;
    std::cout << "Type & Name: " << type << " " << name << std::endl;
    args = std::move(a);
    
}

FuncPrototype::~FuncPrototype() {

}

std::string FuncPrototype::getType() {
    printf("Getting type: %s\n", type.c_str());
    return type;
}

std::string FuncPrototype::getName() {
    printf("Getting name: %s\n", name.c_str());
    return name;
}


const char* FuncPrototype::getNodeType() {
    return "FuncPrototype";
}

Function* FuncPrototype::codegen(){
    
    printf("Inside FPrototypeCodegen()\n");
    
    
    std::vector<Type*> argTypes(args.size());
    
    printf("Assigning parameter types.\n");
    printf("Num of parameters: %d\n", args.size());
    for(int i = 0; i < args.size(); i++){
        if(args[i]->first == std::string("int")) {
            argTypes[i] = Type::getInt32Ty(TheContext);
        }
    }
    
    FunctionType* ftype;
    
    printf("Setting Function Type.\n");
    if (type == "int") {
        printf("Setting type to INT\n");
        ftype = FunctionType::get(Type::getInt32Ty(TheContext), argTypes, false);
    } else {
        exit(99);
    }
    
    printf("Creating Function.\n");
    printf("It broke.\n");
    printf("The name of function will be: %s\n", name.c_str());
    Function* f = Function::Create(ftype, Function::ExternalLinkage, std::string(getName()), TheModule);
    unsigned idx = 0;
    
    printf("Doing argument stuff.\n");
    for(auto &argument : f->args()) {
        printf("Setting name: %s\n", args[idx]->second.c_str());
        argument.setName(args[idx++]->second);
    }
    
    printf("Finishing func prototype codegen()\n");
    return f;
}

bool FuncPrototype::operator==(FuncPrototype& rhs) const {
    if(type == rhs.type && name == rhs.name && args == rhs.args)
        return true;
    else
        return false;
}