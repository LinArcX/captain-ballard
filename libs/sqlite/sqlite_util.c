#include "../../util/cb_vector.h"

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

char** names = NULL;

static int callback(void* files, int argc, char** argv, char** azColName)
{
    return 0;
}

int open_db(sqlite3** db, char* address)
{
    int is_successfull;
    char* errMsg = 0;
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

int exec_sql(sqlite3** db, char* insert_sql)
{
    int rc;
    int is_successfull;
    char* errMsg = 0;

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

void create_settings_table(char* full_address)
{
    sqlite3* db;
    int db_status = open_db(&db, full_address);
    if (db_status) {
        // Create table
        int table_creatad = 0;
        char* create_table_projects = "CREATE TABLE projects("
                                      "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
                                      "path           TEXT    NOT NULL);";
        table_creatad = exec_sql(&db, create_table_projects);
    }
    sqlite3_close(db);
}

void add_project_path(char* full_address, char* path)
{
    sqlite3* db;
    int db_status = open_db(&db, full_address);

    if (db_status) {
        int rc;

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "insert into projects (path) values (?1);",
            -1, &stmt, NULL);

        sqlite3_bind_text(stmt, 1, path, -1, SQLITE_STATIC);

        rc = sqlite3_step(stmt);
        if (rc != SQLITE_DONE) {
            printf("ERROR inserting data: %s\n", sqlite3_errmsg(db));
            goto SHUTDOWN;
        }
        sqlite3_finalize(stmt);
    }
SHUTDOWN:
    sqlite3_close(db);
}

void wipe_out_table(char* full_address)
{
    sqlite3* db;
    int db_status = open_db(&db, full_address);
    if (db_status) {
        // Delete
        char* delete_sql = "DELETE from projects;";
        exec_sql(&db, delete_sql);
    }
    sqlite3_close(db);
}
