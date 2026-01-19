#include <stdio.h>
#include "pointmap.h"

int main(void) {
    PointMap *pm = pm_create();
    Point p = {1, 2};
    pm_put(pm, p);
    printf("Find status of {1, 2}: %d\n", pm_get(pm, p));
    pm_put(pm, p);
    printf("Find status of {1, 2}: %d\n", pm_get(pm, p));
    pm_remove(pm, p);
    printf("Find status of {1, 2}: %d\n", pm_get(pm, p));

    Point p1 = {1, 1};
    Point p2 = {3, 4};
    printf("p1: %d\n", pm_get(pm, p1));
    printf("p2: %d\n", pm_get(pm, p2));

    pm_put(pm, p1);
    pm_put(pm, p2);
    printf("p1: %d\n", pm_get(pm, p1));
    printf("p2: %d\n", pm_get(pm, p2));

    pm_remove(pm, p);
    printf("Find status of {1, 2}: %d\n", pm_get(pm, p));
    printf("p1: %d\n", pm_get(pm, p1));
    printf("p2: %d\n", pm_get(pm, p2));

    pm_remove(pm, p1);
    printf("Find status of {1, 2}: %d\n", pm_get(pm, p));
    printf("p1: %d\n", pm_get(pm, p1));
    printf("p2: %d\n", pm_get(pm, p2));

    pm_free(pm);
    return 0;
}
