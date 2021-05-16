%{
#include<stdio.h>
#include<string.h>
#include "ast.h"
#include "cmp_a_ast.tab.h"
extern int yylex();
extern int yyerror();
Node* top;
%}

%union{
Node* np;
int ival;
char* sp;
}

%token DEFINE ARRAY WHILE IF ELSE SEMIC L_BRACKET R_BRACKET
%token L_PARAN R_PARAN L_BRACE R_BRACE ASSIGN ADD SUB MUL DIV
%token EQ LT GT 
%token <sp>IDENT 
%token <ival>NUMBER

%type <np> expression term factor condition var array
%type <np> program assignment_stmt declarations decl_statement statements
%type <np> statement loop_stmt cond_stmt
%type <ival> add_op mul_op cond_op

%%
program : declarations statements {top = build_node2(PRO_AST,$1,$2);}
;
declarations : decl_statement declarations {$$ = build_node2(DEC_AST,$1,$2);}
| decl_statement {$$ = build_node2(DEC_AST,$1,NULL);}
;
decl_statement : DEFINE IDENT SEMIC 
{$$ = build_node1(DEFINE_AST,build_ident_node(IDENT_AST,$2));}
 | ARRAY array SEMIC {$$ = build_node1(DEFINE_AST,$2);}
;
statements : statement statements {$$ = build_node2(STAT_AST,$1,$2);}
| statement {$$ = build_node2(STAT_AST,$1,NULL);}
;
statement : assignment_stmt| loop_stmt | cond_stmt 
;
assignment_stmt : IDENT ASSIGN expression SEMIC 
{$$ = build_node2(ASSIGN_AST,build_ident_node(IDENT_AST,$1),$3);}
 | array ASSIGN expression SEMIC {$$ = build_node2(ASSIGN_AST,$1,$3);}
;
expression : expression add_op term 
{if ($2 == ADD)$$ = build_node2(PLUS_AST,$1,$3);
else $$ = build_node2(MINUS_AST,$1,$3);
}
| term
;
term : term mul_op factor
{if ($2 == MUL)$$ = build_node2(TIMES_AST,$1,$3);
else $$ = build_node2(DIVIDED_AST,$1,$3);
}
| factor
;
factor : var 
| array 
| L_PARAN expression R_PARAN {$$ = $2;}
;
add_op : ADD {$$ = ADD;}
| SUB {$$ = SUB;}
;
mul_op : MUL {$$ = MUL;}
| DIV {$$ = DIV;}
;
array : IDENT L_BRACKET NUMBER R_BRACKET 
{$$ = build_array_node(ARRAY_AST,$1,$3);}
;
var : IDENT {$$ = build_ident_node(IDENT_AST,$1);}
| NUMBER {$$ = build_num_node(NUM_AST,$1);}
;
loop_stmt : WHILE L_PARAN condition R_PARAN L_BRACE statements R_BRACE
{$$ = build_node2(WHILE_AST,$3,$6);}

;
cond_stmt : IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE
{$$ = build_node2(IF_AST,$3,$6);}
 | IF L_PARAN condition R_PARAN L_BRACE statements R_BRACE ELSE L_BRACE statements R_BRACE
{$$ = build_node3(IF_AST,$3,$6,$10);}
;
condition : expression cond_op expression
{if ($2 == EQ)$$ = build_node2(EQUAL_AST,$1,$3);
else if($2 == LT)$$ = build_node2(LT_AST,$1,$3);
else $$ = build_node2(GT_AST,$1,$3);
}
;
cond_op : EQ {$$ = EQ;}
| LT {$$ = LT;}
| GT {$$ = GT;}
;
%%
