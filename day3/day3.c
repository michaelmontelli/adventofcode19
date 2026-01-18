#include <stdio.h>
#include <limits.h>
#include "pointset.h"

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

void populatePointSet(PointSet *ps, char wirechars[]) {
    Point curr = {0, 0};
    ps_add(ps, curr);

    char *token = strtok(wirechars, ",");
    while (token != NULL) {
        char direction;
        int nsteps;
        if (sscanf(token, "%c%d", &direction, &nsteps) == 2) {
            printf("%c%d\n", direction, nsteps);

            Point dirpoint = getDirectionPoint(direction);
            for (int i = 0; i < nsteps; i++) {
                updatePoint(&curr, dirpoint);
                ps_add(ps, curr);
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

    PointSet *wire1_ps = ps_create();
    populatePointSet(wire1_ps, wire1);

    int mindist = INT_MAX;
    Point curr = {0, 0};
    char *token = strtok(wire2, ",");
    while (token != NULL) {
        char direction;
        int nsteps;
        if (sscanf(token, "%c%d", &direction, &nsteps) == 2) {
            printf("%c%d\n", direction, nsteps);

            Point dirpoint = getDirectionPoint(direction);
            for (int i = 0; i < nsteps; i++) {
                updatePoint(&curr, dirpoint);
                if (ps_find(wire1_ps, curr)) {
                    printf(
                        "Intersection found at x=%d,y=%d\n",
                        curr.x,
                        curr.y
                    );
                    int dist = abs(curr.x) + abs(curr.y);
                    mindist = MIN(mindist, dist);
                }
            }
        }
        token = strtok(NULL, ",");
    }
    ps_free(wire1_ps);

    printf("Min dist: %d\n", mindist);
    return 0;
}
