#pragma once

#include <vector>

#include "Database.h"
#include "User.h"

class UserRepository
{
public:
    explicit UserRepository(Database& db);

    void insert(const User& user);
    std::vector<User> getAll();

private:
    Database& db_;
};

