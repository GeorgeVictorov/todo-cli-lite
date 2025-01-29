#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "todo.h"
#include "db.h"


void add_task(sqlite3 *db, const char *description) {
    if (!description || strlen(description) == 0) {
        fprintf(stderr, "Error: Task description cannot be empty.\n");
        return;
    }

    const char *sql = "INSERT INTO tasks (description) VALUES (?);";
    const int rc = execute_sql_text_param(db, sql, description);
    if (rc != SQLITE_DONE)
        printf("Error: '%s'", sqlite3_errmsg(db));
}


void mark_task_done(sqlite3 *db, int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "UPDATE tasks SET status = 1 WHERE task_id = ?;";
    const int rc = execute_sql_int_param(db, sql, task_id);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
}


void delete_task(sqlite3 *db, const int task_id) {
    if (task_id <= 0) {
        fprintf(stderr, "Error: Invalid task ID.\n");
        return;
    }

    const char *sql = "DELETE FROM tasks WHERE task_id = ?;";
    const int rc = execute_sql_int_param(db, sql, task_id);

    if (rc != SQLITE_DONE) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
    }
}

void list_tasks(sqlite3 *db) {
    const char *sql = "SELECT task_id, description, status, created_at FROM tasks;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("\nID  | Description                 | Status   | Created At\n");
    printf("------------------------------------------------------------------\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int const task_id = sqlite3_column_int(stmt, 0);
        const char *description = (const char *) sqlite3_column_text(stmt, 1);
        int const status = sqlite3_column_int(stmt, 2);
        const char *created_at = (const char *) sqlite3_column_text(stmt, 3);

        printf("%-3d | %-27.27s | %-8s | %s\n",
               task_id,
               description ? description : "(no description)",
               status == 0 ? "Pending" : "Done",
               created_at ? created_at : "(unknown)");
    }
    printf("\n");
    sqlite3_finalize(stmt);
}


void print_report(sqlite3 *db) {
    const char *sql_done = "SELECT COUNT(*) FROM tasks WHERE status = 1;";
    const char *sql_pending = "SELECT COUNT(*) FROM tasks WHERE status = 0;";

    int done_count = get_int_value(db, sql_done);
    int pending_count = get_int_value(db, sql_pending);

    printf("\n  Task Report:\n");
    printf("----------------\n");
    printf("Status  |  Count\n");
    printf("----------------\n");
    printf("Done    |  %d\n", done_count);
    printf("Pending |  %d\n", pending_count);
}


void help() {
    printf("\nWelcome to the Todo Command Line Interface Task Manager!\n");
    printf("\nUsage:");
    printf("\n$ ./todo add \"todo item\"  # Add a new todo ");
    printf("\n$ ./todo ls               # Show remaining todos");
    printf("\n$ ./todo del NUMBER       # Delete a todo");
    printf("\n$ ./todo done NUMBER      # Complete a todo");
    printf("\n$ ./todo help             # Show usage");
    printf("\n$ ./todo report           # Statistics\n");
    exit(1);
}
