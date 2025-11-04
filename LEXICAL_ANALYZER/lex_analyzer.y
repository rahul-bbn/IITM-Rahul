%{
#include <stdio.h>
#include <stdlib.h>
 
int yylex();
void yyerror(const char *s);
 
typedef struct Node {
    char op;
    int value;
    struct Node *left, *right;
} Node;
 
Node* makeNode(char op, Node* left, Node* right);
Node* makeLeaf(int value);
void printTree(Node* root);
int evaluate(Node* root);
 
Node* root = NULL;
%}
 
%union {
    int num;
    struct Node* node;
}
 
%token <num> NUMBER
%type <node> expr term factor
 
%left '+' '-'
%left '*' '/'
 
%start input
 
%%
 
input:
      /* empty */
    | input expr '\n' {
        root = $2; 
        printf("AST: ");
        printTree(root);
        printf("\n");
        printf("Result = %d\n\n", evaluate(root));
      }
;
 
expr:
      expr '+' term { $$ = makeNode('+', $1, $3); }
    | expr '-' term { $$ = makeNode('-', $1, $3); }
    | term          { $$ = $1; }
;
 
term:
      term '*' factor { $$ = makeNode('*', $1, $3); }
    | term '/' factor { $$ = makeNode('/', $1, $3); }
    | factor          { $$ = $1; }
;
 
factor:
      NUMBER        { $$ = makeLeaf($1); }
    | '(' expr ')'  { $$ = $2; }
;
 
%%
 
void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
}
 
Node* makeNode(char op, Node* left, Node* right) {
    Node* n = malloc(sizeof(Node));
    n->op = op;
    n->value = 0;
    n->left = left;
    n->right = right;
    return n;
}
 
Node* makeLeaf(int value) {
    Node* n = malloc(sizeof(Node));
    n->op = '\0';
    n->value = value;
    n->left = n->right = NULL;
    return n;
}
 
void printTree(Node* root) {
    if (!root) return;
    if (root->op == '\0')
        printf("%d", root->value);
    else {
        printf("(");
        printTree(root->left);
        printf(" %c ", root->op);
        printTree(root->right);
        printf(")");
    }
}
 
int evaluate(Node* root) {
    if (!root) return 0;
    if (root->op == '\0') return root->value;
 
    int leftVal = evaluate(root->left);
    int rightVal = evaluate(root->right);
 
    switch (root->op) {
        case '+': return leftVal + rightVal;
        case '-': return leftVal - rightVal;
        case '*': return leftVal * rightVal;
        case '/': return rightVal ? leftVal / rightVal : 0;
        default: return 0;
    }
}
 
int main() {
    printf("Enter expressions (Ctrl+D to exit):\n");
    yyparse();
    return 0;
}