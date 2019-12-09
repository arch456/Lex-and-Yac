%{
#include <stdio.h>
#include <stdlib.h>
#include "interp.c"
void yyerror(char *msg);
extern int yylex();
%}
%union{
	int iValue;
	char id;
	struct treeNode *node;
};

%token <iValue> INTEGER
%token <id> var
%token getstmt
%token putstmt
%token whilestmt
%token ifstmt
%token elsestmt
%token notstmt
%token end_program
%left EQ LE 
%left '+' '-'
%left '*' '/'
%right '~' '!'

%type <node> exp  stmt stmt_list opt_else opt_semi

%%
program :  stmt_list end_program {execute($1); exit(1);}
	    ;
stmt_list : stmt stmt_list			{$$ = buildNodeStmt(STMT, 2, $1, $2);}
		    //| /*empty*/        			 
		    | stmt                                       {$$ = $1;}  
	            ;

stmt : var '=' exp ';' 				{$$ = buildNodeStmt(ASSIGN, 2, buildNodeId($1), $3);} 			
	 | getstmt var ';' 				{ $$ = buildNodeStmt(GET, 1, buildNodeId($2));}
	 | putstmt exp ';' 	  			{$$ = buildNodeStmt(PUT, 1, $2);}
	 | whilestmt '(' exp ')' stmt 	{$$ = buildNodeStmt(WHILE, 2, $3,$5);}
	 | ifstmt '(' exp ')' stmt opt_else 	{$$ = buildNodeStmt(IF,3, $3, $5, $6);}
	 | '{' stmt_list '}'			{$$ = $2;}
	 ;

opt_else : elsestmt stmt          {$$ = $2;}  
	             |  					{;} 		
//| /*empty	*/	  		{$$ = $1;}
		 ;
		 
opt_semi : ';'
		     |
		 
exp : exp '+' exp	{$$ = buildNodeStmt(SUM, 2, $1, $3);}
	| exp '-' exp   {$$ = buildNodeStmt(SUB, 2, $1, $3);}
	| exp '*' exp   {$$ = buildNodeStmt(MUL, 2, $1, $3);}
	| exp '/' exp   {$$ = buildNodeStmt(DIV, 2, $1, $3);}
	| exp EQ exp    {$$ = buildNodeStmt(EQT, 2, $1, $3);}
	| exp LE exp    {$$ = buildNodeStmt(LET, 2, $1, $3);}
	| INTEGER	    {$$ = buildNodeConst($1);}
	| var   	    {$$ = buildNodeId($1);}
	| '(' exp ')' 	{$$ = $2;}
	| '-' INTEGER	{$$ = buildNodeConst(-$2);}
	| notstmt exp 	{$$ = buildNodeStmt(NOT, 1, $2);}
	;


%%

void yyerror(char *msg){
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(){

	for(int i =0; i < 52; i++)
		tableSymbols[i] = 0;

	return yyparse();
}
