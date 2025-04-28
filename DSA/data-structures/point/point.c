#include <stdlib.h> //malloc, free, exit
#include <stdio.h> //printf
#include <math.h> //sqrt
#include "point.h" //point ADT (interface)

// ADT implementation

struct point {
    float x;
    float y;
};

// Build point function
Point *point_build(float x, float y){
    Point *p = (Point *)malloc(sizeof(Point));
    if (p == NULL){
        printf("Error: Memory allocation failed\n");
        exit(1);
    }
    p->x = x;
    p->y = y;
    return p;
}

// Free memory function
void point_free(Point *p){
    free(p);
}

// Accessor function (get)
void point_get(Point *p, float *x, float *y){
    *x = p->x;
    *y = p->y;
}

// Attribute function (set)
void point_set(Point *p, float x, float y){
    p->x = x;
    p->y = y;
}

// Distance function
float point_distance(Point *p1, Point *p2){
    float dx = p2->x - p1->x;
    float dy = p2->y - p1->y;
    return sqrt(dx * dx + dy * dy);
}

// Main function

int main(void){
    Point *p1 = point_build(3.0, 4.0);
    Point *p2 = point_build(2.4, 1.5);
    float d = point_distance(p1, p2);
    printf("Distance between points: %.2f\n", d);
    point_free(p1);
    point_free(p2);
    return 0;
}
