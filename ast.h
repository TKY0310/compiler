typedef enum{
	IDENT_AST,
	NUM_AST,
	PRO_AST,
	STAT_AST,
	DEC_AST,
	ASSIGN_AST,
	PLUS_AST,
	MINUS_AST,
	TIMES_AST,
	DIVIDED_AST,
	EQUAL_AST,
	LT_AST,
	GT_AST,
	WHILE_AST,
	IF_AST,
	ELSE_AST,
	DEFINE_AST,
	ARRAY_AST,
} Ntype;

typedef struct node{
	Ntype type;
	int ivalue; 
	char* variable; 
	struct node *child;
	struct node *brother;
} Node;

Node*build_node1(Ntype,Node*);

Node*build_node2(Ntype,Node*,Node*);

Node*build_node3(Ntype,Node*,Node*,Node*);

Node*build_num_node(Ntype,int);

Node*build_ident_node(Ntype,char*);

Node *build_array_node(Ntype t,char *s,int n);

void printNodeType(Ntype t);

void printTree(Node*);