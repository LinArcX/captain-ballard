#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include <fcntl.h>
#include <linux/fs.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int cb_deamon(void)
{
    pid_t pid;
    int i;

    /* create new process */
    pid = fork();
    if (pid == -1)
        return -1;
    else if (pid != 0)
        exit(EXIT_SUCCESS);

    /* create new session and process group */
    if (setsid() == -1)
        return -1;

    /* set the working directory to the root directory */
    if (chdir("/") == -1)
        return -1;

    /* close all open files--NR_OPEN is overkill, but works */
    //for (i = 0; i < NR_OPEN; i++)
    //    close(i);

    /* redirect fd's 0,1,2 to /dev/null */
    open("/dev/null", O_RDWR);
    /* stdin */
    dup(0);
    /* stdout */
    dup(0);
    /* stderror */

    /* do its daemon thing... */

    return 0;
}

void daemonize()
{
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
    //umask(027);
}
