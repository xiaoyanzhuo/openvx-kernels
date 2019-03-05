#include <stdlib.h>

// compile: gcc dcache.c

#define L2_SIZE 4 * 1024 * 1024  // 4MB

int main() {
  char *c = malloc(L2_SIZE);
  for (int i = 0; i < 0xfff; ++i)
    for (int j = 0; j < L2_SIZE; ++j) 
        c[j] = i * j;
}