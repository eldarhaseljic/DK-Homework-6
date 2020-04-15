%{
#include <stdio.h>
#include <vector>
#include "expression_tree.hpp"
#include "environment.hpp"

#include "y.tab.h"

#if 0
#define DBG(X) (void)fprintf(stderr,"<" X ">\n")
#else
#define DBG(X) 
#endif

char* strclone(const char* s);
%}

ws         [ \t\n]
number     [0-9]
letter     [a-zA-Z_]
id         {letter}({letter}|{number})*

%%

{ws}+        ;
{number}+    { yylval.num = atoi(yytext); DBG("NUMBER");  return NUMBER; }
"if"         { return IF; }
"else"       { return ELSE; }
"while"      { return WHILE; }
"print"      { return PRINT; }
{id}         { DBG("ID"); yylval.str = strclone(yytext); return ID; } 
"=="         { return EQ; }
"!="	       { return NE; }          
[\n]         { DBG("NL"); return *yytext; }
[,;=+*/(){}<>-]     { DBG("OP"); return *yytext; }

%%


char* strclone(const char* s) {
  size_t len = strlen(s);
  char* dest = (char*) malloc(len * sizeof(char));
  strncpy(dest, s, len);
  return dest;
}

int yywrap() {
  return 1;
}
