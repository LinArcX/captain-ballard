#include <libappindicator3-0.1/libappindicator/app-indicator.h>

#include "../libs/libgit/git_common.h"
#include "../libs/sqlite/sqlite_util.h"

#include "../util/cb_util.h"
#include "../util/cb_vector.h"

#include "../ui/cb_window.h"
#include "../ui/system_tray.h"

#define bufSize 1024
#define arrSize 200
#define LOG_FILE "/home/linarcx/captain_ballard.log"

char* address;
char* full_address;

char** files = NULL;
char*** all_files = NULL;

void m_item_close_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    gtk_main_quit();
    //gtk_widget_destroy(window);
}

void m_item_settings_selected(GtkMenuItem* menuitem, gpointer user_data)
{
    show_launcher_window(full_address);
}

void show_tray_window()
{
    GtkWidget* m_item_settings;
    GtkWidget* m_item_close;
    GtkWidget* m_item_separator;
    AppIndicator* indicator;
    GtkWidget* menu;

    menu = gtk_menu_new();

    GtkWidget* box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    GtkWidget* icon = gtk_image_new_from_file("../assets/gears.png"); //, GTK_ICON_SIZE_MENU); //folder-music-symbolic
    gtk_image_set_pixel_size(icon, 8);
    GtkWidget* label = gtk_label_new("Settings");
    m_item_settings = gtk_menu_item_new();
    gtk_container_add(GTK_CONTAINER(box), icon);
    gtk_container_add(GTK_CONTAINER(box), label);
    gtk_container_add(GTK_CONTAINER(m_item_settings), box);

    m_item_close = gtk_menu_item_new_with_label("Close");
    m_item_separator = gtk_separator_menu_item_new();

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_settings);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_separator);
    gtk_menu_shell_append(GTK_MENU_SHELL(menu), m_item_close);

    g_signal_connect(m_item_settings, "activate", G_CALLBACK(m_item_settings_selected), NULL);
    g_signal_connect(m_item_close, "activate", G_CALLBACK(m_item_close_selected), NULL);

    gchar* icon_name = "../assets/captain-cap.png"; //"indicator-messages"
    indicator = app_indicator_new("example-simple-client", icon_name, APP_INDICATOR_CATEGORY_APPLICATION_STATUS);
    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_attention_icon(indicator, "indicator-messages-new");
    app_indicator_set_menu(indicator, GTK_MENU(menu));

    gtk_widget_show_all(menu);
}

