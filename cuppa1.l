%{
#include <stdlib.h>
#include "y.tab.h"

extern void yyerror(char *msg);
%}

%option noyywrap

%%
"get" {return getstmt;}
"put" {return putstmt;}
"if" {return ifstmt;}
"else" {return elsestmt;}
"while" {return whilestmt;}
"stop" {return stopprogram;}
"==" {return EQ;}
"<=" { return LE;}
"-" { return UMINUS;}
"!" {return notstmt;}

"//"  {;}

[0-9]+	{yylval.iValue = atoi(yytext); return INTEGER;}
[a-zA-Z_][a-zA-Z_0-9]* {yylval.id = yytext[0]; return ID;}
[-+*/();={},><] {return yytext[0];}
[//.*]		{;}
[ \t\n]+	{;}
.	{yyerror("Unknown character");}

%%
