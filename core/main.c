#include <gtk/gtk.h>

#include "../libs/libgit/git_common.h"
#include "../libs/sqlite/sqlite_util.h"
#include "../ui/window_app_indicator.h"
#include "../ui/window_settings.h"
#include "../ui/window_status.h"
#include "../util/c/cb_util.h"
#include "../util/c/cb_vector.h"

#define arrSize 200
#define bufSize 1024
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char* db_path_parent_dir;
char* db_path;

char** files = NULL;
char*** all_files = NULL;

int check_projects(char* address)
{
    int error = 0;
    git_libgit2_init();
    git_repository* rep;
    //git_status_list* statuses;

    error = git_repository_open(&rep, address);
    if (error < 0) {
        const git_error* e = giterr_last();
        printf("Error: %d : %s", e->klass, e->message);
        goto SHUTDOWN;
    }

    git_status_list* status;
    if (git_repository_is_bare(rep)) {
        fatal("Cannot report status on bare repository", git_repository_path(rep));
    }

    struct opts o = { GIT_STATUS_OPTIONS_INIT, "." };
    o.statusopt.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
    o.statusopt.flags = GIT_STATUS_OPT_INCLUDE_UNTRACKED | GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX | GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
    check_lg2(git_status_list_new(&status, rep, &o.statusopt), "Could not get status", NULL);

    const char* c;
    const git_status_entry* s;
    size_t i, maxi = git_status_list_entrycount(status);

    for (i = 0; i < maxi; ++i) {
        s = git_status_byindex(status, i);
        c = NULL;
        if (s->status == GIT_STATUS_CURRENT) {
            continue;
        }
        if (s->status == GIT_STATUS_INDEX_NEW) {
            c = s->head_to_index->new_file.path;
            vector_push_back(files, (char*)c);
        }
        if (s->status == GIT_STATUS_WT_NEW) {
            c = s->index_to_workdir->new_file.path;
            vector_push_back(files, (char*)c);
        }
        if (s->status == GIT_STATUS_WT_MODIFIED) {
            c = s->index_to_workdir->new_file.path;
            vector_push_back(files, (char*)c);
        }
    }

    vector_push_back(all_files, files);
    files = NULL;

SHUTDOWN:
    git_repository_free(rep);
    git_libgit2_shutdown();
    return 0;
}

void wipe_out_arrays()
{
    // Empty files and all_files
    if (files) {
        for (size_t i = 0; i < vector_size(files); i++) {
            vector_pop_back(files);
        }
    }

    if (all_files) {
        for (size_t i = 0; i < vector_size(all_files); i++) {
            vector_pop_back(all_files);
            for (size_t j = 0; j < vector_size(all_files[i]); j++) {
                vector_pop_back(all_files[i]);
            }
        }
        //vector_pop_back(all_files);
    }
}

void prepare_status_window(char* db_path)
{
    GList* m_list = gtk_window_list_toplevels();

    if (g_list_length(m_list) <= 3) {
        sqlite3* db;
        int db_status = open_db(&db, db_path);
        if (db_status) {
            wipe_out_arrays();

            sqlite3_stmt* stmt;
            sqlite3_prepare_v2(db, "select distinct path from projects", -1, &stmt, NULL);

            while ((sqlite3_step(stmt)) == SQLITE_ROW) {
                char* project_name = malloc(sizeof(char) * bufSize);
                memset(project_name, 0, sizeof(char) * bufSize);
                strcpy(project_name, sqlite3_column_text(stmt, 0));
                vector_push_back(files, project_name);
                check_projects(project_name);
            }
            sqlite3_finalize(stmt);
            show_status_window(&*all_files);
        } else {
            sqlite3_close(db);
        }
    }
}

static void show_status_window_Func(gpointer user_data)
{
    char* db_path = user_data;
    prepare_status_window(db_path);
}

void prepare_addresses()
{
    db_path = malloc(sizeof(char) * arrSize);
    db_path_parent_dir = malloc(sizeof(char) * arrSize);

    memset(db_path, 0, sizeof(char) * arrSize);
    memset(db_path_parent_dir, 0, sizeof(char) * arrSize);

    char* home_name = "/home/";
    char* user_name = get_current_user_name();
    char* relative_path = "/.config/captain-ballard/config.db";
    char* parent_directory = "/.config/captain-ballard";

    strcat(db_path, home_name);
    strcat(db_path, user_name);
    strcat(db_path, relative_path);

    strcat(db_path_parent_dir, home_name);
    strcat(db_path_parent_dir, user_name);
    strcat(db_path_parent_dir, parent_directory);
}

int main(int argc, char** argv)
{
    // Initialize gtk
    gtk_init(&argc, &argv);

    // create config file
    prepare_addresses();

    /* Check for configurations
     * 1. If it can't find it, create new one and open settings window
     * 2. If config file exists, it open status window
     */
    FILE* fp;
    if ((fp = fopen(db_path, "r")) == NULL) {
        struct stat st = { 0 };
        if (stat(db_path_parent_dir, &st) == -1) {
            mkdir(db_path_parent_dir, 0700);
        }
        show_settings_window(db_path);
    } else {
        // Read "period" vale from database and update combobox, but before that check if database exits or not!
        FILE* fp;
        const char* period;

        if ((fp = fopen(db_path, "r")) != NULL) {
            period = get_period(db_path);
        }
        int i_period = atoi(period) * 60 * 1000;
        //log_message_i(LOG_FILE, i_period);

        gint tag = g_timeout_add(i_period, show_status_window_Func, db_path);
    }

    // Show tray window
    show_tray_window(db_path);

    gtk_main();
    return 0;
}