void prepare_status_window(char* full_address)
{
    sqlite3* db;
    int db_status = open_db(&db, full_address);
    if (db_status) {
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

int check_projects(char* address)
{
    int error = 0;
    git_libgit2_init();
    git_repository* rep;
    git_status_list* statuses;

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
    check_lg2(git_status_list_new(&status, rep, &o.statusopt),
        "Could not get status",
        NULL);

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
    show_tray_window();

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
        show_launcher_window(full_address);
    } else {
        gint tag = g_timeout_add(5000, show_status_window_Func, full_address);
    }

    gtk_main();
    return 0;
}

//void setTimeout(int milliseconds)
//{
//    // If milliseconds is less or equal to 0
//    // will be simple return from function without throw error
//    if (milliseconds <= 0) {
//        fprintf(stderr, "Count milliseconds for timeout is less or equal to 0\n");
//        return;
//    }
//
//    // a current time of milliseconds
//    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
//
//    // needed count milliseconds of return from this timeout
//    int end = milliseconds_since + milliseconds;
//
//    // wait while until needed time comes
//    do {
//        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
//    } while (milliseconds_since <= end);
//}
//
//void repetitive_task()
//{
//    int delay = 5;
//start_point:
//    // counter downtime for run a rocket while the delay with more 0
//    do {
//        // erase the previous line and display remain of the delay
//        printf("\033[ATime left for run rocket: %d\n", delay);
//
//        // a timeout for display
//        setTimeout(1000);
//
//        // decrease the delay to 1
//        delay--;
//
//    } while (delay >= 0);
//
//    show_simple_window();
//    delay = 5;
//    goto start_point;
//}

//#define interval 8 // serve as seconds
//volatile int breakflag = interval; // number of times the handle will run
//
//void handle(int sig)
//{
//    //printf("Hello\n");
//    --breakflag;
//    alarm(1); // fire alarm and it's handler again
//}

//int repetitive_task()
//{
//test:
//    signal(SIGALRM, handle); // register signal handler
//
//    alarm(1); // fire first alarm
//
//    // serve as one second delay
//    while (breakflag) {
//        sleep(1);
//    }
//
//    // reset alarm clock
//    printf("resetting...\n");
//    show_main_window();
//
//    if (!breakflag) {
//        breakflag = interval;
//        goto test;
//    }
//
//    //printf("done\n");
//    return 0;
//}

//char full_address[100] = "";
//char address[100] = "";

//printf("address is: %s\n", address);
//printf("full_address is: %s\n", full_address);
//log_message(LOG_FILE, full_address);
//log_message(LOG_FILE, "\n");

//pthread_t tid_tray_window;
//pthread_t tid_status_window;
//pthread_create(&tid_tray_window, NULL, show_main_window, NULL);

//pthread_create(&tid_status_window, NULL, status_window_func, NULL);

////pthread_join(tid_tray_window, NULL);
////show_system_tray_window();

//show_launcher_window("sd");
//show_simple_window();
//show_status_window("a");

//pthread_join(tid_tray_window, NULL);

//pthread_exit(NULL);

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

// perror("can't open config file!\n");
// printf("main full_address: %s\n", full_address);

//                FILE* fpp;
//                fpp = fopen("/home/linarcx/cap.txt", "a+");
//daemon(1, 0);
//fprintf(fpp, "s");

//#define LOGARITHMIC_GROWTH

//if (all_files) {
//    log_message(LOG_FILE, "all_files not empty!\n");
//}

//daemonize();
//log_message(LOG_FILE, "!\n");
//if_label:
//    if (sqlite3_step(stmt) == SQLITE_ROW) {
//        log_message(LOG_FILE, "projects table has row!\n");
//        goto if_label;
//    }

//free(stmt);

//int condition = 1;
//int counter = 0;
//while (condition > 0) {
//    log_message(LOG_FILE, "while..");
//    counter++;
//    if (counter = 4) {
//        condition = 0;
//    }
//}
//log_message(LOG_FILE, "\n");

//for (int i = 0; i < 5; i++) {
//    log_message(LOG_FILE, "i..");
//}
//log_message(LOG_FILE, "\n");

//if (files) {
//    vector_free(files);
//}
//if (all_files) {
//    //vector_free(all_files);
//}

//sqlite3_close(db);
//return 0;

//////////////////////////////
//void* status_window_func(void* vargp)
//{

//int main(int argc, char** argv)
//{
//    //pthread_t tid_tray_window;
//    //pthread_t tid_status_window;
//    //pthread_create(&tid_tray_window, NULL, tray_window_func, NULL);
//    //pthread_create(&tid_status_window, NULL, status_window_func, NULL);
//
//    ////pthread_exit(NULL);
//    ////pthread_join(tid_tray_window, NULL);
//    //pthread_join(tid_status_window, NULL);
//    ////show_system_tray_window();
//}

//void* tray_window_func(void* vargp)
//{
//    show_system_tray_window();
//    //show_launcher_window("as");
//}

//void* tray_window_func(void* vargp)
//{
//    printf("************************");
//    //show_system_tray_window();
//    //show_launcher_window("as");
//}

// create_settings_table("/home/linarcx/.config/CaptainBallard/config.db");

//////////////////////////////////////
//void show_main_window()
//{
//    char* full_address = malloc(sizeof(char) * arrSize);
//    char* address = malloc(sizeof(char) * arrSize);
//
//    memset(full_address, 0, sizeof(char) * arrSize);
//    memset(address, 0, sizeof(char) * arrSize);
//
//    char* home_name = "/home/";
//    char* user_name = get_current_user_name();
//    char* primitive_path = "/.config/CaptainBallard/config.db";
//    char* primitive_directory = "/.config/CaptainBallard";
//
//    strcat(full_address, home_name);
//    strcat(full_address, user_name);
//    strcat(full_address, primitive_path);
//
//    strcat(address, home_name);
//    strcat(address, user_name);
//    strcat(address, primitive_directory);
//
//    FILE* fp;
//    if ((fp = fopen(full_address, "r")) == NULL) {
//        struct stat st = { 0 };
//        if (stat(address, &st) == -1) {
//            mkdir(address, 0700);
//        }
//        show_launcher_window(full_address);
//    } else {
//        sqlite3* db;
//        int db_status = open_db(&db, full_address);
//        if (db_status) {
//            //// Empty files and all_files
//            //if (files) {
//            //    for (size_t i = 0; i < vector_size(files); i++) {
//            //        vector_pop_back(files);
//            //    }
//            //}
//
//            //if (all_files) {
//            //    for (size_t i = 0; i < vector_size(all_files); i++) {
//            //        vector_pop_back(all_files);
//            //        for (size_t j = 0; j < vector_size(all_files[i]); j++) {
//            //            vector_pop_back(all_files[i]);
//            //        }
//            //    }
//            //    vector_pop_back(all_files);
//            //}
//
//            sqlite3_stmt* stmt;
//            sqlite3_prepare_v2(db, "select distinct path from projects", -1, &stmt, NULL);
//
//            while ((sqlite3_step(stmt)) == SQLITE_ROW) {
//                char* project_name = malloc(sizeof(char) * bufSize);
//                memset(project_name, 0, sizeof(char) * bufSize);
//
//                strcpy(project_name, sqlite3_column_text(stmt, 0));
//                vector_push_back(files, project_name);
//                check_projects(project_name);
//            }
//            sqlite3_finalize(stmt);
//            show_status_window(&*all_files);
//        } else {
//            sqlite3_close(db);
//        }
//    }
//}
