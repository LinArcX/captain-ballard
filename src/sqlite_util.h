#ifndef SQLITE_UITL_H
#define SQLITE_UITL_H

#include <sqlite3.h>

int open_db(sqlite3 **db, char* address);
int exec_sql(sqlite3 **db, char *insert_sql);

#endif
