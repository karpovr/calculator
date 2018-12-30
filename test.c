#include <stdio.h>
#include <stdlib.h>

typedef short (*filter_type)(void *item, size_t i);

typedef void (*map_type)(void *item, void *new_item, size_t i);

typedef void (*reduce_type)(void *acc, void *item, size_t i);

size_t filter(void *arr, size_t arr_len, size_t arr_el_size, void *new_arr, filter_type fun) {
  size_t new_len = 0;
  for (size_t i = 0; i < arr_len; i++) {
    if ( fun(arr + i * arr_el_size, i) ) {
      *(char*)(new_arr + new_len++ * arr_el_size) = *(char *)(arr + i * arr_el_size);
    }
  }
  return new_len;
}

void map(void *arr, size_t arr_len, size_t arr_el_size, void *new_arr, map_type fun) {
  for (size_t i = 0; i < arr_len; i++) {
    new_arr[i] = fun(&arr[i], &new_arr[i], i);
  }
}

void reduce(void *arr, size_t arr_len, size_t arr_el_size, void *acc, reduce_type fun) {
  for (size_t i = 0; i < arr_len; i++) {
    fun(acc, &arr[i], i);
  }
}

short is_even(void *item, size_t i) {
  return !(*(long*)item % 2);
}

void add_1(void *item, void *new_item, size_t i) {
  *(long *)new_item = (*(long *)item + 1);
}

void mul_2(void *item, void *new_item, size_t i) {
  *(long *)new_item = (*(long *)item * 2);
}

void sum(void *acc, void *item, size_t i) {
 *(long *)acc += *(long *)item;
}

void print_array(char *name, long *array, long length) {
  printf("%s = [", name);
  for (long i = 0; i < length; i++) {
    if (i == length - 1) printf("%li", array[i]);
    else printf("%li, ", array[i]);
  }
  printf("]\n");
}

int main(int argc, char *argv[]) {

  long length = 10;

  long *array0 = malloc(sizeof(long) * length);
  long *array1 = malloc(sizeof(long) * length);
  long *array2 = malloc(sizeof(long) * length);
  long *array3 = malloc(sizeof(long) * length);
  for (long i = 0; i < length; i++) {
    array0[i] = i;
  }
  print_array("array0", array0, length);

  map(array0, length, sizeof(long), array1, add1);
  print_array("array1", array1, length);

  map(array0, length, sizeof(long), mul2);
  print_array("array2", array2, length);

  long array3_length = filter(array0, length, sizeof(long), array3, is_even);
  print_array("array3", array3, array3_length);

  long acc = 0;
  reduce(length, array0, sum, &acc);
  printf("sum = %li\n", acc);

  free(array0);
  free(array1);
  free(array2);
  free(array3);
}
