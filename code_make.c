#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include "ast.h"
#include "cmp_a_ast.tab.h"
typedef enum{
	IDENT_ST,
	ARRAY_ST,
} Stype;

Node* top;

#define SIMNAMELEN 64
#define MAXSIMENTRIES 100

typedef struct simTableEntry {
	char name[SIMNAMELEN];
	int offset;
	int size;
	Stype stype;
} SimTableEntry;

SimTableEntry simTable[MAXSIMENTRIES];

int cur_syms = 0;

int g_offset = 0;

void codegen_expression(Node*np,FILE*fp);
void code_gen(Node*np,FILE*fp);
void codegen_condition(Node*np,FILE*fp);

void SymAdd(Node*np){
	
	if(np->type == IDENT_AST){
	strncpy(simTable[cur_syms].name,np->variable,SIMNAMELEN);
	simTable[cur_syms].stype  = IDENT_ST;
	simTable[cur_syms].offset = g_offset;
	simTable[cur_syms].size  = 4;
	}
	
	else if(np->type == ARRAY_AST){
	strncpy(simTable[cur_syms].name,np->variable,SIMNAMELEN);
	simTable[cur_syms].stype  = ARRAY_ST;
	simTable[cur_syms].offset = g_offset;
	simTable[cur_syms].size  = 4 * (np->ivalue);
	}
	
	g_offset += simTable[cur_syms].size;
	cur_syms++;
}

int getoffset(Node*np){
	int i,val;
	if(np->type == IDENT_AST){
	while(i <= cur_syms){
		if(strcmp(np->variable,simTable[i].name) == 0){
			val = simTable[i].offset;
			}
		i++;
		}
	}
	
	if(np->type == ARRAY_AST){
	while(i <= cur_syms){
		if(strcmp(np->variable,simTable[i].name) == 0){
			val = simTable[i].offset + 4*(np->ivalue);
			}
		i++;
		}
	}
	return val;
}

void codegen_number(Node*np,FILE*fp){
	fprintf(fp,"\tli $v0,%d\n",np->ivalue);
}

void codegen_ident(Node*np,FILE*fp){
	fprintf(fp,"\tla $t0,RESULT\n");
	fprintf(fp,"\tlw $v0,%d($t0)\n",getoffset(np));
	fprintf(fp,"\tnop\n");
}

/*代入文*/
void codegen_assign(Node*np,FILE*fp){
	codegen_expression(np->child->brother,fp);
	fprintf(fp,"\tla $t0,RESULT\n");
	fprintf(fp,"\tsw $v0,%d($t0)\n",getoffset(np->child));
}

/*ループ文*/
void codegen_roop(Node*np,FILE*fp){
	fprintf(fp,"L1:\n");
	codegen_condition(np->child,fp);
	fprintf(fp,"\tbeq $t2,$zero,L2\n");
	fprintf(fp,"\tnop\n");
	code_gen(np->child->brother,fp);
	fprintf(fp,"\tj L1\n");
	fprintf(fp,"\tnop\n");
	fprintf(fp,"L2:\n");
}

/*条件分岐文*/
void codegen_stmt(Node*np,FILE*fp){
	fprintf(fp,"L1:\n");
	codegen_condition(np->child,fp);
	fprintf(fp,"\tbeq $t2,$zero,L2\n");
	fprintf(fp,"\tnop\n");
	code_gen(np->child->brother,fp);
	fprintf(fp,"L2:\n");
	if(np->child->brother->brother != NULL){
		code_gen(np->child->brother->brother,fp);
	}
}

/*算術式*/
void codegen_expression(Node*np,FILE*fp){
	if(np == NULL) return;
	
	if(np->type == IDENT_AST){
		codegen_ident(np,fp);
	}
		
	else if(np->type == NUM_AST){
		codegen_number(np,fp);
	}
	
	else if(np->type == PLUS_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,-4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tadd $v0,$v0,$t1\n");
	}
	
	else if(np->type == MINUS_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,-4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tsub $v0,$v0,$t1\n");
	}
	
	else if(np->type == TIMES_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,-4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tmult $v0,$t1\n");
		fprintf(fp,"\tmflo $v0\n");
	}
	
	else if(np->type == DIVIDED_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,-4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tdiv $v0,$v0,$t1\n");
		
	}
}

