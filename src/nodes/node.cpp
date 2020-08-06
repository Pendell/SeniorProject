#include "node.h"

/* Fun Stuff...
Black: \u001b[30m
Red: \u001b[31m
Green: \u001b[32m
Yellow: \u001b[33m
Blue: \u001b[34m
Magenta: \u001b[35m
Cyan: \u001b[36m
White: \u001b[37m

Reset:\u001b[0m
*/
using namespace llvm;

/******************************* AST NODE ************************************/
const char* ASTNode::getNodeType(){
    return "ASTNode";
}

bool ASTNode::equals (ASTNode* node){
    return (getNodeType() == node->getNodeType());
}



/******************************* STMT NODE ***********************************/
const char* StmtNode::getNodeType(){
    return "StmtNode";
}

bool StmtNode::equals(ASTNode* node){
    return (getNodeType() == node->getNodeType());
}

/******************************* EXPR NODE ***********************************/
int ExprNode::getVal(){}
const char* ExprNode::getNodeType(){
    return "ExprNode";
}

bool ExprNode::equals(ASTNode* node){
    return (getNodeType() == node->getNodeType());
}


/******************************* DECL NODE ***********************************/
DeclNode::DeclNode() {
    lhs = nullptr;
    rhs = nullptr;
}
DeclNode::~DeclNode() {}

const char* DeclNode::getName() {
    return NULL;
}

const char* DeclNode::getNodeType(){
    return "DeclNode";
}

bool DeclNode::isLast(){
    return (lhs == nullptr && rhs==nullptr);
}

bool DeclNode::equals(ASTNode* node){      
    /* CHECK NODE TYPES **********************************/
    if (getNodeType() != node->getNodeType()) {
        return false;
    }
    
    DeclNode* node_casted = dynamic_cast<DeclNode*>(node);  
    
    if(isLast() && node_casted->isLast()) {
        return true;
    } else {
        if(rhs != nullptr) {
            return (lhs->equals(node_casted->lhs) && rhs->equals(node_casted->rhs));
        } else {
            return (lhs->equals(node_casted->lhs));
        }
    }
}

int DeclNode::getVal(){
    return 1000;
}

Value* DeclNode::codegen(){
    
    
    if(lhs) {
        lhs->codegen();
    }
    
    if(rhs) {
        rhs->codegen();
    }
}


/***************************** INTEGER NODE **********************************/
// Constructor
IntegerNode::IntegerNode(int v) : value(v) {}

// Return the value held in this integer node
int IntegerNode::getVal(){
    return value;
}

// Return the type of node this is
const char* IntegerNode::getNodeType(){
    return "IntegerNode";
}

// Compare this ASTNode to another ASTNode.
// Returns TRUE if the following are met:
//   this nodeType = that nodeType
//   this value = that value
bool IntegerNode::equals(ASTNode* node){
    printf("Inside IntegerNode equals()\n");
    if (getNodeType() != node->getNodeType()) {
        return false;
    } else {
        IntegerNode* node_casted = dynamic_cast<IntegerNode*>(node);
        if (getVal() == node_casted->getVal()) {
            printf("\tIntegerNodes Equal. Returning true\n");
            return true;
        } else {
            printf("\tIntegerNodes not Equal. Returning false");
            return false;
        }
    }
}

// void IntegerNode::accept(Visitor* v){
    // v->visit(this);
// }

// LLVM Codegen method
Value* IntegerNode::codegen(){
    return ConstantInt::get(TheContext, APInt(32, value, true));
}



/***************************** BinOp NODE ************************************/
BinaryOpNode::BinaryOpNode(const char op, ExprNode* lh, ExprNode* rh) :
        op(op), lhs(lh), rhs(rh) {
        
}

BinaryOpNode::~BinaryOpNode(){}
const char* BinaryOpNode::getNodeType(){
    return "BinaryOpNode";
}


