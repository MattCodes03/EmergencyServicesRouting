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

#include "User.h"
#include <string>

bool User::Login(const std::string &username, const std::string &password)
{
    // Get reference to Database
    std::unique_ptr<Database> db = std::make_unique<Database>();

    // Perform a Search through the Database to see if the entered Username exists, if it does we then compare the passwords.
    SQLite::Statement query(*db->database, "SELECT * FROM users WHERE username = ?");
    query.bind(1, username);

    while (query.executeStep())
    {
        std::string pass = query.getColumn(4);
        if (pass == password)
        {
            string role = query.getColumn(5);
            this->role = role;

            return true;
        }
    }

    return false;
}
