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

    const int rc = execute_sql_text_param(db, "INSERT INTO tasks (description) VALUES (?);", "test_value");
    CU_ASSERT(rc == SQLITE_DONE);

    close_database(db);
}


void test_execute_sql_int_param() {
    sqlite3 *db;
    open_database(&db, "test.db");

    const int rc = execute_sql_int_param(db, "UPDATE tasks SET status = (?) WHERE task_id = 1;", 1);
    CU_ASSERT(rc == SQLITE_DONE);

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
