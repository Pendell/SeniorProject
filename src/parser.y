/* Take 37 of the compiler parser file. I'm still not sure
 * I have a great grasp on how to use Bison and Flex to get
 * everything I need lined up, but we'll see how it goes. */

/* THIS LINE IS TO TEST SOMETHING IN GIT */
/* Second test line for git, should have ssh working now */

/* Prologue ******************************************************************
* The prologue section is where we do basic, C-style includes, defines       *
* and other early declarations. When compiled, Bison simply copies this      *
* section over to the actual C++ file that Bison creates, line for line.     *
*****************************************************************************/
%{ 
    #include "node.h"
    #include "src.h"
    
    #include <iostream>
    #include <stdio.h>
    #include <list>    // For the symbol table list
    
    void yyerror(const char* e);
    
    extern FILE* yyin;
    extern int yyparse();
    extern int yylex();

    #define YYDEBUG 1


    ProgramNode* Program; /* the root of the created AST */
    
    SymbolTable* CurrSymTable;
    
    std::list<SymbolTable*>* SymList; // A list of Symbol Tables
    
    
    
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
    ProgramNode* program;
    DeclNode* decl;
    
    StmtList* stmtList;
    
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
%type<expr>     expr constant
%type<stmt>     stmt returnStmt exprStmt  
%type<stmtList> program stmtList 
%type<string>   typeSpecifier
%type<decl>     declList varDecl funcDecl declaration

%token<token>   VOID "void"  CHAR "char" 
%token<string>  ID NUMCONST CHARCONST STRINGCONST INT "int"
%token          LPAREN "(" RPAREN ")" LBRACKET "{" RBRACKET "}"
%token          RETURN "return"
%token          COLON ":" SEMICOLON ";" COMMA ","
%token<token>   EQUAL "="   
%token<token>   PLUS "+"

// Precedences
%left EQUAL
%left PLUS

%start program

/* Extra notes about tokens so I can reference the terminals/tokens by ID#
 * This is copied directly from the parser.output file. Putting it here
 * in case I need to reference things directly. I haven't needed to so far,
 * and this list might be outdated, but an updated list can be found in the
 * parser.output file.
 *   
 * $end (0) 0
 * error (256)
 * "void" (258)
 * "char" (259) 
 * ID (260) 
 * NUMCONST (261) 
 * CHARCONST (262) 
 * STRINGCONST (263) 
 * "int" (264)
 * "(" (265)
 * ")" (266) 
 * "{" (267)
 * "}" (268) 
 * "return" (269) 
 * ":" (270)
 * ";" (271) 
 * "," (272)
 * "=" (273) 
 * "+" (274)
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

program:        declList
                {   
                    $1 = new DeclNode();
                    Program = new ProgramNode();
                    Program->start = $1;
                    
                    // Create the Global Symbol Table
                    // Unused for now since the grammar doesn't allow
                    // global variables for the time being, but it's there
                    SymbolTable* GlobalST = new SymbolTable();
                    
                    
                    // Push the symbol table onto the list.
                    SymList->push_front(GlobalST);
                    //printf("New symbol table created\n");
                    //printf("Symbol Table added to the SymList\n");
                }
;

declList:       declaration declList { $$ = new DeclNode(); $$->lhs = $1; $$->rhs = $2; }
|               %empty { }
;

declaration:    funcDecl { printf("\n\nFuncDecl pushed into DeclNode LHS\n\n"); }
|               varDecl { printf("\n\nVarDecl pushed into DeclNode RHS\n\n"); }
;

funcDecl:       typeSpecifier ID "(" ")" "{" stmtList "}" 
                {   
                    //printf("Before adding SymTable: %d", CurrSymTable->size());
                    $$ = new FuncDeclNode($1, $2, $<stmtList>6, CurrSymTable);
                }
;

stmtList:       stmt stmtList { }
|               %empty { }
;

stmt:           varDecl 
|               returnStmt
|               exprStmt
;

params:         %empty
;

varDecl:        typeSpecifier ID "=" expr ";" 
                {   
                    std::string name = *$2;
                    std::string type = *$1;
                    
                    if(checkAndAdd(CurrSymTable, name, type)){
                        
                        // Variable Node Construction
                        $$ = new VarDeclNode($1, $2, $4);
                        
                    } else {
                    
                        // Handle the error
                        char s[100] = {  };
                        sprintf(s, "Redefinition of %s\n", name.c_str());
                        yyerror(s);
                        YYABORT;
                        
                    }
                    
                }
                
|               typeSpecifier ID ";" 
                { 
                    std::string name = *$2;
                    std::string type = *$1;
                    
                    if(checkAndAdd(CurrSymTable, name, type)){
                        
                        // Variable Node Construction
                        $$ = new VarDeclNode($1, $2);
                        
                    } else {
                    
                        // Handle the error
                        char s[100] = {  };
                        sprintf(s, "Redefinition of %s\n", name.c_str());
                        yyerror(s);
                        YYABORT;
                        
                    }
                }
;

returnStmt:     "return" expr ";" 
                { 
                    $$ = new ReturnNode($2);
                }
;

exprStmt:       ";" {}
|               expr {}
;

expr:           constant
;
 
constant:       NUMCONST { $$ = new IntegerNode(atoi($1->c_str())); delete $1; }
|               CHARCONST
|               STRINGCONST
;

typeSpecifier:  "int" { $$ = new std::string("int"); }
|               "char" { $$ = new std::string("char"); }
;


%%

/* Epilogue ******************************************************************
* Similar to the prologue, this section of code is basically copied line for *
* line over to the C++ file that Bison generates. I'm using the epilogue to  *
* create the main function that my parser will use, as well as the definition*
* of yyerror, so that I can handle errors in a way that's meaningful to me.  *
* It's not meaningful to me yet, but it will be in the future. Probably.     *
* I'm in the process of moving everything out of the epilogue into it's own  *
* file so that the package looks nicer.                                      *
*****************************************************************************/




