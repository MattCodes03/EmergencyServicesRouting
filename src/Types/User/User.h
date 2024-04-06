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

#ifndef USER_H
#define USER_H

#include <string>
#include "../../Server/Database.h"

using namespace std;

class User
{
public:
    User(){};
    User(const string &username) : username(username) {}

    bool Login(const string &username, const string &password);

    string getRole() const { return role; };
    string getUsername() const { return username; };

private:
    string username;
    string role = "";
};

#endif