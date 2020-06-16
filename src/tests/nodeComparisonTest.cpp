#include <memory> // Smart Pointers


#include "../node.h"


/* Fun Stuff...
Black: \u001b[30m
Red: \u001b[31m
Green: \u001b[32m
Yellow: \u001b[33m
Blue: \u001b[34m
Magenta: \u001b[35m
Cyan: \u001b[36m
White: \u001b[37m

Reset: \u001b[0m
*/



/* testComplexNodes is for testing the more complex nodes of an AST.
 * this constitutes tests for more complicated nodes that carry more data than 
 * simply what kind of node they are.
 */
bool testComplexNodes(){
    /* IntegerNode **********************************************************/
    printf("Testing class IntegerNode.\n");
    IntegerNode* node1 = new IntegerNode(10);
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    printf("\tNode 1 has value: %d\n", node1->getVal());
    IntegerNode* node2 = new IntegerNode(10);
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    printf("\tNode 2 has value: %d\n", node2->getVal());
    
    printf("Running comparisons...\n");
    if (!(node1->equals(node2)) || !(node2->equals(node1))) {
        printf("IntegerNode test failed. %s is", node1->getNodeType());
        printf("\u001b[31mnot\u001b[0m of type \u001b[33m%s\u001b[0m\n", node2->getNodeType());
        return false;
    } else {
        printf("\tNode1(%s) == Node2(%s)? \u001b[32mpassed\u001b[0m\n",node1->getNodeType(), node2->getNodeType());
    }
    
    
    delete node2;
    node2 = new IntegerNode(15);
    
    
    printf("Testing non-equal values...\n");
    if ((node1->equals(node2)) || (node2->equals(node1))) {
        printf("IntegerNode test \u001b[31mfailed\u001b[0m.\n");
        return false;
    } else {
        printf("\t%d != %d? \u001b[32mpassed\u001b[0m\n\n", node1->getVal(), node2->getVal());
    }
    
    
    delete node2;
    
    node2 = new IntegerNode(10); // Will use this in ReturnNode testing
    
    /* ReturnNode ***********************************************************/
    printf("Testing class ReturnNode.\n");
    ReturnNode* rNode1 = new ReturnNode();
    ReturnNode* rNode2 = new ReturnNode();
    
    // Checking both null
    printf("Comparing bothchildren as nullptr\n");
    if(!(rNode1->equals(rNode2)) || !(rNode2->equals(rNode1))){
        printf("\tnullptr != nullptr? \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tnullptr == nullptr? \u001b[32mpassed\u001b[0m\n");
    }
    printf("\n");
    
    // Checking childNode1 not null, Node2 null
    printf("Comparing child1 IntegerNode, child2 as nullptr\n");
    delete rNode1;
    rNode1 = new ReturnNode(node1);
    if((rNode1->equals(rNode2)) || (rNode2->equals(rNode1))){
        printf("\t%d == nullptr? \u001b[31mfailed\u001b[0m\n", rNode1->retVal->getVal());
        return false;
    } else {
        printf("\t%d != nullptr? \u001b[32mpassed\u001b[0m\n", rNode1->retVal->getVal());
    }
    printf("\n");
    
   
    // Checking childNode1 null, Node2 not null
    delete rNode1;
    delete rNode2;
    
    rNode1 = new ReturnNode();
    rNode2 = new ReturnNode(node2);
    printf("Comparing child1 as nullptr, child2 as IntegerNode\n");
    if((rNode1->equals(rNode2)) || (rNode2->equals(rNode1))){
        printf("\tnullptr == %d? \u001b[31mfailed\u001b[0m\n", rNode2->retVal->getVal());
        return false;
    } else {
        printf("\tnullptr != %d? \u001b[32mpassed\u001b[0m\n", rNode2->retVal->getVal());
    }
    printf("\n");
    
    // Checking both not null
    delete rNode1;
    delete rNode2;
    rNode1 = new ReturnNode(node1);
    rNode2 = new ReturnNode(node2);
    
    // Print out node types for visual debugging
    
    printf("Accessing ChildNode1 & ChildNode2 literally.\n");
    printf("\tChildNode1 is a %s whose value is %d\n", node1->getNodeType(), node1->getVal());
    printf("\tChildNode2 is a %s whose value is %d\n", node2->getNodeType(), node2->getVal());
    
    printf("Accessing node1 & node2 through parents.\n");
    printf("\tChildNode1 is an %s ", rNode1->retVal->getNodeType());
    printf("whose value is %d\n", rNode1->retVal->getVal());
    
    printf("\tChildNode1 is an %s ", rNode2->retVal->getNodeType());
    printf("whose value is %d\n", rNode2->retVal->getVal());
    
    
    printf("Running comparisons...\n");
    
    if((!rNode1->equals(rNode2)) || (!rNode2->equals(rNode1))){
        printf("ReturnNode test \u001b[31mfailed\u001b[0m.\n");
        return false;
    } else {
        printf("\tReturn Values: %d == %d? \u001b[32mpassed\u001b[0m\n\n", rNode1->getVal(), rNode2->getVal());
    }
    
    printf("Comparing non-equal return values...\n");
    delete node2;
    delete rNode2;
    
    node2 = new IntegerNode(15);
    rNode2 = new ReturnNode(node2);
    
    printf("\tNew Return Values: %d != %d?\n", rNode1->getVal(), rNode2->getVal());
    
    if((rNode1 == rNode2) || (rNode2 == rNode1)){
        printf("ReturnNode test \u001b[31mfailed\u001b[0m.\n");
        return false;
    } else {
        printf("\tReturn Values: %d != %d? \u001b[32mpassed\u001b[0m\n", rNode1->getVal(), rNode2->getVal());
    }
    
    return true;

}

