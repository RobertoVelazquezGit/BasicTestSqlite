#pragma once

#include <string>
#include "sqlite3.h"

class Statement
{
public:
    Statement(sqlite3* db, const std::string& sql);
    ~Statement();

    sqlite3_stmt* get() const;

private:
    sqlite3_stmt* stmt_{ nullptr };
};

