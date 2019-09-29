#include <stdio.h>

void log_message(char* filename, char* message)
{
    FILE* logfile;
    logfile = fopen(filename, "a");
    if (!logfile)
        return;
    fprintf(logfile, "%s", message);
    fclose(logfile);
}

void log_message_i(char* filename, int message)
{
    FILE* logfile;
    logfile = fopen(filename, "a");
    if (!logfile)
        return;
    fprintf(logfile, "%d", message);
    fclose(logfile);
}
