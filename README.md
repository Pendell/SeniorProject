Alex Pendell
C-Lite Compiler
June 11th, 2020
alex.pendell@gmail.com


# The README is a _lie_

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
