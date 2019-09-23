#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void daemonize() {
    int i, lfp;
    char str[10];

    if (getppid() == 1)
        return;
    i = fork();

    if (i < 0)
        exit(1);
    if (i > 0)
        exit(0);
    setsid();

    for (i = getdtablesize(); i >= 0; --i)
        close(i);

    i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);
    umask(027);
}
