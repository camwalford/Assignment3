#include <stdio.h>
#include <stdlib.h>
#include "uthash.h"

typedef struct Node {
    int data;
    int count;
    UT_hash_handle hh;
} Node;

typedef struct SortedListNode {
    int data;
    struct SortedListNode *next;
} SortedListNode;

Node *findNode(Node *hashmap, int data) {
    Node *node;
    HASH_FIND_INT(hashmap, &data, node);
    return node;
}

SortedListNode *insertSorted(SortedListNode *head, int data) {
    SortedListNode *newNode = (SortedListNode *)malloc(sizeof(SortedListNode));
    newNode->data = data;
    newNode->next = NULL;

    if (head == NULL) {
        return newNode;
    }

    if (data < head->data) {
        newNode->next = head;
        return newNode;
    }

    SortedListNode *current = head;
    while (current->next != NULL && current->next->data < data) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
    return head;
}
void freeSortedList(SortedListNode *head) {
    SortedListNode *current = head;
    SortedListNode *temp;

    while (current != NULL) {
        temp = current->next;
        free(current);
        current = temp;
    }
}
void insertOrUpdateNode(Node **hashmap, int data) {
    Node *node = findNode(*hashmap, data);

    if (node == NULL) {
        node = (Node *)malloc(sizeof(Node));
        node->data = data;
        node->count = 1;
        HASH_ADD_INT(*hashmap, data, node);
    } else {
        node->count++;
    }
}

void freeHashMap(Node **hashmap) {
    Node *current, *tmp;
    HASH_ITER(hh, *hashmap, current, tmp) {
        HASH_DEL(*hashmap, current);
        free(current);
    }
}

void writeOutput(Node *hashmap, FILE *output) {
    Node *node, *tmp;
    HASH_ITER(hh, hashmap, node, tmp) {
        if (node->count % 2 == 1) {
            fprintf(output, "%d\n", node->data);
        }
    }
}

SortedListNode *collectOddCountElements(Node *hashmap) {
    Node *node, *tmp;
    SortedListNode *sortedList = NULL;

    HASH_ITER(hh, hashmap, node, tmp) {
        if (node->count % 2 == 1) {
            sortedList = insertSorted(sortedList, node->data);
        }
    }
    
    return sortedList;
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

    Node *hashmap = NULL;
    int data;

    while (fscanf(input1, "%d", &data) != EOF) {
        insertOrUpdateNode(&hashmap, data);
    }

    while (fscanf(input2, "%d", &data) != EOF) {
        insertOrUpdateNode(&hashmap, data);
    }

    SortedListNode *sortedList = collectOddCountElements(hashmap);

    SortedListNode *current = sortedList;
    while (current != NULL) {
        fprintf(output, "%d\n", current->data);
        current = current->next;
    }

    fclose(input1);
    fclose(input2);
    fclose(output);

    freeSortedList(sortedList);
    freeHashMap(&hashmap);

    return 0;
}