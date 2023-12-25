#include "polygon.h"

#include <iostream>
#include <vector>

int main() {
  // Example usage of the Polygon class
  std::vector<Point> p1 = {{1.0, 2.0}, {4.0, 5.0}, {2.0, 3.0}, {5.0, 6.0}};

  Polygon poly1(p1);
  Polygon poly2(p1);

  if (poly1 == poly2) {
    std::cout << "Polygons are equal." << std::endl;
  } else {
    std::cout << "Polygons are not equal." << std::endl;
  }

  return 0;
}
