#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10000

typedef struct Node {
    int data;
    int count;
} Node;

typedef struct Bucket {
    Node *nodes;
    size_t size;
    size_t capacity;
} Bucket;

Bucket *createBucket() {
    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
    bucket->size = 0;
    bucket->capacity = 10;
    bucket->nodes = (Node *)malloc(bucket->capacity * sizeof(Node));
    return bucket;
}

unsigned int hash(int data) {
    return (unsigned int)data % TABLE_SIZE;
}

Node *findNode(Bucket *bucket, int data) {
    for (size_t i = 0; i < bucket->size; i++) {
        if (bucket->nodes[i].data == data) {
            return &(bucket->nodes[i]);
        }
    }
    return NULL;
}

void insertOrUpdateNode(Bucket *hashmap[], int data) {
    unsigned int index = hash(data);
    Bucket *bucket = hashmap[index];
    Node *node = findNode(bucket, data);

    if (node == NULL) {
        if (bucket->size == bucket->capacity) {
            bucket->capacity *= 2;
            bucket->nodes = (Node *)realloc(bucket->nodes, bucket->capacity * sizeof(Node));
        }
        node = &(bucket->nodes[bucket->size++]);
        node->data = data;
        node->count = 1;
    } else {
        node->count++;
    }
}

void freeHashMap(Bucket *hashmap[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        free(hashmap[i]->nodes);
        free(hashmap[i]);
    }
}

void writeOutput(Bucket *hashmap[], FILE *output) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Bucket *bucket = hashmap[i];

        for (size_t j = 0; j < bucket->size; j++) {
            if (bucket->nodes[j].count % 2 == 1) {
                fprintf(output, "%d\n", bucket->nodes[j].data);
            }
        }
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

    Bucket *hashmap[TABLE_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashmap[i] = createBucket();
    }

    int data;

    while (fscanf(input1, "%d", &data) != EOF) {
        insertOrUpdateNode(hashmap, data);
    }

    while (fscanf(input2, "%d", &data) != EOF) {
        insertOrUpdateNode(hashmap, data);
    }

    writeOutput(hashmap, output);
    fclose(input1);
    fclose(input2);
    fclose(output);

    freeHashMap(hashmap);

    return 0;
}