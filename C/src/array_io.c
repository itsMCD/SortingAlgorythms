#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "array_io.h"
/**
 * Reads a file where the first entry is the size of the file and the rest is all integers that are read into an array
 * @param filename: The name of the file to be read
 * @param array: the array that will be populated, You are now responsible for freeing this array!
 *
 */
void read_file(char *filename, int **array)
{
  FILE *fptr;
  int size = 0;
  fptr = fopen(filename, "r");
  if (fptr == NULL)
  {
    printf("Error opening file: %s\n", filename);
    exit(1);
  }
  fscanf(fptr, "%d", &size);

  *array = malloc((size + 1) * sizeof(int));
  if (*array == NULL)
  {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  (*array)[0] = size;
  for (int i = 1; i <= size; i++)
  {
    fscanf(fptr, "%d", &(*array)[i]);
  }

  fclose(fptr);
}

/**
 * Print out an array of integers as created in this library
 * @param array: the array to be printed
 */
void print_array(int *array)
{
  int size = array[0];
  for (int i = 0; i <= size; i++)
  {
    printf("%d\t", array[i]);
    if (i % 15 == 0)
    {
      printf("\n");
    }
  }
  printf("\n");
}

/**
 * Creates a random number array of certain length
 * @param size: the size of the array
 * @param array: the pointer to the array that will be generated, You a responsible for freeing
*/
void create_random_array(int size, int **array)
{
  srand(time(NULL));
  *array = malloc((size + 1) * sizeof(int));
  if (*array == NULL)
  {
    printf("Memory allocation failed.\n");
    exit(1);
  }

  (*array)[0] = size;
  for (int i = 1; i <= size; i++)
  {
    (*array)[i] = rand() % MAX_RANDOM_SIZE;
  }
}
