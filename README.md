# ParallelMergeSort
Sequential Merge Sort:
The baseline merge sort algorithm is implemented recursively. The mergesort function splits the array and uses the merge function to combine sorted subarrays.

Parallel Merge Sort:
For large subarrays (size exceeds a defined threshold (1000 in this case), the algorithm spawns two threads to sort each half concurrently. When the array segment is small, sequential merge sort is used to avoid thread creation overhead.

Merging and Thread Management:
After both threads complete their work, the merge function combines the two sorted halves into one sorted array. Threads are properly joined using join() to ensure synchronized execution.


# Compilation
To compile the project, use the provided Makefile. Run the following command in the project directory:


make
This will compile the code with C++11 support and link the pthread library.

# Running the Program
After compilation, you can run the executable mergesort. The program expects a command-line argument that specifies the number of elements to sort. For example:


./mergesort 1000000
This command sorts an array of 1,000,000 elements and prints the elapsed time along with validation of the sorted data and the time ran in sequential and parallel.
