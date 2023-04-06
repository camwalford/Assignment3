#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define TABLE_SIZE 1000003
#define INITIAL_BUCKET_CAPACITY 1000
#define NUM_THREADS 2

typedef struct Node {
    int data;
    int count;
} Node;

typedef struct Bucket {
    Node *nodes;
    size_t size;
    size_t capacity;
    pthread_mutex_t lock;
} Bucket;

typedef struct ThreadArgs {
    Bucket **hashmap;
    const char *filename;
} ThreadArgs;

Bucket *createBucket() {
    Bucket *bucket = (Bucket *)malloc(sizeof(Bucket));
    bucket->size = 0;
    bucket->capacity = INITIAL_BUCKET_CAPACITY;
    bucket->nodes = (Node *)malloc(bucket->capacity * sizeof(Node));
    pthread_mutex_init(&bucket->lock, NULL);
    return bucket;
}

unsigned int hash(int data) {
    return (unsigned int)data % TABLE_SIZE;
}

void insertOrUpdateNode(Bucket *hashmap[], int data) {
    unsigned int index = hash(data);
    Bucket *bucket = hashmap[index];
    pthread_mutex_lock(&bucket->lock);

    size_t left = 0, right = bucket->size, mid;

    while (left < right) {
        mid = left + (right - left) / 2;

        if (bucket->nodes[mid].data < data) {
            left = mid + 1;
        } else if (bucket->nodes[mid].data > data) {
            right = mid;
        } else {
            bucket->nodes[mid].count++;
            pthread_mutex_unlock(&bucket->lock);
            return;
        }
    }

    if (bucket->size == bucket->capacity) {
        bucket->capacity *= 2;
        bucket->nodes = (Node *)realloc(bucket->nodes, bucket->capacity * sizeof(Node));
    }

    if (left < bucket->size) {
        memmove(&bucket->nodes[left + 1], &bucket->nodes[left], (bucket->size - left) * sizeof(Node));
    }

    bucket->nodes[left].data = data;
    bucket->nodes[left].count = 1;
    bucket->size++;

    pthread_mutex_unlock(&bucket->lock);
}

void freeHashMap(Bucket *hashmap[]) {
    for (int i = 0; i < TABLE_SIZE; i++) {
        pthread_mutex_destroy(&hashmap[i]->lock);
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

void *processFile(void *args) {
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    Bucket **hashmap = threadArgs->hashmap;
    const char *filename = threadArgs->filename;

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file for reading");
        return NULL;
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Error getting file size");
        close(fd);
        return NULL;
    }

    size_t size = sb.st_size;
    char *data = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (data == MAP_FAILED) {
        perror("Error mapping file");
        close(fd);
        return NULL;
    }

    close(fd);

    int start = 0, end = 0;
    int value;

    while (end < size) {
        if (data[end] == '\n' || end == size - 1) {
            value = atoi(&data[start]);
            insertOrUpdateNode(hashmap, value);
            start = end + 1;
        }
        end++;
    }

    if (munmap(data, size) == -1) {
        perror("Error un-mapping file");
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("COMP2510ERROR: Usage: %s <input file1> <input file2> <output file>\n", argv[0]);
        return 1;
    }

    Bucket *hashmap[TABLE_SIZE];

    for (int i = 0; i < TABLE_SIZE; i++) {
        hashmap[i] = createBucket();
    }

    pthread_t threads[NUM_THREADS];
    ThreadArgs threadArgs[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        threadArgs[i].hashmap = hashmap;
        threadArgs[i].filename = argv[i + 1]; // Pass the respective input file to each thread
        pthread_create(&threads[i], NULL, processFile, &threadArgs[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    FILE *output = fopen(argv[3], "w");

    if (output == NULL) {
        printf("COMP2510ERROR: Problem opening output file\n");
        return 1;
    }

    writeOutput(hashmap, output);
    fclose(output);
    freeHashMap(hashmap);

    return 0;
}