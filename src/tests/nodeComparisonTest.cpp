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


bool TEST_SUCCESS = false;


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
    
    printf("Running compareNode()...\n");
    if (!node1->compareNode(node2) || !node2->compareNode(node1)) {
        printf("IntegerNode test failed. %s is", node1->getNodeType());
        printf("\u001b[31mnot\u001b[0m of type \u001b[33m%s\u001b[0m\n", node2->getNodeType());
        return (TEST_SUCCESS = false);
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n",node1->getNodeType(), node2->getNodeType());
    }
    
    
    delete node2;
    node2 = new IntegerNode(15);
    
    
    printf("Testing non-equal values...\n");
    if (node1->compareNode(node2) || node2->compareNode(node1)) {
        printf("IntegerNode test \u001b[31mfailed\u001b[0m.\n");
        return (TEST_SUCCESS = false);
    } else {
        printf("\t%d != %d? \u001b[32mpassed\u001b[0m\n\n", node1->getVal(), node2->getVal());
    }
    
    
    delete node2;
    node2 = new IntegerNode(10);
    
    /* ReturnNode ***********************************************************/
    printf("Testing class ReturnNode.\n");
    
    ReturnNode* rNode1 = new ReturnNode(node1);
    ReturnNode* rNode2 = new ReturnNode(node2);
    
    
    
    printf("Are the nodes as they should be...\n");
    printf("rNode1 is a %s\n", rNode1->getNodeType());
    printf("rNode2 is a %s\n", rNode2->getNodeType());
    printf("node1 is a %s\n", node1->getNodeType());
    printf("node2 is a %s\n", node2->getNodeType());
    
    printf("\nAccessing node1 & node2 through parents...\n");
    printf("node1 is an %s\n", rNode1->retVal->getNodeType());
    printf("node2 is an %s\n", rNode2->retVal->getNodeType());
    
    
    printf("Running compareNode()...\n");
    
    if(!rNode1->compareNode(rNode2) || !rNode2->compareNode(rNode1)){
        printf("ReturnNode test \u001b[31mfailed\u001b[0m.\n");
        return (TEST_SUCCESS = false);
    } else {
        printf("\tReturn Values: %d == %d? \u001b[32mpassed\u001b[0m\n\n", rNode1->getVal(), rNode2->getVal());
    }
    
    printf("Comparing non-equal return values...\n");
    //delete node2;
    //delete rNode2;
    
    node2 = new IntegerNode(15);
    rNode2 = new ReturnNode(node2);
    
    printf("\tNew Return Values: %d != %d?\n", rNode1->getVal(), rNode2->getVal());
    
    if(rNode1->compareNode(rNode2) || rNode2->compareNode(rNode1)){
        printf("TESTING FAILED... WHY THOUGH? JUST, WHY???\n");
        printf("ReturnNode test \u001b[31mFailed\u001b[0m.\n");
        return (TEST_SUCCESS = false);
    } else {
        printf("\tReturn Values: %d != %d? \u001b[32mpassed\u001b[0m\n\n", rNode1->getVal(), rNode2->getVal());
    }
    
    
    
    return(TEST_SUCCESS = true);

}

/* Test ASTNode comparison -- the first comparison test in a long chain
 * of tests. 
 */
