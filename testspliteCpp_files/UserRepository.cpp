#include "UserRepository.h"

#include "Statement.h"

#include <stdexcept>

UserRepository::UserRepository(Database& db)
	: db_(db)
{
}

void UserRepository::insert(const User& user)
{
	static const std::string sql =
		"INSERT INTO users (name, age) VALUES (?, ?);";

	Statement stmt(db_.get(), sql);

	sqlite3_bind_text(
		stmt.get(),            // Statement preparado (sqlite3_stmt*)
		1,                     // Índice del parámetro SQL (? empieza en 1, no en 0)
		user.name.c_str(),     // Puntero al texto C (char*) del std::string
		-1,                    // Longitud del texto:
		//  -1 significa que SQLite calculará la longitud
		//  usando strlen() (espera texto terminado en '\0')
		SQLITE_TRANSIENT       // Indica que SQLite debe hacer una COPIA del texto
							   //  porque el buffer original puede desaparecer
	);

	sqlite3_bind_int(
		stmt.get(),   // Statement preparado (sqlite3_stmt*)
		2,            // Índice del parámetro SQL:
		//  corresponde al SEGUNDO '?' del SQL
		user.age      // Valor entero que se va a insertar en la columna
	);

	if (sqlite3_step(stmt.get()) != SQLITE_DONE)
	{
		throw std::runtime_error("Insert failed");
	}
}

std::vector<User> UserRepository::getAll()
{
	static const std::string sql =
		"SELECT id, name, age FROM users;";

	Statement stmt(db_.get(), sql);
	std::vector<User> users;

	while (sqlite3_step(stmt.get()) == SQLITE_ROW)
	{
		User user;

		user.id = sqlite3_column_int(stmt.get(), 0);

		const unsigned char* raw =
			sqlite3_column_text(stmt.get(), 1);

		user.name = raw
			? reinterpret_cast<const char*>(raw)
			: "";

		user.age = sqlite3_column_int(stmt.get(), 2);

		users.push_back(std::move(user));
	}

	return users;
}
