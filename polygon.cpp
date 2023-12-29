#include "polygon.h"
#include "external.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>

namespace { /**< Internal helper functions */

/**
 * @brief Function to calculate the polar angle of a point with respect to the
 * reference point.
 *
 * @param reference Reference point.
 * @param point Point to calculate angle from.
 *
 * @return double angle in radians.
 */
inline double polar_angle(const Point &reference, const Point &point) {
  return atan2(point.y - reference.y, point.x - reference.x);
}

/**
 * @brief Function to check if point2 is more counterclockwise than point1 with
 * respect to the reference point.
 *
 * @param reference Reference point.
 * @param pointA Point A.
 * @param pointB Point B.
 *
 * @return True if A is more CCW than B.
 */
bool compare_counter_clockwise(const Point &reference, const Point &pointA,
                               const Point &pointB) {
  double angleA = polar_angle(reference, pointA);
  double angleB = polar_angle(reference, pointB);
  return angleA > angleB;
}

/**
 * @brief Function to sort 2D points in ccw order.
 *
 * @param points Vector of points.
 */
void sort_points_counter_clockwise(std::vector<Point> &points) {
  if (points.size() < 3) {
    /**<  Sorting is not meaningful for fewer than three points. */
    std::cout << "Not enough points to sort\n";
    return;
  }

  /**<  Find the centroid. */
  Point reference;
  for (const Point &point : points) {
    reference.x += point.x;
    reference.y += point.y;
  }
  reference.x /= points.size();
  reference.y /= points.size();

  /**< Sort the points based on polar angle wrt the reference point.*/
  std::sort(points.begin(), points.end(),
            [reference](const Point &p1, const Point &p2) {
              return compare_counter_clockwise(reference, p1, p2);
            });
}
} // namespace

/**< Required to use Points in sets. */
bool Point::operator<(const Point &other) const {
  if (x == other.x)
    return y < other.y;

  return x < other.x;
}

/**< Default constructor. */
Polygon::Polygon() {}

/**< Parameterized constructor */
Polygon::Polygon(const std::vector<Point> &input) : points(input) {
  /**< some algorithms need the points sorted in ccw order. */
  sort_points_counter_clockwise(points);
}

/**< Copy constructor */
Polygon::Polygon(const Polygon &other) : points(other.points) {
  /**< some algorithms need the points sorted in ccw order. */
  sort_points_counter_clockwise(points);
}

/**< Destructor. */
Polygon::~Polygon() { points.clear(); }

/**< Overloaded equality operator. */
bool Polygon::operator==(const Polygon &other) const {
  if (this->is_valid() && other.is_valid()) {
    /**<  check if number of points are same. */
    if (this->points.size() != other.points.size()) {
      return false;
    }

    /**<  Check if each element is equal. */
    for (unsigned int i = 0; i < this->points.size(); ++i)
      if ((this->points[i].x != other.points[i].x) ||
          (this->points[i].y != other.points[i].y))
        return false;

    return true;
  }
  return false;
}

/**<  Assignment operator overload */
Polygon &Polygon::operator=(const Polygon &other) {
  /**<  Check for self-assignment. */
  if (this != &other) {
    this->points.assign(other.points.begin(), other.points.end());
  }
  return *this;
}

/**<  Returns number of vertices.*/
unsigned int Polygon::get_number_of_points() const { return points.size(); }

/**<  Sanity Checks.*/
bool Polygon::is_valid() const {
  if (points.size() < 3)
    return false;

  /**<  Check for self intersecting polygon.*/
  for (size_t i = 0; i < points.size(); i++) {
    for (size_t j = 0; j < points.size(); j++) {
      if ((i != j) && ((i + 1) % points.size() != j) &&
          ((j + 1) % points.size() != i)) {
        Point intersection;
        if (do_lines_intersect(points[i], points[(i + 1) % points.size()],
                               points[j], points[(j + 1) % points.size()],
                               intersection))
          return false;
      }
    }
  }

  return true;
}

/**<  Reads a polygon from file.*/
bool Polygon::read_file(const std::string &filename) {
  points.clear();
  std::ifstream inputFile(filename);

  if (!inputFile.is_open()) {
    std::cerr << "Error opening file: " << filename << std::endl;
    return false; /**< Return an empty vector on error. */
  }

  /**< Read each line from the file */
  std::string line;
  Point temp;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);

    /**< Parse x and y values from the line */
    if (iss >> temp.x >> temp.y) {
      points.emplace_back(temp);
    } else {
      std::cerr << "Error parsing line: " << line << std::endl;
    }
  }

  inputFile.close();
  /**< some algorithms need the points sorted in ccw order */
  sort_points_counter_clockwise(points);
  return true;
}

