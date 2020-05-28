/* Alex Pendell : alex.pendell@gmail.com
 * SeniorProject - Compiler : node.h
 * Spring 2020
 * Senior Project Advisor: Dr. Ladd
 */

#include <iostream>
#include <vector>


// Statement Node
 class StmtNode;

// Expression Node
 class ExprNode;

// Variable Declarations
 class VarDeclNode;


// Define the base 'node' objects we will derive everything else from...
// Everything is empty right now because all nodes will be some form of
// derivative of these nodes. I think I'll need the codeGen in here when
// I get llvm working.
class ASTNode {};
class StmtNode : public ASTNode {};
class ExprNode : public ASTNode {
  public:
    void printVal() {}
};


// Typedef the lists, because I'm lazy
typedef std::vector<StmtNode*> StmtList;
typedef std::vector<ExprNode*> ExprList;


/****************************** EXPRESSIONS **********************************/


/* Block
 * This is basically a collection of statements; so there's only one entry
 * point and one exit point with no branching in a block. So the statements
 * are just basically executed line by line.
 */
class Block : public ExprNode {
    
  public:
    StmtList statements;
    
    Block() {
        std::cout << std::endl;
        std::cout << "Block created!" << std::endl;
        std::cout << std::endl;
    }
    
    
};

/* IdNode
 * This node is for creating and storing names and identifiers
 */
class IdNode : public ExprNode {
  public:
    std::string name;
    
    /*IdNode(std::string& na) : name(na) {
        std::cout << "ID Node Created! The ID: " << name << std::endl;
    }*/
    
    IdNode(std::string& na) : name(na) {
        std::cout << std::endl << std::endl;
        std::cout << "ID Node Created! The ID: " << name << std::endl;
        std::cout << std::endl << std::endl;
    }
};


/* IntegerNode
 * for IntegerConstants
 */
class IntegerNode : public ExprNode {
  public:
    int value;
    
    // Constructor
    IntegerNode(int v) : value(v) {
        std::cout << std::endl << std::endl;
        std::cout << "Integer Node Created! Value = " << value << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    // Print out the value for debugging purposes.
    int printVal(){
        std::cout << value;
    
    }
};

/* ReturnNode
 * for return expressions
 */
class ReturnNode : public ExprNode {
  public:
    ExprNode* retVal;
    
    ReturnNode(ExprNode) { // return null for now
        retVal = NULL;
    }
};

/* BinaryOpNode
 * A node created for binary operations (+, -, *, /, etc...)
 */
class BinaryOpNode : public ExprNode {
  public:
    int op;        // Operation
    ExprNode& lhs;  // left-hand side
    ExprNode& rhs;  // right-hand side
    
    BinaryOpNode(int op, ExprNode& lh, ExprNode& rh) :
        op(op), lhs(lh), rhs(rh) {
            
      std::cout << std::endl << std::endl;
      std::cout << "Creating a BinaryOpNode!" << std::endl;
      std::cout << "The operation is : " << op;
      std::cout << std::endl << std::endl;
    }
};

/* FunctionCallNode
 * This node is created when a function is called.
 */
class FunctionCallNode : public ExprNode {
  public:
  
    IdNode& id;     // Name of function being called
    ExprList args;  // Parameters passed to the function
    
    FunctionCallNode(IdNode& i, ExprList& ar) : id(i), args(ar) {
        std::cout << std::endl << std::endl;
        std::cout << "Creating a MethodCallNode!" << std::endl;
        std::cout << "Though implementation not quite finished" << std::endl;
        std::cout << std::endl << std::endl;
    }

};



/****************************** STATEMENTS ***********************************/

/* ExpressionStatement
 * I'm... not comfortable enough with the information on why we need
 * expression statements, so I won't comment on it here
 *
 * But I'm pretty sure this how they're constructed.
 */
class ExpressionStatement : public StmtNode {
  public:
    ExprNode& expr;
    
    ExpressionStatement(ExprNode& ex) : expr(ex) {
        std::cout << std::endl << std::endl;
        std::cout << "ExpressionStatement Node created!" << std::endl;
        std::cout << std::endl << std::endl;
    }
    
};

/* VariableDeclarationNode
 * for creating nodes containing variable definitions
 */
class VarDeclNode : public StmtNode {
  public:
  
    const std::string* type;
    const std::string* name;
    
    ExprNode* value;
    
    VarDeclNode(const std::string* ty, const std::string* na) : type(ty), name(na) {
        std::cout << std::endl << std::endl;
        std::cout << "VarDeclNode created(No init)!" << std::endl;
        std::cout << "Type: " << type << std::endl;
        std::cout << "Name: " << name << std::endl;
        std::cout << std::endl << std::endl;
    }
    
    VarDeclNode(const std::string* ty, const std::string* na, ExprNode* va) :
                            type(ty), name(na), value(va) {
        std::cout << std::endl << std::endl;
        std::cout << "VarDeclNode created(Init)!" << std::endl;
        std::cout << "Type: " << *type << std::endl;
        std::cout << "Name: " << *name << std::endl;
        assignExpr->printVal();
        std::cout << std::endl << std::endl;
                               
    }

};

typedef std::vector<VarDeclNode*> VarList;

/* FunctionDeclarationNode
 * for creating nodes containing functions
 */
class FunctionDeclNode : public StmtNode {
  public:
    std::string type;   // return type
    std::string name;   // name of function
    VarList args;   // arguments passed
    StmtList stmtList;   // statements to be executed
    
    FunctionDeclNode(std::string ty, std::string na, VarList ar, StmtList stl) :
                            type(ty), name(na), args(ar), stmtList(stl) {
    }
        
};