#include "cb_util.h"
#include "git_status.h"
#include "cb_vector.h"

#define bufSize 1024
#define LOGARITHMIC_GROWTH
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

    int error = 0;
    git_libgit2_init();
    git_repository *rep;
    git_status_list *statuses;

    FILE *fp;
    char* buffer;
    buffer = malloc (bufSize * 50);
    if ((fp = fopen(full_address, "r")) == NULL) {
        perror("fopen source-file");
        return 1;
    }

    while (fgets(buffer, 255, (FILE *)fp)) {
        buffer[strlen(buffer) - 1] = '\0'; // eat the newline fgets() stores
        vector_push_back(files, buffer);

        error = git_repository_open(&rep, files[0]);
        if (error < 0) {
            const git_error *e = giterr_last();
            printf("Error: %d : %s", e->klass, e->message);
            goto SHUTDOWN;
        }

        git_status_list *status;
        if (git_repository_is_bare(rep)){
            fatal("Cannot report status on bare repository", git_repository_path(rep));
        }
        struct opts o = {GIT_STATUS_OPTIONS_INIT, "."};
        o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
        o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
        check_lg2(git_status_list_new(&status, rep, &o.statusopt), "Could not get status", NULL);

        size_t i, maxi = git_status_list_entrycount(status);
        const git_status_entry *s;
        const char *c;

        for (i = 0; i < maxi; ++i) {
            s = git_status_byindex(status, i);
            c = NULL;
            if (s->status == GIT_STATUS_CURRENT)
                continue;
            if (s->index_to_workdir) {
                c = s->index_to_workdir->new_file.path;
            }
            vector_push_back(files, (char*)c);
        }
        vector_push_back(projects, files);
        files = NULL;
    }

    if(projects) {
        for(size_t i = 0; i < vector_size(projects); ++i) {
            for(size_t j = 0; j < vector_size(projects[i]); ++j) {
                printf("v[%lu][%lu] = %s\n", i, j, projects[i][j]);
            }
        }
    }

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();

    vector_free(files);
    vector_free(projects);

    fclose(fp);
    return 0;
}
