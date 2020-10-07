# This README is largely outdated and will be updated in the future.
There is still much to be done before this project is complete.
I would still like to explore:

Error Handling

Expanding types

More 'hands on' code generation.

# Compiling and running the nodeTest
To run nodeTest (which tests the equals() funcationality of the ASTNodes), change into the _src_ directory and enter:
`make clean`
to ensure that there are no fragments of old builds in your directory. Next, enter:
`make test; ./nodeTest`
This will build the testing code and executables, and then subsequently run them.

# Reading Test Results
The testing executable will display testing results and information to the terminal.
The test is split into two pieces; simple and complex node testing.

The simple node portion tests the ASTNode, StmtNode, and ExprNode `equals()` functionality.

The complex node portion tests the more complicated nodes that carry more data with them, and 
need to be explored further than just comparing the types of nodes.

Currently, tests have been implemented for the following nodes:     
IntegerNode     
ReturnNode      

The **IntegerNode** tests consist of creating two IntegerNodes, and ensuring that they both have the same value stored. After,
the second IntegerNode is re-created to a different value, and then the two nodes are compared once more to ensure that equals()
will return false on different entries.

The **ReturnNode** tests begin by creating two ReturnNodes whose children are null. The two nodes are then compared against eachother to
verify they are of the same type. If this test passes the two IntegerNodes from the previous test are then recreated with the same values,
and will act as the children(return values) of the ReturnNodes. 

IntegerNodes of the same value (just as in the previous test). These IntegerNodes will act as the children (or return values) of the parent
ReturnNodes, and are passed to the ReturnNode constructors as such. When equals() is ran, it first invokes the `ReturnNode::equals()` and 
ensures that both parent nodes are of type ReturnNode, and that the children are either both null, or contain some value. 
After confirming that each ReturnNode has *some* value as their return values, the children then invoke equals to compare the two children
nodes(in this case, `IntegerNode::equals()`.
