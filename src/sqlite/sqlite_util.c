#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include "../util/cb_vector.h"

char** names = NULL;

static int callback(void *files, int argc, char **argv, char **azColName) {
    return 0;
}

int open_db(sqlite3 **db, char *address) {
    int is_successfull;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open(address, db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(*db));
        is_successfull = 0;
    } else {
        fprintf(stderr, "Opened database successfully\n");
        is_successfull = 1;
    }
    return is_successfull;
}

int exec_sql(sqlite3 **db, char *insert_sql) {
    int rc;
    int is_successfull;
    char *errMsg = 0;

    rc = sqlite3_exec(*db, insert_sql, callback, 0, &errMsg);

    if (rc != SQLITE_OK) {
        is_successfull = 0;
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        is_successfull = 1;
        fprintf(stdout, "Done!\n");
    }
    return is_successfull;
}

void create_settings_table(char *full_address) {
    sqlite3 *db;
    int db_status = open_db(&db, full_address);
    if (db_status) {
        // Create table
        int table_creatad = 0;
        char *create_table_projects = "CREATE TABLE projects("
            "ID INT PRIMARY KEY     NOT NULL,"
            "path           TEXT    NOT NULL);";
        table_creatad = exec_sql(&db, create_table_projects);
    }
    sqlite3_close(db);
}

void add_project_path(char *full_address, char path[512]) {
    sqlite3 *db;
    int db_status = open_db(&db, full_address);

    char * pch;
    pch = strtok (path,"\n");
    vector_push_back(names, pch);
    while (pch != NULL)
    {
        pch = strtok (NULL, "\n");
        vector_push_back(names, pch);
    }

    if (db_status && vector_size(names)>0) {
        for (size_t i = 0; i < sizeof(names); i++) {
            int rc;

            sqlite3_stmt *stmt;
            sqlite3_prepare_v2(db, "insert into projects (ID, path) values (?1, ?2);",
                    -1, &stmt, NULL);

            sqlite3_bind_int(stmt, 1, i);
            sqlite3_bind_text(stmt, 2, names[i], -1, SQLITE_STATIC);

            rc = sqlite3_step(stmt);
            if (rc != SQLITE_DONE) {
                printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
                goto SHUTDOWN;
            }
            sqlite3_finalize(stmt);
        }
    }
SHUTDOWN:
    sqlite3_close(db);
}

// char *insert_sql = "INSERT INTO projects (ID,path) "
//                   "VALUES (1, '/mnt/D/WorkSpace/C/NeoDM'); "
//                   "INSERT INTO projects (ID,path) "
//                   "VALUES (2, '/mnt/D/WorkSpace/C/learning_c'); "
//                   "INSERT INTO projects (ID,path)"
//                   "VALUES (3, '/mnt/D/WorkSpace/C/CaptainBallard'); "
//                   "INSERT INTO projects (ID,path)"
//                   "VALUES (4, '/mnt/D/WorkSpace/C/ncurses_tutorial');
//                   ";
// exec_sql(&db, insert_sql);


//    int i = 0;
//    char *p = strtok (path, "\n");
//    char *array[50];
//
//    while (p != NULL)
//    {
//        vector_push_back(names, p);
//        array[i++] = p;
//        p = strtok (NULL, "/");
//    }
//
//    for (size_t i = 0; i < vector_size(names); i++) {
//        printf("names[%lu] = %s\n", i, names[i]);
//    }


//printf("%lu: ",sizeof(array)/sizeof(array[0]) );
//for (i = 0; i < sizeof(array)/sizeof(array[0]); ++i)
//    printf("%s\n", array[i]);


//if(vector_size(names)>0){
//    for (size_t i = 0; i < vector_size(names); i++) {
//        printf("names[%lu] = %s\n", i, names[i]);
//    }
//}
