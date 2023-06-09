#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "array_io.h"

#define MIN_SAMPLE_SIZE 16384
#define MAX_SAMPLE_SIZE 8388608
#define STEP_SIZE 2
#define RUNS_PER_POINT 10
void quick_sort_serial_centre_pivot(int *array);
void recursive_quicksort_centre_pivot(int low, int high, int *array);
void quick_sort_serial_left_pivot(int *array);
void recursive_quicksort_left_pivot(int low, int high, int *array);
void quick_sort_serial_right_pivot(int *array);
void recursive_quicksort_right_pivot(int low, int high, int *array);
void swap_indices(int a, int b, int *array);
void recursive_quick_sort_concurrent(int low, int high, int *array);
void quick_sort_concurrent(int *array);
/**
 * The main method for testing the sorting algorithm
 */
int rank, size;
int main(int argc, char *argv[])
{
  int *array = NULL;
  create_random_array(16, &array);
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  
  quick_sort_concurrent(array);

  MPI_Finalize();
}

void quick_sort_concurrent(int *array)
{
  
  int *local_array_pointer = array;
  srand(time(NULL));
  //send pointer of array to the other processors
  //we want all processes to recieve this array
  MPI_Bcast(&local_array_pointer, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0)
  {
    recursive_quick_sort_concurrent(1, array[0], local_array_pointer);
  }
  while (1)
  {
    int *bounds = malloc(2*sizeof(int));
    MPI_Recv(bounds, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    recursive_quick_sort_concurrent(bounds[0], bounds[1], local_array_pointer);
  }
}

void recursive_quick_sort_concurrent(int low, int high, int *array)
{
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
  // move pivot to the final position
  swap_indices(pivot_index, high, array);
  int i = low, j = high - 1;
  while (i < j)
  {
    // move left to first one larger than pivot
    while (array[i] <= array[high] && i < high)
    {
      i++;
    }
    while (array[j] > array[high] && j > low)
    {
      j--;
    }
    if (i < j)
    {
      swap_indices(i, j, array);
    }
    j--;
  }
  swap_indices(i, high, array);
  int *high_arr = malloc(2 * sizeof(int));
  int *low_arr = malloc(2 * sizeof(int));
  MPI_Request req1;
  MPI_Request req2;
  high_arr[0] = i + 1;
  high_arr[1] = high;
  low_arr[0] = low;
  low_arr[1] = i-1;
  MPI_Isend(low_arr, 2, MPI_INT, rand()%size, 0, MPI_COMM_WORLD,&req1);
  MPI_Isend(high_arr, 2, MPI_INT, rand()%size, 0, MPI_COMM_WORLD,&req2);
}

void measure_serial() {
int *sort_me_centre = NULL;
  int *sort_me_left = NULL;
  int *sort_me_right = NULL;
  clock_t start , diff_centre = 0, diff_left = 0, diff_right = 0;
  printf("n;time_left;time_centre;time_right\n");
  for (int i = MIN_SAMPLE_SIZE; i < MAX_SAMPLE_SIZE; i = i*STEP_SIZE)
  {
    for (int j = i; j < i*STEP_SIZE; j += (i*STEP_SIZE - i)/RUNS_PER_POINT)
    {
    create_random_array(j, &sort_me_centre);
    copy_array(sort_me_centre, &sort_me_left);
    copy_array(sort_me_centre, &sort_me_right);
    start = clock();
    quick_sort_serial_centre_pivot(sort_me_centre);
    diff_centre = clock() - start;

    start = clock();
    quick_sort_serial_left_pivot(sort_me_left);
    diff_left = clock() - start;

    start = clock();
    quick_sort_serial_right_pivot(sort_me_right);
    diff_right = clock() - start;

    printf("%d;%ld;%ld;%ld\n", j, diff_left, diff_centre, diff_right);
    free(sort_me_centre);
    free(sort_me_left);
    free(sort_me_right);
    }
  }
}

void quick_sort_serial_centre_pivot(int *array)
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

void quick_sort_serial_left_pivot(int *array)
{
#ifdef DEBUG
  printf("we will be sorting the following array of length ");
  print_array(array);
#endif
  recursive_quicksort_left_pivot(1, array[0], array);
#ifdef DEBUG
  printf("The sorted array is ");
  print_array(array);
#endif
}

void recursive_quicksort_left_pivot(int low, int high, int *array)
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
  int pivot_index = low;
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

void quick_sort_serial_right_pivot(int *array)
{
#ifdef DEBUG
  printf("we will be sorting the following array of length ");
  print_array(array);
#endif
  recursive_quicksort_right_pivot(1, array[0], array);
#ifdef DEBUG
  printf("The sorted array is ");
  print_array(array);
#endif
}

void recursive_quicksort_right_pivot(int low, int high, int *array)
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
  int pivot_index = high;
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
