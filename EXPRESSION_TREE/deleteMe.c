#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef enum { NUM, ADD, SUB, MUL, DIV, GT, LT, EQ, AND, OR } NodeType;

typedef struct Node {
    NodeType type;
    double value;
    struct Node *left, *right;
} Node;

Node* make_node(NodeType type, double value, Node* l, Node* r) {
    Node* n = malloc(sizeof(Node));
    n->type = type;
    n->value = value;
    n->left = l;
    n->right = r;
    return n;
}

double eval(Node* n) {
    if (!n) return 0;
    switch (n->type) {
        case NUM: return n->value;
        case ADD: return eval(n->left) + eval(n->right);
        case SUB: return eval(n->left) - eval(n->right);
        case MUL: return eval(n->left) * eval(n->right);
        case DIV: return eval(n->left) / eval(n->right);
        case GT:  return eval(n->left) > eval(n->right);
        case LT:  return eval(n->left) < eval(n->right);
        case EQ:  return eval(n->left) == eval(n->right);
        case AND: return eval(n->left) && eval(n->right);
        case OR:  return eval(n->left) || eval(n->right);
    }
    return 0;
}

int main() {
    // Example: (5 + 3) * 2 > 10 AND 1
    Node *expr =
        make_node(AND, 0,
            make_node(GT, 0,
                make_node(MUL, 0,
                    make_node(ADD, 0,
                        make_node(NUM, 5, NULL, NULL),
                        make_node(NUM, 3, NULL, NULL)
                    ),
                    make_node(NUM, 2, NULL, NULL)
                ),
                make_node(NUM, 10, NULL, NULL)
            ),
            make_node(NUM, 1, NULL, NULL)
        );

    printf("Result = %.0f\n", eval(expr));
    return 0;
}