#ifndef SQLITE_UITL_H
#define SQLITE_UITL_H

#include <sqlite3.h>

int open_db(sqlite3 **db, char *address);
int exec_sql(sqlite3 **db, char *insert_sql);
void create_settings_table(char *full_address);
void add_project_path(char *full_address, char path[512]);

#endif
