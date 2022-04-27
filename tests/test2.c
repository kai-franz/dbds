

int foo(int x, int y, int z) {
  int p = 0;
  int q;
  while (1) {
    if (p == 0) {
      q = 2;
    } else {
      q = p + 6;
    }
    p = q + 3;
  }
  return q;
}