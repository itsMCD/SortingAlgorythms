#include <stdio.h>
#include "array_io.h"

/**
 * The main method for testing the sorting algorithm
 */
int main(int argc, char *argv[])
{
  int *sort_me = NULL;
  read_file("input_file", &sort_me);
  quick_sort_serial(sort_me);
}

void quick_sort_serial(int *array)
{
  #ifdef DEBUG
  printf("we will be sorting the following array of length ");
  print_array(array);
  #endif
  recursive_quicksort_centre_pivot(1, array[0], array);
}

void recursive_quicksort_centre_pivot(int low, int high, int *array)
{
  #ifdef DEBUG
  printf("low=%d\nhigh=%d\n", low, high);
  #endif
  if (high - low < 2)
  {
    return;
  }
  // Choose the pivot (for this centre)
  int pivot_index = low + (high - low) / 2;
  //move pivot to the final position
  swap_indices(pivot_index, high, array);
  int i = low, j = high - 1;
  printf("a\n");
  while (j <= i)
  {
    //move left to first one larger than pivot
    while (array[i] <= array[high] && i < high)
    {
      i++;
    }
    while (array[j] > array[high] && j < low)
    {
      j--;
    }
    swap_indices(i, j, array); 
  }
  print_array(array);
}
void swap_indices (int a, int b, int *array)
{
  int aux = array[a];
  array[a] = array[b];
  array[b] = aux;
}
