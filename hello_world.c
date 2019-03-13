#include <stdio.h>

// Функтция g(x) умножает на два
int g(int x);

int main() {
  int x = 1073741823;
  int y = g(x);
  printf("%i\n", y);
  printf("%li\n", sizeof(int));
  return 0;
}

int g(int x) {
  return 2 * x;
}
