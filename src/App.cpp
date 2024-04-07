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

#include "App.h"
#include "MainFrame.h"
#include "UI/Dialogs/LoginDialog.h"
#include "Server/Database.h"
#include "Types/User/User.h"
#include <memory>

wxIMPLEMENT_APP(App);

void App::Restart()
{

    // Destroy the main frame
    wxWindow *mainFrame = wxTheApp->GetTopWindow();
    if (mainFrame)
    {
        delete mainFrame;
    }

    // Reinitialize the application
    OnInit();
}

App &App::GetInstance()
{
    static App instance;
    return instance;
}

bool App::OnInit()
{
    // Initialise the Database - Using Smart Pointers here to avoid manual memory cleanup, although later on I create a Raw Pointer but its the thought that counts.
    unique_ptr<Database> database = make_unique<Database>();
    if (database->GetUserCount() == 0)
    {
        database->InitializeDatabase();
    }

    // Setup the GUI
    unique_ptr<MainFrame> mainFrame = make_unique<MainFrame>("PriorityEMS");
    mainFrame->SetClientSize(800, 600);
    mainFrame->SetMinSize(mainFrame->GetSize());
    mainFrame->SetMaxSize(mainFrame->GetSize());
    mainFrame->Center();
    mainFrame->Show();

    // Display the Login Modal allowing the user to log into the app, this process will result in the user been shown the correct dashboard based on their credentials.
    LoginDialog login(mainFrame.get(), wxID_ANY, _("Login"));
    login.Center();
    /*
    1. Create a User instance with the inputted username and password
    2. Call the User Login Method, this will determione if the input data is correct
    3. Set an active user instance within MainFrame, this is then handled and user is created with correct class i.e. CallHandler, EmergencyResponder
    */
    bool loggedIn = false;
    User user((std::string)login.getUsername());
    while (!loggedIn)
    {
        if (login.ShowModal() == wxID_OK)
        {
            if (user.Login((std::string)login.getUsername(), (std::string)login.getPassword()))
            {
                loggedIn = true;
                mainFrame->SetViewType(user.getRole());
                mainFrame->SetActiveUser(user);
                mainFrame->Initialize();
            }
            else
            {
                wxMessageBox("Login Credentials Incorrect!", "Login Failed", wxOK | wxICON_ERROR, mainFrame.get());
            }
        }
    };

    MainFrame *rawMainFrame = mainFrame.release();

    return true;
}
