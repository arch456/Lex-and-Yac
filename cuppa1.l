%{
#include <stdlib.h>
#include "y.tab.h"

extern void yyerror(char *msg);
%}

%option noyywrap

%%
"while" {return whilestmt;}
"get" {return getstmt;}
"put" {return putstmt;}
"if" {return ifstmt;}
"else" {return elsestmt;}
"end" {return end_program;}
"==" {return EQ;}
"<=" { return LE;}
"!" {return notstmt;}

[0-9]+	{yylval.iValue = atoi(yytext); return INTEGER;}
[a-zA-Z_][a-zA-Z_0-9]* {yylval.id = yytext[0]; return var;}
[-+*/();={},><] {return yytext[0];}

[ \t\n]+	{;}
.	{yyerror("Unknown character");}

%%
