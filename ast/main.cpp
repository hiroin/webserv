#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>

typedef enum _token_type {
	INITIAL,
	IDENT,    /* 識別子 */
	NUM,      /* 数値 */
	NUM_I,    /* 数値（整数）*/
	NUM_D,    /* 数値（実数）*/
	SYMBOL,   /* 記号   */
	PLUS,     /* + */
	MINUS,    /* - */
	MULTI,    /* * */
	DIV,      /* / */
	LB,       /* ( */
	RB,       /* ) */
	END
} TOKEN_TYPE;

typedef struct _token {
	TOKEN_TYPE    type;
	char          *token;
	double        value_d;
	long          value_l;
	struct _token *next;
	struct _token *prev;
} TOKEN;

typedef struct _node {
	char *name;
	struct _node *left;
	struct _node *right;
	double rval;
	TOKEN *token;
} NODE;

NODE *evaluateAddSub();
NODE *evaluateMulDiv();
NODE *evaluateFactor();

TOKEN *token;

NODE *createNode(TOKEN* token)
{
	NODE *node = (NODE *)calloc(1, sizeof(NODE));
	if(node == NULL){
		fprintf(stderr, "Memory Allocation Error");
		exit(EXIT_FAILURE);
	}
	node->left  = NULL;
	node->right = NULL;
	node->name = token->token;
	node->token = token;
	return node;
}

void addChild(NODE *node, NODE *left, NODE *right)
{
	node->left = left;
	node->right = right;
}

// <addsub> ::= <muldiv> ( '+' <muldiv> | '-' <muldiv> )*
NODE *evaluateAddSub()
{
	NODE *left = evaluateMulDiv();
	while(token != NULL && (token->type == PLUS || token->type == MINUS)) {
		NODE *node = createNode(token);
		token = token->next;
		NODE *right = evaluateMulDiv();
		addChild(node, left, right);
		left = node;
	}
	return left;
}

// <muldiv> ::= <factor> ( '*' <factor> | '/' <factor>)*
NODE *evaluateMulDiv()
{
	NODE *left = evaluateFactor();
	while(token != NULL && (token->type == MULTI || token->type == DIV)){
		NODE *node = createNode(token);
		token = token->next;
		NODE *right = evaluateFactor();
		addChild(node, left, right);
		left = node;
	}
	return left;
}

// <factor> ::= NUM | '(' <addsub> ')'
NODE *evaluateFactor()
{
	NODE *node;
	if(token->type == LB){
		token = token->next;
		node = evaluateAddSub();
		if(token->type != RB){
			fprintf(stderr, "Missing ) ?");
			exit(EXIT_FAILURE);
		}
		token = token->next;
	} else {
		node = createNode(token);
		node->rval = token->value_l;
		token = token->next;
	}
	return node;
}

double calc(NODE *node)
{
	if(node->token->type == NUM_I || node->token->type == NUM_D){
		return node->rval;
	}
	double left = calc(node->left);
	double right = calc(node->right);
	if(node->token->type == PLUS) return left + right;
	if(node->token->type == MINUS) return left - right;
	if(node->token->type == MULTI) return left * right;
	if(node->token->type == DIV) return left / right;
  return 0;
}

