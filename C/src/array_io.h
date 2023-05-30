#ifndef ARRAY_IO_H
#define ARRAY_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_RANDOM_SIZE 1000

void read_file(char *filename, int **array);
void print_array(int *array);
void create_random_array(int size, int **array);

#endif /* ARRAY_IO_H */
