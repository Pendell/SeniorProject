#include "node.h"

Parameter::Parameter(const char* ty, const char* na) {
    type = std::string(ty);
    name = std::string(na);
}

Parameter::~Parameter() {

}

const char* Parameter::getType() {
    return type.c_str();
}

const char* Parameter::getName() {
    return name.c_str();
}

Value* Parameter::codegen() {
    // return NamedValues[name];
}