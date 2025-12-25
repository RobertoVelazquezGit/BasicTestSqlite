
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

#include "sqlite3.h"

// ---- Database -------------------------------------------------

class Database
{
public:
	explicit Database(const std::string& filename)
	{
		if (sqlite3_open(filename.c_str(), &db_) != SQLITE_OK)
		{
			throw std::runtime_error("Cannot open database");
		}
	}

	~Database()
	{
		sqlite3_close(db_);
	}

	sqlite3* get() const { return db_; }

	void execute(const std::string& sql)
	{
		char* errMsg = nullptr;

		if (sqlite3_exec(db_, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
		{
			std::string error = errMsg ? errMsg : "Unknown error";
			sqlite3_free(errMsg);
			throw std::runtime_error(error);
		}
	}

private:
	sqlite3* db_{ nullptr };
};

// ---- Statement ------------------------------------------------

class Statement
{
public:
	Statement(sqlite3* db, const std::string& sql)
	{
		if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt_, nullptr) != SQLITE_OK)
		{
			throw std::runtime_error("Failed to prepare statement");
		}
	}

	~Statement()
	{
		sqlite3_finalize(stmt_);
	}

	sqlite3_stmt* get() const { return stmt_; }

private:
	sqlite3_stmt* stmt_{ nullptr };
};

// ---- User -----------------------------------------------------

struct User
{
	int id{};
	std::string name;
	int age{};
};

// ---- UserRepository ------------------------------------------

class UserRepository
{
public:
	explicit UserRepository(Database& db)
		: db_(db)
	{
	}

	void insert(const User& user)
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

	std::vector<User> getAll()
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
				? std::string(reinterpret_cast<const char*>(raw))
				: std::string{};

			user.age = sqlite3_column_int(stmt.get(), 2);

			users.push_back(std::move(user));
		}

		return users;
	}

private:
	Database& db_;
};


int main()
{
	try
	{
		Database db("users.db");

		const std::string createTableSql =
			"CREATE TABLE IF NOT EXISTS users ("
			"id INTEGER PRIMARY KEY AUTOINCREMENT, "
			"name TEXT NOT NULL, "
			"age INTEGER"
			");";
		db.execute(createTableSql);

		UserRepository repo(db);

		// 1️⃣ INSERT (como en el ejemplo C)
		repo.insert({ 0, "Ana", 30 });
		repo.insert({ 0, "Luis", 25 });
		repo.insert({ 0, "Marta", 40 });

		// 2️⃣ SELECT (como en el ejemplo C)
		auto users = repo.getAll();

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
	}

	return 0;
}

