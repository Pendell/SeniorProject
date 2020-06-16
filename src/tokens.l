%{

#include "node.h"
#include "parser.hpp"

#include <stdio.h>
#include <string>

// Include <iostream> for debugging purposes noinput nounput nodefault
#include <iostream>



/* Some quick definitions to save time.
 * These declarations are used to pass information between Flex and Bison.
 * We need to use the first one when there is more inforamtion to save about
 * the token than just what token it is. An example being an ID token:
 * We need to return that's it an ID token, but also being able to talk about
 * what the ID actually is. So we save it.
 * The second definition is just shorthand for telling Flex to return just the
 * token type to Bison. 
 */
#define SAVE_TOKEN yylval.string = new std::string(yytext, yyleng)
#define TOKEN(t) (yylval.token = t)

%}

%option noyywrap 


%%

"="                             { return TOKEN(EQUAL);  }
"+"                             { return TOKEN(PLUS);   }

"("                             { return TOKEN(LPAREN);   }
")"                             { return TOKEN(RPAREN);   }
"{"                             { return TOKEN(LBRACKET); }
"}"                             { return TOKEN(RBRACKET); }

";"                             { return TOKEN(SEMICOLON);}
":"                             { return TOKEN(COLON);    }
","                             { return TOKEN(COMMA);    }


"int"                           { return TOKEN(INT);    }
"char"                          { return TOKEN(CHAR);   }
"void"                          { return TOKEN(VOID);   }
"return"                        { return TOKEN(RETURN); }

[0-9]+                          { SAVE_TOKEN; return NUMCONST; }
[a-zA-Z_][a-zA-Z0-9_]*          { SAVE_TOKEN; return ID; }
.|\n                            { /* pass over bad characters for now */ }

%%
