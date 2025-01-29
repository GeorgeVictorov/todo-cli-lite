#ifndef CONSTANTS_H
#define CONSTANTS_H

#define DATABASE_PATH "tasks.db"
#define TABLE_NAME "tasks"

typedef enum {
    CMD_ADD,
    CMD_LS,
    CMD_DONE,
    CMD_REPORT,
    CMD_DEL,
    CMD_HELP,
    CMD_UNKNOWN
} Command;


#endif
