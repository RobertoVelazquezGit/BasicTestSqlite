#include "Statement.h"

#include <stdexcept>

Statement::Statement(sqlite3* db, const std::string& sql)
{
    if (
        sqlite3_prepare_v2(
            db,                 // Base de datos abierta (sqlite3*)
            // sobre la que se va a preparar el SQL

            sql.c_str(),        // Texto SQL en formato C (char*)
            // Ejemplo:
            // "INSERT INTO users (name, age) VALUES (?, ?);"

            -1,                 // Longitud del SQL en bytes:
            //  -1 significa que SQLite calculará la longitud
            //  usando strlen(), esperando un '\0' al final

            &stmt_,             // Puntero de salida:
            //  SQLite inicializa aquí el sqlite3_stmt*
            //  que representa el statement preparado

            nullptr             // Puntero al resto del SQL no procesado:
                               //  se usa solo cuando pasas múltiples sentencias
                               //  en un mismo string (no es nuestro caso)
        )
        != SQLITE_OK
        )
    {
        throw std::runtime_error("Failed to prepare statement");
    }
}


Statement::~Statement()
{
    sqlite3_finalize(stmt_);
}

sqlite3_stmt* Statement::get() const
{
    return stmt_;
}