/* Test ASTNode comparison -- the first comparison test in a long chain
 * of tests. 
 */
bool testSimpleNodeComparisons(){
    printf("Testing Simple Nodes (AST, Stmt, Expr)\n");
    
    /* AST *******************************************************************/
    printf("Testing class ASTNode.\n");
    
    ASTNode* node1 = new ASTNode();
    ASTNode* node2 = new ASTNode();
    
    printf("Running comparisons...\n");
    if (!node1->equals(node2)){
        printf("Test failed. ASTNodes not created/compared correctly\n");
        return false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
    }
    
    delete node1;
    delete node2;
    
    /* STMT ******************************************************************/
    printf("Testing class StmtNode.\n");
    node1 = new StmtNode();
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    node2 = new StmtNode();
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    
    
    printf("Running comparisons...\n");
    if (!node1->equals(node2)){
        printf("Test failed. StmtNodes not created/compared correctly");
        return false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
    }
    
    delete node1;
    delete node2;
    
    /* EXPR ******************************************************************/
    printf("Testing class ExprNode.\n");
    node1 = new ExprNode();
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    node2 = new ExprNode();
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    
    printf("Running comparisons...\n");
    if (!node1->equals(node2)){
        printf("Test failed. ExprNodes not created/compared correctly");
        return false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
    }
    
    delete node1;
    delete node2;
    
    
    /* Comparing the 3 Simple Nodes against each other ***********************/
    node1 = new ASTNode();
    node2 = new ExprNode();
    StmtNode* node3 = new StmtNode();
    
    printf("Testing nodes against each other.\n");
    
    // AST vs Expr
    printf("\t%s != %s? ", node1->getNodeType(), node2->getNodeType());
    if(node1->equals(node2)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    // AST vs Stmt
    printf("\t%s != %s? ", node1->getNodeType(), node3->getNodeType());
    if(node1->equals(node3)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    
    // Expr vs AST
    printf("\t%s != %s? ", node2->getNodeType(), node1->getNodeType());
    if(node2->equals(node1)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    // Expr vs Stmt
    printf("\t%s != %s? ", node2->getNodeType(), node3->getNodeType());
    if(node2->equals(node3)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    
    // Stmt vs AST
    printf("\t%s != %s? ", node3->getNodeType(), node1->getNodeType());
    if(node3->equals(node1)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    // Stmt vs Expr
    printf("\t%s != %s? ", node3->getNodeType(), node2->getNodeType());
    if(node3->equals(node2)) {
        printf("\u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\u001b[32mpassed\u001b[0m\n");
    }
    
    // All tests passed
    return true;

    
}

bool testVarDeclNode(){
    
    VarDeclNode* v1 = new VarDeclNode("int", "x", new IntegerNode(10));
    VarDeclNode* v2 = new VarDeclNode("int", "y", new IntegerNode(10));
    
    if(v1->equals(v2)){
        printf("Variable testing failed. Testing yields v1 == v2\n");
        return false;
    } else {
        return true;
    }
    
    // Different Types
    
    // Different Values

}

bool testFuncDeclNode(){
    
    /* Book Keeping **********************************************************/
    // New SymbolTable; empty symbol table for now.
    // SymTable checking not implemented.
    SymbolTable* symTable = new SymbolTable;
    
    // List one will be empty; []
    StmtList* list1 = new StmtList;
    
    // List2 will contain a return node; ["return 0"];
    StmtList* list2 = new StmtList;
    
    // List3 will contain a var and return nodes
    // but the var is initialized; ["int x = 10", "return 0"]
    StmtList* list3 = new StmtList;
    
    // List4 will contain a var, and return
    // But the var is uninitialized; ["int y", "return 0"]
    StmtList* list4 = new StmtList;
    
    ReturnNode* r1 = new ReturnNode(new IntegerNode(0));
    list2->push_front(r1);
    list3->push_front(r1);
    list4->push_front(r1);
    
    
    VarDeclNode* x = new VarDeclNode("int", "x", new IntegerNode(10));
    list3->push_front(x);
    
    
    VarDeclNode* y = new VarDeclNode("int", "y");
    list4->push_front(y);
    /* !Book Keeping *********************************************************/
    
    
    // Now we test function nodes.
    // Test mismatched types and names
    FuncDeclNode* func1 = new FuncDeclNode("int", "main", list1, symTable);
    FuncDeclNode* func2 = new FuncDeclNode("char", "main", list1, symTable);
    FuncDeclNode* func3 = new FuncDeclNode("int", "man", list1, symTable);
    
    printf("\u001b[36mFunc1\u001b[0m := \n");
    printf("int main() { }\n\n");
    
    printf("\u001b[36mFunc2\u001b[0m := \n");
    printf("char main() { }\n\n");
    
    printf("\u001b[36mFunc3\u001b[0m := \n");
    printf("int man() { }\n\n");
    
    // Testing func1 against itself, should pass.
    printf("Comparing \u001b[36mFunc1\u001b[0m vs self -- should be EQUAL\n");
    if(!func1->equals(func1)) {
        // If we're in here, there's a problem
        printf("\tComparison against self: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tComparison against self: \u001b[32mpassed\u001b[0m\n");
    }
    
    
    printf("\n");
    printf("Comparing \u001b[36mFunc2\u001b[0m vs self-- should show type mismatch\n");
    if(func1->equals(func2)){
        printf("\tComparison against func2: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tComparison against func2: \u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    printf("Comparing \u001b[36mFunc3\u001b[0m vs self -- should show name mismatch\n");
    if(func1->equals(func3)){
        printf("\tComparison against func3: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tComparison against func3: \u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    printf("Type & Name checking \u001b[32mpassed\u001b[0m\n");
    
    
    delete (func1, func2, func3);
    
    printf("\nRedefining Function Nodes...\n");
    func1 = new FuncDeclNode("int", "main", list1, symTable);
    func2 = new FuncDeclNode("int", "main", list2, symTable);
    func3 = new FuncDeclNode("int", "main", list3, symTable);
    FuncDeclNode* func4 = new FuncDeclNode("int", "main", list4, symTable);
    printf("\n");
    
    printf("\u001b[36mFunc1\u001b[0m := \n");
    printf("int main() { }\n\n");
    
    printf("\u001b[36mFunc2\u001b[0m := \n");
    printf("int main(){ return 0; }\n\n");
    
    printf("\u001b[36mFunc3\u001b[0m := \n");
    printf("int main(){ int x = 10; return 0; }\n\n");
    
    printf("\u001b[36mFunc4\u001b[0m := \n");
    printf("int main(){ int y; return 0; }\n\n");
    
    printf("Comparing Function Nodes to themselves -- Should be EQUAL\n");
    printf("\u001b[36mFunc1\u001b[0m vs \u001b[36mFunc1\u001b[0m\n");
    if(!func1->equals(func1)) {
        // If we're in here, there's a problem
        printf("\tFunc1 comparison against self: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tFunc1 comparison against self: \u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    printf("\u001b[36mFunc2\u001b[0m vs \u001b[36mFunc2\u001b[0m\n");
    if(!func2->equals(func2)) {
        // If we're in here, there's a problem
        printf("\tFunc2 comparison against self: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tFunc2 comparison against self: \u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    printf("\u001b[36mFunc3\u001b[0m vs \u001b[36mFunc3\u001b[0m\n");
    if(!func3->equals(func3)) {
        // If we're in here, there's a problem
        printf("\tFunc3 comparison against self: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tFunc3 comparison against self: \u001b[32mpassed\u001b[0m\n");
    }
    
    printf("\n");
    printf("\u001b[36mFunc4\u001b[0m vs \u001b[36mFunc4\u001b[0m\n");
    if(!func4->equals(func4)) {
        // If we're in here, there's a problem
        printf("\tFunc4 comparison against self: \u001b[31mfailed\u001b[0m\n");
        return false;
    } else {
        printf("\tFunc4 comparison against self: \u001b[32mpassed\u001b[0m\n");
    }
    
    
    printf("\n");
    printf("Comparing Function Nodes to Each other\n");
    
    // Func1 to Func2
    printf("\u001b[36mFunc1\u001b[0m vs \u001b[36mFunc2\u001b[0m\n");
    if(func1->equals(func2)) {
        printf("Incorrect comparison. Func1 == Func2\n");
        return false;
    } 
    
    
    // Func1 to Func3
    printf("\n");
    printf("\u001b[36mFunc1\u001b[0m vs \u001b[36mFunc3\u001b[0m\n");
    if(func1->equals(func3)) {
        printf("Incorrect comparison. Func1 == Func3\n");
        return false;
    } 
    // Func1 to Func4
    printf("\n");
    printf("\u001b[36mFunc1\u001b[0m vs \u001b[36mFunc4\u001b[0m\n");
    if(func1->equals(func4)) {
        printf("Incorrect comparison. Func1 == Func4\n");
        return false;
    }
    
    // Func2 to Func1
    printf("\n");
    printf("\u001b[36mFunc2\u001b[0m vs \u001b[36mFunc1\u001b[0m\n");
    if(func2->equals(func1)) {
        printf("Incorrect comparison. Func2 == Func1\n");
        return false;
    } 
    
    // Func2 to Func3
    printf("\n");
    printf("\u001b[36mFunc2\u001b[0m vs \u001b[36mFunc3\u001b[0m\n");
    if(func2->equals(func3)) {
        printf("Incorrect comparison. Func2 == Func3\n");
        return false;
    } 
    // Func2 to Func4
    printf("\n");
    printf("\u001b[36mFunc2\u001b[0m vs \u001b[36mFunc4\u001b[0m\n");
    if(func2->equals(func4)) {
        printf("Incorrect comparison. Func2 == Func4\n");
        return false;
    } 
    
    // Func3 to Func1
    printf("\n");
    printf("\u001b[36mFunc3\u001b[0m vs \u001b[36mFunc1\u001b[0m\n");
    if(func3->equals(func1)) {
        printf("Incorrect comparison. Func3 == Func1\n");
        return false;
    } 
    // Func3 to Func2
    printf("\n");
    printf("\u001b[36mFunc3\u001b[0m vs \u001b[36mFunc2\u001b[0m\n");
    if(func3->equals(func2)) {
        printf("Incorrect comparison. Func3 == Func2\n");
        return false;
    } 
    // Func3 to Func4
    printf("\n");
    printf("\u001b[36mFunc3\u001b[0m vs \u001b[36mFunc4\u001b[0m\n");
    if(func3->equals(func4)) {
        printf("Incorrect comparison. Func3 == Func4\n");
        return false;
    } 
    
    // Func4 to Func1
    printf("\n");
    printf("\u001b[36mFunc4\u001b[0m vs \u001b[36mFunc1\u001b[0m\n");
    if(func4->equals(func1)) {
        printf("Incorrect comparison. Func4 == Func1\n");
        return false;
    } 
    // Func4 to Func2
    printf("\n");
    printf("\u001b[36mFunc4\u001b[0m vs \u001b[36mFunc2\u001b[0m\n");
    if(func4->equals(func2)) {
        printf("Incorrect comparison. Func4 == Func2\n");
        return false;
    } 
    // Func4 to Func3
    printf("\n");
    printf("\u001b[36mFunc4\u001b[0m vs \u001b[36mFunc3\u001b[0m\n");
    if(func4->equals(func3)) {
        printf("Incorrect comparison. Func4 == Func3\n");
        return false;
    } 
    delete (func1, func2, func3, func4);
    delete (list1, list2, list3, list4);
    
}

int main(){
    
    // Tests AST, Stmt, and Expr Nodes -- Nodes that don't have data to compare
    
    printf("\n\nTesting Nodes and Node Comparison...\n");
    if (testSimpleNodeComparisons()){
        printf("\u001b[32mALL SIMPLE TESTS PASS\u001b[0m\n\n");
    } else {
        printf("Simple Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 1;
    }
    
    // Tests the more complex nodes, like Int and Return -- these nodes
    // have more data to compare and contrast and more points of failure.
    if (testComplexNodes()){
        printf("\u001b[32mALL COMPLEX TESTS PASS\u001b[0m\n\n");
    } else {
        printf("Complex Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 2;
    }
    
    if(testVarDeclNode()){
        printf("\u001b[32mALL VARIABLE TESTS PASS\u001b[0m\n\n");
    } else {
        printf("Variable Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 3;
    }
    
    // Test equals capability when referring to functions.
    if(testFuncDeclNode()){
        printf("\u001b[32mALL FUNCTION TESTS PASS\u001b[0m\n\n");
    } else {
        printf("Function Tests \u001b[31mFailed\u001b[0m.\n");
        return 4;
    }
    
    printf("\u001b[32mAll Tests Passed\u001b[0m\n");
}

