/* if this is defined, then the vector will double in capacity each
 * time it runs out of space. if it is not defined, then the vector will
 * be conservative, and will have a capcity no larger than necessary.
 * having this defined will minimize how often realloc gets called.
 */
#define LOGARITHMIC_GROWTH

#include "vector.h"
#include <stdio.h>

int main(int argc, char *argv[]) {

    /* this is the variable that will store the array, you can have
     * a vector of any type! For example, you may write float *v = NULL,
     * and you'd have a vector of floats :-). NULL will have a size
     * and capacity of 0. additionally, vector_begin and vector_end will
     * return NULL on a NULL vector.
     */
    char*** projects= NULL;
    char** files = NULL;

    (void)argc;
    (void)argv;

    /* add some elements to the back */
    vector_push_back(files, "linarcx");
    vector_push_back(files, "max");
    vector_push_back(files, "james");

    /* and remove one too */
    vector_pop_back(files);

    vector_push_back(projects, files);
    if(projects) {
        for(size_t i = 0; i < vector_size(projects); ++i) {
            for(size_t j = 0; j < vector_size(files); ++j) {
                printf("v[%lu][%lu] = %s\n", i, j,  projects[i][j]);
            }
        }
    }

    /* well, we don't have destructors, so let's clean things up */
    vector_free(files);
    vector_free(projects);

    return 0;

    /* print out some stats about the vector */
    //    printf("pointer : %p\n",  (void *)files);
    //    printf("capacity: %lu\n", vector_capacity(files));
    //    printf("size    : %lu\n", vector_size(files));
    //
    //    /* iterator over the vector standard indexing too! */
    //    if(files) {
    //        size_t i;
    //        for(i = 0; i < vector_size(files); ++i) {
    //            printf("v[%lu] = %s\n", i, files[i]);
    //        }
    //    }

}

/* iterator over the vector using "iterator" style */
//if(v) {
//    char** it;
//    int i = 0;
//    for(it = vector_begin(v); it != vector_end(v); ++it) {
//        printf("v[%d] = %s\n", i, *it);
//        ++i;
//    }
//}
