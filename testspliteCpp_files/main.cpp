
#include <iostream>
#include <string>
#include <stdexcept>

// Clases propias
#include "Database.h"
#include "UserRepository.h"
#include "User.h"

int main()
{
    try
    {
        // 1 Abrir (o crear) la base de datos
        Database db("users.db");

        // 2 Crear la tabla si no existe
        const std::string createTableSql =
            "CREATE TABLE IF NOT EXISTS users ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
            "name TEXT NOT NULL, "
            "age INTEGER"
            ");";

        db.execute(createTableSql);

        // 3 Crear el repositorio
        UserRepository repo(db);

        // 4 INSERT (id se ignora, SQLite lo genera)
        repo.insert({ 0, "Ana", 30 });
        repo.insert({ 0, "Luis", 25 });
        repo.insert({ 0, "Marta", 40 });

        // 5 SELECT
        auto users = repo.getAll();

        // 6 Mostrar resultados
        for (const auto& u : users)
        {
            std::cout << u.id << " | "
                << u.name << " | "
                << u.age << '\n';
        }
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}

// sqlite3_lib project must be added as reference to this one project 

