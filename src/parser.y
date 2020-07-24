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
    #define YYDEBUG 1
    
    using namespace llvm;
    
    extern void yyerror(const char* e);
    extern FILE* yyin;
    extern int yyparse();
    extern int yylex();
    
    /* the root of the created AST */
    extern ProgramNode* program; 
    
    ASTNode* scan(const char* target, SymbolTable* st);
    
    // A vector of pairs
    // -> Position in Vector = Index of argument in func prototype
    // -> The pair of string are type and name of variable (ie "int" "x")
    
    static std::stack<SymbolTable*> symStack;
    static std::vector<std::pair<std::string, std::string>*> params;
    static std::vector<ExprNode*> args;
    static FuncPrototype* protoRef;
    
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
    BinaryOpNode* binaryOp;
    UnaryOpNode* unaryOp;
    MutateVarNode* mutVar;
    BoolExprNode* boolEx;
    FCallNode* fCall;
    Parameter* param;
    VarDeclNode* vDecl;
    IfNode* ifNode;
    ForNode* forNode;
    DoWhileNode* dwNode;
    
    
    std::pair<std::string, std::string>* pairs;
    
    // Nodes
    ExprNode* expr;
    StmtNode* stmt;
    
    // Other
    std::string* string;
    int token;
    char op;
    
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
%type<binaryOp> binaryOpExpr
%type<unaryOp>  unaryOpExpr
%type<fCall>    funcCall
%type<boolEx>   boolExpr

%type<dwNode>   doWhileLoop
%type<ifNode>   ifLoop
%type<forNode>  forLoop

%type<mutVar>   varMutate
%type<op>       binaryOp boolOp unaryOp
%type<stmt>     stmt exprStmt loop initStmt
%type<stmtList> stmtList 
%type<string>   typeSpecifier
%type<decl>     declList declaration varDecl funcDecl param
%type<program>  program
%type<rtrn>     returnStmt

%token<token>   VOID "void"  CHAR "char" 
%token<string>  ID NUMCONST CHARCONST STRINGCONST INT "int"
%token          LPAREN "(" RPAREN ")" LBRACKET "{" RBRACKET "}" SQUOTE "'"
%token          RETURN "return"
%token          IF "if" FOR "for" DO "do" WHILE "while"

%token          COLON ":" SEMICOLON ";" COMMA ","
%token<token>   EQUAL "=" BOOLEQ "==" BOOLNEQ "!="
%token<token>   ADD "+" SUB "-" MUL "*" DIV "/"
%token<token>   INC "++" DEC "--"

// Precedences
%left EQUAL
%left MUL DIV
%left PLUS MINUS

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

program:       { symStack.push(new GlobalSymbolTable()); } declList { program->start = $2; }
;

declList:       declaration declList {
                    $$ = new DeclNode(); $$->lhs = $1; $$->rhs = $2;
                }
                
|               %empty { $$ = nullptr; }
;

