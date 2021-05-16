#include "ast.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAXBUF 69

Node *createASTNode(Ntype t,Node* obj1,Node* obj2,Node* obj3){
	Node *p;
	if((p = (Node *)malloc(sizeof(Node))) == NULL){
	printf("out of memory");
	}
	
	p->type = t;
	p->child = obj1;
	if(obj1 != NULL){
	obj1->brother = obj2;
		if(obj2 != NULL){
		obj2->brother = obj3;
		}
	}
	p->brother = NULL;
	return p;
}

Node *build_node1(Ntype t,Node* obj1){
	createASTNode(t,obj1,NULL,NULL);
}

Node *build_node2(Ntype t,Node* obj1,Node* obj2){
	createASTNode(t,obj1,obj2,NULL);
}

Node *build_node3(Ntype t,Node* obj1,Node* obj2,Node* obj3){
	createASTNode(t,obj1,obj2,obj3);
}

Node*build_num_node(Ntype t,int n){
	Node *p;
	if((p = (Node *)malloc(sizeof(Node))) == NULL){
	printf("out of memory");
	}
	
	p->type = t;
	p->ivalue = n;
	p->child = NULL;
	p->brother = NULL;
	return p;
}

Node *build_ident_node(Ntype t,char *s){
	Node *p;
	if((p = (Node *)malloc(sizeof(Node))) == NULL){
	printf("out of memory");
	}
	
	p->type = t;
	if((p->variable = (char *)malloc(MAXBUF)) == NULL){
	printf("out of memory");
	}
	strncpy(p->variable,s,MAXBUF);
	p->child = NULL;
	p->brother = NULL;
	return p;
}

Node *build_array_node(Ntype t,char *s,int n){
	Node *p;
	if((p = (Node *)malloc(sizeof(Node))) == NULL){
	printf("out of memory");
	}
	
	p->type = t;
	if((p->variable = (char *)malloc(MAXBUF)) == NULL){
	printf("out of memory");
	}
	strncpy(p->variable,s,MAXBUF);
	p->ivalue = n;
	p->child = NULL;
	p->brother = NULL;
	return p;
}

void printNodeType(Ntype t){
	switch (t){
		case PRO_AST : printf("PRO_AST\t"); break;
		case STAT_AST : printf("STAT_AST\t"); break;
		case DEC_AST : printf("DEC_AST\t"); break;
		case ASSIGN_AST : printf("ASSIGN_AST\t"); break;
		case PLUS_AST : printf("PLUS_AST\t"); break;
		case MINUS_AST : printf("MINUS_AST\t"); break;
		case TIMES_AST : printf("TIMES_AST\t"); break;
		case DIVIDED_AST : printf("DIVIDED_AST\t"); break;
		case EQUAL_AST : printf("EQUAL_AST\t"); break;
		case LT_AST : printf("LT_AST\t"); break;
		case GT_AST : printf("GT_AST\t"); break;
		case WHILE_AST : printf("WHILE_AST\t"); break;
		case IF_AST : printf("IF_AST\t"); break;
		case ELSE_AST : printf("ELSE_AST\t"); break;
		case DEFINE_AST : printf("DEFINE_AST\t"); break;
		case ARRAY_AST : printf("ARRAY_AST\t"); break;
		default : break;
	}
	
}

void printTree(Node *obj){
 if (obj != NULL) {
 	if (obj->child != NULL) {
 	printTree(obj->child);
 	}
 	if(obj->type == NUM_AST){
 		printf("%d\t", obj->ivalue);
 	}
 	else if(obj->type == IDENT_AST || obj->type == ARRAY_AST){
 		printf("%s\t", obj->variable);
 	}
 	else printNodeType(obj->type);
 	if (obj->brother != NULL) {
 	printTree(obj->brother);
 		}
 	printf("\n");
 	}
}
