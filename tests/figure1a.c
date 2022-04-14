// Example program from paper

int foo (int x) {
  int phi;
  if (x > 0) phi = x;
  else phi = 0;
  int a = 5 + phi;
  int b = a + 1;
  int c = b * 3;
  return c;
}