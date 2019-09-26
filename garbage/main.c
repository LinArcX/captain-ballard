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