/**<  Writes a polygon to file.*/
bool Polygon::write_file(const std::string &filename) {
  /**< Open the file for writing. */
  std::ofstream outputFile(filename);
  if (!outputFile.is_open()) {
    std::cerr << "Error opening file for writing: " << filename << std::endl;
    return false;
  }

  /**< Write header. */
  outputFile << "x,y" << std::endl;

  /**< Write polygon coordinates. */
  for (size_t i = 0; i < points.size(); ++i) {
    outputFile << points[i].x << "," << points[i].y << std::endl;
  }

  outputFile.close();
  std::cout << "Polygon written to file: " << filename << std::endl;
  return true;
}

/**< Implementation of the << operator. */
std::ostream &operator<<(std::ostream &os, const Polygon &polygon) {
  os << "Polygon coordinates:" << std::endl;
  for (size_t i = 0; i < polygon.points.size(); ++i) {
    os << "(" << polygon.points[i].x << "," << polygon.points[i].y << ")"
       << (i + 1 == polygon.points.size() ? "\n" : ",");
  }
  return os;
}

/**< Calculates the union of 2 polygons uses the algo described on
 * https://stackoverflow.com/questions/7915734/intersection-and-union-of-polygons
 */
Polygon Polygon::compute_union(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertexSet;
    vertexSet.insert(A.points.begin(), A.points.end());
    vertexSet.insert(B.points.begin(), B.points.end());

    Point intersection;

    /**< add all interesection points of edges. */
    for (int i = 0; i < A.points.size(); i++) {
      for (int j = 0; j < B.points.size(); j++) {
        if (do_lines_intersect(A.points[i], A.points[(i + 1) % A.points.size()],
                               B.points[j], B.points[(j + 1) % B.points.size()],
                               intersection)) {
          vertexSet.insert(intersection);
        }
      }
    }

    /**< remove internal points from resultant set. */
    for (auto &p : vertexSet) {
      if (!((is_point_inside_polygon(p, A.points) == 1) ||
            (is_point_inside_polygon(p, B.points) == 1))) {
        result.points.emplace_back(p);
      }
    }

    sort_points_counter_clockwise(result.points);
  }

  return result;
}

/**< calculate the intersection of 2 polygons, Inspired by the function above.
 */
Polygon Polygon::compute_intersection(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertexSet;
    Point intersection;

    for (auto &pa : A.points)
      if (is_point_inside_polygon(pa, B.points) >= 0)
        vertexSet.insert(pa);

    for (auto &pb : B.points)
      if (is_point_inside_polygon(pb, A.points) >= 0)
        vertexSet.insert(pb);

    /**< add all interesection points of edges. */
    for (int i = 0; i < A.points.size(); i++) {
      for (int j = 0; j < B.points.size(); j++) {
        if (do_lines_intersect(A.points[i], A.points[(i + 1) % A.points.size()],
                               B.points[j], B.points[(j + 1) % B.points.size()],
                               intersection)) {
          vertexSet.insert(intersection);
        }
      }
    }

    /**< remove external points from resultant set. */
    for (auto &p : vertexSet) {
      if (!((is_point_inside_polygon(p, A.points) == -1) ||
            (is_point_inside_polygon(p, B.points) == -1)))
        result.points.emplace_back(p);
    }

    sort_points_counter_clockwise(result.points);
  }

  return result;
}

/**< Calculates the subtraction of 2 polygons A-B. */
Polygon Polygon::compute_subtraction(Polygon A, Polygon B) {
  Polygon result;

  if (A.is_valid() && B.is_valid()) {
    std::set<Point> vertexSet;
    vertexSet.insert(A.points.begin(), A.points.end());
    Point intersection;

    /**< add points of B that lie in A. */
    for (auto &p : B.points) {
      if (is_point_inside_polygon(p, A.points) == 1)
        result.points.emplace_back(p);
    }

    /**< add all interesection points of edges. */
    for (int i = 0; i < A.points.size(); i++) {
      for (int j = 0; j < B.points.size(); j++) {
        if (do_lines_intersect(A.points[i], A.points[(i + 1) % A.points.size()],
                               B.points[j], B.points[(j + 1) % B.points.size()],
                               intersection)) {
          vertexSet.insert(intersection);
        }
      }
    }

    /**< remove points that lie in B from resultant set. */
    for (auto &p : vertexSet) {
      if (!(is_point_inside_polygon(p, B.points) == 1))
        result.points.emplace_back(p);
    }

    sort_points_counter_clockwise(result.points);
  }

  return result;
}
