#include "User.h"

class CallHandler : User
{
public:
    CallHandler(const string &username, const string &firstname, const string &lastname);

    void AcceptEmergency();
    void PrioritiseEmergency();
    void SendMessage();

    string GetName() { return username; };

    string firstname;
    string lastname;
    string username;
};