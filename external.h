#include <assert.h>
#include <cmath>
#include <iostream>

// place to dump code snippets taken from the web

struct Point; // forward decleration

// the following 2 functions for line intersection were adapted from
// https://gist.github.com/TimSC/47203a0f5f15293d2099507ba5da44e6#file-linelineintersect-cpp-L21

/** Calculate determinant of matrix:
        [a b]
        [c d]
*/
inline double Det(double a, double b, double c, double d) {
  return a * d - b * c;
}

/// Calculate intersection of two lines.
///  return true if found, false if not found or error
bool LineLineIntersect(Point p1,         // Line 1 start
                       Point p2,         // Line 1 end
                       Point p3,         // Line 2 start
                       Point p4,         // Line 2 end
                       Point &intersect) // Output
{
  // http://mathworld.wolfram.com/Line-LineIntersection.html

  double detL1 = Det(p1.x, p1.y, p2.x, p2.y);
  double detL2 = Det(p3.x, p3.y, p4.x, p4.y);
  double x1mx2 = p1.x - p2.x;
  double x3mx4 = p3.x - p4.x;
  double y1my2 = p1.y - p2.y;
  double y3my4 = p3.y - p4.y;

  double xnom = Det(detL1, x1mx2, detL2, x3mx4);
  double ynom = Det(detL1, y1my2, detL2, y3my4);
  double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
  if (denom == 0.0) // Lines don't seem to cross
  {
    intersect.x = NAN;
    intersect.y = NAN;
    return false;
  }

  intersect.x = xnom / denom;
  intersect.y = ynom / denom;
  if (!std::isfinite(intersect.x) ||
      !std::isfinite(intersect.y)) // Probably a numerical issue
    return false;

  return true; // All OK
}

// the following 2 functions are used to detect if a point lies in a polygon
// based on winding number algo and is adapted from
// https://github.com/anirudhtopiwala/OpenSource_Problems/blob/2fd0f600cdded7b86c7c4d6609884b7da1abda36/Point_In_Polygon/src/point_in_polygon.cpp
/**
 * @brief The result can be used to test if the query point lies on the left
 * or right side of the line formed by pt1 and pt2 when viewed in
 *        anticlockwise  direction.
 *
 * @param pt1: First point to form equation of line.
 * @param pt2: Second point to form equation of line.
 * @param query_point: Query point
 * @return: > 0: Query point lies on left of the line.
 *          = 0: Query point lies on the line.
 *          < 0: Query point lies on right of the line.
 */
double substitute_point_in_line(const Point &pt1, const Point &pt2,
                                const Point &query_point) {
  return ((query_point.y - pt1.y) * (pt2.x - pt1.x)) -
         ((query_point.x - pt1.x) * (pt2.y - pt1.y));
}

/**
 * @brief Check if a point lies inside, on or outside any polygon.
 *
 * Winding number algorithm can be used to check if any point lies inside a
 * polygon.
 *
 * @param query_point Point to check.
 * @param vertices Vertices making up the polygon in anticlockwise direction.
 * @return  = 1: query_point lies inside the polygon.
 *          = 0: query_point lies on the polygon.
 *          =-1: query_point lies outside the polygon.
 */
int is_point_inside_polygon(const Point &query_point,
                            std::vector<Point> &vertices) {
  int wn = 0; // the  winding number counter
  const int num_sides_of_polygon = vertices.size();

  for (size_t i = 0; i < num_sides_of_polygon; ++i) {
    const auto point_in_line = substitute_point_in_line(
        vertices[i], vertices[(i + 1) % num_sides_of_polygon], query_point);

    // Check if the point lies on the polygon.
    if (point_in_line == 0) {
      return 0;
    }
    if (vertices[i].y <= query_point.y) {
      // Upward crossing.
      if (vertices[(i + 1) % num_sides_of_polygon].y > query_point.y) {
        if (point_in_line > 0) {
          ++wn; // query point is left of edge
        }
      }
    } else {
      // Downward crossing.
      if (vertices[(i + 1) % num_sides_of_polygon].y < query_point.y) {
        if (point_in_line < 0) {
          --wn; // query point is right of edge
        }
      }
    }
  }
  return (wn != 0) ? 1 : -1; // Point is inside polygon only if wn != 0
}
