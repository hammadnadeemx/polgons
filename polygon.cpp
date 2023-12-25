#include "polygon.h"
#include "external.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

namespace { // internal helper functions
// Function to calculate the polar angle of a point with respect to the
// reference point
inline double polar_angle(const Point &reference, const Point &point) {
  return atan2(point.y - reference.y, point.x - reference.x);
}

// Function to check if point2 is more counterclockwise than point1 with respect
// to the reference point
bool compare_counter_clockwise(const Point &reference, const Point &point1,
                               const Point &point2) {
  double angle1 = polar_angle(reference, point1);
  double angle2 = polar_angle(reference, point2);
  return angle1 > angle2;
}

// Function to sort 2D points in ccw order
void sort_points_counter_clockwise(std::vector<Point> &points) {
  if (points.size() < 3) {
    // Sorting is not meaningful for fewer than three points
    return;
  }

  // Find the reference point (e.g., centroid)
  Point reference;
  for (const Point &point : points) {
    reference.x += point.x;
    reference.y += point.y;
  }
  reference.x /= points.size();
  reference.y /= points.size();

  // Sort the points based on polar angle with respect to the reference point
  std::sort(points.begin(), points.end(),
            [reference](const Point &p1, const Point &p2) {
              return compare_counter_clockwise(reference, p1, p2);
            });
}
} // namespace

// Default constructor
Polygon::Polygon() {}

// Parameterized constructor
Polygon::Polygon(const std::vector<Point> &input) : points(input) {
  // some algorithms need the points sorted in ccw order
  sort_points_counter_clockwise(points);
}

// Copy constructor
Polygon::Polygon(const Polygon &other) : points(other.points) {
  // some algorithms need the points sorted in ccw order
  sort_points_counter_clockwise(points);
}

// Destructor
Polygon::~Polygon() { points.clear(); }

// Overloaded equality operator
bool Polygon::operator==(const Polygon &other) const {
  if (this->is_valid() && other.is_valid()) {
    // check if number of points are same
    if (this->points.size() != other.points.size()) {
      return false;
    }

    // Check if each element is equal
    for (unsigned int i = 0; i < this->points.size(); ++i) {
      if ((this->points[i].x != other.points[i].x) ||
          (this->points[i].y != other.points[i].y)) {
        return false;
      }
    }
    return true;
  }
  return false;
}

// Assignment operator overload
Polygon &Polygon::operator=(const Polygon &other) {
  // Check for self-assignment
  if (this != &other) {
    this->points.assign(other.points.begin(), other.points.end());
  }
  return *this;
}

unsigned int Polygon::get_number_of_points() const { return points.size(); }

bool Polygon::is_valid() const { return points.size() > 2; }

bool Polygon::read_file(const std::string &filename) {
  // Clear any existing data
  points.clear();

  // Open the file
  std::ifstream inputfile(filename);
  if (!inputfile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false;
  }

  // Read each line from the file
  std::string line;
  while (std::getline(inputfile, line)) {
    // Use a stringstream to parse the comma-separated values
    std::istringstream iss(line);
    std::string token;
    Point temp;

    // Read x and y coordinates
    if (std::getline(iss, token, ',')) {
      try {
        temp.x = std::stod(token);
      } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid x-coordinate format: " << token << std::endl;
        return false;
      }
    } else {
      std::cerr << "Missing x-coordinate in line: " << line << std::endl;
      return false;
    }

    if (std::getline(iss, token, ',')) {
      try {
        temp.y = std::stod(token);
      } catch (const std::invalid_argument &e) {
        std::cerr << "Invalid y-coordinate format: " << token << std::endl;
        return false;
      }
    } else {
      std::cerr << "Missing y-coordinate in line: " << line << std::endl;
      return false;
    }
    this->points.emplace_back(temp);
  }

  // Close the file
  inputfile.close();

  // some algorithms need the points sorted in ccw order
  sort_points_counter_clockwise(points);
  return true;
}

