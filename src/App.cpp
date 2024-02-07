#include "App.h"
#include "MainFrame.h"
#include "UI/LoginDialog.h"
#include "Server/Database.h"
#include "Types/User.h"
#include <memory>

wxIMPLEMENT_APP(App);

bool App::OnInit()
{
    // Initialise the Database
   std::unique_ptr<Database> database = std::make_unique<Database>();
   database->InitializeDatabase();

    // Setup the GUI
    std::unique_ptr<MainFrame> mainFrame = std::make_unique<MainFrame>("Emergency Services Routing");
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
    3. Create specific user i.e. CallHandler, EmergencyResponder and then run the software with access to this users methods. (NOT SURE HOW TO DO THIS YET!)
    */
    bool loggedIn = false;
    User user((string)login.getUsername());
    while (!loggedIn)
    {
        if (login.ShowModal() == wxID_OK)
        {
            if (user.login((string)login.getUsername(), (string)login.getPassword()))
            {
                loggedIn = true;
                mainFrame->SetViewType(user.getRole());
                mainFrame->SetActiveUser(user);
                mainFrame->Initialize();
            }
            else
            {
                cout << "Failed to login!" << endl;
            }
        }
    };

    MainFrame* rawMainFrame = mainFrame.release();

    return true;
}
