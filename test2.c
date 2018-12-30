#include <stdio.h>
#include <stdlib.h>

typedef short (*filter_type)(void *item, size_t i);
typedef void (*map_type)(void *item, void *new_item, size_t i);
typedef void (*reduce_type)(void *acc, void *item, size_t i);

// Generic array functions

// Returns length of filtered array
size_t filter(void *arr, size_t arr_len, size_t arr_el_size, void *new_arr, filter_type fun) {
  size_t new_len = 0;
  for (size_t i = 0; i < arr_len; i++) {
    if ( fun( (arr + i * arr_el_size), i ) ) {
      for (size_t j = 0; j < arr_el_size; j++ ) {
        *(char *)(new_arr + new_len * arr_el_size + j) = *(char *)(arr + i * arr_el_size + j);
      }
      new_len++;
    }
  }
  return new_len;
}

void map(void *arr, size_t arr_len, size_t arr_el_size, void *new_arr, map_type fun) {
  for (size_t i = 0; i < arr_len; i++) {
    fun(arr + i * arr_el_size, new_arr + i * arr_el_size, i);
  }
}

void reduce(void *arr, size_t arr_len, size_t arr_el_size, void *acc, reduce_type fun) {
  for (size_t i = 0; i < arr_len; i++) {
    fun(acc, arr + i * arr_el_size, i);
  }
}

// Custom functions

short every_second(void *item_p, size_t i) {
  return !(i % 2);
}

void mul_2(void *item, void *new_item, size_t i) {
  *(long*)new_item = (*(long*)item * 2);
}

void sum(void *acc, void *item, size_t i) {
 *(long*)acc += *(long*)item;
}

void printer(void *item, void *new_item, size_t i) {
  printf("array[%li] = %li\n", i, *(long*)item);
}

void indexer(void *item, void *new_item, size_t i) {
  *(long*)new_item = i;
}

// Main

int main(int argc, char *argv[]) {

  //long length = 1024 * 1024 * 128;
  long length = 10;

  printf("\nOriginal array ----------------------------------------\n");
  printf("Memory allocated: %lu bytes\n", sizeof(long) * length);
  long *array1 = malloc(sizeof(long) * length);
  if (array1 == NULL) {
    printf("\nNot enough memory\n");
    return 1;
  }

  map(array1, length, sizeof(long), array1, indexer);
  map(array1, length, sizeof(long), array1, printer);

  printf("\nFiltered array ----------------------------------------\n");
  long *array2 = malloc(sizeof(long) * length);
  size_t array2_length = filter(array1, length, sizeof(long), array2, every_second);
  map(array2, array2_length, sizeof(long), array2, printer);

  printf("\nMapped array ------------------------------------------\n");
  long *array3 = malloc(sizeof(long) * length);
  map(array1, length, sizeof(long), array3, mul_2);
  map(array3, length, sizeof(long), array3, printer);

  printf("\nReduced array -----------------------------------------\n");
  long total = 0;
  reduce(array1, length, sizeof(long), &total, sum);
  printf("Sum = %li\n", total);

  free(array1);
  free(array2);
  free(array3);
}