void lexer(char *s)
{
	unsigned int idx;
	unsigned int bgn;
	TOKEN *t;
	TOKEN_TYPE tt;
	for(idx = 0; s[idx] != '\0'; ) {

		/* 空白文字の処理 */
		if(s[idx] <= ' '){
			idx++;
			continue;
		}

		/* Token */
		bgn = idx;
		/* [a-zA-Z][a-zA-Z0-9]* */
		if(isalpha(s[idx])){
			while(isalnum(s[idx])){
				idx++;
			}
			tt = IDENT;
		}
		/* ([0-9]+)|([0-9]+\.[0-9]+) */
		else if(isdigit(s[idx]) || s[idx] == '.' || (s[idx] == '-' && isdigit(s[idx+1]))) {
			int c = 0;
			if(s[idx] == '.'){
				c += 1;
			}
			idx += 1;
			while(isdigit(s[idx]) || s[idx] == '.') {
				if(s[idx] == '.'){
					c += 1;
				}
				idx++;
			}
			if(c == 0){
				tt = NUM_I;
			} else if(c == 1) {
				tt = NUM_D;
			} else {
				fprintf(stderr, "Parse Error\n");
				return;
			}
		}
		/* それ以外 */
		else {
			switch(s[idx]){
			case '+':
				tt = PLUS;
				break;
			case '-':
				tt = MINUS;
				break;
			case '*':
				tt = MULTI;
				break;
			case '/':
				tt = DIV;
				break;
			case '(':
				tt = LB;
				break;
			case ')':
				tt = RB;
				break;
			default:
				tt = SYMBOL;
				break;
			}	
			idx++;
		}
		
		t = (TOKEN*)malloc(sizeof(TOKEN));
		t->token = (char *)malloc(idx - bgn + 1);
		strncpy(t->token, &s[bgn], idx - bgn);
		t->token[idx - bgn] = '\0';
		t->type = tt;
		if(tt == NUM_I){
			sscanf(t->token, "%ld", &t->value_l);
		} else if(tt == NUM_D) {
			sscanf(t->token, "%lf", &t->value_d);
		}
		t->next = NULL;
		token->next = t;
		t->prev = token;
		token = t;
	}
	return ;
}

void printAST(NODE *node)
{
	int n;
	if(node == NULL){
		return;
	}
	printf(" (%s", node->name);
	NODE *child;
	child = node->left;
	if(child != NULL && (child->token->type == NUM_I || child->token->type == NUM_D)) {
		printf(" %lf", child->rval);
	} else {
		printAST(child);
	}
	child = node->right;
	if(child != NULL && (child->token->type == NUM_I || child->token->type == NUM_D)) {
		printf(" %lf", child->rval);
	} else {
		printAST(child);
	}	
	printf(")");
}


NODE *parser()
{
	NODE *expr;
	if(token->type == INITIAL){
		token = token->next;
	}
	expr = evaluateAddSub();
	printAST(expr);
	printf("\n");
	return expr;
}

void free_token()
{
	TOKEN *t;
	while(1){
		t = token->next;
		free(token);
		
		if(t == NULL){
			break;
		}
		token = t;
	}
	token = NULL;
}

void free_node(NODE* node)
{
	if(node->token->type == NUM_I || node->token->type == NUM_D){
		free(node);
		return;
	}
	free_node(node->left);
	free_node(node->right);
	free(node);
	return;
}

void print_token(TOKEN* token)
{
	while(1) {
		if(token->type == END){
			break;
		}
		if(token->type != INITIAL){
			printf("TOKEN TYPE: ");
			switch(token->type){
			case IDENT:
				printf("ID   ");
				break;
			case NUM:
				printf("NM   ");
			case NUM_I:
				printf("NI   ");
				break;
			case NUM_D:
				printf("ND   ");
				break;
			case SYMBOL:
				printf("SM   ");
				break;
			case PLUS:
				printf("PLS  ");
				break;
			case MINUS:
				printf("MNS  ");
				break;
			case MULTI:
				printf("MUL  ");
				break;
			case DIV:
				printf("DIV  ");
				break;
			case LB:
				printf("LB   ");
				break;
			case RB:
				printf("RB   ");
				break;
			default:
				printf("UK   ");
			}
			printf("TOKEN: %s", token->token);
		}
		if(token->type == NUM_I) {
			printf(" VALUE: %ld", token->value_l);
		}
		if(token->type == NUM_D) {
			printf(" VALUE: %lf", token->value_d);
		}

		printf("\n");
		if(token->next == NULL){
			break;
		}
		token  = token->next;
	}
}

int main(int argc, char* argv[])
{
	char *cmd;
	TOKEN *o_token;
	NODE *expr;
	double result;
	using_history();
	read_history(".calc_history");

	while(1){
		token = (TOKEN*)malloc(sizeof(TOKEN));
		o_token = token;
		token->type = INITIAL;
		token->prev = NULL;
		token->next = NULL;
		
		cmd = readline(">> ");
		if(cmd == NULL){
			free(cmd);
			break;
		}
		add_history(cmd);
		
		lexer(cmd);
		//print_token(o_token);

		token = o_token;
		expr = parser();
		result = calc(expr);
		printf("Result-> %lf\n", result);
		token = o_token;

		free_node(expr);		
		free_token();
		free(cmd);
	}
	write_history(".calc_history");
	return EXIT_SUCCESS;
}