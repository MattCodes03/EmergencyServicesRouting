#include "User.h"

class CallHandler : User
{
public:
    CallHandler(const string &username, const string &firstname, const string &lastname);

    void acceptEmergency();
    void prioritiseEmergency();
    void sendMessage();

    string firstname;
    string lastname;
};