#include <stdio.h>

int bar (int i)
{
  return i;
}

int foo (int x) {
  int i = 0;
  while (1) {
    bar(i + 0);
    i = i + 1;
  }
  return x;
}