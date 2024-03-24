#include "Database.h"
#include <iostream>

Database::Database()
{
    database = new SQLite::Database("EmergencyRouting.db3", SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);
};

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

    // Commit Transaction
    transaction.commit();

    GeneratePsuedoData();
};

void Database::AddListener(DatabaseListener *listener)
{
    listeners.push_back(listener);
}

// void Database::RemoveListener(DatabaseListener *listener)
// {
//     listeners.erase(remove(listeners.begin(), listeners.end(), listener), listeners.end());
// }

void Database::NotifyListeners()
{
    for (auto listener : listeners)
    {
        listener->OnDatabaseChange();
    }
}

void Database::UpdateRecord(const string &tableName, const vector<string> &columns, const vector<string> &values, const string &conditions)
{
    if (columns.size() != values.size())
    {
        throw invalid_argument("Must have same number of values as columns!");
    }

    SQLite::Transaction transaction(*database);

    stringstream query;
    query << "UPDATE " << tableName << " SET ";

    for (size_t i = 0; i < columns.size(); i++)
    {
        query << columns[i] << " = '" << values[i] << "'";
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
    updateStatement.exec();

    transaction.commit();

    NotifyListeners();
};

void Database::InsertRecord(const string &tableName, const vector<string> &values)
{
    stringstream query;
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
    database->exec("INSERT INTO ambulance VALUES (9, \"(60, 80)\", 1, 1, 0)");

    // Commit Transaction
    psuedoDataTransaction.commit();
};

pair<int, int> Database::ConvertLocation(const string &location)
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
    istringstream iss(location.substr(1, location.size() - 2)); // Exclude parentheses
    if (!(iss >> x >> discard >> y) || discard != ',')
    {
        // Handle extraction failure
        throw std::runtime_error("Invalid location format: " + location);
    }

    return {x, y};
};

vector<Emergency> Database::GetEmergencies()
{
    vector<Emergency> emergencies;
    SQLite::Statement query(*database, "SELECT * FROM emergencies");
    while (query.executeStep())
    {
        pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool respondedTo = query.getColumn(3).getInt() != 0;
        bool complete = query.getColumn(4).getInt() != 0;

        emergencies.push_back(Emergency((int)query.getColumn(0).getInt(), location, (int)query.getColumn(2), (string)query.getColumn(5), respondedTo, complete));
    }

    return emergencies;
};

vector<Emergency> Database::GetUnRespondedEmergencies()
{
    vector<Emergency> emergencies;
    SQLite::Statement query(*database, "SELECT * FROM emergencies WHERE respondedTo = False");
    while (query.executeStep())
    {
        pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool respondedTo = query.getColumn(3).getInt() != 0;
        bool complete = query.getColumn(4).getInt() != 0;

        emergencies.push_back(Emergency((int)query.getColumn(0).getInt(), location, (int)query.getColumn(2), (string)query.getColumn(5), respondedTo, complete));
    }

    return emergencies;
};

vector<Ambulance> Database::GetAmbulances()
{
    vector<Ambulance> ambulances;
    SQLite::Statement query(*database, "SELECT * FROM ambulance");
    while (query.executeStep())
    {
        pair<int, int> location = ConvertLocation((string)query.getColumn(1));
        bool status = query.getColumn(2).getInt() != 0;
        bool available = query.getColumn(3).getInt() != 0;
        ambulances.push_back(Ambulance(query.getColumn(0), location, status, available));
    }

    return ambulances;
};