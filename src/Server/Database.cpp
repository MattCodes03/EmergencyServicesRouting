#include "Database.h"

Database::Database()
{
    database = new SQLite::Database("EmergencyRouting.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
};

void Database::InitializeDatabase()
{
    database->exec("DROP TABLE IF EXISTS users");

    // Start Transaction
    SQLite::Transaction transaction(*database);

    // Setup Tables
    // User Table
    database->exec("CREATE TABLE users (userID INTEGER PRIMARY KEY, firstname VARCHAR, lastname VARCHAR, username VARCHAR, password VARCHAR, role VARCHAR)");

    // Test Data
    database->exec("INSERT INTO users VALUES (1, \"Matthew\", \"McCann\", \"MMcCann0210\", \"Test123\", \"HANDLER\")");

    // Commit Transaction
    transaction.commit();
};

// This function is used solely for testing purposes as in release data will be real and accurate.
void Database::GeneratePsuedoData(){

};