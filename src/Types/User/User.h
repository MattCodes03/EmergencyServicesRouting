#pragma once
#include <string>
#include "../../Server/Database.h"

using namespace std;

class User
{
public:
    User(const string &username) : username(username) {}

    bool login(const string &username, const string &password);

    string getRole() const { return role; };
    string getUsername() const { return username; };

private:
    string username;
    string role = "";
};