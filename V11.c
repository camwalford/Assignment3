#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10007
#define INITIAL_BUCKET_CAPACITY 100

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
    bucket->capacity = INITIAL_BUCKET_CAPACITY;
    bucket->nodes = (Node *)malloc(bucket->capacity * sizeof(Node));
    return bucket;
}

unsigned int hash(int data) {
    return (unsigned int)data % TABLE_SIZE;
}

Node *findNode(Bucket *bucket, int data) {
    size_t left = 0, right = bucket->size, mid;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (bucket->nodes[mid].data < data) {
            left = mid + 1;
        } else if (bucket->nodes[mid].data > data) {
            right = mid;
        } else {
            return &(bucket->nodes[mid]);
        }
    }

    return NULL;
}

int compareNodes(const Node *a, const Node *b) {
    return a->data - b->data;
}

void insertOrUpdateNode(Bucket *hashmap[], int data) {
    unsigned int index = hash(data);
    Bucket *bucket = hashmap[index];
    Node key = { .data = data };

    size_t left = 0;
    size_t right = bucket->size;
    size_t mid;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (bucket->nodes[mid].data < data) {
            left = mid + 1;
        } else if (bucket->nodes[mid].data > data) {
            right = mid;
        } else {
            bucket->nodes[mid].count++;
            return;
        }
    }

    if (bucket->size == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->nodes = (Node *)realloc(bucket->nodes, bucket->capacity * sizeof(Node));
    }

    if (left < bucket->size) {
        memmove(&bucket->nodes[left + 1], &bucket->nodes[left],
                (bucket->size - left) * sizeof(Node));
    }

    bucket->nodes[left].data = data;
    bucket->nodes[left].count = 1;
    bucket->size++;
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