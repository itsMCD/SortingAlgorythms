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
  int pivot_index = low + (high - low) / 2;
  int i = low;
  int j = high - 1;
  int aux = array[high];
  array[high] = array[pivot_index];
  array[pivot_index] = aux;
  while(i != j && i < j)
  {
    while (i <= j)
    {
      if (array[i] > array[high])
      {
        break;
      }
      i++;
    }
    while (j >= i)
    {
      if(array[j] < array[high])
      {
        break;
      }
      j--;
    }
    aux = array[i];
    array[i] = array[j];
    array[j] = aux; 
  
  }
  printf("%d, %d length=", i, j);
  print_array(array);
  aux = array[i];
  array[i] = array[high];
  array[high] = aux;
  


  
   

}
