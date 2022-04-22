

int foo(int x, int y, int z) {
  int a;
  if (x > 0) {
    a = y;
  } else {
    a = 0;
  }
  int b = a + 2;
  int c;
  if (y > 0) {
    c = b;
  } else {
    c = 0;
  }
  return b + c;
}