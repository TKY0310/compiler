%{
#include "ast.h"
#include "compiler_a_ast.tab.h"

%}
%%
"define" return DEFINE;
"array" return ARRAY; 
"while" return WHILE;
"if" return IF;
"else" return ELSE;
";" return SEMIC;
"[" return L_BRACKET;
"]" return R_BRACKET;
"(" return L_PARAN;
")" return R_PARAN;
"{" return L_BRACE;
"}" return R_BRACE;
"=" return ASSIGN;
"+" return ADD;
"-" return SUB;
"*" return MUL;
"/" return DIV;
"==" return EQ;
"<" return LT;
">" return GT;
[0-9]+ return NUMBER;
[a-zA-Z][a-zA-Z0-9]* return IDENT;
[ \t\n] ;
%% 