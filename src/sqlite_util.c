#include "cb_vector.h"
#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

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
