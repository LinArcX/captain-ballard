#include <stdio.h>

void log_message(char *filename, char *message) {
    FILE *logfile;
    logfile = fopen(filename, "a");
    if (!logfile)
        return;
    fprintf(logfile, "%s", message);
    fclose(logfile);
}
