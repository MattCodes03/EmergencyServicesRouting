#pragma once
#include <SQLiteCpp/SQLiteCpp.h>

using namespace std;

class Database
{
public:
    Database();
    SQLite::Database *database;

    void InitializeDatabase();

private:
    void GeneratePsuedoData();
};