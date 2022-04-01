// tests the traversal order of our pass

int f(int x, int y, int z) {
  if (x < 0) {

  } else {
    if (y < 0) {
      y = 10;
    } else {
      if (z < 0) {
        x += 10;
      } else {
        return x + y + z;
      }
    }
  }
  int a = x - y + z;
  if (a < 0) {
    a = 10;
  } else {
    if (a > 0) {
      a = 20;
    } else {
      a = 30;
    }
  }
  return a - x;
}