bool testSimpleNodeComparisons(){
    printf("Testing Simple (AST, Stmt, Expr) Nodes\n");
    
    /* AST *******************************************************************/
    printf("Testing class ASTNode.\n");
    
    ASTNode* node1 = new ASTNode();
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    ASTNode* node2 = new ASTNode();
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    
    printf("Running compareNode()...\n");
    if (!node1->compareNode(node2)){
        printf("Test failed. ASTNodes not created/compared correctly\n");
        TEST_SUCCESS = false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
        TEST_SUCCESS = true;
    }
    
    delete node1;
    delete node2;
    
    if (!TEST_SUCCESS) {
        return TEST_SUCCESS;
    } 
    
    /* STMT ******************************************************************/
    printf("Testing class StmtNode.\n");
    node1 = new StmtNode();
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    node2 = new StmtNode();
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    
    printf("Running compareNode()...\n");
    if (!node1->compareNode(node2)){
        printf("Test failed. StmtNodes not created/compared correctly");
        TEST_SUCCESS = false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
        TEST_SUCCESS = true;
    }
    
    delete node1;
    delete node2;
    
    if (!TEST_SUCCESS) {
        printf("Test Failed StmtNode Comparison");
        return TEST_SUCCESS;
    } 
    
    /* EXPR ******************************************************************/
    printf("Testing class ExprNode.\n");
    node1 = new ExprNode();
    printf("\tNode 1 is of type: %s\n", node1->getNodeType());
    node2 = new ExprNode();
    printf("\tNode 2 is of type: %s\n", node2->getNodeType());
    
    printf("Running compareNode()...\n");
    if (!node1->compareNode(node2)){
        printf("Test failed. ExprNodes not created/compared correctly");
        TEST_SUCCESS = false;
    } else {
        printf("\t%s == %s? \u001b[32mpassed\u001b[0m\n\n",node1->getNodeType(), node2->getNodeType());
        TEST_SUCCESS = true;
    }
    
    delete node1;
    delete node2;
    
    if (!TEST_SUCCESS) {
        printf("Test Failed ExprNode Comparison\n");
        return TEST_SUCCESS;
    } 
    
    return (TEST_SUCCESS );

    /* Testing Against each other ********************************************/
    
    /* This is testing to ensure that the nodes comparison function is working
     * properly. They should fail each time: 
     * as ASTNode != Stmt Node != ExprNode.
     *
     
    printf("Testing Simple Nodes against each other...");
   
    ASTNode* astNode = new ASTNode();
    printf("\tNode 1 is of type: \u001b[33m%s\u001b[0m\n", astNode->getNodeType());
    StmtNode* stmtNode = new StmtNode();
    printf("\tNode 2 is of type: \u001b[33m%s\u001b[0m\n", stmtNode->getNodeType());
    ExprNode* exprNode = new ExprNode();
    printf("\tNode 3 is of type: \u001b[33m%s\u001b[0m\n", exprNode->getNodeType());
    
    bool testNotEqual = true;
    
    // ASTNode vs others.
    if ((TEST_SUCCESS = astNode->compareNode(stmtNode))) {
        printf("astNode comparison against stmtNode \u001b[31mfailed\u001b[0m.\n");
    } else if ((TEST_SUCCESS = astNode->compareNode(exprNode))){
        printf("astNode comparison against exprNode \u001b[31mfailed\u001b[0m.\n");
    }
    
    // StmtNode vs others.
    if ((TEST_SUCCESS = stmtNode->compareNode(astNode))) {
        printf("stmtNode comparison against astNode \u001b[31mfailed\u001b[0m.\n");
    } else if ((TEST_SUCCESS = astNode->compareNode(exprNode))) {
        printf("stmtNode comparison against exprNode \u001b[31mfailed\u001b[0m.\n");
    }
    
    // ExprNode vs others.
    if ((TEST_SUCCESS = exprNode->compareNode(astNode))) { 
        printf("exprNode comparison against astNode \u001b[31mfailed\u001b[0m.\n");
    } else if ((TEST_SUCCESS = exprNode->compareNode(stmtNode))) {
        printf("exprNode comparison against stmtNode \u001b[31mfailed\u001b[0m.\n");
    }*/
}


int main(){
    
    printf("\n\nTesting Nodes and Node Comparison...\n");
    if ((TEST_SUCCESS = testSimpleNodeComparisons())){
        printf("\u001b[32mALL SIMPLE NODE TESTS PASS\u001b[0m\n\n");
    } else if (!TEST_SUCCESS) {
        printf("Simple Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 1;
    }
   
    
    if ((TEST_SUCCESS = testComplexNodes())){
        printf("All Complex Node tests \u001b[32mpassed\u001b[0m\n");
    } else {
        printf("Complex Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 2;
    }
    
    
}

