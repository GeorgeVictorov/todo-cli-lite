#include <stdio.h>
#include "cmd_handler.h"
#include "constants.h"
#include "db.h"

int main(const int argc, char *argv[]) {
    sqlite3 *db;
    if (open_database(&db, DATABASE_PATH) != SQLITE_OK) {
        fprintf(stderr, "Failed to open database.\n");
        return 1;
    }

    create_table(db);

    handle_command(db, argc, argv);

    close_database(db);
    return 0;
}
