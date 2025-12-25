#include <iostream>
#include <string>
#include "sqlite3.h"

int main()
{
    sqlite3* db = nullptr;

    if (sqlite3_open("users.db", &db) != SQLITE_OK)
    {
        std::cerr << "Error opening database\n";
        return 1;
    }

    const char* createTableSql =
        "CREATE TABLE IF NOT EXISTS users ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "name TEXT NOT NULL,"
        "age INTEGER);";

    sqlite3_exec(db, createTableSql, nullptr, nullptr, nullptr);

    const char* insertSql =
        "INSERT INTO users (name, age) VALUES "
        "('Ana', 30),"
        "('Luis', 25),"
        "('Marta', 40);";

    sqlite3_exec(db, insertSql, nullptr, nullptr, nullptr);

    const char* selectSql =
        "SELECT id, name, age FROM users;";

    sqlite3_stmt* stmt = nullptr;

    sqlite3_prepare_v2(db, selectSql, -1, &stmt, nullptr);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        std::string name =
            reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        int age = sqlite3_column_int(stmt, 2);

        std::cout << id << " | "
            << name << " | "
            << age << '\n';
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}







