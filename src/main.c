#include "cb_util.h"
#include "cb_vector.h"
#include "git_common.h"
#include "nk_window.h"
#include "sqlite_util.h"

#define bufSize 1024
#define LOGARITHMIC_GROWTH
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char **files = NULL;
char ***all_files = NULL;

int check_projects();

int main() {
    char full_address[100] = "";
    char *home_name = "/home/";
    char *user_name = get_current_user_name();
    char *primitive_path = "/.config/CaptainBallard/config.db";

    strcat(full_address, home_name);
    strcat(full_address, user_name);
    strcat(full_address, primitive_path);

    FILE *fp;
    if ((fp = fopen(full_address, "r")) == NULL) {
        show_launcher_window(full_address);
        perror("can't open config file!");
        return (EXIT_FAILURE);
    } else {
        sqlite3 *db;
        int db_status = open_db(&db, full_address);
        if (db_status) {
            int rc;
            sqlite3_stmt *stmt;
            sqlite3_prepare_v2(db, "select distinct path from projects", -1, &stmt,
                    NULL);
            while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
                char *project_name = malloc(sizeof(char) * bufSize);
                strcpy(project_name, sqlite3_column_text(stmt, 0));
                vector_push_back(files, project_name);
                check_projects(project_name);
            }
            sqlite3_finalize(stmt);

            show_status_window(&*all_files);
            // daemonize();
            // while (1) {
            //    sleep(10);
            //}

            vector_free(files);
            vector_free(all_files);
            sqlite3_close(db);
            return 0;
        }
        else {
            sqlite3_close(db);
            return 1;
        }
    }
}

int check_projects(char *address) {
    int error = 0;
    git_libgit2_init();
    git_repository *rep;
    git_status_list *statuses;

    error = git_repository_open(&rep, address);
    if (error < 0) {
        const git_error *e = giterr_last();
        printf("Error: %d : %s", e->klass, e->message);
        goto SHUTDOWN;
    }

    git_status_list *status;
    if (git_repository_is_bare(rep)) {
        fatal("Cannot report status on bare repository", git_repository_path(rep));
    }

    struct opts o = {GIT_STATUS_OPTIONS_INIT, "."};
    o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED |
        GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX |
        GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
    check_lg2(git_status_list_new(&status, rep, &o.statusopt),
            "Could not get status", NULL);

    const char *c;
    const git_status_entry *s;
    size_t i, maxi = git_status_list_entrycount(status);

    for (i = 0; i < maxi; ++i) {
        s = git_status_byindex(status, i);
        c = NULL;
        if (s->status == GIT_STATUS_CURRENT) {
            continue;
        }
        if (s->status == GIT_STATUS_INDEX_NEW) {
            c = s->head_to_index->new_file.path;
            vector_push_back(files, (char *)c);
        }
        if (s->status == GIT_STATUS_WT_NEW) {
            c = s->index_to_workdir->new_file.path;
            vector_push_back(files, (char *)c);
        }
    }

    vector_push_back(all_files, files);
    files = NULL;

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}

// if (s->index_to_workdir) {
//}

// memset(&buffer[0], 0, sizeof(buffer));
// free(buffer);
// buffer = NULL;
// vector_free(files);

//    FILE *fp;
//    char* buffer;
//    buffer = malloc (bufSize * 50);
//    if ((fp = fopen(full_address, "r")) == NULL) {
//        perror("fopen source-file");
//        return 1;
//    }
//
//    while (fgets(buffer, 255, (FILE *)fp)) {
// buffer[strlen(buffer) - 1] = '\0'; // eat the newline fgets() stores

// fclose(fp);
//    // Select from a table
//    char *select_sql = "SELECT * from COMPANY";
//    exec_sql(&db, select_sql);
//
//    // Update a table
//    char *update_sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; "
//        "SELECT * from COMPANY";
//    exec_sql(&db, update_sql);
//
//    // Delete
//    char* delete_sql = "DELETE from COMPANY where ID=2; " \
//                        "SELECT * from COMPANY";
//    exec_sql(&db, delete_sql);

// if(project_titles){
//    for(size_t i=0; i < vector_size(project_titles); i++)
//        printf("\nTitle: %s", project_titles[i]);
//}

// for (size_t k = 0; k < vector_size(projects[i][j]); k++) {
// printf("vector_size(project): %d", (int)vector_size(projects));
// printf("vector_size(project[%d]): %d", (int)i,
// (int)vector_size(projects[i])); printf("vector_size(project[%d][%d]): %d", i,
// j, (int)vector_size(projects[i][j]));
// }

// vector_push_back(project, names);
// vector_push_back(projects, project);

//        if (unstaged) {
//            for (size_t i = 0; i < vector_size(unstaged); i++) {
//                for (size_t j = 0; j < vector_size(unstaged[i]); j++) {
//                    printf("unstaged[%lu][%lu] = %s\n", i, j,
//                    unstaged[i][j]);
//                }
//            }
//        }
//
//        if (staged) {
//            for (size_t i = 0; i < vector_size(staged); i++) {
//                for (size_t j = 0; j < vector_size(staged[i]); j++) {
//                    printf("staged[%lu][%lu] = %s\n", i, j, staged[i][j]);
//                }
//            }
//        }
//
//        if (project_names) {
//            for (size_t i = 0; i < vector_size(project_names); i++) {
//                printf("project[%lu] = %s\n", i, project_names[i]);
//            }
//        }
