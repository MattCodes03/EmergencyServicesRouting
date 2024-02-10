#include "CallHandler.h"

CallHandler::CallHandler(const string &username, const string &firstname, const string &lastname) : User(username)
{
    this->username = username;
    this->firstname = firstname;
    this->lastname = lastname;
}

void CallHandler::AcceptEmergency()
{
}

void CallHandler::PrioritiseEmergency()
{
}

void CallHandler::SendMessage()
{
}
