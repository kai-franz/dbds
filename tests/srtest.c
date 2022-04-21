// strength reduction example

int foo (int x) {
  int phi;
  if (x > 0) {
    phi = x;
  } else {
    phi = 2048;
  }
  int c = x * phi;
  return c;
}