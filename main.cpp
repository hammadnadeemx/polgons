#include "polygon.h"

#include <iostream>
#include <vector>

int main() {
  /**<  Example usage of the Polygon class */
  Polygon triangle, square, result;

  if (!square.read_file("/home/hammad/workspace/polgons/square.csv")) {
    std::cout << "Failed to read square data ! \n";
    return -1;
  }

  if (!triangle.read_file("/home/hammad/workspace/polgons/triangle.csv")) {
    std::cout << "Failed to read triangle data ! \n";
    return -2;
  }

  result = result.compute_union(triangle, square);
  std::cout << "union\n";
  std::cout << result << std::endl;

  result = result.compute_intersection(triangle, square);
  std::cout << "intersection\n";
  std::cout << result << std::endl;

  result = result.compute_subtraction(triangle, square);
  std::cout << "traingle - square\n";
  std::cout << result << std::endl;

  result = result.compute_subtraction(square, triangle);
  std::cout << "square - traingle\n";
  std::cout << result << std::endl;

  std::vector<Polygon> list;
  list.emplace_back(triangle);
  list.emplace_back(square);
  result = result.apply_ops(list, SetOperation::Union);
  result.write_file("/home/hammad/workspace/polgons/output.csv");

  if (square == triangle) {
    std::cout << "Polygons are equal." << std::endl;
  } else {
    std::cout << "Polygons are not equal." << std::endl;
  }

  return 0;
}
