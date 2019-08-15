#ifndef CB_ARRAY_H
#define CB_ARRAY_H

#include <unistd.h>

typedef struct {
  char *array;
  size_t used;
  size_t size;
} Array;

void insertArray(Array *a, char *element);
void initArray(Array *a, size_t initialSize);
void freeArray(Array *a);
#endif
