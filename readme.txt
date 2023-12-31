Build instructions for linux:
mkdir build
cd build
cmake ..
make 
./Polygon

To generate docs via doxygen:
doxygen Doxyfile

The implementation assumes that the polygons are without holes and non overlapping. The points should form a line from the first point till the end (and loops around to the start). The code has sanity checks for self intersecting polygons as the logic currently does not support this type of polygon. The logic is based on the winding number algorithm which is used to determine if a point lies inside a polygon. It also checks if the point lies on the line segment or not (since this case seems to confuse the winding number algorithm implementation). The code can read polygons as defined in the example csv files and output the results to file. 
To explain how union is computed, we start with a set of all the vertices of both polygons and to this set we add all the intersection points between their edges. Then we remove any points from this set that lie inside any of the 2 polygons. Lastly the points are sorted in counter clockwise order.
To explain how intersection is computed, we add all the points of polygon A that lie inside or on the edges of B to a set. We then add all the points of polygon B that lie inside or on the edges of A. Then we add all the intersection points between their edges. Then we remove all the points that lie outside both A and B. Lastly the points are sorted in counter clockwise order.
To explain how difference (A-B) is computed, we add the points of polygon A to a set. Then we add the points of B that lie inside A. Then we add the points of intersection between their edges. Then we remove any points that lie inside B. Lastly the points are sorted in counter clockwise order. 
To compute the results of a vector of polygons, the operation is applied again and again on the result of the first 2 polygons. The assumption is here is that the order for union and intersection don’t matter and the order specified in the vector is the respected for difference operator.
The code was written with Codelite IDE on Ubuntu 22.04 and compiled with gcc 11.4 using cmake 3.22.1 build system. Doxygen 1.9.1 was used to create documentation.
