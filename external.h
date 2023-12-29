#include <assert.h>
#include <cmath>
#include <iostream>

/**< place to dump code snippets adapted from the web. */

struct Point; /**< forward decleration. */

namespace {
const double epsilon = 1e-10; /**< Minimum error. */
}

/**
 * @brief Checks if the point is within the epsilon range of the line segment.
 * Assumes the points are colinear.
 *
 * @param point: Point to check.
 * @param start: First point to form equation of line.
 * @param end: Second point to form equation of line.
 *
 * @return: True if point lies within range.
 */
bool point_on_line_segment(const Point &point, const Point &start,
                           const Point &end) {
  return (point.x >= std::min(start.x, end.x) - epsilon &&
          point.x <= std::max(start.x, end.x) + epsilon &&
          point.y >= std::min(start.y, end.y) - epsilon &&
          point.y <= std::max(start.y, end.y) + epsilon);
}

/**
 * @brief Checks if the lines formed by p1->p2 and p3->p4 intersect. Function
 * logic is adapted from
 * https://paulbourke.net/geometry/pointlineplane/Helpers.cs
 *
 * @param p1: Line A start point.
 * @param p2: Line A end point.
 * @param p3: Line B start point.
 * @param p4: Line B end point.
 * @param ptIntersection: Computed intersection result.
 *
 * @return: True if lines intersect, false otherwise.
 */
bool do_lines_intersect(const Point &p1, const Point &p2, const Point &p3,
                        const Point &p4, Point &ptIntersection) {
  /**< Denominator for ua and ub are the same, so store this calculation */
  double denom = (p4.y - p3.y) * (p2.x - p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

  /**< normalA and normalB are calculated as separate values for readability */
  double normalA =
      (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);

  double normalB =
      (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);

  /**< Make sure there is not a division by zero - this also indicates that
   * the lines are parallel.
   * If normalA and normalB were both equal to zero, the lines would be on top
   * of each other (coincidental).  This check is not done because it is not
   * necessary for this implementation (the parallel check accounts for this).*/
  if (denom == 0)
    return false;

  /**< Calculate the intermediate fractional point that the lines potentially
   * intersect. */
  double fractA = normalA / denom;
  double fractB = normalB / denom;

  /**< The fractional point will be between 0 and 1 inclusive if the lines
   * intersect.  If the fractional calculation is larger than 1 or smaller
   * than 0, the lines would need to be longer to intersect.*/
  if (fractA >= 0.0 && fractA <= 1.0 && fractB >= 0.0 && fractB <= 1.0) {
    ptIntersection.x = p1.x + (fractA * (p2.x - p1.x));
    ptIntersection.y = p1.y + (fractA * (p2.y - p1.y));
    return true;
  }

  return false;
}

/**
 * @brief The result can be used to test if the query point lies on the left
 * or right side of the line formed by pt1 and pt2 when viewed in anticlockwise
 * direction. Taken from
 * https://github.com/anirudhtopiwala/OpenSource_Problems/blob/2fd0f600cdded7b86c7c4d6609884b7da1abda36/Point_In_Polygon/src/point_in_polygon.cpp
 *
 * @param pt1: First point to form equation of line.
 * @param pt2: Second point to form equation of line.
 * @param queryPoint: Query point
 *
 * @return: > 0: Query point lies on left of the line.
 *          = 0: Query point lies on the line.
 *          < 0: Query point lies on right of the line.
 */
double inline substitute_point_in_line(const Point &pt1, const Point &pt2,
                                       const Point &queryPoint) {
  return ((queryPoint.y - pt1.y) * (pt2.x - pt1.x)) -
         ((queryPoint.x - pt1.x) * (pt2.y - pt1.y));
}

/**
 * @brief Check if a point lies inside, on or outside any polygon. Uses winding
 * number algorithm. Taken from
 * https://github.com/anirudhtopiwala/OpenSource_Problems/blob/2fd0f600cdded7b86c7c4d6609884b7da1abda36/Point_In_Polygon/src/point_in_polygon.cpp
 *
 * @param queryPpoint Point to check.
 * @param vertices Vertices making up the polygon in anticlockwise direction.
 *
 * @return  = 1: queryPpoint lies inside the polygon.
 *          = 0: queryPpoint lies on the polygon.
 *          =-1: queryPpoint lies outside the polygon.
 */
int is_point_inside_polygon(const Point &queryPpoint,
                            std::vector<Point> &vertices) {
  int windingNumber = 0; /**< the winding number counter */
  const int num_sides_of_polygon = vertices.size();

  for (size_t i = 0; i < num_sides_of_polygon; ++i) {
    const auto point_in_line = substitute_point_in_line(
        vertices[i], vertices[(i + 1) % num_sides_of_polygon], queryPpoint);

    /**< Check if the points are collinear (within epsilon) */
    if (std::abs(point_in_line) < epsilon) {
      /**< Check collinear points iff they are within line bounds */
      bool onSegment = point_on_line_segment(
          queryPpoint, vertices[i], vertices[(i + 1) % num_sides_of_polygon]);
      return onSegment ? 0 : -1;
    }

    if (vertices[i].y <= queryPpoint.y) {
      /**< Upward crossing. */
      if (vertices[(i + 1) % num_sides_of_polygon].y > queryPpoint.y) {
        if (point_in_line > epsilon) {
          ++windingNumber; /**< query point is left of edge */
        }
      }
    } else {
      /**< Downward crossing. */
      if (vertices[(i + 1) % num_sides_of_polygon].y < queryPpoint.y) {
        if (point_in_line < -epsilon) {
          --windingNumber; /**< query point is right of edge */
        }
      }
    }
  }
  return (windingNumber != 0)
             ? 1
             : -1; /**< Point is inside polygon only if windingNumber != 0 */
}
