#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "constants.h"
#include "todo.h"


Command get_cmd(const char *cmd) {
    if (strcmp(cmd, "add") == 0) return CMD_ADD;
    if (strcmp(cmd, "ls") == 0) return CMD_LS;
    if (strcmp(cmd, "done") == 0) return CMD_DONE;
    if (strcmp(cmd, "report") == 0) return CMD_REPORT;
    if (strcmp(cmd, "del") == 0) return CMD_DEL;
    if (strcmp(cmd, "help") == 0) return CMD_HELP;
    return CMD_UNKNOWN;
}


void handle_command(sqlite3 *db, int argc, char *argv[]) {
    if (argc < 2)
        help();

    const Command cmd = get_cmd(argv[1]);

    if ((cmd == CMD_ADD || cmd == CMD_DONE || cmd == CMD_DEL) && argc < 3) {
        printf("\nError: missing argument for '%s'\n", argv[1]);
        help();
    }

    switch (cmd) {
        case CMD_ADD:
            add_task(db, argv[2]);
            break;
        case CMD_LS:
            list_tasks(db);
            break;
        case CMD_DONE:
            mark_task_done(db, atoi(argv[2]));
            break;
        case CMD_REPORT:
            print_report(db);
            break;
        case CMD_DEL:
            delete_task(db, atoi(argv[2]));
            break;
        case CMD_HELP:
            help();
        case CMD_UNKNOWN:
        default:
            printf("\nError: Unknown command '%s'\n", argv[1]);
            help();
    }
}
