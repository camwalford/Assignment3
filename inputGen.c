#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <number of lines> <character>\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]); // Convert input argument to integer
    char* character = argv[2]; // Get the second argument (the character in the input file name [A-Z])
    char outputFilename1[20];
    char outputFilename2[20];
    sprintf(outputFilename1, "input%c1.txt", *character); // Create filename for input1
    sprintf(outputFilename2, "input%c2.txt", *character); // Create filename for input2
    FILE *outputFiles[2] = {fopen(outputFilename1, "w"), fopen(outputFilename2, "w")}; // Open the output files

    srand(time(NULL));
    for (int i = 0; i < 2; i++) {  
        srand(rand());

        if (outputFiles[i] == NULL) {
            printf("Error opening file.\n");
            return 1;
        }
        for (int j = 0; j < n; j++) {
            //generate random int in range of int
            int num = (rand() % 2147483646);
            int sign = rand() % 2;
            if (sign == 1) {
                num = num * -1;
            }
            fprintf(outputFiles[i], "%d\n", num); // Write the number to the file
            if (j % 1000000 == 0) {
                // printf("Line %d\r", j);
            }
        }

        fclose(outputFiles[i]);
    }
    return 0;
}