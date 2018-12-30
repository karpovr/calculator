#include <stdio.h>

#define COMPARE(type) short inline compare(void *a, void *b) { return ( *(type *) a == *(type *) b); }

long search(void *array, long length, void *elem, short elem_size, short compare(void *a, void *b)) {
  for (long i = 0; i < length; i++) {
    if ( compare( array + i * elem_size, elem) ) {
      return i;
    }
  }
  return -1;
}

short compareI(void *a, void *b) {
  return ( *(int *) a == *(int *) b);
}

short compareF(void *a, void *b) {
  return ( *(float *) a == *(float *) b);
}

short compareD(void *a, void *b) {
  return ( *(double *) a == *(double *) b);
}


int main(int argc, char *argv[]) {

  int i_arr[] = {1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7};
  int i_elem = 3;
  long i_found = search(i_arr, sizeof(i_arr) / sizeof(i_arr[0]), &i_elem, sizeof(i_arr[0]), compareI);
  printf("found = %li\n", i_found);

  float f_arr[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
  float f_elem = 7.7;
  long f_found = search(f_arr, sizeof(f_arr) / sizeof(f_arr[0]), &f_elem, sizeof(f_arr[0]), compareF);
  printf("found = %li\n", f_found);

  double d_arr[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
  double d_elem = 4.4;
  long d_found = search(d_arr, sizeof(d_arr) / sizeof(d_arr[0]), &d_elem, sizeof(d_arr[0]), compareD);
  printf("found = %li\n", d_found);

  double dd_arr[] = {1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
  double dd_elem = 4.4;
  long dd_found = search(dd_arr, sizeof(dd_arr) / sizeof(dd_arr[0]), &dd_elem, sizeof(dd_arr[0]), COMPARE(double));
  printf("found = %li\n", dd_found);




  printf("sizeof(i_elem) = %li\n", sizeof(i_elem));
  printf("sizeof(f_elem) = %li\n", sizeof(f_elem));
  printf("sizeof(d_elem) = %li\n", sizeof(d_elem));


}
