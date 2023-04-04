#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;

Node *createNode(int data) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = newNode->right = NULL;
    return newNode;
}

// Function to find the largest node in the given subtree
Node* maxValueNode(Node* node) {
    Node* current = node;

    // Find the largest node in the subtree
    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

// Function to delete a node with the specified data in a binary search tree
Node* deleteNode(Node* root, int data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    } else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        } else {
            Node* temp = maxValueNode(root->left);
            root->data = temp->data;
            root->left = deleteNode(root->left, temp->data);
        }
    }
    return root;
}

Node* insertOrDelete(Node* root, int data) {
    if (root == NULL) {
        // If the root is NULL, the data is not in the tree, so insert a new node with the given data
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insertOrDelete(root->left, data);
    } else if (data > root->data) {
        root->right = insertOrDelete(root->right, data);
    } else {
        // If the node with the given data is found, delete it
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        } else {
            Node* temp = maxValueNode(root->left);
            root->data = temp->data;
            root->left = deleteNode(root->left, temp->data);
        }
    }
    return root;
}

void preOrderTraversal(Node *root, FILE *output) {
    if (root != NULL) {
        fprintf(output, "%d\n", root->data);
        preOrderTraversal(root->left, output);
        preOrderTraversal(root->right, output);
    }
}

void freeTree(Node *root) {
    if (root == NULL) return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}


int main(int argc, char *argv[]) {

    if (argc != 4) {
        printf("COMP2510ERROR: Usage: %s <input file1> <input file2> <output file>\n", argv[0]);
        return 1;
    }

    FILE *input1 = fopen(argv[1], "r");
    FILE *input2 = fopen(argv[2], "r");
    FILE *output = fopen(argv[3], "w");

    if (input1 == NULL || input2 == NULL || output == NULL) {
        printf("COMP2510ERROR: Problem opening files\n");
        return 1;
    }

    Node *root = NULL;
    int data;

    while (fscanf(input1, "%d", &data) != EOF) {
        root = insertOrDelete(root, data);
    }

    while (fscanf(input2, "%d", &data) != EOF) {
        root = insertOrDelete(root, data);
    }

    preOrderTraversal(root, output);

    // Free the memory of the tree
    freeTree(root);

    fclose(input1);
    fclose(input2);
    fclose(output);

    return 0;
}