#include <stdio.h>
#include "pointset.h"

int main(void) {
    PointSet *ps = ps_create();
    Point p = {1, 2};
    ps_add(ps, p);
    printf("Find status of {1, 2}: %d\n", ps_find(ps, p));
    ps_add(ps, p);
    printf("Find status of {1, 2}: %d\n", ps_find(ps, p));
    ps_remove(ps, p);
    printf("Find status of {1, 2}: %d\n", ps_find(ps, p));

    Point p1 = {1, 1};
    Point p2 = {3, 4};
    printf("p1: %d\n", ps_find(ps, p1));
    printf("p2: %d\n", ps_find(ps, p2));

    ps_add(ps, p1);
    ps_add(ps, p2);
    printf("p1: %d\n", ps_find(ps, p1));
    printf("p2: %d\n", ps_find(ps, p2));

    ps_remove(ps, p);
    printf("Find status of {1, 2}: %d\n", ps_find(ps, p));
    printf("p1: %d\n", ps_find(ps, p1));
    printf("p2: %d\n", ps_find(ps, p2));

    ps_remove(ps, p1);
    printf("Find status of {1, 2}: %d\n", ps_find(ps, p));
    printf("p1: %d\n", ps_find(ps, p1));
    printf("p2: %d\n", ps_find(ps, p2));

    ps_free(ps);
    return 0;
}
