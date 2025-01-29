#include <CUnit/Basic.h>
#include "db.h"

void test_execute_sql() {
    sqlite3 *db;
    open_database(&db, "test.db");

    const int rc = create_table(db);
    CU_ASSERT(rc == SQLITE_OK);

    close_database(db);
}

void test_execute_sql_text_param() {
    sqlite3 *db;
    open_database(&db, "test.db");


    execute_sql_text_param(db, "INSERT INTO tasks (description) VALUES (?);", "test_value");

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT description FROM tasks WHERE description = 'test_value';", -1, &stmt, NULL);
    const int rc = sqlite3_step(stmt);

    CU_ASSERT(rc == SQLITE_ROW);

    sqlite3_finalize(stmt);


    close_database(db);
}

void test_execute_sql_int_param() {
    sqlite3 *db;
    open_database(&db, "test.db");

    execute_sql_int_param(db, "UPDATE tasks SET status = (?) WHERE task_id = 1;", 1);

    sqlite3_stmt *stmt;
    sqlite3_prepare_v2(db, "SELECT status FROM tasks WHERE task_id = 1;", -1, &stmt, NULL);
    sqlite3_step(stmt);
    const int status = sqlite3_column_int(stmt, 0);
    sqlite3_finalize(stmt);

    CU_ASSERT(status == 1);

    close_database(db);
}

void test_get_int_value() {
    sqlite3 *db;
    open_database(&db, "test.db");
    execute_sql(db, "INSERT INTO tasks (description, status) VALUES ('Test Task', 1);");

    const int value = get_int_value(db, "SELECT task_id FROM tasks WHERE description='Test Task';");
    CU_ASSERT(value > 0);

    close_database(db);
}

void cleanup() {
    remove("test.db");
}

int main() {
    CU_initialize_registry();
    CU_pSuite pSuite = CU_add_suite("Database Tests", 0, cleanup);

    CU_add_test(pSuite, "Test Execute SQL", test_execute_sql);
    CU_add_test(pSuite, "Test Execute SQL with Text Param", test_execute_sql_text_param);
    CU_add_test(pSuite, "Test Execute SQL with Int Param", test_execute_sql_int_param);
    CU_add_test(pSuite, "Test Get Int Value", test_get_int_value);

    CU_basic_run_tests();
    CU_cleanup_registry();
    return 0;
}
