#ifndef POINTMAP_H
#define POINTMAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y;
} Point;

typedef struct Entry {
    struct Entry *next;
    Point p; // Key
    int dist; // Value
} Entry;

typedef struct {
    Entry **buckets;
    size_t size;
    size_t capacity;
} PointMap;

/* Creates PointMap structure, with buckets zeroed out--each bucket
 * is thus initially set to NULL */
#define PM_INITIAL_CAPACITY 2
PointMap *pm_create() {
    PointMap *pm = malloc(sizeof(*pm));
    if (pm == NULL) {
        perror("Error allocating PointMap");
        exit(1);
    }

    // Initialize each pointer to NULL
    pm->buckets = calloc(PM_INITIAL_CAPACITY, sizeof(pm->buckets[0]));
    if (pm->buckets == NULL) {
        perror("Error allocating PointMap buckets");
        exit(1);
    }

    pm->capacity = PM_INITIAL_CAPACITY;
    pm->size = 0;

    return pm;
}

#define HASH_FACTOR 31
static unsigned long hash(void *x, size_t size) {
    unsigned long h = 0;
    unsigned char *p = x;
    unsigned char *end = p + size;
    for (; p < end; p++) {
        h = HASH_FACTOR*h + *p;
    }

    return h;
}

static Entry *createEntry(Point p, int dist) {
    Entry *entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        perror("Error allocating Entry");
        exit(1);
    }

    entry->p = p;
    entry->dist = dist;
    entry->next = NULL;
    return entry;
}

static void _pm_free_chain(Entry *entry) {
    if (entry == NULL) return;
    _pm_free_chain(entry->next);

    free(entry);
}

void pm_free(PointMap *pm) {
    for (int i = 0; i < pm->capacity; i++) {
        _pm_free_chain(pm->buckets[i]);
    }
    free(pm->buckets);
    free(pm);
}

int point_equals(Point a, Point b) {
    // Be careful doing memcmp
    // memcmp(&a, &b, sizeof(Point)) == 0
    //
    // This works for Point, but won't work if
    // there are extra bytes for alignment filled with garbage
    return a.x == b.x && a.y == b.y;
}

static int _pm_put(Entry **buckets, size_t capacity, Point p, int dist) {
    int idx = hash(&p, sizeof(p)) % capacity;
    if (buckets[idx] == NULL) {
        buckets[idx] = createEntry(p, dist);
        return 1;
    }

    Entry *prev = NULL;
    Entry *curr = buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) return 0;
        prev = curr;
        curr = curr->next;
    }

    prev->next = createEntry(p, dist);
    return 1;
}

static void _pm_resize(PointMap *pm) {
    int newCapacity = pm->capacity * 2;
    Entry **newBuckets = calloc(newCapacity, sizeof(*newBuckets));
    if (newBuckets == NULL) {
        perror("Error allocating new buckets during resize");
        exit(1);
    }

    for (int i = 0; i < pm->capacity; i++) {
        Entry *curr = pm->buckets[i];
        
        while (curr) {
            _pm_put(newBuckets, newCapacity, curr->p, curr->dist);
            curr = curr->next;
        }
        _pm_free_chain(pm->buckets[i]);
    }
    free(pm->buckets);

    pm->buckets = newBuckets;
    pm->capacity = newCapacity;
}


#define SET_LOAD_FACTOR 0.75
void pm_put(PointMap *pm, Point p, int dist) {
    if ((double)pm->size / pm->capacity >= SET_LOAD_FACTOR) {
        _pm_resize(pm);
    }
    if (_pm_put(pm->buckets, pm->capacity, p, dist)) {
        pm->size++;  // Only increment if actually puted
    }
}

/* Returns the distance corresponding to p if p is in the map.
 * Returns -1 if p is not in the map. */
int pm_get(PointMap *pm, Point p) {
    int idx = hash(&p, sizeof(p)) % pm->capacity;

    Entry *curr = pm->buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) return curr->dist;
        curr = curr->next;
    }

    return -1;
}

void pm_remove(PointMap *pm, Point p) {
    int idx = hash(&p, sizeof(p)) % pm->capacity;

    Entry *prev = NULL;
    Entry *curr = pm->buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) {
            if (prev == NULL) {
                pm->buckets[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            pm->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

#endif
