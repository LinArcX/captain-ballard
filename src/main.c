#include <git2.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cb_array.h"
#include "cb_daemon.h"
#include "cb_file.h"
#include "cb_log.h"
#include "cb_nuklear.h"
#include "cb_util.h"

#define bufSize 1024
#define LOG_FILE "/home/linarcx/captain_ballard.log"

int check_projects();
int status_cb(const char *path, unsigned int status_flags, void *payload) {
    printf("\n\nflag: %d", status_flags);
    printf(", path: %s", path);
    return 0;
}

int main() {
    check_projects();
    // daemonize();
    // while (1) {
    //    sleep(10);
    //}
    return 0;
}

int check_projects() {
    git_libgit2_init();

    git_repository *rep;
    git_status_list *statuses;

    int error = 0;
    size_t count = 0;
    git_status_options opt = GIT_STATUS_OPTIONS_INIT;

    char *home_name = "/home/";
    char *user_name = get_current_user_name();
    char *primitive_path = "/.config/CaptainBallard/projects";

    char full_address[100] = "";
    strcat(full_address, home_name);
    strcat(full_address, user_name);
    strcat(full_address, primitive_path);

    //Array a;
    FILE *fp;
    char buf[bufSize];
    if ((fp = fopen(full_address, "r")) == NULL) {
        /* Open source file. */
        perror("fopen source-file");
        return 1;
    }

    //initArray(&a, 1); // initially 5 elements
    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        //insertArray(&a, buf);        // automatically resizes as necessary

        // show_nuklear_window();
        // log_message(LOG_FILE, "Time's out.\n");

        printf("\n*** %s ****", buf);
        // git open
        error = git_repository_open(&rep, buf);
        if (error < 0)
        {
            const git_error *e = giterr_last();
            printf("Error: %d : %s",  e->klass, e->message);
            goto SHUTDOWN;
        }

        // simple
        error = git_status_foreach(rep, status_cb, NULL);
        if (error < 0)
        {
            const git_error *e = giterr_last();
            printf("Error: %d : %s",  e->klass, e->message);
        }
    }
    fclose(fp);
    return 0;

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}

//printf("%s\n", buf);
//printf("Internal Size: %d", a.used);

// read_file_line_by_line(full_address, &a);
//printf("Size: %d", a.used);
// printf(*&a.array[1]);
// for(int i=0; i<a.size; i++){
//}

// read_file_line_by_line(full_address);
