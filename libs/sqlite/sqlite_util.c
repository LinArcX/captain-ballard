#include "../../util/c/cb_vector.h"

#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#define LOG_FILE "/home/linarcx/captain_ballard.log"

char** names = NULL;
#define bufSize 1024

static int callback(void* files, int argc, char** argv, char** azColName)
{
    return 0;
}

int open_db(sqlite3** db, char* db_path)
{
    int is_successfull;
    char* errMsg = 0;
    int rc;

    rc = sqlite3_open(db_path, db);
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

void create_table(char* db_path, char* stmt)
{
    sqlite3* db;
    int db_status = open_db(&db, db_path);
    if (db_status) {
        exec_sql(&db, stmt);
    }
    sqlite3_close(db);
}

void wipe_table(char* db_path, char* stmt)
{
    sqlite3* db;
    int db_status = open_db(&db, db_path);
    if (db_status) {
        exec_sql(&db, stmt);
    }
    sqlite3_close(db);
}

char** get_projects(char* db_path)
{
    char** paths = NULL;
    sqlite3* db;
    int db_status = open_db(&db, db_path);

    if (db_status) {
        int rc;

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "select DISTINCT path from projects", -1, &stmt, NULL);
        while ((sqlite3_step(stmt)) == SQLITE_ROW) {
            char* project_name = malloc(sizeof(char) * bufSize);
            memset(project_name, 0, sizeof(char) * bufSize);
            strcpy(project_name, sqlite3_column_text(stmt, 0));
            vector_push_back(paths, project_name);
        }
        sqlite3_finalize(stmt);
    }
SHUTDOWN:
    sqlite3_close(db);
    return paths;
}

char* get_period(char* db_path)
{
    sqlite3* db;
    int db_status = open_db(&db, db_path);

    char* period = malloc(sizeof(char) * bufSize);
    memset(period, 0, sizeof(char) * bufSize);

    if (db_status) {
        int rc;

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "select DISTINCT period from settings", -1, &stmt, NULL);
        while ((sqlite3_step(stmt)) == SQLITE_ROW) {
            strcpy(period, sqlite3_column_text(stmt, 0));
        }
        sqlite3_finalize(stmt);
    }
SHUTDOWN:
    sqlite3_close(db);

    return period;
}

void insert_path(char* db_path, char* path)
{
    sqlite3* db;
    int db_status = open_db(&db, db_path);

    if (db_status) {
        int rc;

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "insert into projects (path) values (?1);", -1, &stmt, NULL);
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

void insert_settings(char* db_path, char* period)
{
    sqlite3* db;
    int db_status = open_db(&db, db_path);

    if (db_status) {
        int rc;

        sqlite3_stmt* stmt;
        sqlite3_prepare_v2(db, "insert into settings (period) values (?1);", -1, &stmt, NULL);
        sqlite3_bind_text(stmt, 1, period, -1, SQLITE_STATIC);

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
