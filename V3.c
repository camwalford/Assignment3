#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int count;
    struct Node *left, *right;
} Node;

Node *createNode(int data) {
    Node *newNode = (Node *)calloc(1, sizeof(Node));
    newNode->data = data;
    newNode->count = 1;
    return newNode;
}

Node* maxValueNode(Node* node) {
    Node* current = node;

    while (current->right != NULL) {
        current = current->right;
    }
    return current;
}

Node* insertOrDelete(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insertOrDelete(root->left, data);
    } else if (data > root->data) {
        root->right = insertOrDelete(root->right, data);
    } else {
        root->count++;

        if (root->count % 2 == 0) {
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
                root->count = temp->count;
                root->left = insertOrDelete(root->left, temp->data);
            }
        }
    }
    return root;
}

void preOrderTraversalAndFree(Node *root, FILE *output) {
    if (root != NULL) {
        fprintf(output, "%d\n", root->data);
        preOrderTraversalAndFree(root->left, output);
        preOrderTraversalAndFree(root->right, output);
        free(root);
    }
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

    preOrderTraversalAndFree(root, output);

    fclose(input1);
    fclose(input2);
    fclose(output);

    return 0;
}