declaration:    funcDecl { symStack.top()->add($1); // Full function declaration and definition }
|               fProto   { symStack.top()->add($1); // Declaration of function prototype }
|               gVarDecl { symStack.top()->add($1); // Declaring a global variable }
;

funcDecl:       typeSpecifier ID "{" stmtList "}" {
                    symStack.top()->lookup(*$2);
                }
;

gVarDecl:       typeSpecifier ID ";" {
                    
                }
|               typeSpecifier ID "=" expr ";" {
                    
                }
;

paramDelim:     paramList
|               %empty
;

paramList:      paramList "," param 
|               param 
;

param:          typeSpecifier ID {
                    std::string type($1->c_str());
                    std::string name($2->c_str());
                    params.push_back(new std::pair<std::string, std::string>(type, name));
                    symStack.top()->push_back(new VarDeclNode($1->c_str(), $2->c_str()));
                } 
;



stmtList:       loop stmtList {
                    stmtStack.top()->push_front($<stmt>1);
                }
                
|               stmt ";" stmtList { 
                    stmtStack.top()->push_front($<stmt>1);
                }
                
|               %empty { }
;


stmt:           varDecl
|               varMutate
|               returnStmt
|               exprStmt
;


exprStmt:       expr
;



varDecl:        typeSpecifier ID "=" expr {
                    
                    const char* name = $2->c_str();
                    
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    
                    while(it != ST->end()){
                        if(strcmp((*it)->getName(), name) == 0) {
                            printf("Variable %s already declared\n", name);
                            return 1;
                        } else {
                        }
                        ++it;
                    }
                    
                    $$ = new VarDeclNode($1->c_str(), $2->c_str(), $4);
                    symStack.top()->push_back($$); 
                } 
                
|               typeSpecifier ID {

                    const char* name = $2->c_str();
                    
                    // Check the symbol table for the variable
                    SymbolTable::iterator it = ST->begin();
                    
                    while(it != ST->end()){
                        if(strcmp((*it)->getName(), name) == 0) {
                            printf("Variable %s already declared\n", name);
                            return 1;
                        } else {
                        }
                        ++it;
                    }
                    
                    $$ = new VarDeclNode($1->c_str(), $2->c_str());
                    symStack.top()->push_back($$);
                }
;

loop:           ifLoop
|               doWhileLoop
|               forLoop
;

doWhileLoop:    "do" "{" {
                    stmtStack.push(new StmtList);
                } stmtList "}" "while" "(" boolExpr ")" ";" {
                    StmtList* s = stmtStack.top();
                    $$ = new DoWhileNode(s, 'd', $8);
                    stmtStack.pop();
                    
                }
                
|               "while" "(" boolExpr ")" "{" {
                    stmtStack.push(new StmtList);
                } stmtList "}" {
                    StmtList* s = stmtStack.top();
                    $$ = new DoWhileNode(s, 'w', $3);
                    stmtStack.pop();
                }
;

ifLoop:         "if" "(" boolExpr ")" "{" {
                    stmtStack.push(new StmtList);
                
                } stmtList "}" {
                    StmtList* s = stmtStack.top();
                    $$ = new IfNode($3, s);
                    stmtStack.pop();
                    
                }
;

forLoop:        "for" "(" initStmt ";" boolExpr ";" stmt ")" "{" {
                    stmtStack.push(new StmtList);
                    
                } stmtList "}" {
                    StmtList* s = stmtStack.top();
                    $$ = new ForNode($3, $5, $7, s);
                    stmtStack.pop();
                    
                }
;

initStmt:       varDecl
|               expr
;

varMutate:      ID "=" expr  {
                    // Scan the symbol table for the variable, if it doesn't exist, error
                    const char* name = $1->c_str();
                    ASTNode* target = scan(name, symStack.top());
                    if(target){
                        VarDeclNode* var = dynamic_cast<VarDeclNode*>(target);
                        $$ = new MutateVarNode(var, $3);
                        
                        
                    } else {
                        printf("Variable %s not declared\n", name);
                        
                    }
                
                }
|               ID unaryOp {
                    // Scan the symbol table for the variable, if it doesn't exist, error
                    const char* name = $1->c_str();
                    
                    ExprNode* rhs;
                    
                    ASTNode* target = scan(name, symStack.top());
                    if(target){
                        rhs = new BinaryOpNode($2, new LoadVarNode(name), new IntegerNode(1));
                        
                        VarDeclNode* var = dynamic_cast<VarDeclNode*>(target);
                        
                        $$ = new MutateVarNode(var, rhs);
                    } else {
                        printf("Variable %s not declared\n", name);
                    }
                }
|               unaryOp ID {
                    // Scan the symbol table for the variable, if it doesn't exist, error
                    const char* name = $2->c_str();
                    
                    ExprNode* rhs;
                    
                    ASTNode* target = scan(name, symStack.top());
                    if(target){
                        rhs = new BinaryOpNode($1, new LoadVarNode(name), new IntegerNode(1));
                        
                        VarDeclNode* var = dynamic_cast<VarDeclNode*>(target);
                        
                        $$ = new MutateVarNode(var, rhs);
                    } else {
                        printf("Variable %s not declared\n", name);
                    }
                }
;

returnStmt:     "return" expr { $$ = new ReturnNode($2); }
;

boolExpr:       expr boolOp expr {
                    $$ = new BoolExprNode($1, $3, $2);
                }
;

expr:           funcCall
|               constant 
|               binaryOpExpr
|               unaryOpExpr
|               ID {
                    const char* name = $1->c_str();
                    ASTNode* target = scan(name, symStack.top());
                    
                    // find the variable 
                    if(target){
                        $$ = new LoadVarNode(name);
                        
                    } else {
                        printf("\u001b[31mVariable %s not declared.\n \u001b[0m", name);
                        
                    }
                }
;

funcCall:       ID  {
                    args.clear();
                } "(" exprListDelim ")" {
                
                    $$ = new FCallNode($1->c_str(), args);
                    
                }
;

exprListDelim:  exprList
|               %empty
;

exprList:       exprList "," expr { args.push_back($3); }
|               expr { args.push_back($1); }
;





binaryOpExpr:   expr binaryOp expr {
                    $$ = new BinaryOpNode($2, $1, $3);
                }
;

unaryOpExpr:    unaryOp expr { 
                    $$ = new UnaryOpNode($1, $2, "pre");
                }
                
|               expr unaryOp {
                    $$ = new UnaryOpNode($2, $1, "post");
                }
;

boolOp:         "==" { $$ = '='; }
|               "!=" { $$ = '!'; }
;

binaryOp:       "+" { $$ = '+'; }
|               "-" { $$ = '-'; }
|               "*" { $$ = '*'; }
|               "/" { $$ = '/'; }
;

unaryOp:        "++" { $$ = '+'; }
|               "--" { $$ = '-'; }
;  

constant:       NUMCONST { $$ = new IntegerNode(atoi($1->c_str())); }
|               "'" CHARCONST "'" { 
                    const char* s = $2->c_str();
                    char c = s[0];
                    printf("The character is %c\n", c);
                    exit(99);
                }
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

ASTNode* scan(const char* target, SymbolTable* st) {

    SymbolTable::iterator it = st->begin();
    while(it != st->end()) {
        if(strcmp((*it)->getName(), target) == 0) {
            
            if((*it)->value)
                printf("Value at %s = %d\n", target, (*it)->getVal());
            
            return (*it);
        }
        
        ++it;
    }
    
    return nullptr;
}
