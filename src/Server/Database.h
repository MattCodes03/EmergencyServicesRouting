/*
Copyright (c) 2024, Matthew McCann
All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to no conditions.
*/

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

    void UpdateRecord(const std::string &tableName, const std::vector<std::string> &columns, const std::vector<std::string> &values, const std::string &conditions);
    void InsertRecord(const std::string &tableName, const std::vector<std::string> &values);

    std::vector<Emergency> GetEmergencies();
    std::vector<Emergency> GetUnRespondedEmergencies();
    std::vector<Ambulance> GetAmbulances();
    std::vector<Hospital> GetHospitals();

    std::pair<int, int> ConvertLocation(const std::string &location);

private:
    void GeneratePsuedoData();
    vector<DatabaseListener *> listeners;
};

#endif