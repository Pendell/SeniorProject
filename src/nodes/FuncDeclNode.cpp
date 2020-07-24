#include "node.h"

FuncDeclNode::FuncDeclNode(FuncPrototype* p, StmtList* stl, SymbolTable* st) : proto(p), statements(stl), SymTable(st) {
    
}

FuncDeclNode::~FuncDeclNode() {
    
}
    
const char* FuncDeclNode::getType() {
    return proto->getType();
}

const char* FuncDeclNode::getName() {
    return proto->getName();
}
    
const char* FuncDeclNode::getNodeType() {
    return "FuncDeclNode";
}

bool FuncDeclNode::equals(ASTNode* node) {
    
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
        
        std::list<StmtNode*>::iterator it1 = statements->begin();
        std::list<StmtNode*>::iterator it2 = node_casted->statements->begin();
        /* Notes:
         * Iterators point at the objects that make up the thing we're iterating over.
         * So in this case, I think they should all point at StmtNodes of some kind.
         */
         
        int count = 0; 
        
        if (it1 == statements->end() && it2 == node_casted->statements->end()) {
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

// void FuncDeclNode::accept(Visitor* v) {
    // // Navigate through statement list
    // if (!statements->empty()){
        // std::list<StmtNode*>::iterator it = statements->begin();
        // while(it != statements->end()) {
            // (*it)->accept(v);
            // ++it;
        // }
    // }
    // v->visit(this);
// }

Value* FuncDeclNode::codegen() {
    
    
    Function* f = TheModule->getFunction(proto->getName());
    
    if(!f)
        f = proto->codegen();
    if(!f)
        return nullptr;
    if(!f->empty()) {
        return nullptr;
        exit(99);
    }
    
    fref = f;
    
    // Construct the basic block - one entry, one exit. Basic string of instructions
    BasicBlock* bb = BasicBlock::Create(TheContext, "entry", f);
    // Store a reference of the current basic block
    bbreference = bb;
    // Tells the builder to insert new instructions to the end of this block
    builder.SetInsertPoint(bb);
    
    
    // Allocate space for parameters
    NamedValues.clear();
    int idx = 0;
    for(auto& argument : f->args()) {
        std::string type(proto->args[idx]->first);
        
        Type* t;
        if (type == "int") {
            t = Type::getInt32Ty(TheContext);
        }
        AllocaInst* alloca = createEntryBlockAlloca(f, argument.getName());
        builder.CreateStore(&argument, alloca);
        NamedValues[std::string(argument.getName())] = alloca;
        idx++;
    }
    // Iterate through statements and generate code
    StmtList::iterator it = statements->begin();
    int count = 0;
    while(it != statements->end()){
        (*it)->codegen();
        ++count;
        ++it;
    }
    
    verifyFunction(*f, &errs());
}

AllocaInst* FuncDeclNode::createEntryBlockAlloca(Function* f, const std::string& name) {
    
    
    // Create a builder object to place things at the beginning of a function block
    IRBuilder<> tmpbuilder(&f->getEntryBlock(), f->getEntryBlock().begin());
    
    // Return the alloca instruction
    unsigned int addr = 0;
    return tmpbuilder.CreateAlloca(Type::getInt32Ty(TheContext), 0, name.c_str());
    
}