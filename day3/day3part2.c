#include <stdio.h>
#include <limits.h>
#include "pointmap.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

#define INPUT_FILENAME "input.txt"

void updatePoint(Point *p, Point direction) {
    p->x += direction.x;
    p->y += direction.y;
}

Point getDirectionPoint(char direction) {
    Point p;
    switch (direction) {
        case 'R':
        {
            p = (Point){1, 0};
            break;
        }
        case 'L':
        {
            p = (Point){-1, 0};
            break;
        }
        case 'U':
        {
            p = (Point){0, 1};
            break;
        }
        case 'D':
        {
            p = (Point){0, -1};
            break;
        }
    }
    return p;
}

void populatePointMap(PointMap *pm, char wirechars[]) {
    Point curr = {0, 0};
    int dist = 0;

    char *token = strtok(wirechars, ",");
    while (token != NULL) {
        char direction;
        int nsteps;
        if (sscanf(token, "%c%d", &direction, &nsteps) == 2) {
            printf("%c%d\n", direction, nsteps);

            Point dirpoint = getDirectionPoint(direction);
            for (int i = 0; i < nsteps; i++) {
                updatePoint(&curr, dirpoint);
                dist++;
                pm_put(pm, curr, dist);
            }
        }
        token = strtok(NULL, ",");
    }
}

int main(void) {
    FILE *fp = fopen(INPUT_FILENAME, "r");
    if (fp == NULL) {
        fprintf(stderr, "File %s not found\n", INPUT_FILENAME);
        return 1;
    }

    char wire1[2048];
    fgets(wire1, sizeof(wire1), fp);

    char wire2[2048];
    fgets(wire2, sizeof(wire2), fp);
    fclose(fp);

    PointMap *pm = pm_create();
    populatePointMap(pm, wire1);

    int mindist = INT_MAX;
    Point curr = {0, 0};
    int dist = 0;
    char *token = strtok(wire2, ",");
    while (token != NULL) {
        char direction;
        int nsteps;
        if (sscanf(token, "%c%d", &direction, &nsteps) == 2) {
            printf("%c%d\n", direction, nsteps);

            Point dirpoint = getDirectionPoint(direction);
            for (int i = 0; i < nsteps; i++) {
                updatePoint(&curr, dirpoint);
                dist++;

                int wire1dist = pm_get(pm, curr);
                if (wire1dist != -1) {
                    printf(
                        "Intersection found at x=%d,y=%d\n",
                        curr.x,
                        curr.y
                    );
                    mindist = MIN(mindist, wire1dist + dist);
                }
            }
        }
        token = strtok(NULL, ",");
    }
    pm_free(pm);

    printf("Min dist: %d\n", mindist);
    return 0;
}
