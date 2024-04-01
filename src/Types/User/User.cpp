#include "User.h"
#include <string>

using namespace std;

bool User::Login(const string &username, const string &password)
{
    // Get reference to Database
    std::unique_ptr<Database> db = std::make_unique<Database>();

    // Perform a Search through the Database to see if the entered Username exists, if it does we then compare the passwords.
    SQLite::Statement query(*db->database, "SELECT * FROM users WHERE username = ?");
    query.bind(1, username);

    while (query.executeStep())
    {
        string pass = query.getColumn(4);
        if (pass == password)
        {
            string role = query.getColumn(5);
            this->role = role;

            return true;
        }
    }

    return false;
}
