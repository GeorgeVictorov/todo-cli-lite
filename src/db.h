#ifndef DB_H
#define DB_H

#include <sqlite3.h>

int open_database(sqlite3 **db, const char *path);

void close_database(sqlite3 *db);

int create_table(sqlite3 *db);

int execute_sql(sqlite3 *db, const char *sql);

int execute_sql_text_param(sqlite3 *db, const char *sql, const char *param);

int execute_sql_int_param(sqlite3 *db, const char *sql, int param);

int get_int_value(sqlite3 *db, const char *sql);

#endif
