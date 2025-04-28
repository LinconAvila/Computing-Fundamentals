// ADT: Point (interface)

// Exported Type
typedef struct point Point;

// Exported Functions
// Build function
// Allocate and return a point with coordinates (x, y)
Point *point_build(float x, float y);

// Free function
// Free the memory allocated for the point
void point_free(Point *p);

// Accessor function (get)
// Return the coordinate values of a point
void point_get(Point *p, float *x, float *y);

// Attribute function (set)
// Set new coordinate values for a point
void point_set(Point *p, float x, float y);

// Distance function
// Return the distance between two points
float point_distance(Point *p1, Point *p2);