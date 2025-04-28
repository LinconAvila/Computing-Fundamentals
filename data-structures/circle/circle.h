#include "point.h"

typedef struct circle Circle;

// Build function
Circle *circle_build(float x, float y, float radius);

// Free function
void circle_free(Circle *c);

// Accessor function (get)
void circle_get(Circle *c, float *x, float *y, float *radius);

// Attribute function (set)
void circle_set(Circle *c, float x, float y, float radius);

// Area function
float circle_area(Circle *c);

// Circumference function
float circle_circumference(Circle *c);

// is point inside function
int circle_is_point_inside(Circle *c, Point *p);
