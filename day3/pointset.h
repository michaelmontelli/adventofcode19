#ifndef POINTSET_H
#define POINTSET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y;
} Point;

typedef struct SetEntry {
    Point p;
    struct SetEntry *next;
} SetEntry;

typedef struct {
    SetEntry **buckets;
    size_t size;
    size_t capacity;
} PointSet;

/* Creates PointSet structure, with buckets zeroed out--each bucket
 * is thus initially set to NULL */
#define PS_INITIAL_CAPACITY 2
PointSet *ps_create() {
    PointSet *ps = malloc(sizeof(*ps));
    if (ps == NULL) {
        perror("Error allocating PointSet");
        exit(1);
    }

    // Initialize each pointer to NULL
    ps->buckets = calloc(PS_INITIAL_CAPACITY, sizeof(ps->buckets[0]));
    if (ps->buckets == NULL) {
        perror("Error allocating PointSet buckets");
        exit(1);
    }

    ps->capacity = PS_INITIAL_CAPACITY;
    ps->size = 0;

    return ps;
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

static SetEntry *createSetEntry(Point p) {
    SetEntry *entry = malloc(sizeof(*entry));
    if (entry == NULL) {
        perror("Error allocating SetEntry");
        exit(1);
    }

    entry->p = p;
    entry->next = NULL;
    return entry;
}

static void _ps_free_chain(SetEntry *entry) {
    if (entry == NULL) return;
    _ps_free_chain(entry->next);

    free(entry);
}

void ps_free(PointSet *ps) {
    for (int i = 0; i < ps->capacity; i++) {
        _ps_free_chain(ps->buckets[i]);
    }
    free(ps->buckets);
    free(ps);
}

int point_equals(Point a, Point b) {
    // Be careful doing memcmp
    // memcmp(&a, &b, sizeof(Point)) == 0
    //
    // This works for Point, but won't work if
    // there are extra bytes for alignment filled with garbage
    return a.x == b.x && a.y == b.y;
}

static int _ps_add(SetEntry **buckets, size_t capacity, Point p) {
    int idx = hash(&p, sizeof(p)) % capacity;
    if (buckets[idx] == NULL) {
        buckets[idx] = createSetEntry(p);
        return 1;
    }

    SetEntry *prev = NULL;
    SetEntry *curr = buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) return 0;
        prev = curr;
        curr = curr->next;
    }

    prev->next = createSetEntry(p);
    return 1;
}

static void _ps_resize(PointSet *ps) {
    int newCapacity = ps->capacity * 2;
    SetEntry **newBuckets = calloc(newCapacity, sizeof(*newBuckets));
    if (newBuckets == NULL) {
        perror("Error allocating new buckets during resize");
        exit(1);
    }

    for (int i = 0; i < ps->capacity; i++) {
        SetEntry *curr = ps->buckets[i];
        
        while (curr) {
            _ps_add(newBuckets, newCapacity, curr->p);
            curr = curr->next;
        }
        _ps_free_chain(ps->buckets[i]);
    }
    free(ps->buckets);

    ps->buckets = newBuckets;
    ps->capacity = newCapacity;
}


#define SET_LOAD_FACTOR 0.75
void ps_add(PointSet *ps, Point p) {
    if ((double)ps->size / ps->capacity >= SET_LOAD_FACTOR) {
        _ps_resize(ps);
    }
    if (_ps_add(ps->buckets, ps->capacity, p)) {
        ps->size++;  // Only increment if actually added
    }
}

int ps_find(PointSet *ps, Point p) {
    int idx = hash(&p, sizeof(p)) % ps->capacity;
    if (ps->buckets[idx] == NULL) {
        return 0;
    }

    SetEntry *curr = ps->buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) return 1;
        curr = curr->next;
    }

    return 0;
}

void ps_remove(PointSet *ps, Point p) {
    int idx = hash(&p, sizeof(p)) % ps->capacity;
    if (ps->buckets[idx] == NULL) {
        return;
    }

    SetEntry *prev = NULL;
    SetEntry *curr = ps->buckets[idx]; 
    while (curr) {
        if (point_equals(curr->p, p)) {
            if (prev == NULL) {
                ps->buckets[idx] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr);
            ps->size--;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

#endif
