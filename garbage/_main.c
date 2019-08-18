#include <stdio.h>

#include "my_vector_char.h"

int main() {
    // declare and initialize a new vector
    Vector vector;
    vector_init(&vector);

    // fill it up with 150 arbitrary values
    // this should expand capacity up to 200
    int i;
    for (i = 0; i < 10; i++) {
        vector_append(&vector, "hi");
    }

    // set a value at an arbitrary index
    // this will expand and zero-fill the vector to fit
    vector_set(&vector, 13, "Bye");

    // print out an arbitrary value in the vector
    printf("Heres the value at 8: %s\n", vector_get(&vector, 8));

    for(int j=0; j< vector.size; j++){
        printf("\nString at: %d is %s", j, vector.data[j]);
    }

    // we're all done playing with our vector,
    // so free its underlying data array
    vector_free(&vector);
}
