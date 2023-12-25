#include <string>
#include <vector>

enum class SetOperation { Union, Intersection, Difference };

struct Point { // struct to hold x and y
  double x = 0.0;
  double y = 0.0;

  bool operator<(const Point &other) const { // needed to use Points in sets
    if (x == other.x)
      return y < other.y;

    return x < other.x;
  }
};

class Polygon {
private:
  std::vector<Point> points;

public:
  // Default constructor
  Polygon();

  // Parameterized constructor
  Polygon(const std::vector<Point> &input);

  // Copy constructor
  Polygon(const Polygon &other);

  // Destructor
  ~Polygon();

  // Overloaded equality operator
  bool operator==(const Polygon &other) const;

  // Assignment operator overload
  Polygon &operator=(const Polygon &other);

  // get total number of points
  unsigned int get_number_of_points() const;

  // basic sanity checks
  bool is_valid() const;

  // loads a polygon from csv file
  bool read_file(const std::string &filename);

  // writes a polygon to csv file
  bool write_file(const std::string &filename);

  // overload for cout
  friend std::ostream &operator<<(std::ostream &os, const Polygon &polygon);

  // calculate the union of 2 polygons
  Polygon compute_union(Polygon A, Polygon B);

  // calculate the intersection of 2 polygons
  Polygon compute_intersection(Polygon A, Polygon B);

  // calculate the subtraction of 2 polygons A-B
  Polygon compute_subtraction(Polygon A, Polygon B);
};
