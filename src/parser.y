/* Take 37 of the compiler parser file. I'm still not sure
 * I have a great grasp on how to use Bison and Flex to get
 * everything I need lined up, but we'll see how it goes. */



/* Prologue ******************************************************************
* The prologue section is where we do basic, C-style includes, defines       *
* and other early declarations. When compiled, Bison simply copies this      *
* section over to the actual C++ file that Bison creates, line for line.     *
*****************************************************************************/
%{ 
    #include "node.h"
    
    #include <iostream>
    #include <string>
    #include <stdio.h>
    
    void yyerror(const char* e);
    
    extern FILE* yyin;
    extern int yyparse();
    extern int yylex();

    #define YYDEBUG 1


    Block* programBlock; /* the root of the created AST */
%} 



/* Union *********************************************************************
* The union section of the Bison grammar file is the medium by which Bison   *
* and flex have meaningful conversations with each other.                    *
*                                                                            *
* The token definitions below the union section declare what kind of tokens  *
* we would see moving between Bison and Flex, but in order to carry any type *
* of data between them, we have to define them here so we can give Flex and  *
* and Bison an idea of what kind of data is being carried as tokens passed   *
* through the lexer.                                                         *
*****************************************************************************/
%union { 
    
    Block* block;
    
    // Nodes
    ExprNode* expr;
    StmtNode* stmt;
    
    // Other
    std::string* string;
    int token;
    
}



/* Tokens & Types ************************************************************
* This is how we define the tokens to be used between Bison and Flex.        *
* the '%token' tells bison we're defining a token.                           *
*                                                                            *
* The <token> or <string> tells Bison what kind of data we expect each       *
* one of these tokens to be carrying. The typing inside the <> are taken     *
* directly from the union section above (see union section).                 *
*                                                                            *
* The name of a token is written in all caps here, and in the grammar.       *
* Anything inside double-quotes here is an alias to the token that precedes  *
* it. This allows us to use the alias when referring to it in the grammar    *
* section. Example using ")" instead of typing "LPAREN" improves readabilit  *
* of the grammar section.                                                    *  
*****************************************************************************/
%type<expr>     expr 
%type<stmt>     stmt returnStmt exprStmt
%type<block>    program funcDeclList

%token<token>   VOID "void" INT "int" CHAR "char" 
%token<string>  ID NUMCONST CHARCONST STRINGCONST
%token          LPAREN "(" RPAREN ")" LBRACKET "{" RBRACKET "}" RETURN "return"
%token          COLON ":" SEMICOLON ";" COMMA ","
%token          EQUAL "="   
%token<token>   PLUS "+"

// Precedences
%left EQUAL
%left PLUS

/* Extra notes about tokens so I can reference the terminals/tokens by ID#
 *
 * $end (0)
 * error (256)
 * VOID (258)
 * INT (259) 
 * CHAR (260) 
 * IDENTIFIER (261)
 * INTCONSTANT (262)
 * "=" (263) 
 * "(" (264) 
 * ")" (265) 
 * "{" (266) 
 * "}" (267) 
 * RETURN (268) 
 *
 */


/* Grammar section ***********************************************************
* The grammar section allows us to define our languages' grammar in a        *
* notation similar to BNF, with the additions of actions being declared      *
* on the right hand side of the rule, inside the brackets.                   *
*                                                                            *
* When creating actions, we can refer to the pieces that make up the rule    *
* by using $ followed by an integer that refers to the index of the token    *
* in the rule. For example if we wanted to reference the second token in the *
* rule, we would use $2 to reference the second token in the line.           *
*****************************************************************************/
%% 

program:        funcDeclList {}
;

funcDeclList:   funcDecl funcDeclList {}
|               %empty {}
;

funcDecl:       typeSpecifier ID "(" params ")" "{" stmtList "}"
;

params:         %empty
;

stmtList:       stmt ";" stmtList
|               %empty
;

stmt:           varDecl {}
|               returnStmt
|               exprStmt
;

varDecl:        typeSpecifier ID
|               typeSpecifier ID "=" expr
;

returnStmt:     "return" expr {}
;

exprStmt:       ";" {}
|               expr {}
;

expr:           constant {}
;

constant:       NUMCONST
|               CHARCONST
|               STRINGCONST
;

typeSpecifier:  "int"
|               "char"
;


%%

/* Epilogue ******************************************************************
* Similar to the prologue, this section of code is basically copied line for *
* line over to the C++ file that Bison generates. I'm using the epilogue to  *
* create the main function that my parser will use, as well as the definition*
* of yyerror, so that I can handle errors in a way that's meaningful to me.  *
*****************************************************************************/
int main(int argc, char** argv){
    yydebug = 1;
    // Open the file, read if good
    FILE* f = fopen(argv[1], "r");
    if(!f)  printf("Error: Bad Input\n");
    else {
        yyin = f;
        yyparse();
        fclose(yyin);
    }
}

void yyerror(const char* s) {
    printf("Help! Error! Help! --> %s\n", s);
}