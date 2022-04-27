
int foo(int x, int y, int z) {
  x = z;
  while (1) {
    int a = x + 3;
    if (x % 2 == 0) {
      x = 7;
    } else {
      x = z + a;
    }
  }
  return x;
}