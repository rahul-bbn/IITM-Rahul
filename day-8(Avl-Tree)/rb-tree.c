#include <stdio.h>
#include <stdlib.h>

typedef enum {
    RED = 0,
    BLACK = 1
} Color;

typedef struct RBNode {
    int key;
    Color color;
    struct RBNode *left;
    struct RBNode *right;
    struct RBNode *parent;
} RBNode;

typedef struct {
    RBNode *root;
} RBTree;

RBNode *NIL;

RBNode* newNode(int key) {
    RBNode *node = (RBNode*)malloc(sizeof(RBNode));
    node->key = key;
    node->color = RED;
    node->left = NIL;
    node->right = NIL;
    node->parent = NIL;
    return node;
}


//init tree
void init(RBTree *tree) {
    NIL = (RBNode*)malloc(sizeof(RBNode));
    NIL->color = BLACK;
    NIL->left = NIL->right = NIL->parent = NIL;
    NIL->key = 0; // unused
    tree->root = NIL;
}

// Left rotate around x 
void leftRotate(RBTree *tree, RBNode *x) {
    RBNode *y = x->right;
    x->right = y->left;
    if (y->left != NIL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NIL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// Right rotate around y 
void rightRotate(RBTree *tree, RBNode *y) {
    RBNode *x = y->left;
    y->left = x->right;
    if (x->right != NIL) x->right->parent = y;
    x->parent = y->parent;
    if (y->parent == NIL) tree->root = x;
    else if (y == y->parent->left) y->parent->left = x;
    else y->parent->right = x;
    x->right = y;
    y->parent = x;
}

// insert fix
void insertFixup(RBTree *tree, RBNode *z) {
    while (z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            RBNode *y = z->parent->parent->right; // uncle
            if (y->color == RED) {
                // Case 1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    // Case 2
                    z = z->parent;
                    leftRotate(tree, z);
                }
                // Case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            RBNode *y = z->parent->parent->left; // uncle
            if (y->color == RED) {
                // Mirror Case 1
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    // Mirror Case 2
                    z = z->parent;
                    rightRotate(tree, z);
                }
                // Mirror Case 3
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

//insert
void insert(RBTree *tree, int key) {
    RBNode *z = newNode(key);
    RBNode *y = NIL;
    RBNode *x = tree->root;

    while (x != NIL) {
        y = x;
        if (z->key < x->key) x = x->left;
        else if (z->key > x->key) x = x->right;
        else { /* duplicate: ignore & free new node */
            free(z);
            return;
        }
    }
    z->parent = y;
    if (y == NIL) tree->root = z;
    else if (z->key < y->key) y->left = z;
    else y->right = z;

    z->left = NIL;
    z->right = NIL;
    z->color = RED;

    insertFixup(tree, z);
}

//search
RBNode* search(RBTree *tree, int key) {
    RBNode *x = tree->root;
    while (x != NIL) {
        if (key == x->key) return x;
        x = (key < x->key) ? x->left : x->right;
    }
    return NULL;
}


RBNode* treeMinimum(RBNode *x) {
    while (x->left != NIL) x = x->left;
    return x;
}

void transplant(RBTree *tree, RBNode *u, RBNode *v) {
    if (u->parent == NIL) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    v->parent = u->parent;
}

//delete fix
void deleteFixup(RBTree *tree, RBNode *x) {
    while (x != tree->root && x->color == BLACK) {
        if (x == x->parent->left) {
            RBNode *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if (w->left->color == BLACK && w->right->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right->color == BLACK) {
                    w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            RBNode *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if (w->right->color == BLACK && w->left->color == BLACK) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left->color == BLACK) {
                    w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    x->color = BLACK;
}

//delete
void delete(RBTree *tree, int key) {
    RBNode *z = tree->root;
    while (z != NIL) {
        if (key == z->key) break;
        z = (key < z->key) ? z->left : z->right;
    }
    if (z == NIL) return; /* not found, nothing to delete */

    RBNode *y = z;
    Color yOriginalColor = y->color;
    RBNode *x;

    if (z->left == NIL) {
        x = z->right;
        transplant(tree, z, z->right);
    } else if (z->right == NIL) {
        x = z->left;
        transplant(tree, z, z->left);
    } else {
        y = treeMinimum(z->right);
        yOriginalColor = y->color;
        x = y->right;
        if (y->parent == z) {
            x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    if (yOriginalColor == BLACK) {
        deleteFixup(tree, x);
    }
}

// print tree
void display(RBNode *node, int space) {
    if (node == NIL) return;

    space += 6;
    display(node->right, space);

    printf("\n");
    for (int i = 6; i < space; i++) printf(" ");
    printf("%d(%c)\n", node->key, node->color == RED ? 'R' : 'B');

    display(node->left, space);
}

void displayTree(RBTree *tree) {
    if (tree->root == NIL) {
        printf("[empty]\n");
        return;
    }
    display(tree->root, 0);
}

int main() {
    RBTree tree;
    init(&tree);

    int n, value;

    printf("Enter number of inputs: ");
    if (scanf("%d", &n) != 1 || n < 0) return 0;

    printf("Enter %d values:\n", n);
    for (int i = 0; i < n; i++) {
        if (scanf("%d", &value) != 1) return 0;
        insert(&tree, value);
    }

    printf("\nRed-Black Tree (key(color)):\n");
    displayTree(&tree);

    int keyToBeSearched;
    printf("\nEnter key to search: ");
    if (scanf("%d", &keyToBeSearched) == 1) {
        RBNode *found = search(&tree, keyToBeSearched);
        if (found) printf("Key %d is found, and its (%c) in color.\n", keyToBeSearched, found->color == RED ? 'Red' : 'Black');
        else printf("Key %d is not found.\n", keyToBeSearched);
    }

    int keyToBeDeleted;
    printf("\nEnter key to delete: ");
    if (scanf("%d", &keyToBeDeleted) == 1) {
        delete(&tree, keyToBeDeleted);
        printf("\nAfter deletion:\n");
        displayTree(&tree);
    }

    return 0;
}