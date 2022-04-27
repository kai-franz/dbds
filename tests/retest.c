
int foo (int x, int y, int z) {
  int a;
  if (x == 0) {
    a = y + z;
  } else {
    a = y - z;
  }
  int b = y + z;
  return b;
}