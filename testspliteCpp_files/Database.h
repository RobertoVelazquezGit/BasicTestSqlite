#pragma once

#include <string>
#include "sqlite3.h"

class Database
{
public:
    explicit Database(const std::string& filename);
    ~Database();

    sqlite3* get() const;
    void execute(const std::string& sql);

private:
    sqlite3* db_{ nullptr };
};


