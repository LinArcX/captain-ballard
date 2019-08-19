#include "cb_util.h"
#include "cb_vector.h"
#include "git_common.h"
#include "sqlite_util.h"

#define bufSize 1024
#define LOGARITHMIC_GROWTH
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char** files = NULL;
char*** projects= NULL;
int check_projects();

int main() {
    sqlite3 *db;

    char *home_name = "/home/";
    char *user_name = get_current_user_name();
    char *primitive_path = "/.config/CaptainBallard/config.db";

    char full_address[100] = "";
    strcat(full_address, home_name);
    strcat(full_address, user_name);
    strcat(full_address, primitive_path);

    int db_status = open_db(&db, full_address);
    if(db_status){
        // Create table
        int table_creatad = 0;
        char *create_table_projects = "CREATE TABLE projects(" \
                                       "ID INT PRIMARY KEY     NOT NULL," \
                                       "path           TEXT    NOT NULL);";
        table_creatad = exec_sql(&db, create_table_projects);

        // Insert into table
        char *insert_sql = "INSERT INTO projects (ID,path) " \
                            "VALUES (1, '/mnt/D/WorkSpace/C/NeoDM'); " \
                            "INSERT INTO projects (ID,path) " \
                            "VALUES (2, '/mnt/D/WorkSpace/C/learning_c'); " \
                            "INSERT INTO projects (ID,path)" \
                            "VALUES (3, '/mnt/D/WorkSpace/C/CaptainBallard'); " \
                            "INSERT INTO projects (ID,path)" \
                            "VALUES (4, '/mnt/D/WorkSpace/C/ncurses_tutorial'); ";
        exec_sql(&db, insert_sql);

        sqlite3_stmt *stmt;
        int rc;

        sqlite3_prepare_v2(db, "select distinct path from projects", -1,
                &stmt, NULL);

        while ( (rc = sqlite3_step(stmt)) == SQLITE_ROW) {
            char *project_name = malloc (sizeof(char) * bufSize);
            strcpy(project_name, sqlite3_column_text(stmt, 0));
            vector_push_back(files, project_name);
            check_projects(project_name);
        }

        sqlite3_finalize(stmt);

        // daemonize();
        // while (1) {
        //    sleep(10);
        //}

        if(projects) {
            for(size_t i = 0; i < vector_size(projects); i++) {
                for(size_t j = 0; j < vector_size(projects[i]); j++) {
                    printf("v[%lu][%lu] = %s\n", i, j, projects[i][j]);
                }
            }
        }

        vector_free(projects);
        sqlite3_close(db);
        return 0;
    }else{
        sqlite3_close(db);
        return 1;
    }
}

int check_projects(char* address) {
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
SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}

//memset(&buffer[0], 0, sizeof(buffer));
//free(buffer);
//buffer = NULL;
//vector_free(files);

//    FILE *fp;
//    char* buffer;
//    buffer = malloc (bufSize * 50);
//    if ((fp = fopen(full_address, "r")) == NULL) {
//        perror("fopen source-file");
//        return 1;
//    }
//
//    while (fgets(buffer, 255, (FILE *)fp)) {
//buffer[strlen(buffer) - 1] = '\0'; // eat the newline fgets() stores


//fclose(fp);
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


//if(project_titles){
//    for(size_t i=0; i < vector_size(project_titles); i++)
//        printf("\nTitle: %s", project_titles[i]);
//}
