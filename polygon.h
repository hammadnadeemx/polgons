#ifndef POLYGON_H
#define POLYGON_H

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Enumeration representing set operations on polygons.
 */
enum class SetOperation { Union, Intersection, Difference };

/**
 * @brief Struct to represent a 2D point with x and y coordinates.
 */
struct Point {
  double x = 0.0; /**< x-coordinate of the point. */
  double y = 0.0; /**< y-coordinate of the point. */

  /**
   * @brief Overloaded less-than operator for comparing points.
   *
   * @param other The other point to compare.
   *
   * @return True if this point is less than the other, false otherwise.
   */
  bool operator<(const Point &other) const;
};

/**
 * @brief Class representing a polygon in 2D space.
 */
class Polygon {
private:
  std::vector<Point> points; /**< Vector of points representing the polygon. */

public:
  /**
   * @brief Default constructor for the Polygon class.
   */
  Polygon();

  /**
   * @brief Parameterized constructor for the Polygon class.
   *
   * @param input Vector of points to initialize the polygon.
   */
  Polygon(const std::vector<Point> &input);

  /**
   * @brief Copy constructor for the Polygon class.
   *
   * @param other The polygon to copy.
   */
  Polygon(const Polygon &other);

  /**
   * @brief Destructor for the Polygon class.
   */
  ~Polygon();

  /**
   * @brief Overloaded equality operator for comparing polygons.
   *
   * @param other The other polygon to compare.
   *
   * @return True if the polygons are equal, false otherwise.
   */
  bool operator==(const Polygon &other) const;

  /**
   * @brief Assignment operator overload for assigning one polygon to another.
   *
   * @param other The polygon to assign.
   *
   * @return Reference to the assigned polygon.
   */
  Polygon &operator=(const Polygon &other);

  /**
   * @brief Get the total number of points in the polygon.
   *
   * @return Total number of points in the polygon.
   */
  unsigned int get_number_of_points() const;

  /**
   * @brief Perform basic sanity checks on the polygon.
   *
   * @return True if the polygon is valid, false otherwise.
   */
  bool is_valid() const;

  /**
   * @brief Load a polygon from a CSV file.
   *
   * @param filename The name of the CSV file.
   *
   * @return True if the file is successfully loaded, false otherwise.
   */
  bool read_file(const std::string &filename);

  /**
   * @brief Write the polygon to a CSV file.
   *
   * @param filename The name of the CSV file.
   *
   * @return True if the file is successfully written, false otherwise.
   */
  bool write_file(const std::string &filename);

  /**
   * @brief Overloaded stream insertion operator for outputting the polygon.
   *
   * @param os The output stream.
   * @param polygon The polygon to output.
   *
   * @return Reference to the output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const Polygon &polygon);

  /**
   * @brief Compute the union of two polygons.
   *
   * @param A The first polygon.
   * @param B The second polygon.
   *
   * @return The polygon representing the union of A and B.
   */
  static Polygon compute_union(Polygon A, Polygon B);

  /**
   * @brief Compute the intersection of two polygons.
   *
   * @param A The first polygon.
   * @param B The second polygon.
   *
   * @return The polygon representing the intersection of A and B.
   */
  static Polygon compute_intersection(Polygon A, Polygon B);

  /**
   * @brief Compute the subtraction of two polygons (A - B).
   *
   * @param A The first polygon.
   * @param B The second polygon.
   *
   * @return The polygon representing the subtraction of B from A.
   */
  static Polygon compute_subtraction(Polygon A, Polygon B);

  /**
   * @brief Apply the same operation to a vector of polygons
   *
   * @param polygons Vector of polygons.
   * @param op The specified operation eg Union, Intersection or Difference.
   *
   * @return The resulting polygon.
   */
  static Polygon apply_ops(std::vector<Polygon> polygons, SetOperation op);
};

#endif // POLYGON_H
