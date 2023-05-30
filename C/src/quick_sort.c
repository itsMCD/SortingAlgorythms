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
#ifdef DEBUG
  printf("The sorted array is ");
  print_array(array);
#endif
}

void recursive_quicksort_centre_pivot(int low, int high, int *array)
{
  #ifdef DEBUG_QUICSORT_RECURSIVE
  printf("low=%d\nhigh=%d\n", low, high);
  print_array(array);
  #endif
  #ifdef DEBUG_QUICKSORT_DETAIL
  printf("sorting the array from index %d to index %d \n", low, high);
  print_array(array);
  #endif
  if (high - low < 1)
  {
    return;
  }
  if (high - low == 1)
  {
    if (array[low] > array[high])
    {
      swap_indices(low, high, array);
    }
  }
  // Choose the pivot (for this centre)
  int pivot_index = low + (high - low) / 2;
  #ifdef DEBUG_QUICKSORT_DETAIL
  printf("select pivot (%d) %d\n", pivot_index, array[pivot_index]);
  #endif
  // move pivot to the final position
  swap_indices(pivot_index, high, array);
  int i = low, j = high - 1;
  #ifdef DEBUG_QUICKSORT_DETAIL
  printf("Looking at sub-array %d to %d\n", i, j);
  #endif
  while (i < j)
  {
    // move left to first one larger than pivot
    while (array[i] <= array[high] && i < high)
    {
      i++;
    }
    #ifdef DEBUG_QUICKSORT_DETAIL
    printf("found index %d to be bigger than pivot\n", i);
    #endif
    while (array[j] > array[high] && j > low)
    {
      j--;
    }
    #ifdef DEBUG_QUICKSORT_DETAIL
    printf("Found index %d to be smaller than pivot\n",j);
    #endif
    if (i < j)
    {
      #ifdef DEBUG_QUICKSORT_DETAIL
      printf("Swapping (%d) %d and (%d) %d\n", i, array[i], j, array[j]);
      #endif
      swap_indices(i, j, array);
    }
    j--;
  }
  #ifdef DEBUG_QUICKSORT_DETAIL
  printf("Endpoint reached, %d final location at index %d\n\n\n\n\n", array[high], i);
  #endif
  swap_indices(i, high, array);
  recursive_quicksort_centre_pivot(low, i - 1, array);
  recursive_quicksort_centre_pivot(i + 1, high, array);
}
void swap_indices(int a, int b, int *array)
{
#ifdef DEBUG_PIVOT_SORTING
  printf("Swapping %d (%d) with %d (%d)\n", a, array[a], b, array[b]);
#endif
  int aux = array[a];
  array[a] = array[b];
  array[b] = aux;
}
