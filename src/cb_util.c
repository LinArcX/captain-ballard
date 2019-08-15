#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

void concatenate(char s1[], char s2[]) {
    int i, j;

    i = 0;
    while (s1[i] != '\0') {
        i++;
    }

    j = 0;
    while (s2[j] != '\0') {
        s1[i] = s2[j];
        i++;
        j++;
    }
    s1[i] = '\0';
}

char *get_current_user_name() {
    char *p = getenv("USER");
    if (p == NULL) {
        return " ";
    } else {
        return p;
    }
}