Value* BinaryOpNode::codegen(){
    
    Value* lhsval = lhs->codegen();
    Value* rhsval = rhs->codegen();
    switch(op){
        case '+':
            return builder.CreateAdd(lhsval, rhsval);
        case '-':
            return builder.CreateSub(lhsval, rhsval);
        case '*':
            return builder.CreateMul(lhsval, rhsval);
        case '/':
            return builder.CreateUDiv(lhsval, rhsval);
    }
    
    return nullptr;
    
}

/***************************** RETURN NODE ***********************************/
// Constructors
ReturnNode::ReturnNode() : value(nullptr) {}
ReturnNode::ReturnNode(ExprNode* rv) : value(rv) {
    
    
}

const char* ReturnNode::getNodeType(){
    return "ReturnNode";
}

int ReturnNode::getVal(){
    return value->getVal();
}


bool ReturnNode::equals(ASTNode* node){
    if (getNodeType() != node->getNodeType())
        return false;
    
    else {
        ReturnNode* node_casted = dynamic_cast<ReturnNode*>(node);
        // Check Both Null
        if(!value && !node_casted->value) {
            return true;
            
        // Check Both not null, if so, compare children
        } else if (value && node_casted->value) {
            
            if (value->equals(node_casted->value)){
                printf("Return Nodes Equal. Returning true\n");
                return true;
            } else {
                printf("Return Nodes not Equal. Returning false\n");
                return false;
            }
            
        } else {
            printf("Return Nodes not Equal. Returning false\n");
            return false;
        }
    }
}

// void ReturnNode::accept(Visitor* v){
    // v->visit(this);
    // if (value)
        // value->accept(v);
// }

Value* ReturnNode::codegen(){
    
    
    Value* retptr;
    
        
    if(strcmp(value->getNodeType(), "VarDeclNode") == 0){
        VarDeclNode* value_casted = dynamic_cast<VarDeclNode*>(value);
        retptr = builder.CreateLoad(NamedValues[std::string(value_casted->getName())]);
        
    } else if (strcmp(value->getNodeType(), "IntegerNode") == 0) {
        IntegerNode* value_casted = dynamic_cast<IntegerNode*>(value);
        retptr = ConstantInt::get(TheContext, APInt(32, value->getVal()));
    } else {
        retptr = value->codegen();
    }
    
    return builder.CreateRet(retptr);
}

/***************************** VARDECL NODE **********************************/


VarDeclNode::VarDeclNode(const char* ty, const char* na, ExprNode* va = nullptr) :
                        type(ty), name(na), value(va) {
}

VarDeclNode::~VarDeclNode(){
    
}

// Returns the typing of the data stored -- NOT the type of the node
const char* VarDeclNode::getType(){
    return type;
}

const char* VarDeclNode::getName(){
    return name;
}

int VarDeclNode::getVal(){
    return value->getVal();
}

const char* VarDeclNode::getNodeType(){
    return "VarDeclNode";
}

bool VarDeclNode::equals(ASTNode* node) {
    if(getNodeType() != node->getNodeType()) {
        return false;
    }
    
    VarDeclNode* node_casted = dynamic_cast<VarDeclNode*>(node);
    printf("\tVarDeclNode -- THIS node & THAT node info:\n");
    printf("\tTHIS node type and name: %s %s\n", getType(), getName());
    printf("\tTHAT node type and name: %s %s\n", node_casted->getType(), node_casted->getName());
    
    
    // compare types, names, then values
    if (strcmp(getType(), node_casted->getType()) != 0) {
        printf("\t\u001b[35mVarDeclNode Type mismatch\u001b[0m: THIS type %s != THAT type %s\n", getType(), node_casted->getType());
        return false;
    } else if (strcmp(getName(), node_casted->getName()) != 0) {
        printf("\t\u001b[35mVarDeclNode Name mismatch\u001b[0m: THIS name %s != THAT name %s\n", getName(), node_casted->getName());
        return false;
    } else { // Compare values
    
        if (value == nullptr && node_casted->value == nullptr) {
            return true;
            
        } else if (value == nullptr ^ node_casted->value == nullptr) {
            
            if(value == nullptr) {
                printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS nullptr != THAT %s", node_casted->getVal());
            } else if (node_casted == nullptr){
                printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS %s != THAT nullptr",getVal());
            }
            
        return false;
        } else if (!value->equals(node_casted->value)) {
            printf("\t\u001b[35m VarDeclNode Value Mismatch: THIS %s != THAT %s",getVal(), node_casted->getVal());
        } else {
            return true;
        }
    }
}

