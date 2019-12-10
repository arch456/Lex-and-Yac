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
	struct node *nd;
};

%token <iValue> INTEGER
%token <id> ID
%token getstmt
%token putstmt
%token ifstmt
%token elsestmt
%token whilestmt
%token notstmt
%token stopprogram
%left EQ LE 
%left '+' '-'
%left '*' '/'
%right UMINUS '!'

%type <nd> exp  stmt stmt_list opt_else opt_semi

%%
program :  stmt_list stopprogram {walk($1); exit(1);}
	    ;
stmt_list : stmt stmt_list			{$$ = nstmt_gen(STMT, 2, $1, $2);}
		    //| /*empty*/        			{;} 
		    | stmt                                       {$$ = $1;}  
	            ;

stmt : ID '=' exp opt_semi 				{$$ = nstmt_gen(ASSIGN, 2, nid_gen($1), $3);} 			
	    | getstmt ID opt_semi 				{$$ = nstmt_gen(GET, 1, nid_gen($2));}
	    | putstmt exp opt_semi 	  		{$$ = nstmt_gen(PUT, 1, $2);}
	    | whilestmt '(' exp ')' stmt 			{$$ = nstmt_gen(WHILE, 2, $3,$5);}
	    | ifstmt '(' exp ')' stmt opt_else 	{$$ = nstmt_gen(IF,3, $3, $5, $6);}
	    | '{' stmt_list '}'						{$$ = $2;}
	    ;

opt_else : elsestmt stmt          {$$ = $2;}  
	             |  					{;} 		
	             ;
		 
opt_semi : ';'
		     |
		 
exp : exp '+' exp				{$$ = nstmt_gen(SUM, 2, $1, $3);}
	| exp '-' exp   				{$$ = nstmt_gen(SUB, 2, $1, $3);}
	| exp '*' exp   				{$$ = nstmt_gen(MUL, 2, $1, $3);}
	| exp '/' exp   				{$$ = nstmt_gen(DIV, 2, $1, $3);}
	| exp EQ exp    			{$$ = nstmt_gen(EQT, 2, $1, $3);}
	| exp LE exp    				{$$ = nstmt_gen(LET, 2, $1, $3);}
	| INTEGER	    				{$$ = nconst_gen($1);}
	| ID   	    					{$$ = nid_gen($1);}
	| '(' exp ')' 					{$$ = $2;}
	| '-' exp %prec UMINUS  	{$$ = nstmt_gen(UMINUST,1,$2);}	
	//| '-' INTEGER %prec UMINUS	{$$ = nconst_gen(-$2);}
	| notstmt exp 				{$$ = nstmt_gen(NOT, 1, $2);}
	;


%%

void yyerror(char *msg){
	fprintf(stderr, "%s\n", msg);
	exit(1);
}

int main(){

	for(int i =0; i < 52; i++)
		symbol_table[i] = 0;

	return yyparse();
}
