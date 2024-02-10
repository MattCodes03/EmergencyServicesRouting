#pragma once
#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include "../Types/Nodes/Emergency.h"
#include "../Types/Nodes/Ambulance.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class Database
{
public:
    Database();
    SQLite::Database *database;

    void InitializeDatabase();

    vector<Emergency> GetEmergencies();
    vector<Ambulance> GetAmbulances();

private:
    void GeneratePsuedoData();

    pair<int, int> ConvertLocation(const string &location);
};