// Function to write a polygon to a CSV file
bool Polygon::write_file(const std::string &filename) {
  // Open the file for writing
  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return false;
  }

  // Write header
  outputFile << "x,y" << std::endl;

  // Write polygon coordinates
  for (size_t i = 0; i < points.size(); ++i) {
    outputFile << points[i].x << "," << points[i].y << std::endl;
  }

  // Close the file
  outputFile.close();
  std::cout << "Polygon written to file: " << filename << std::endl;
  return true;
}

// Implementation of the << operator
std::ostream &operator<<(std::ostream &os, const Polygon &polygon) {
  os << "Polygon coordinates:" << std::endl;
  for (size_t i = 0; i < polygon.points.size(); ++i) {
    os << "Point " << i + 1 << ": (" << polygon.points[i].x << ", "
       << polygon.points[i].y << ")" << std::endl;
  }
  return os;
}

// calculate the union of 2 polygons uses the algo described
// https://stackoverflow.com/questions/7915734/intersection-and-union-of-polygons
Polygon Polygon::compute_union(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertex_set;
    vertex_set.insert(A.points.begin(), A.points.end());
    vertex_set.insert(B.points.begin(), B.points.end());

    Point intersection;

    // add all interesection points of edges
    for (int i = 1; i < A.points.size(); i++) {   // edge 1
      for (int j = 1; j < B.points.size(); j++) { // edge 2
        if (LineLineIntersect(A.points[i - 1], A.points[i], B.points[j - 1],
                              B.points[j], intersection)) {
          vertex_set.insert(intersection);
        }
      }
    }

    // remove internal points from resultant set
    for (auto &p : vertex_set) {
      if ((is_point_inside_polygon(p, A.points) != 1) ||
          (is_point_inside_polygon(p, B.points) != 1)) {
        result.points.emplace_back(p);
      }
    }

    sort_points_counter_clockwise(result.points);
  }

  return result;
}

// calculate the intersection of 2 polygons, Inspired by the function above
Polygon Polygon::compute_intersection(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertex_set;
    Point intersection;

    // add all interesection points of edges
    for (int i = 1; i < A.points.size(); i++) {   // edge 1
      for (int j = 1; j < B.points.size(); j++) { // edge 2
        if (LineLineIntersect(A.points[i - 1], A.points[i], B.points[j - 1],
                              B.points[j], intersection))
          vertex_set.insert(intersection);
      }
    }

    // if point lies in or on the polygon add to the set
    for (auto &pA : A.points)
      if (is_point_inside_polygon(pA, B.points) != -1)
        vertex_set.insert(pA);

    // if point lies in or on the polygon add to the set
    for (auto &pB : B.points)
      if (is_point_inside_polygon(pB, A.points) != -1)
        vertex_set.insert(pB);

    for (auto &p : vertex_set)
      result.points.emplace_back(p);

    sort_points_counter_clockwise(result.points);
  }

  return result;
}

// calculate the subtraction of 2 polygons A-B.
// Assumes that A and B dont have any identical vertex other wise needs more
// checks
Polygon Polygon::compute_subtraction(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertex_set;
    vertex_set.insert(A.points.begin(), A.points.end());
    Point intersection;

    // add all interesection points of edges
    for (int i = 1; i < A.points.size(); i++) {   // edge 1
      for (int j = 1; j < B.points.size(); j++) { // edge 2
        if (LineLineIntersect(A.points[i - 1], A.points[i], B.points[j - 1],
                              B.points[j], intersection)) {
          vertex_set.insert(intersection);
        }
      }
    }

    // remove points that lie in B from resultant set
    for (auto &p : vertex_set) {
      if (is_point_inside_polygon(p, B.points) != 1)
        result.points.emplace_back(p);
    }

    // add points of B that lie in A
    for (auto &p : B.points) {
      if (is_point_inside_polygon(p, A.points) == 1)
        result.points.emplace_back(p);
    }

    sort_points_counter_clockwise(result.points);
  }

  return result;
}