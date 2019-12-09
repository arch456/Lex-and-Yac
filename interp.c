#include <ctype.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>

int tableSymbols[52];
int value1, x;
int getValueFromTable(char c); // gets value from the symbol table
void updateTable(char c, int value); //updates the symbol table


int getIdentifierFromTable(char c){
	int index = -1;
	if(islower(c)){	
		index =  c - 'a' + 26;
	}
	else if(isupper(c)){
		index = c - 'A';
	}
	return index;	
};

int getValueFromTable(char c){
	int index = getIdentifierFromTable(c);
	return tableSymbols[index];
};

void updateTable(char c, int value){
	int index = getIdentifierFromTable(c);
	tableSymbols[index] = value;
};


enum typeNode{typeId, typeConst, typeOp};
enum typeOperation{STMT, ASSIGN, GET, PUT, IF, WHILE, MUL, DIV, SUM, SUB,LET,EQT,NOT };

struct nodeId{
	char id;
};

struct nodeConst{
	int value;
};

struct nodeStmt{
	int typeOperation;
	int numArgs;
	struct treeNode *op[];
};
struct treeNode{
	int typeNode;
	union {
		struct nodeId nodeId;
		struct nodeConst nodeConst;
		struct nodeStmt nodeStmt;
	};
};

struct treeNode * buildNodeId(char var){
	struct treeNode *node;
	node = (struct treeNode *)malloc(sizeof(struct treeNode));
	node->typeNode = typeId;
	node->nodeId.id = var;
	return node;
};

struct treeNode * buildNodeConst(int value){
	struct treeNode *node;
	node = (struct treeNode *)malloc(sizeof(struct treeNode));
	node->typeNode = typeConst;
	node->nodeConst.value = value;
	return node;
};

struct treeNode *buildNodeStmt(int op, int numArgs, ...){
	va_list ap;
	struct treeNode *node = (struct treeNode *)malloc(sizeof(struct treeNode) + (numArgs)*sizeof(struct treeNode *));
	node->typeNode = typeOp;
	node->nodeStmt.typeOperation = op;
	node->nodeStmt.numArgs = numArgs;
	va_start(ap, numArgs);
	for(int i =0; i < numArgs; i++){
		node->nodeStmt.op[i] = va_arg(ap, struct treeNode *);
	} 
	va_end(ap);
	return node;
};


int execute(struct treeNode *node){
	switch(node->typeNode){
		case typeConst : return node->nodeConst.value;
	    case typeId    : return getValueFromTable(node->nodeId.id);
		case typeOp    :{
			switch (node->nodeStmt.typeOperation) {		
			    case STMT   : {execute(node->nodeStmt.op[0]); execute(node->nodeStmt.op[1]);return 0;}	    
			    case ASSIGN : {value1 = execute(node->nodeStmt.op[1]); updateTable(node->nodeStmt.op[0]->nodeId.id, value1);return 0; }
			    case WHILE : {
							   while(execute(node->nodeStmt.op[0])){
									execute(node->nodeStmt.op[1]); 
									value1 = execute(node->nodeStmt.op[0]);}
							return 0;}
				case IF  : { value1 = execute(node->nodeStmt.op[0]); if(value1){execute(node->nodeStmt.op[1]);}else {if (node->nodeStmt.numArgs > 2) execute(node->nodeStmt.op[2]);} return 0;} 
                                case GET	: {scanf("%d\n", &value1); updateTable(node->nodeStmt.op[0]->nodeId.id, value1);return 0;}
				case PUT	:{value1 = execute(node->nodeStmt.op[0]); printf("print %d\n",value1 ); return 0;}
				case MUL	: return execute(node->nodeStmt.op[0]) * execute(node->nodeStmt.op[1]);
				case DIV	: return execute(node->nodeStmt.op[0]) / execute(node->nodeStmt.op[1]);
				case SUM  	: return execute(node->nodeStmt.op[0]) + execute(node->nodeStmt.op[1]);
				case SUB  	: return execute(node->nodeStmt.op[0]) - execute(node->nodeStmt.op[1]);
				case LET  	: return execute(node->nodeStmt.op[0]) <= execute(node->nodeStmt.op[1]);
				case EQT 	: return execute(node->nodeStmt.op[0]) == execute(node->nodeStmt.op[1]);
				case NOT   : return !execute(node->nodeStmt.op[0]);
//{  value1 = execute(node->nodeStmt.op[0]); if (value1 == 0) return 1; else return 0;}

			}
 		}
	}
}





