# makefile for thread pool
#

CC=gcc
CFLAGS=-Wall
PTHREADS=-lpthread

all: example generator

example: client.o threadpool.o Queue.o matrix_generator.o
	$(CC) $(CFLAGS) -o example client.o threadpool.o Queue.o matrix_generator.o $(PTHREADS)

generator: matrix_generator_main.o matrix_generator.o
	$(CC) $(CFLAGS) -o generator matrix_generator_main.o matrix_generator.o $(PTHREADS)

client.o: client.c matrix_generator.h
	$(CC) $(CFLAGS) -c client.c $(PTHREADS)

threadpool.o: threadpool.c threadpool.h
	$(CC) $(CFLAGS) -c threadpool.c $(PTHREADS)

Queue.o: Queue.c Queue.h
	$(CC) $(CFLAGS) -c Queue.c $(PTHREADS)

matrix_generator.o: matrix_generator.c matrix_generator.h
	$(CC) $(CFLAGS) -c matrix_generator.c $(PTHREADS)

matrix_generator_main.o: matrix_generator_main.c matrix_generator.h
	$(CC) $(CFLAGS) -c matrix_generator_main.c $(PTHREADS)

clean:
	rm -rf *.o
	rm -rf example
	rm -rf generator
	rm -rf *.txt

