#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "circle.h"

#define PI 3.14159

struct circle {
    Point *center;
    float radius;
};

Circle *circle_build(float x , float y, float radius){
    Circle *c = (Circle *)malloc(sizeof(Circle));
    c->center = point_build(x, y);
    c->radius = radius;
    return c;
}

void circle_free(Circle *c){
    point_free(c->center);
    free(c);
}

void circle_get(Circle *c, float *x, float *y, float *radius){
    point_get(c->center, x, y);
    *radius = c->radius;
}

void circle_set(Circle *c, float x, float y, float radius){
    point_set(c->center, x, y);
    c->radius = radius;
}

float circle_area(Circle *c){
    return PI * c->radius * c->radius;
}

float circle_circumference(Circle *c){
    return 2 * PI * c->radius;
}

int circle_is_point_inside(Circle *c, Point *p){
    float distance = point_distance(c->center, p);
    return (distance < c->radius);
}

int main(void){
    Circle *c = circle_build(5,5,2.0);
    float area = circle_area(c);
    float circumference = circle_circumference(c);
    printf("Circle area: %.2f\n", area);
    printf("Circle circumference: %.2f\n", circumference);
    Point *p = point_build(4,4);
    if (circle_is_point_inside(c, p)){
        printf("Point is inside the circle\n");
    } else {
        printf("Point is outside the circle\n");
    }
    point_free(p);
    circle_free(c);
    return 0;
}


