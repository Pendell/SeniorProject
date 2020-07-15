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

Reset:  \u001b[0m
*/
using namespace llvm;

SymbolTable* ST = new SymbolTable();
SymbolTable* GST = new SymbolTable();


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

int DeclNode::getVal(){
    return 1000;
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

Value* DeclNode::codegen(){
    
    printf("Decl Segfaulting here...\n");
    
    if(lhs) {
        printf("LHS Codegen.\n");
        lhs->codegen();
        printf("Not breaking in LHS\n");
    }
    if(rhs) {
        printf("RHS Codegen.\n");
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

void IntegerNode::accept(Visitor* v){
    v->visit(this);
}

// LLVM Codegen method
Value* IntegerNode::codegen(){
    return ConstantInt::get(TheContext, APInt(32, value, true));
}

/***************************** BinOp NODE ************************************/
BinaryOpNode::BinaryOpNode(const char op, ExprNode* lh, ExprNode* rh) :
        op(op), lhs(lh), rhs(rh) {
            
      std::cout << std::endl << std::endl;
      std::cout << "Creating a BinaryOpNode!" << std::endl;
      std::cout << "The operation is : " << op;
      printf("This should get printed out.\n");
      printf("It should segfault here...");
      printf("Nope... it did not.\n");
      
      printf("Right here boys.\n");
      printf("It segfaulted after eval()\n");
}

BinaryOpNode::~BinaryOpNode(){}
const char* BinaryOpNode::getNodeType(){
    return "BinaryOpNode";
}


Value* BinaryOpNode::codegen(){
    switch(op){
        case '+':
            return builder.CreateAdd(lhs->codegen(), rhs->codegen());
        case '-':
            return builder.CreateSub(lhs->codegen(), rhs->codegen());
        case '*':
            return builder.CreateMul(lhs->codegen(), rhs->codegen());
        case '/':
            return builder.CreateUDiv(lhs->codegen(), rhs->codegen());
    }
    
    return nullptr;
    
}

/***************************** RETURN NODE ***********************************/
// Constructors
ReturnNode::ReturnNode() : value(nullptr) {}
ReturnNode::ReturnNode(ExprNode* rv) : value(rv) {
    printf("Constructing ReturnNode that evaluates to: %d", value->getVal());
    
    
}

const char* ReturnNode::getNodeType(){
    return "ReturnNode";
}

int ReturnNode::getVal(){
    return value->getVal();
}


bool ReturnNode::equals(ASTNode* node){
    printf("Inside ReturnNode equals()\n");
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

void ReturnNode::accept(Visitor* v){
    v->visit(this);
    if (value)
        value->accept(v);
}

Value* ReturnNode::codegen(){
    
    printf("\u001b[32mTHIS SHOULD  PRINT OUT!!!\u001b[0m");
    
    Value* retptr;
    
    if(strcmp(value->getNodeType(), "VarDeclNode") == 0){
        
        VarDeclNode* value_casted = dynamic_cast<VarDeclNode*>(value);
        printf("Returning a variable\n");
        retptr = builder.CreateLoad(NamedValues[std::string(value_casted->getName())]);
        
    } else if (strcmp(value->getNodeType(), "IntegerNode") == 0) {
        IntegerNode* value_casted = dynamic_cast<IntegerNode*>(value);
        printf("Returning an IntConstant\n");
        retptr = ConstantInt::get(TheContext, APInt(32, value->getVal()));
    } else {
        printf("Codegen'ing something else!\n");
        retptr = value->codegen();
    }
    printf("\u001b[31mTHIS SHOULD NOT PRINT OUT!!!\u001b[0m");
    return builder.CreateRet(retptr);
}

/***************************** VARDECL NODE **********************************/

VarDeclNode::VarDeclNode(const char* ty, const char* na) : type(ty), name(na) {
    value = nullptr;
}

VarDeclNode::VarDeclNode(const char* ty, const char* na, ExprNode* va) :
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

Value* VarDeclNode::codegen() {
    
    std::string name(getName());
    return builder.CreateStore(value->codegen(), NamedValues[name]);
    
}

AllocaInst* VarDeclNode::initialize() {
    
    std::string name(getName());
    init = true;
    return builder.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
    
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

Value* PrototypeNode::codegen(){
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
    StmtList::iterator it = stl->begin();
    
    int count = 0;
    while(it != stl->end()){
        printf("Statement at position %d: %s\n", count, (*it)->getNodeType());
        ++it;
        count++;
    }
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

Value* FuncDeclNode::codegen() {
    
    FunctionType* ft;
    
    // Iterates through the symbol table and allocates space for each symbol in memory
    
    printf("Inside FuncDeclNode codegen()\n");
    
    if (strcmp(getType(), "int") == 0){
        ft = FunctionType::get(Type::getInt32Ty(TheContext), false);
    }
    
    printf("1\n");
    
    
    
    Function* f = Function::Create(ft, Function::ExternalLinkage, StringRef(getName()), TheModule);
   
    // Construct the basic block - one entry, one exit. Basic string of instructions
    BasicBlock* bb = BasicBlock::Create(TheContext, "entry", f);
    
    // Store a reference of the current basic block
    bbreference = bb;
    
    // Tells the builder to insert new instructions to the end of this block
    builder.SetInsertPoint(bb);
    
    
    SymbolTable::iterator its = SymTable->begin();
    
    // Allocate memory for all local variables, and assign their allocation location
    // to the NamedValues map. This will give us a point of reference should we need
    // to mutate a variable. That is, if we need to change the variable, we change
    // what is stored at the allocation location.
    while(its != SymTable->end()) {
        printf("Inside while loop\n");
        std::string name ((*its)->getName());
        std::cout << "Created name " << name << std::endl;
        
        VarDeclNode* var = dynamic_cast<VarDeclNode*>(*its);
        NamedValues[name] = var->initialize();
        ++its;
    }
    
    printf("4\n");
    

    
    // Iterate through statements...
    StmtList::iterator it = statements->begin();
    
    int count = 1;
    
    while(it != statements->end()){
        (*it)->codegen();
        ++it;
        ++count;
    }
    
    
    
    
    
    
    verifyFunction(*f, &errs());
    printf("5\n");
}

AllocaInst* FuncDeclNode::createEntryBlockAlloca(Function* f, const std::string& name) {
    
    printf("Inside entry block alloca\n");
    
    // Create a builder object to place things at the beginning of a function block
    IRBuilder<> tmpbuilder(&f->getEntryBlock(), f->getEntryBlock().begin());
    
    // Return the alloca instruction
    unsigned int addr = 0;
    return tmpbuilder.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
    
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

void ProgramNode::accept(Visitor* v) {
    v->visit(this);
    
    if (start) {
        start->accept(v);
    }
}

void ProgramNode::compile(){
    
    printf("Starting compilation...\n");
    
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
    
    // We're at the top of the AST -> get a new module to build.
    TheModule = new Module(std::string(getName()), TheContext);
    
    // If we're not the only node, call codegen on the rest of the tree
    if(start)
        start->codegen();
    
    for (auto &funct : *TheModule) {
        for (auto &basic_block : funct) {
            StringRef bbName(basic_block.getName());
            errs() << "BasicBlock: "  << bbName << "\n";
            Instruction* i = basic_block.getTerminator();
            if(!i) {
                printf("No termination.\n");
                builder.CreateRet(ConstantInt::get(TheContext, APInt(32, 1)));
            }
        }
    }

    
    
    
    
    // Once the module is built, compile llvm IR to object code.
    compile();
}
















