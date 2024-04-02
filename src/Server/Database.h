#ifndef DATABASE_H
#define DATABASE_H

#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>
#include "../Types/Nodes/Emergency.h"
#include "../Types/Nodes/Ambulance.h"
#include "../Types/Nodes/Hospital.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class DatabaseListener
{
public:
    virtual void OnDatabaseChange() = 0;
};

class Database
{
public:
    Database();
    SQLite::Database *database;

    void InitializeDatabase();
    int GetUserCount();

    void AddListener(DatabaseListener *listener);
    void RemoveListener(DatabaseListener *listener);
    void NotifyListeners();

    void UpdateRecord(const string &tableName, const vector<string> &columns, const vector<string> &values, const string &conditions);
    void InsertRecord(const string &tableName, const vector<string> &values);

    vector<Emergency> GetEmergencies();
    vector<Emergency> GetUnRespondedEmergencies();
    vector<Ambulance> GetAmbulances();
    vector<Hospital> GetHospitals();

    pair<int, int> ConvertLocation(const string &location);

private:
    void GeneratePsuedoData();
    vector<DatabaseListener *> listeners;
};

#endif