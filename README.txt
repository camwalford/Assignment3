The C assignment wants us to make a binary tree using two input files of integers of up to 10,000,000,000.
The first input creates the tree with no deletions. 
The second input file will cause a deletion if that data already exists in the tree, otherwise a node will be added. 
There can be duplicate values in the first input, but those values won't be in the second input
The tree must use the in-order predecessor (largest in the left subtree) to replace nodes that are deleted.
The result of a pre-order traversal of the tree will be printed to an output file.
The data structure doesn't have to be a tree the output just has to be the same as if it were one.
The values in the input and output files won't be pre-sorted.
The run command will be 
>> ./<name of executable> <input file1> <input file 2> <outputfile>
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



There are several methods that can be used to optimize the code for speed without changing the output. Here are some suggestions:

Use iterative methods instead of recursive ones:
Recursion can be slow and consume more memory due to the function call stack. You can replace the recursive functions with iterative ones using loops and data structures such as a stack.

Improve memory allocation:
The current code uses calloc to allocate memory for each new node. This can be optimized by using a memory pool or a custom allocator. A memory pool allocates a large chunk of memory at once, and then you can use it to store new nodes as they are created. This can reduce the overhead of memory allocation calls and deallocations.

Optimize the tree structure:
The tree structure in the code is an unbalanced binary search tree. You can use a self-balancing binary search tree, such as an AVL tree or a Red-Black tree, to keep the tree balanced. This will improve the time complexity of insertions and deletions from O(n) to O(log n), where n is the number of nodes.

Read input files and process them simultaneously:
Instead of reading input files sequentially, read and process them simultaneously. This can be done using multi-threading, where one thread reads and processes the first file while another reads and processes the second file. This can speed up the overall processing time.

Use compiler optimizations:
Use compiler flags to optimize the code. For example, in GCC, you can use the -O3 flag to enable the highest level of optimization.

Minimize the use of standard I/O functions:
Standard I/O functions like fscanf and fprintf can be slow. Instead, use lower-level I/O functions like read and write, or read the entire file into a buffer and process it in memory.

Please note that each optimization should be tested for correctness and performance improvements, as different optimizations may have different effects on different systems or inputs.