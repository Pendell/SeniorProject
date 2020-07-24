#include "node.h"

CharacterNode::CharacterNode(int v) : value(v) {

}

CharacterNode::~CharacterNode() {}

int CharacterNode::getVal() {
    return value;
}

const char* CharacterNode::getNodeType() {
    return "CharacterNode";
}

Value* CharacterNode::codegen() {
    return ConstantInt::get(TheContext, APInt(8, value, false));
}