/*条件文*/
void codegen_condition(Node*np,FILE*fp){
	
	if(np == NULL) return;
	
	if(np->type == IDENT_AST){
		codegen_ident(np,fp);
	}
		
	else if(np->type == NUM_AST){
		codegen_number(np,fp);
	}
	
	else if(np->type == EQUAL_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,-4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tseq $t2,$v0,$t1\n");
	}
	else if(np->type == LT_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tslt $t2,$v0,$t1\n");
	}
	else if(np->type == GT_AST){
		codegen_expression(np->child,fp);
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tsw $v0,0($sp)\n");
		codegen_expression(np->child->brother,fp);
		fprintf(fp,"\tadd $t1,$v0,$zero\n");
		fprintf(fp,"\tlw $v0,0($sp)\n");
		fprintf(fp,"\taddi $sp,$sp,4\n");
		fprintf(fp,"\tslt $t2,$t1,$v0\n");
	}

}

void code_gen(Node*np,FILE*fp){
	if(np == NULL) return;
	switch(np->type){
	case PRO_AST:
	case DEC_AST:
	case STAT_AST:
		code_gen(np->child,fp);
		code_gen(np->child->brother,fp);
		break;
	case DEFINE_AST:
		SymAdd(np->child);
		break;
	case ASSIGN_AST:
		codegen_assign(np,fp);
		break;
	case WHILE_AST:
		codegen_roop(np,fp);
		break;
	case IF_AST:
		codegen_stmt(np,fp);
		break;
	default: break;
	}
}

void initialize(FILE*fp){
	fprintf(fp,"#\n");
	fprintf(fp,"\t#\n");
	fprintf(fp,"\tINITIAL_GP = 0x10008000\n");
	fprintf(fp,"\tINITIAL_SP = 0x7ffffffc\n");
	fprintf(fp,"\t# system call service number\n");
	fprintf(fp,"\tsnp_service = 99\n");
	fprintf(fp,"\n");
	fprintf(fp,"\t.text\n");
	fprintf(fp,"init:\n");
	fprintf(fp,"\t# initialize $gp (global pointer) and $sp (stack pointer)\n");
	fprintf(fp,"\tla $gp, INITIAL_GP\n");
	fprintf(fp,"\tla $sp, INITIAL_SP\n");
	fprintf(fp,"\tjal main # jump to 'main'\n");
	fprintf(fp,"\tnop\n");
	fprintf(fp,"\tli $v0, snp_service\n");
	fprintf(fp,"\tsyscall\n");
	fprintf(fp,"\tnop\n");
	fprintf(fp,"\t# not reach here\n");
	fprintf(fp,"stop:\n");
	fprintf(fp,"\tj stop\n");
	fprintf(fp,"\tnop\n");
	fprintf(fp,"\n");
	fprintf(fp,"\t.text 0x00001000\n");
	fprintf(fp,"main:\n");
}

void settle(FILE*fp){
	int i;
	fprintf(fp,"\tjr $ra\n");
	fprintf(fp,"\tnop\n");
	fprintf(fp,"\n");
	fprintf(fp,"\t#\n");
	fprintf(fp,"\t#data segment\n");
	fprintf(fp,"\t#\n");
	fprintf(fp,"\t.data 0x10004000\n");
	fprintf(fp,"RESULT:\n");
	for(i=0;i < cur_syms;i++){
		fprintf(fp,"\t.word 0x0000 #%s\n",simTable[i].name);
	}
}

int main(int argc,char *argv[]){
	FILE*fp;
	int result = yyparse();
	
	if((fp = fopen(argv[1],"w"))== NULL){
		fprintf(stderr,"error : could not open file\n");
		exit(1);
	}
	
	if(result == 0){
	initialize(fp);
	code_gen(top,fp);
	settle(fp);
	}
	return 0;
}
