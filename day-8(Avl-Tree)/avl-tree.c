#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;


int max(int value1, int value2) {
    return (value1 > value2) ? value1 : value2;
}

int height(Node *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

//createNode
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

// Right rotate
Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x; // New root
}

// Left rotate
Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y; // New root
}

// Get balance factor of node
int getBalanceFactor(Node* node) {
    if (node == NULL)
        return 0;
    return height(node->left) - height(node->right);
}

// Insert into AVL tree
Node* insert(Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node; // Duplicate keys not allowed

    // Update height
    node->height = 1 + max(height(node->left), height(node->right));

    // Get balance factor
    int balance = getBalanceFactor(node);

    // Perform rotations

    // Left Left Case
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node; // Return unchanged node pointer
}

// Pretty print tree (rotated 90 degrees)
void displayTree(Node* root, int space) {
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 5;

    // Display right subtree first
    displayTree(root->right, space);

    // Display current node after space
    printf("\n");
    for (int i = 5; i < space; i++)
        printf(" ");
    printf("%d\n", root->key);

    // Display left subtree
    displayTree(root->left, space);
}

Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;

    if (key < root->key)
        return search(root->left, key);
    else
        return search(root->right, key);
}


// Get node with minimum value (used in deletion)
Node* minimumValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete node from AVL tree
Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    // Step 1: Perform standard BST delete
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        // Node with one child or no child
        if ((root->left == NULL) || (root->right == NULL)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) { // No child
                temp = root;
                root = NULL;
            } else { // One child
                *root = *temp;
            }
            free(temp);
        } else {
            // Node with two children: get inorder successor (smallest in right subtree)
            Node* temp = minimumValueNode(root->right);

            // Copy inorder successor's key to this node
            root->key = temp->key;

            // Delete inorder successor
            root->right = deleteNode(root->right, temp->key);
        }
    }

    // If tree had only one node
    if (root == NULL)
        return root;

    // Step 2: Update height
    root->height = 1 + max(height(root->left), height(root->right));

    // Step 3: Rebalance
    int balance = getBalanceFactor(root);

    // Left Left Case
    if (balance > 1 && getBalanceFactor(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case
    if (balance > 1 && getBalanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case
    if (balance < -1 && getBalanceFactor(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case
    if (balance < -1 && getBalanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}


int main() {
    Node* root = NULL;
    int n, value;

    printf("Enter number of inputs: ");
    scanf("%d", &n);

    printf("Enter %d values:\n", n);
    for (int i = 0; i < n; i++) {
        scanf("%d", &value);
        root = insert(root, value);
    }

    printf("\nAVL Tree structure:\n");
    displayTree(root, 0);

    // Search
    int keyToBeSearched;
    printf("\nEnter key to search: ");
    scanf("%d", &keyToBeSearched);
    Node* found = search(root, keyToBeSearched);
    if (found)
        printf("Key %d is found in AVL tree.\n", keyToBeSearched);
    else
        printf("Key %d is not found.\n", keyToBeSearched);

    // Delete
    int keyToBeDeleted;
    printf("\nEnter key to delete: ");
    scanf("%d", &keyToBeDeleted);
    root = deleteNode(root, keyToBeDeleted);

    printf("\nAVL Tree after deletion:\n");
    displayTree(root, 0);

    return 0;
}
