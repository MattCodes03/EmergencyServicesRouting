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

#include "Database.h"
#include <iostream>

Database::Database()
{
    database = new SQLite::Database("EmergencyRouting.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
};

// Function will count number of users in user table and return a count. Used to determine if the database is populated
int Database::GetUserCount()
{
    int count = 0;
    try
    {
        SQLite::Statement query(*database, "SELECT COUNT(*) FROM users");
        if (query.executeStep())
        {
            count = query.getColumn(0).getInt();
        }
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Error getting user count: " << ex.what() << std::endl;
    }
    return count;
}

// Setup Database Tables
void Database::InitializeDatabase()
{
    database->exec("DROP TABLE IF EXISTS users");
    database->exec("DROP TABLE IF EXISTS emergencies");
    database->exec("DROP TABLE IF EXISTS ambulance");

    // Start Transaction
    SQLite::Transaction transaction(*database);

    // Setup Tables
    // User Table
    database->exec("CREATE TABLE users (userID INTEGER PRIMARY KEY, firstname VARCHAR, lastname VARCHAR, username VARCHAR, password VARCHAR, role VARCHAR)");

    // Emergencies Table
    database->exec("CREATE TABLE emergencies (emergencyID INTEGER PRIMARY KEY, location VARCHAR ,priority INTEGER, respondedTo INTEGER, complete INTEGER, description VARCHAR)");

    // Ambulance Table
    database->exec("CREATE TABLE ambulance (unitNumber INTEGER PRIMARY KEY, location VARCHAR, status INTEGER, available INTEGER, active_emergency INTEGER, CONSTRAINT fk_active_emergency FOREIGN KEY(active_emergency) REFERENCES emergencies(emergencyID))");

    database->exec("CREATE TABLE hospital (hospitalNumber INTEGER PRIMARY KEY, name VARCHAR, location VARCHAR, status INTEGER, pin INTEGER)");

    // Commit Transaction
    transaction.commit();

    GeneratePsuedoData();
};

/*
Function will add listener to database.
@param  listener - Pointer to object that will listen to the database

 */
void Database::AddListener(DatabaseListener *listener)
{
    listeners.push_back(listener);
}

// Function will send a notification to all database listeners that the database has changed
void Database::NotifyListeners()
{
    for (auto listener : listeners)
    {
        listener->OnDatabaseChange();
    }
}

/*
Function will update chosen values in the database
@param  tableName - Name of table that is being updated
@param  columns - Vector of column(s) that will be updated
@param  values - Vector of new value(s)
@param  conditions - Condition of update
 */
void Database::UpdateRecord(const std::string &tableName, const std::vector<std::string> &columns, const std::vector<std::string> &values, const std::string &conditions)
{
    if (columns.size() != values.size())
    {
        throw invalid_argument("Must have same number of values as columns!");
    }

    SQLite::Transaction transaction(*database);

    std::stringstream query;
    query << "UPDATE " << tableName << " SET ";

    for (size_t i = 0; i < columns.size(); i++)
    {
        query << columns[i] << " = ?";
        if (i < columns.size() - 1)
        {
            query << ", ";
        }
    }

    if (!conditions.empty())
    {
        query << " WHERE " << conditions;
    }

    SQLite::Statement updateStatement(*database, query.str());

    // Bind values to the statement
    for (size_t i = 0; i < values.size(); i++)
    {
        updateStatement.bind(i + 1, values[i]);
    }

    updateStatement.exec();

    transaction.commit();

    NotifyListeners();
};

/*
Function will insert new values in the database
@param  tableName - Name of table that is being updated
@param  values - Vector of new value(s)
 */
void Database::InsertRecord(const std::string &tableName, const std::vector<std::string> &values)
{
    std::stringstream query;
    query << "INSERT INTO" << tableName << "VALUES (";
    for (size_t i = 0; i < values.size(); i++)
    {
        query << values[i];
        if (i < values.size() - 1)
        {
            query << ", ";
        }
    }

    query << ")";

    SQLite::Statement insertStatement(*database, query.str());
    insertStatement.exec();
};

// This function is used solely for testing purposes as in release data will be real and accurate.
void Database::GeneratePsuedoData()
{
    SQLite::Transaction psuedoDataTransaction(*database);
    // Fake Users
    database->exec("INSERT INTO users VALUES (1, \"Matthew\", \"McCann\", \"MMcCann0210\", \"Test123\", \"HANDLER\")");
    database->exec("INSERT INTO users VALUES (2, \"Daniel\", \"Whyte\", \"DWhyte2209\", \"Test123\", \"RESPONDER\")");
    database->exec("INSERT INTO users VALUES (3, \"Taylor\", \"Hay\", \"THay1402\", \"Test123\", \"HOSPITAL\")");

    // Fake Emergencies - Update this with better data once app development is further along.
    database->exec("INSERT INTO emergencies VALUES (1, \"(40, 150)\", 1, 0, 0, \"Test1\")");
    database->exec("INSERT INTO emergencies VALUES (2, \"(200, 300)\", 1, 0, 0, \"Test2\")");
    database->exec("INSERT INTO emergencies VALUES (3, \"(50, 60)\", 1, 0, 0, \"Test3\")");

    // Fake Ambulances
    database->exec("INSERT INTO ambulance VALUES (8, \"(50, 50)\", 1, 1, 0)");
    database->exec("INSERT INTO ambulance VALUES (9, \"(100, 80)\", 1, 1, 0)");

    // Fake Hospitals
    database->exec("INSERT INTO hospital VALUES (15, \"Univeristy Hospital Crosshouse\", \"(60, 300)\", 1, 1234)");
    database->exec("INSERT INTO hospital VALUES (16, \"Glasgow Royal Infirmary\", \"(150, 260)\", 1, 1234)");
    database->exec("INSERT INTO hospital VALUES (17, \"Queen Elizabeth University Hospital\", \"(50, 260)\", 0, 1234)");

    // Commit Transaction
    psuedoDataTransaction.commit();
};

/*
Function will take location in format (X, Y) and convert it into a std::pair format for use
@param  location - (X, Y) location string
 */
std::pair<int, int> Database::ConvertLocation(const std::string &location)
{
    // Check if the format is correct
    if (location.size() < 5 || location[0] != '(' || location[location.size() - 1] != ')' || location.find(',') == string::npos)
    {
        // Handle invalid format
        throw std::runtime_error("Invalid location format: " + location);
    }

    // Extract coordinates from the format "(x, y)"
    int x, y;
    char discard;
    std::istringstream iss(location.substr(1, location.size() - 2)); // Exclude parentheses
    if (!(iss >> x >> discard >> y) || discard != ',')
    {
        // Handle extraction failure
        throw std::runtime_error("Invalid location format: " + location);
    }

    return {x, y};
};

// Function will return a vector of all emergencies in the database
std::vector<Emergency> Database::GetEmergencies()
{
    std::vector<Emergency> emergencies;
    SQLite::Statement query(*database, "SELECT * FROM emergencies");
    while (query.executeStep())
    {
        std::pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool respondedTo = query.getColumn(3).getInt() != 0;
        bool complete = query.getColumn(4).getInt() != 0;

        emergencies.push_back(Emergency((int)query.getColumn(0).getInt(), location, (int)query.getColumn(2), (string)query.getColumn(5), respondedTo, complete));
    }

    return emergencies;
};

// Function will return a vector of all emergencies in the database where respondedTo value is false
std::vector<Emergency> Database::GetUnRespondedEmergencies()
{
    std::vector<Emergency> emergencies;
    SQLite::Statement query(*database, "SELECT * FROM emergencies WHERE respondedTo = False");
    while (query.executeStep())
    {
        std::pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool respondedTo = query.getColumn(3).getInt() != 0;
        bool complete = query.getColumn(4).getInt() != 0;

        emergencies.push_back(Emergency((int)query.getColumn(0).getInt(), location, (int)query.getColumn(2), (string)query.getColumn(5), respondedTo, complete));
    }

    return emergencies;
};

// Function will return a vector of all ambulances in the database
std::vector<Ambulance> Database::GetAmbulances()
{
    std::vector<Ambulance> ambulances;
    SQLite::Statement query(*database, "SELECT * FROM ambulance");
    while (query.executeStep())
    {
        std::pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool status = query.getColumn(2).getInt() != 0;
        bool available = query.getColumn(3).getInt() != 0;
        ambulances.push_back(Ambulance(query.getColumn(0), location, status, available, query.getColumn(4).getInt()));
    }

    return ambulances;
};

// Function will return a vector of all hospitals in the database
std::vector<Hospital> Database::GetHospitals()
{
    std::vector<Hospital> hospitals;
    SQLite::Statement query(*database, "SELECT * FROM hospital");
    while (query.executeStep())
    {
        std::pair<int, int> location = ConvertLocation((string)query.getColumn(2));
        bool status = query.getColumn(3).getInt() != 0;
        hospitals.push_back(Hospital(query.getColumn(0), (string)query.getColumn(1), location, status));
    }

    return hospitals;
};