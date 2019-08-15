#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "cb_array.h"

void initArray(Array *a, size_t initialSize) {
    a->array = (char *)malloc(initialSize * sizeof(char*));
    a->used = 0;
    a->size = initialSize;
}

void insertArray(Array *a, char *element) {
    // a->used is the number of used entries, because a->array[a->used++] updates
    // a->used only *after* the array has been accessed. Therefore a->used can go
    // up to a->size
    if (a->used == a->size) {
        a->size *= 2;
        a->array = (char *)realloc(a->array, a->size * sizeof(char*));
    }
    a->array[a->used++] = *element;
}

void freeArray(Array *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
