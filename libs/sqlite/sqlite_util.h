#ifndef SQLITE_UITL_H
#define SQLITE_UITL_H
#include <sqlite3.h>

int open_db(sqlite3** db, char* db_path);
int exec_sql(sqlite3** db, char* stmt);

void create_table(char* db_path, char* stmt);
void wipe_table(char* db_path, char* stmt);

char* get_period(char* db_path);
char** get_projects(char* db_path);

void insert_path(char* db_path, char path[512]);
void insert_settings(char* db_path, char* period);
#endif
