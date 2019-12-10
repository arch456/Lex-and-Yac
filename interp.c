#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int symbol_table[52];
int value1, x;
int get_table(char ch); 
void update_table(char ch, int val); 


int get_ID(char ch){
	int id = -1;
	if(islower(ch)){	
		id =  ch - 'a' + 26;
	}
	else if(isupper(ch)){
		id = ch - 'A';
	}
	return id;	
};

// This function is used for getting a value from symbol table
int get_value(char ch){
	int id = get_ID(ch);
	return symbol_table[id];
};

//This function is used for updating the symbol table with a value
void update_table(char ch, int val){
	int id = get_ID(ch);
	symbol_table[id] = val;
};


enum node_type{t_id, n_const, n_op};
enum node_func{STMT, ASSIGN, GET, PUT, IF, WHILE, MUL, DIV, SUM, SUB,LET,EQT,NOT,UMINUST };

struct n_id{
	char id;
};

struct n_const{
	int val;
};

struct n_stmt{
	int node_func;
	int num_arg;
	struct node *op[];
};
struct node{
	int node_type;
	union {
		struct n_id n_id;
		struct n_const n_const;
		struct n_stmt n_stmt;
	};
};

struct node * nid_gen(char c){
	struct node *nd;
	nd = (struct node *)malloc(sizeof(struct node));
	nd->node_type = t_id;
	nd->n_id.id = c;
	return nd;
};

struct node * nconst_gen(int val){
	struct node *nd;
	nd = (struct node *)malloc(sizeof(struct node));
	nd->node_type = n_const;
	nd->n_const.val = val;
	return nd;
};

struct node *nstmt_gen(int op, int num_arg, ...){
	va_list ap;
	struct node *nd = (struct node *)malloc(sizeof(struct node) + (num_arg)*sizeof(struct node *));
	nd->node_type = n_op;
	nd->n_stmt.node_func = op;
	nd->n_stmt.num_arg = num_arg;
	va_start(ap, num_arg);
	for(int i =0; i < num_arg; i++){
		nd->n_stmt.op[i] = va_arg(ap, struct node *);
	} 
	va_end(ap);
	return nd;
};


int walk(struct node *nd){
	switch(nd->node_type){
		case n_const : return nd->n_const.val;
	    case t_id    : return get_value(nd->n_id.id);
		case n_op    :{
			switch (nd->n_stmt.node_func) {		
			    case STMT   : {walk(nd->n_stmt.op[0]); walk(nd->n_stmt.op[1]);return 0;}	    
			    case ASSIGN : {value1 = walk(nd->n_stmt.op[1]); update_table(nd->n_stmt.op[0]->n_id.id, value1);return 0; }
			    case WHILE : {
							   while(walk(nd->n_stmt.op[0])){
									walk(nd->n_stmt.op[1]); 
									value1 = walk(nd->n_stmt.op[0]);}
							return 0;}
				case IF  : { value1 = walk(nd->n_stmt.op[0]); if(value1){walk(nd->n_stmt.op[1]);}else {if (nd->n_stmt.num_arg > 2) walk(nd->n_stmt.op[2]);} return 0;} 
                                case GET	: {scanf("%d\n", &value1); update_table(nd->n_stmt.op[0]->n_id.id, value1);return 0;}
				case PUT	:{value1 = walk(nd->n_stmt.op[0]); printf("%d\n",value1 ); return 0;}
				case MUL	: return walk(nd->n_stmt.op[0]) * walk(nd->n_stmt.op[1]);
				case DIV	: return walk(nd->n_stmt.op[0]) / walk(nd->n_stmt.op[1]);
				case SUM  	: return walk(nd->n_stmt.op[0]) + walk(nd->n_stmt.op[1]);
				case SUB  	: return walk(nd->n_stmt.op[0]) - walk(nd->n_stmt.op[1]);
				case LET  	: return walk(nd->n_stmt.op[0]) <= walk(nd->n_stmt.op[1]);
				case EQT 	: return walk(nd->n_stmt.op[0]) == walk(nd->n_stmt.op[1]);
				case NOT   : return !walk(nd->n_stmt.op[0]);
				case UMINUST	: return - walk(nd->n_stmt.op[0]);
//{  value1 = walk(node->nodeStmt.op[0]); if (value1 == 0) return 1; else return 0;}

			}
 		}
	}
}





