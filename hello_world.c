#include <stdio.h>

int main() {
  int i;
  double sum;
  for (i = 0, sum = 0; i < 100; i++) {
    sum = sum + i + 1;
  }
  sum = sum / i;
  printf("sum = %lf\n", sum);
}
