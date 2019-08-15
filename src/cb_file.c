#include "cb_array.h"
#include <stdio.h>
#include <string.h>

#define bufSize 1024

void write_char_to_file(char *address, char *str) {
    FILE *fptr;
    fptr = fopen(address, "a");
    fputs(str, fptr);
    fclose(fptr);
}

int read_file_line_by_line(char *path, Array *a) {
    FILE *fp;
    char buf[bufSize];
    if ((fp = fopen(path, "r")) == NULL) {
        /* Open source file. */
        perror("fopen source-file");
        return 1;
    }

    initArray(a, 1); // initially 5 elements
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        insertArray(a, &buf);         // automatically resizes as necessary
        printf("%s\n", buf);
        printf("Internal Size: %d", a->used);
    }
    fclose(fp);
    return 0;
}

//  int i = 0;
//  for (i = 0; i < 100; i++) {
//    insertArray(a, i); // automatically resizes as necessary
//  }

// void read_file_line_by_line_ref(Array *a) {
//  int i;
//  initArray(a, 5); // initially 5 elements
//  for (i = 0; i < 100; i++) {
//    insertArray(a, i); // automatically resizes as necessary
//  }
//  //  printf("%d\n", a->array[9]); // print 10th element
//  //  printf("%d\n", a->used);     // print number of elements
//  //  freeArray(a);
//}
