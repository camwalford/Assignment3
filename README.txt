The C assignment wants us to make a binary tree using two input files of integers.
The first input creates the tree with no deletions. 
The second input file will cause a deletion if that data already exists in the tree, otherwise a node will be added. 
The tree must use the in-order predecessor (largest in the left subtree) to replace nodes that are deleted.
The result of a pre-order traversal of the tree will be printed to an output file.
The data structure doesn't have to be a tree the output just has to be the same as if it were one.
The values in the input and output files won't be pre-sorted.
**The goal is speed, so we can use as much memory as we see fit.

The input size will be 10,000,000,000 integers for input1 AND input2.
Processor used: Intel® Core™ i5-1145G7 Processor, 8M Cache, up to 4.40 GHz, with IPU
Physical memory: 16GB
It will be run on WSL, OS: Linux CAS-Y62767 5.15.90.1-microsoft-standard-WSL2 #1 SMP Fri Jan 27 02:56:13 UTC 2023 x86_64 x86_64 x86_64 GNU/Linux
Compiler: gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0

Below are the specifications of the lab.
1. The run command will be
>> ./<name of executable> <input file1> <input file 2> <output
file>
2. There will be duplicates.
3. Values in both input files are not sorted.
4. There will be corner case testing in this assignment.
5. No limit on the number of lines in input files
6. Guaranteed to have no empty lines, no integer outside integer range and no data other than
int type in the input file
7. No extra new line


-- HOW TO USE THE SCRIPT.BASH -- 
*only tested on WSL so far*

./script.bash           Will compile and run every V[X].c file for every input.
./script.bash V[X]      Will compile and run V[X].c for every input.

This script compiles and runs all the C files with all the inputs, comparing them with the expected outputs
and printing the time each takes to a times.txt file. It also indicates if the output files are the same or different than expected.

Name C files:               V[X].c where X is a number
Name input files:           input[A-Z]1.txt and input[A-Z]2.txt
Name expected output files: expected_output[A-Z].txt

V1.c can be used create the expected output for the input files. I have used it to create the current ones and believe it is correct.

