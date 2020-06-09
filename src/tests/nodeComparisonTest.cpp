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
    printf("Testing equal IntegerNodes...\n");
    IntegerNode* node1 = new IntegerNode(10);
    printf("\tNode 1 is of type: \u001b[33m%s\u001b[0m\n", node1->getNodeType());
    printf("\tNode 1 has value: \u001b[33m%d\u001b[0m\n", node1->getVal());
    IntegerNode* node2 = new IntegerNode(10);
    printf("\tNode 2 is of type: \u001b[33m%s\u001b[0m\n", node2->getNodeType());
    printf("\tNode 2 has value: \u001b[33m%d\u001b[0m\n", node2->getVal());
    
    printf("Testing types...\n");
    if (node1->getNodeType() != node2->getNodeType()) {
        printf("IntegerNode test failed. \u001b[33m%s\u001b[0m is", node1->getNodeType());
        printf("\u001b[31mnot\u001b[0m of type \u001b[33m%s\u001b[0m\n", node2->getNodeType());
        return (TEST_SUCCESS = false);
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
    }
    if (node1->getNodeType() != node2->getNodeType()) {
        printf("IntegerNode test failed. \u001b[33m%s\u001b[0m is");
        printf("\u001b[31mnot\u001b[0m of type \u001b[33m%s\u001b[0m\n");
        return (TEST_SUCCESS = false);
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
    }
    
    printf("Testing values...\n");
    if (node1->getVal != node2->getVal()) {
        printf("IntegerNode test failed. \u001b[33m%d\u001b[0m is");
        printf("\u001b[31mnot\u001b[0m of type \u001b[33m%s\u001b[0m\n");
        return (TEST_SUCCESS = false);
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
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
    printf("Checking types...\n");
    printf("\tNode 1 is of type: \u001b[33m%s\u001b[0m\n", node1->getNodeType());
    ASTNode* node2 = new ASTNode();
    printf("\tNode 2 is of type: \u001b[33m%s\u001b[0m\n", node2->getNodeType());
    
    if (!node1->compareNode(node2)){
        printf("Test failed. ASTNodes not created/compared correctly\n");
        TEST_SUCCESS = false;
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
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
    printf("Checking types...\n");
    printf("\tNode 1 is of type: \u001b[33m%s\u001b[0m\n", node1->getNodeType());
    node2 = new StmtNode();
    printf("\tNode 2 is of type: \u001b[33m%s\u001b[0m\n", node2->getNodeType());
    
    if (!node1->compareNode(node2)){
        printf("Test failed. StmtNodes not created/compared correctly");
        TEST_SUCCESS = false;
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
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
    printf("Checking types...\n");
    printf("\tNode 1 is of type: \u001b[33m%s\u001b[0m\n", node1->getNodeType());
    node2 = new ExprNode();
    printf("\tNode 2 is of type: \u001b[33m%s\u001b[0m\n", node2->getNodeType());
    
    if (!node1->compareNode(node2)){
        printf("Test failed. ExprNodes not created/compared correctly");
        TEST_SUCCESS = false;
    } else {
        printf("\t\u001b[33m%s\u001b[0m == \u001b[33m%s\u001b[0m? \u001b[32m passed\u001b[0m \n\n",node1->getNodeType(), node2->getNodeType());
        TEST_SUCCESS = true;
    }
    
    delete node1;
    delete node2;
    
    if (!TEST_SUCCESS) {
        printf("Test Failed ExprNode Comparison\n");
        return TEST_SUCCESS;
    } 
    
    return (TEST_SUCCESS = testComplexNodes());

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
        printf("All tests \u001b[32mpassed\u001b[0m\n");
    } else if (!TEST_SUCCESS) {
        printf("Simple Node Tests \u001b[31mFailed\u001b[0m.\n");
        return 1;
    }
    
    
}

