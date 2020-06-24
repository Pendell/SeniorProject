#include "visitor.h"

// Visitor
Visitor::Visitor(){
    printf("New Visitor Created\n");
}

Visitor::~Visitor() { }

void Visitor::visit(ASTNode* node) { }
void Visitor::visit(DeclNode* node) { }
void Visitor::visit(IntegerNode* node) { }
void Visitor::visit(ReturnNode* node) { }
void Visitor::visit(VarDeclNode* node) { }
void Visitor::visit(FuncDeclNode* node) { }
void Visitor::visit(PrototypeNode* node) { }
void Visitor::visit(ProgramNode* node) { }


PrintVisitor::PrintVisitor() {
    printf("New Print Visitor Created\n");
}

PrintVisitor::~PrintVisitor() {
    printf("Deleting Print Visitor\n");
}


void PrintVisitor::visit(ASTNode* node) {
    printf("Visiting an AST Node!\n");
}

void PrintVisitor::visit(DeclNode* node){
    printf("Visiting a DeclNode!\n");
}

void PrintVisitor::visit(IntegerNode* node){
    printf("Visiting an Integer Node!\n");
}

void PrintVisitor::visit(ReturnNode* node){
    printf("Visiting a Return Node!\n");
}

void PrintVisitor::visit(VarDeclNode* node){
    printf("Visiting a VarDeclNode!\n");
}

void PrintVisitor::visit(FuncDeclNode* node){
    printf("Visiting a FuncDecl Node!\n");
}

void PrintVisitor::visit(ProgramNode* node){
    printf("Visiting a Program Node!\n");
}

void PrintVisitor::visit(PrototypeNode* node){
    printf("Visiting a Prototype Node!\n");
}

using namespace llvm;

// CodeGenVisitor
CodeGenVisitor::CodeGenVisitor() {
    printf("New CodeGenVisitor Created\n");
}

CodeGenVisitor::~CodeGenVisitor() {}

Value* CodeGenVisitor::visit(ASTNode* node) {}
Value* CodeGenVisitor::visit(DeclNode* node) {}

Value* CodeGenVisitor::visit(IntegerNode* node) {
    printf("Generating code for IntegerNode\n");
    // Creates a 32bit integer constant whose value is node->getVal(), is it signed? True
    Value* i = ConstantInt::get(TheContext, APInt(32, node->getVal(), true));
    return i;
    
}

Value* CodeGenVisitor::visit(ReturnNode* node) {
    printf("Generating code for a ReturnNode\n");
}
Value* CodeGenVisitor::visit(VarDeclNode* node) {
    printf("Generating code for VarDeclNode\n");
    // // Check if the value is in the llvm symbol table
    // Value* v = NamedValues[node->getName()]; 
    // if(!v) {
        // printf("Error! Unknown variable name: %s\n", node->getName());
        // return nullptr;
    // } else {
        // return v;
    // }
    
}

/* The precursor to the function code generation
 * The 'integers' vector is later going to be used to track, declare, and
 * assign names to the arguments passed into the function (but args isn't 
 * implemented yet so...) 
 * The prototype node is a node that contains basic function information
 * like name, type, and arguments. 
 */
Function* CodeGenVisitor::visit(PrototypeNode* node) {
    printf("Generating code for PrototypeNode\n");
    // // function arguments/parameters are still unimplemented
    // std::vector<Type*> integers(0, Type::getInt32Ty(TheContext));
    
    // // Function typing
    // FunctionType* FT;
    
    // FT = FunctionType::get(Type::getInt32Ty(TheContext), integers, false);
    
    // Function* F = Function::Create(FT, Function::ExternalLinkage, node->getName(), TheModule.get());
    
    // /* This function will assign temp names to the arguments when they get implemented
    // unsigned Idx = 0;
    // for (auto &args : F->args())
        // Args.setName(Args[Idx++]);
    // */
    
    // return F;
    
}

/* The actual function code generation method to emit llvm IR. First we generate
 * code using the basic information from the prototype, then we round out the 
 * code generation by using blocks. Right now, functions are just large blocks
 * because I have zero control flow implemented.
 */
Function* CodeGenVisitor::visit(FuncDeclNode* node) {
    printf("Generating code for FuncDeclNode\n");
    
    // // Check for an existing function (searching for 'extern's, I think...
    // Function* TheFunction = TheModule->getFunction(node->prototype->getName());
    
    // // Create a function from the prototype;
    // if(!TheFunction)
        // TheFunction = visit(node->prototype);
    
    // if(!TheFunction) { // Either print an error, or continue with code generation
        // printf("Error... Function not created.\n");
        // return nullptr;
        
    // } else {
        // // Create a new basic block named "entry" to start insertion
        // BasicBlock* block = BasicBlock::Create(TheContext, "entry", TheFunction);
        // Builder.SetInsertPoint(block);
        
        // NamedValues.clear();
        // /* Record function arguments in the symbol table
        // for (auto &Args : TheFunction->args())
            // NamedValues[Arg.getname()] = &Arg;
        // */
        
        // if (Value* retVal = Body->accept(this)) {
            // Builder.CreateRet(retVal);
            // verifyFunction(*TheFunction);
            // return TheFunction;
            
        // } else {
            // // Error...
            // printf("Error creating the function.\n");
            // TheFunction->eraseFromParent();
            // return nullptr;
            
        // }
    // }
    
    
}
Value* CodeGenVisitor::visit(ProgramNode* node) {
    printf("Generating code for ProgramNode\n");
    
    // Creating an llvm module (translational unit?)
    TheModule = std::make_unique<Module>(StringRef(node->getName()), TheContext);
    
    // TheModule.setModuleIdentifer(tringRef(node->getName()));
    // TheModule.setSourceFileName(StringRef(node->getName()));
    
    
}