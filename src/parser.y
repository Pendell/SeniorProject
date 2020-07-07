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
    #include "./nodes/node.h"
    
    #include <iostream>
    #include <stdio.h>
    #include <list>    // For the symbol table list
    #include <unordered_map>
    #include <utility>
    
    extern void yyerror(const char* e);
    extern FILE* yyin;
    extern int yyparse();
    extern int yylex();
    

    #define YYDEBUG 1

    StmtList* currStmtList = new StmtList;
    
    // These are defined in main.cpp
    extern ProgramNode* program; /* the root of the created AST */
    // extern SymbolTable* globalST;
    // extern SymbolTable* currSymTable;
    
    // Symbol Tables -- Defined in node.h
    extern SymbolTable* GST;
    extern SymbolTable* ST;
    
    
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
    
    ProgramNode* program; 
    
    DeclNode* decl;
    StmtList* stmtList;
    ReturnNode* rtrn;
    
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
%type<stmt>     stmt
%type<stmtList> stmtList 
%type<string>   typeSpecifier
%type<decl>     declList declaration varDecl funcDecl
%type<program>  program
%type<rtrn>     returnStmt

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

program:        declList { program->start = $1; }
;

declList:       declaration declList { $$ = new DeclNode(); $$->lhs = $1; $$->rhs = $2; }
|               %empty { $$ = nullptr; }
;

declaration:    funcDecl { }
|               varDecl { }
;

funcDecl:       typeSpecifier ID "(" ")" "{" stmtList "}" {
                    printf("FuncDeclNode breaking?\n\n");
                    const char* type = "int";
                    const char* name = "main";
                    $$ = new FuncDeclNode(type, name, currStmtList, ST);
                    currStmtList = new StmtList(); // Reset stmtlist
                    printf("Nope.\n");
                }
;

stmtList:       stmt stmtList { 
                    printf("StmtList breaking?\n\n");
                    currStmtList->push_front($<stmt>1);
                    printf("Nope.\n");
                }
|               %empty {}
;

stmt:           varDecl 
|               returnStmt
;

varDecl:        typeSpecifier ID "=" expr ";" {
                    printf("\nExpression\n\n");
                    
                    const char* name = $2->c_str();
                    
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    
                    while(it != ST->end()){
                        printf("\n\n %s == %s ?\n", (*it)->getName(), name);
                        if(strcmp((*it)->getName(), name) == 0) {
                            printf("Yep! Throwing error.\n");
                            printf("Variable %s already declared\n", name);
                            return 1;
                        } else {
                            printf("Nope... continuing parse.\n\n");
                        }
                        ++it;
                    }
                    
                    $$ = new VarDeclNode($1->c_str(), $2->c_str(), $4);
                    ST->push_back($$); 
                } 
                
|               typeSpecifier ID ";" {
                    printf("\nNullptr\n\n");
                    
                    
                    const char* name = $2->c_str();
                    
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    
                    while(it != ST->end()){
                        printf("\n\n %s == %s ?\n", (*it)->getName(), name);
                        if(strcmp((*it)->getName(), name) == 0) {
                            printf("Yep! Throwing error.\n");
                            printf("Variable %s already declared\n", name);
                            return 1;
                        } else {
                            printf("Nope... continuing parse.\n\n");
                        }
                        ++it;
                    }
                    
                    $$ = new VarDeclNode($1->c_str(), $2->c_str());
                    ST->push_back($$); 
                }
                
|               typeSpecifier ID "=" ID ";" {
                    
                    printf("\n\nVariable assigned to another variable.\n");
                    
                    
                    const char* name = $2->c_str();
                    const char* name2 = $4->c_str();
                    
                    printf("%s being assigned %s\n", name, name2);
                    
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    
                    // Check to make sure the variable doesn't already exist
                    while (it != ST->end()){
                        printf("Inside first while loop.\n");
                        if(strcmp((*it)->getName(), name) == 0){
                            printf("Error: Variable %s already declared.\n", name);
                            return 1;
                        }
                        ++it;
                    }
                    
                    // If we're here, we've ensured that the variable hasn't been declared yet
                    // Now we search for the second variable, return an error if we can't find it.
                    it = ST->begin();
                    bool found = false;
                    while(it != ST->end()){
                        printf("Inside second while loop.\n");
                        if(strcmp((*it)->getName(), name2) == 0) {
                            VarDeclNode* node_casted = dynamic_cast<VarDeclNode*>(*it);
                            $$ = new VarDeclNode($1->c_str(), name, node_casted->value);
                            ST->push_back($$);
                            found = true;
                            break;
                        }
                        ++it;
                    }
                    if(!found){
                        printf("Error: Variable %s is undefined.\n", name2);
                        return 1;
                    }
                }
;

returnStmt:     "return" expr ";"{ $$ = new ReturnNode($2); }
|               "return" ID ";"{
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    const char* name = $2->c_str();
                    
                    printf("Looking for %s\n", name);
                    while(it != ST->end()){
                        printf("\n\n %s == %s ?\n", (*it)->getName(), name);
                        
                        if(strcmp((*it)->getName(), name) == 0) {
                            VarDeclNode* var = dynamic_cast<VarDeclNode*>(*it);
                            
                            if (var->value == nullptr) {
                                printf("Error: Variable '%s' not defined.\n", var->getName());
                                return 1;
                            }
                                
                            $$ = new ReturnNode(var->value);
                            printf("About to segfault...\n");
                            printf("\n\nReturn Value: %d\n\n\n", var->getVal());
                            break;
                        }
                        ++it;
                    }
                    if(it == ST->end()){
                        printf("Error: Variable '%s' not declared.\n", name);
                        return 1;
                    }
                }
;

expr:           constant
;
 
constant:       NUMCONST { $$ = new IntegerNode(atoi($1->c_str())); }
|               CHARCONST { }
|               STRINGCONST { }
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




