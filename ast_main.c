int main(void)
{
	int result;
	result = yyparse();
 if (!result){
  printTree(top);
 }
 return 0;
}