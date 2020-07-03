#include "node.h"

using namespace llvm;

/******************************* AST NODE ************************************/
const char* ASTNode::getNodeType(){
    return "ASTNode";
}

bool ASTNode::equals (ASTNode* node){
    printf("Inside ASTNode equals()\n");
    return (getNodeType() == node->getNodeType());
}

void ASTNode::accept(Visitor* v){
    v->visit(this);
}

/******************************* STMT NODE ***********************************/
const char* StmtNode::getNodeType(){
    return "StmtNode";
}

bool StmtNode::equals(ASTNode* node){
    printf("Inside StmtNode equals()\n");
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

const char* DeclNode::getNodeType(){
    return "DeclNode";
}

bool DeclNode::isLast(){
    return (lhs == nullptr && rhs==nullptr);
}

bool DeclNode::equals(ASTNode* node){
    printf("Inside DeclNode equals()\n");        
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

void DeclNode::accept(Visitor* v){
    printf("Inside DeclNode accept().\n");
    
    v->visit(this);
    
    printf("Is the DeclNode visit breaking?\n");
    
    if(lhs) {
        printf("lhs? Yep!\n");
        lhs->accept(v);
    }
    
    if(rhs) {
        printf("rhs? Yessir!\n");
        rhs->accept(v);
    }
}

void DeclNode::codegen(){
    
    if(lhs)
        lhs->codegen();
    
    if(rhs)
        rhs->codegen();
    
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

void IntegerNode::accept(Visitor* v){
    v->visit(this);
}

// LLVM Codegen method
void IntegerNode::codegen(){
    //return ConstantInt::get(TheContext, APInt(32, value, true));
}

/***************************** RETURN NODE ***********************************/
// Constructors
ReturnNode::ReturnNode() : retVal(nullptr) {}
ReturnNode::ReturnNode(ExprNode* rv) : retVal(rv) {}

const char* ReturnNode::getNodeType(){
    return "ReturnNode";
}

int ReturnNode::getVal(){
    return retVal->getVal();
}


bool ReturnNode::equals(ASTNode* node){
    printf("Inside ReturnNode equals()\n");
    if (getNodeType() != node->getNodeType())
        return false;
    
    else {
        ReturnNode* node_casted = dynamic_cast<ReturnNode*>(node);
        // Check Both Null
        if(!retVal && !node_casted->retVal) {
            return true;
            
        // Check Both not null, if so, compare children
        } else if (retVal && node_casted->retVal) {
            
            if (retVal->equals(node_casted->retVal)){
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

void ReturnNode::accept(Visitor* v){
    v->visit(this);
    if (retVal)
        retVal->accept(v);
}

void ReturnNode::codegen(){
    builder.CreateRet(ConstantInt::get(Type::getInt32Ty(TheContext), retVal->getVal()));
}

/***************************** VARDECL NODE **********************************/

VarDeclNode::VarDeclNode(const char* ty, const char* na) : type(ty), name(na) {
    value = nullptr;
}

VarDeclNode::VarDeclNode(const char* ty, const char* na, ExprNode* va) :
                        type(ty), name(na), value(va) { }

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
    printf("Inside VarDeclNode equals()\n");
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

void VarDeclNode::accept(Visitor* v){
    v->visit(this);
    if (value)
        value->accept(v);
}

void VarDeclNode::codegen() {
    
    // printf("Hmm.... inside vardeclnode stuff.\n");
    // printf("Varname: %s\n", this->getName());
    // Value* V = NamedValues[this->getName()];
    // if(!V)
        // printf("Unknown Variable Name\n");
    // return V;
}

/***************************** PROTOTYPE NODE ********************************/
PrototypeNode::PrototypeNode(const char* ty, const char* na) : type(ty), name(na) { }

PrototypeNode::~PrototypeNode() {

}

void PrototypeNode::accept(Visitor* v) {
    v->visit(this);
}

const char* PrototypeNode::getName(){
    return name;
}

const char* PrototypeNode::getType(){
    return type;
}

void PrototypeNode::codegen(){
    // std::vector<Type*> Ints(args.size(), Type::getInt32Ty(TheContext));
    
    // //
    // FunctionType* ft = FunctionType::get(Type::getInt32Ty(TheContext), Ints, false);
    
    // Function *f = Function::Create(ft, Function::InternalLinkage, name, TheModule.get());
    
    // // Set the name of each of the functions arguments according to the names given
    // // in the prototype.
    // unsigned Idx = 0;
    // for(auto &Arg : f->args())
        // Arg.setName(args[Idx++]);
    
    // return f;
    
}

/***************************** FUNCDECL NODE *********************************/
FuncDeclNode::FuncDeclNode(const char* ty, const char* na, StmtList* stl, SymbolTable* st) : type(ty), name(na), statements(stl), SymTable(st){
    // prototype = new PrototypeNode(ty, na);
}

FuncDeclNode::~FuncDeclNode() {
    delete prototype;
}
    
const char* FuncDeclNode::getType() {
    return type; //prototype->getType();
}

const char* FuncDeclNode::getName() {
    return name; //prototype->getName();
}
    
const char* FuncDeclNode::getNodeType() {
    return "FuncDeclNode";
}

bool FuncDeclNode::equals(ASTNode* node) {
    
    printf("Inside FuncDeclNode equals()\n");
    
    if (getNodeType() != node->getNodeType()) {
        return false;
    } else {
        
        FuncDeclNode* node_casted = dynamic_cast<FuncDeclNode*>(node);
        
        // Simple Comparisons -- Types and names
        if (strcmp(getType(), node_casted->getType()) != 0) {      
            printf("\t\u001b[35mType mismatch\u001b[0m: THIS type (%s) != THAT type (%s)\n", getType(), node_casted->getType());
            return false;
        } else if (strcmp(getName(), node_casted->getName()) != 0) { 
            printf("\t\u001b[35mName mismatch\u001b[0m: THIS name (%s) != THAT name (%s)\n", getName(), node_casted->getName());
            return false;
        }
        
        // Checking statement lists
        if (statements->empty() ^ node_casted->statements->empty()){
            printf("\t\u001b[35mStatement List mismatch\u001b[0m: ");
            if(statements->empty())
                printf("This statement list empty\n");
            else
                printf("Node_casted statement list empty\n");
            return false;
        }
        
        // Parameter Checking ...
        // Parameters themselves aren't implemented so ...
        // NOT YET IMPLEMENTED
        
        // Symbol Table Checking ...
        // NOT YET IMPLEMENTED
        
        
        
        // Now we compare statement lists, which is actually
        // just a list of nodes to run equals() on.
        StmtList::iterator it1 = statements->begin();
        StmtList::iterator it2 = node_casted->statements->begin();
        /* Notes:
         * Iterators point at the objects that make up the thing we're iterating over.
         * So in this case, I think they should all point at StmtNodes of some kind.
         */
         
        int count = 0; 
        
        if (it1 == statements->end()  && it2 == node_casted->statements->end()) {
            return true;
        } else if (it1 == statements->end() ^ it2 == node_casted->statements->end()) {
            return false;
        }
        
        // Check Statement lists now.
        printf("Navigating and comparing Statement Lists...\n");
        while(it1 != statements->end() && it2 != node_casted->statements->end()){
            
            printf("\nNodes at position %d in StatementLists:\n", count);
            
            printf("\t\tTHIS type: %s", (*it1)->getNodeType());
            printf("\tTHAT type: %s\n\n", (*it2)->getNodeType());
            if (!((*it1)->equals(*it2))){
                printf("\t\u001b[35mASTNode Type mismatch\u001b[0m: THIS %s != THAT %s\n", (*it1)->getNodeType(), (*it2)->getNodeType());
                return false;
            } else {
                ++it1;
                ++it2;
                ++count;
            }
        }
        return true;
    }
}

void FuncDeclNode::accept(Visitor* v) {
    
    // Navigate through statement list
    if (!statements->empty()){
        
        StmtList::iterator it = statements->begin();
        
        while(it != statements->end()) {
            (*it)->accept(v);
            ++it;
            
        }
    }
    
    v->visit(this);
}

void FuncDeclNode::codegen() {
    
    FunctionType* ft;
    
    if (strcmp(getType(), "int") == 0){
        ft = FunctionType::get(Type::getInt32Ty(TheContext), false);
    }
    
    Function* f = Function::Create(ft, Function::ExternalLinkage, StringRef(getName()), TheModule);
    
    // Construct the basic block - one entry, one exit. Basic string of instructions
    BasicBlock* bb = BasicBlock::Create(TheContext, "entry", f);
    
    // Tells the builder to insert new instructions to the end of this block
    builder.SetInsertPoint(bb);
    
    // Iterate through statements...
    StmtList::iterator it = statements->begin();
    
    int count = 1;
    while(it != statements->end()){
        (*it)->codegen();
        ++it;
        ++count;
    }
    
    verifyFunction(*f, &errs());
    
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
    
    if (getNodeType() != node->getNodeType())
        return false;
    else {
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

void ProgramNode::accept(Visitor* v) {
    v->visit(this);
    
    if (start) {
        start->accept(v);
    }
}

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

void ProgramNode::codegen() {
    
    TheModule = new Module(std::string(getName()), TheContext);
    if(start)
        start->codegen();
    
    compile();
}
