Value* VarDeclNode::codegen() {
    
    std::string name(getName());
    
    AllocaInst* alloca = builder.CreateAlloca(getLLVMType(), 0, name);
    // Insert a new allocation into the allocation table
    symref->allocations[name] = alloca;
    if(value) {
        return builder.CreateStore(value->codegen(), symref->allocations[name]);
    } else
        return alloca;
    
}


// AllocaInst* VarDeclNode::initialize() {
    // printf("VarDeclNode initialize()\n");
    // std::string name(getName());
    // init = true;
    // printf("Returning from init()\n");
    // return builder.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
    
// }

Type* VarDeclNode::getLLVMType(){
    if(strcmp(getType(), "int") == 0){
        return Type::getInt32Ty(TheContext);
    }
}



/***************************** PROGRAM NODE *********************************/
ProgramNode::ProgramNode() { 
    start = nullptr;
}

ProgramNode::ProgramNode(char* src) : srcName(src) {
    start = nullptr;
}
    
const char* ProgramNode::getNodeType() {
    return "ProgramNode";
}

bool ProgramNode::equals(ASTNode* node) {
    
    if (getNodeType() != node->getNodeType()) {
        return false;
    } else {
        
        ProgramNode* node_casted = dynamic_cast<ProgramNode*>(node);
        
        if (start == nullptr ^ node_casted->start == nullptr)
            return false;
        else
            return start->equals(node_casted->start);
    }
}

const char* ProgramNode::getName() {
    return srcName;
}

// void ProgramNode::accept(Visitor* v) {
    // v->visit(this);
    
    // if (start) {
        // start->accept(v);
    // }
// }

void ProgramNode::compile(){
    
    auto TargetTriple = sys::getDefaultTargetTriple();
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();
    
    std::string error;
    
    // Grab the target Triple we're compiling to.
    auto Target = TargetRegistry::lookupTarget(TargetTriple, error);
    
    if(!Target){
        errs() << error;
        return;
    }
    
    // Generic CPU without any additional features, options, or anything.
    auto CPU = "generic";
    auto Features = "";
    TargetOptions opt;
    
    // No idea what this line does...
    auto RM = Optional<Reloc::Model>();
    
    // Define the target machine for the module to compile to.
    auto TargetMachine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);
    
    // Configure the module to specify the target and data layout.
    TheModule->setDataLayout(TargetMachine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);
    
    // OUTPUT FILE STUFF
    // Declare the name of the file and error codes
    auto Filename = "test.o";
    std::error_code EC;
    // set the destination.
    raw_fd_ostream dest(Filename, EC, sys::fs::OF_None);
    
    // Print errors
    if (EC) {
      errs() << "Could not open file: " << EC.message();
      return;
    }
    
    // Create a new passmanager -- this is what passes over the module and emits code
    legacy::PassManager pass;
    
    // Define the object type to be an object file
    auto FileType = TargetMachine::CGFT_ObjectFile;
    
    // Catch errors
    if (TargetMachine->addPassesToEmitFile(pass, dest, nullptr, FileType)) {
      errs() << "TargetMachine can't emit a file of this type";
      return;
    }

    // Run the pass
    pass.run(*TheModule);
    
    // Print the pass to the file.
    dest.flush();
    
    // Print out the module to the terminal for debugging
    TheModule->print(errs(), nullptr);
}

Value* ProgramNode::codegen() {
    
    printf("Beginning CODE GENERATION\n");
    
    std::string name(getName());
    // We're at the top of the AST -> get a new module to build.
    TheModule = new Module(name, TheContext);
    
    // If we're not the only node, call codegen on the rest of the tree
    if(start)
        start->codegen();
    
    
    // Once the module is built, compile llvm IR to object code.
    compile();
}
















