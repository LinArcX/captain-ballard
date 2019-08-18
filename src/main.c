#include "cb_util.h"
#include "git_status.h"
#include "cb_vector.h"

#define bufSize 1024
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char*** projects= NULL;
char** files = NULL;
int check_projects();

int main() {
    check_projects();
    // daemonize();
    // while (1) {
    //    sleep(10);
    //}
    return 0;
}

int check_projects() {
    char *home_name = "/home/";
    char *user_name = get_current_user_name();
    char *primitive_path = "/.config/CaptainBallard/projects";

    char full_address[100] = "";
    strcat(full_address, home_name);
    strcat(full_address, user_name);
    strcat(full_address, primitive_path);

    FILE *fp;
    char buf[bufSize];
    if ((fp = fopen(full_address, "r")) == NULL) {
        perror("fopen source-file");
        return 1;
    }

    int error = 0;
    git_libgit2_init();
    git_repository *rep;
    git_status_list *statuses;

    while (fgets(buf, sizeof(buf), fp) != NULL) {
        buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
        vector_push_back(files, buf);
        show_status_of_git_repo(files);
        vector_push_back(projects, files);
    }


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

    fclose(fp);
    return 0;
}

//#include <git2.h>
//#include <stdio.h>
//#include <string.h>
//#include <unistd.h>

//#include "cb_array.h"
//#include "cb_daemon.h"
//#include "cb_file.h"
//#include "cb_log.h"
//#include "cb_nuklear.h"


//for(int j=0; j < vector.size; j++){
//    printf("\nString at: %d is %s", j, vector.data[j]);
//}

//char** project_address = (char** ) malloc (1 * sizeof(char*));
//project_address[0] = (char* ) malloc (256 * sizeof(char));
//strcpy(project_address[0], "/mnt/D/WorkSpace/C/CaptainBallard");
//show_status_of_git_repo(project_address);


//    // show_nuklear_window();
//    // log_message(LOG_FILE, "Time's out.\n");
//printf("\n*** %s ****", buf);






//SHUTDOWN:
//    git_repository_free(rep);
//    git_libgit2_shutdown();
//    return 0;




//error = git_repository_open(&rep, buf);
//if (error < 0) {
//    const git_error *e = giterr_last();
//    printf("Error: %d : %s", e->klass, e->message);
//    goto SHUTDOWN;
//}

//        git_status_list *status;
//        struct opts o = {GIT_STATUS_OPTIONS_INIT, "."};
//
//        o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
//        o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
//            GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
//            GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
//
//        parse_opts(&o, argc, argv);
//
//        if (git_repository_is_bare(repo))
//            fatal("Cannot report status on bare repository", git_repository_path(repo));
//
//show_status:
//        if (o.repeat)
//            printf("\033[H\033[2J");
//
//
//
//        lg2_status(rep, 1, address);


//int status_cb(const char *path, unsigned int status_flags, void *payload) {
//    printf("\n\nflag: %d", status_flags);
//    printf(", path: %s", path);
//    return 0;
//}


////size_t count = 0;
//int error = 0;
//git_status_options opt = GIT_STATUS_OPTIONS_INIT;

//git_libgit2_init();
//git_repository *rep;
//git_status_list *statuses;

////initArray(&a, 1); // initially 5 elements
//while (fgets(buf, sizeof(buf), fp) != NULL) {
//    buf[strlen(buf) - 1] = '\0'; // eat the newline fgets() stores
//    //insertArray(&a, buf);        // automatically resizes as necessary

//    // show_nuklear_window();
//    // log_message(LOG_FILE, "Time's out.\n");

//    printf("\n*** %s ****", buf);
//    // git open
//    error = git_repository_open(&rep, buf);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        printf("Error: %d : %s",  e->klass, e->message);
//        goto SHUTDOWN;
//    }

//    // simple
//    error = git_status_foreach(rep, status_cb, NULL);
//    if (error < 0)
//    {
//        const git_error *e = giterr_last();
//        printf("Error: %d : %s",  e->klass, e->message);
//    }
//}
//fclose(fp);
//return 0;

//SHUTDOWN:
//    git_repository_free(rep);
//    git_libgit2_shutdown();
//    return 0;





//printf("%s\n", buf);
//printf("Internal Size: %d", a.used);

// read_file_line_by_line(full_address, &a);
//printf("Size: %d", a.used);
// printf(*&a.array[1]);
// for(int i=0; i<a.size; i++){
//}

// read_file_line_by_line(full_address);


//char** project_address;
//project_address = (char**)malloc(2048);
//project_address[0] = "/mnt/D/WorkSpace/C/NeoDM";



//Array a;
// declare and initialize a new vector
//Vector vector;
//vector_init(&vector);


/* add some elements to the back */
//vector_push_back(files, "linarcx");
//vector_push_back(files, "max");
//vector_push_back(files, "james");
//vector_pop_back(files);


