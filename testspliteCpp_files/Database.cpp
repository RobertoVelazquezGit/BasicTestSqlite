#include "Database.h"

#include <stdexcept>

Database::Database(const std::string& filename)
{
    if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK)
    {
        throw std::runtime_error("Cannot open database");
    }
}

Database::~Database()
{
    sqlite3_close(db_);
}

sqlite3* Database::get() const
{
    return db_;
}

void Database::execute(const std::string& sql)
{
    char* errMsg = nullptr;

    if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg ? errMsg : "Unknown error";
        sqlite3_free(errMsg);
        throw std::runtime_error(error);
    }
}
