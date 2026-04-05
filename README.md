These files support the POSIX implementation of the thread pool project.

Source files:
* client.c (client program that uses thread pool to perform matrix multiplication)
* threadpool.c (implementation of thread pool)
* threadpool.h (header file containing function prototypes)
* matrix_generator.c
* matrix_generator.h
* matrix_generator_main.c
* Queue.c
* Queue.h 
* Makefile

To run the make file, enter "make"

To generate matrices for multiplication, enter "./generator" with the following arguments:
    --m : matrix A dimenson m x n
    --n : matrix A dimenson m x n and matrix B dimension n x p
    --p : matrix B dimenson n x p
    --mode : mode selection
             test : uses original simple test method
             random : generates a dense matrix
             sparse : generates a sparse matrix
    --sparsity : level of sparsity when using sparse mode

To perform the the matrix multiplication once the files are generated, enter "./example"
