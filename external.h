#include <assert.h>
#include <cmath>
#include <iostream>

// place to dump code snippets taken from the web

struct Point; // forward decleration

namespace {
const double epsilon = 1e-10;
}

// Check to see if the point lies on the line segment
bool PointOnLineSegment(const Point &p, const Point &start, const Point &end) {
  // Check if the point is within the epsilon range of the line segment
  return (p.x >= std::min(start.x, end.x) - epsilon &&
          p.x <= std::max(start.x, end.x) + epsilon &&
          p.y >= std::min(start.y, end.y) - epsilon &&
          p.y <= std::max(start.y, end.y) + epsilon);
}

// The following function is adapted from
//  https://paulbourke.net/geometry/pointlineplane/Helpers.cs
bool DoLinesIntersect(const Point &p1, const Point &p2, const Point &p3,
                      const Point &p4, Point &ptIntersection) {
  // Denominator for ua and ub are the same, so store this calculation
  double d = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

  // n_a and n_b are calculated as separate values for readability
  double n_a = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);

  double n_b = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);

  // Make sure there is not a division by zero - this also indicates that
  // the lines are parallel.
  // If n_a and n_b were both equal to zero, the lines would be on top of each
  // other (coincidental).  This check is not done because it is not
  // necessary for this implementation (the parallel check accounts for this).
  if (d == 0)
    return false;

  // Calculate the intermediate fractional point that the lines potentially
  // intersect.
  double ua = n_a / d;
  double ub = n_b / d;

  // The fractional point will be between 0 and 1 inclusive if the lines
  // intersect.  If the fractional calculation is larger than 1 or smaller
  // than 0, the lines would need to be longer to intersect.
  if (ua >= 0.0 && ua <= 1.0 && ub >= 0.0 && ub <= 1.0) {
    ptIntersection.x = p1.x + (ua * (p2.x - p1.x));
    ptIntersection.y = p1.y + (ua * (p2.y - p1.y));
    return true;
  }

  return false;
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
  int wn = 0; // the winding number counter
  const int num_sides_of_polygon = vertices.size();

  for (size_t i = 0; i < num_sides_of_polygon; ++i) {
    const auto point_in_line = substitute_point_in_line(
        vertices[i], vertices[(i + 1) % num_sides_of_polygon], query_point);

    // Check if the points are collinear (within epsilon)
    if (std::abs(point_in_line) < epsilon) {
      // Check collinear points iff they are within line bounds
      bool onSegment = PointOnLineSegment(
          query_point, vertices[i], vertices[(i + 1) % num_sides_of_polygon]);
      return onSegment ? 0 : -1;
    }

    if (vertices[i].y <= query_point.y) {
      // Upward crossing.
      if (vertices[(i + 1) % num_sides_of_polygon].y > query_point.y) {
        if (point_in_line > epsilon) {
          ++wn; // query point is left of edge
        }
      }
    } else {
      // Downward crossing.
      if (vertices[(i + 1) % num_sides_of_polygon].y < query_point.y) {
        if (point_in_line < -epsilon) {
          --wn; // query point is right of edge
        }
      }
    }
  }
  return (wn != 0) ? 1 : -1; // Point is inside polygon only if wn != 0
}
