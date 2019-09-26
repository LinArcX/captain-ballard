#include <gtk/gtk.h>

#include "../libs/libgit/git_common.h"
#include "../libs/sqlite/sqlite_util.h"
#include "../ui/window_app_indicator.h"
#include "../ui/window_settings.h"
#include "../ui/window_status.h"
#include "../util/cb_util.h"
#include "../util/cb_vector.h"

#define arrSize 200
#define bufSize 1024
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char* address;
char* full_address;
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

void prepare_status_window(char* full_address)
{
    GList* m_list = gtk_window_list_toplevels();
    if (g_list_length(m_list) <= 3) {
        sqlite3* db;
        int db_status = open_db(&db, full_address);
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
    char* full_address = user_data;
    prepare_status_window(full_address);
}

void prepare_addresses()
{
    full_address = malloc(sizeof(char) * arrSize);
    address = malloc(sizeof(char) * arrSize);

    memset(full_address, 0, sizeof(char) * arrSize);
    memset(address, 0, sizeof(char) * arrSize);

    char* home_name = "/home/";
    char* user_name = get_current_user_name();
    char* primitive_path = "/.config/CaptainBallard/config.db";
    char* primitive_directory = "/.config/CaptainBallard";

    strcat(full_address, home_name);
    strcat(full_address, user_name);
    strcat(full_address, primitive_path);

    strcat(address, home_name);
    strcat(address, user_name);
    strcat(address, primitive_directory);
}

int main(int argc, char** argv)
{
    // Initialize gtk
    gtk_init(&argc, &argv);

    // Show tray window
    show_tray_window(full_address);

    // create config file
    prepare_addresses();

    /* Check for configurations
     * 1. If it can't find it, create new one and open settings window
     * 2. If config file exists, it open status window
     */
    FILE* fp;
    if ((fp = fopen(full_address, "r")) == NULL) {
        struct stat st = { 0 };
        if (stat(address, &st) == -1) {
            mkdir(address, 0700);
        }
        show_settings_window(full_address);
    } else {
        gint tag = g_timeout_add(5000, show_status_window_Func, full_address);
    }

    gtk_main();
    return 0;
}
