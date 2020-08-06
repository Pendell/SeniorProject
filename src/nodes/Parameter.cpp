#include "node.h"

Parameter::Parameter(std::string ty, std::string na) {
    type = ty;
    name = na;
}

Parameter::~Parameter() {

}

std::string Parameter::getType() {
    return type;
}

std::string Parameter::getName() {
    return name;
}

Value* Parameter::codegen() {
    // return NamedValues[name];
}