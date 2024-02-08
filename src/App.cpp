#include "App.h"
#include "MainFrame.h"
#include "UI/LoginDialog.h"
#include "Server/Database.h"
#include "Types/User.h"
#include <memory>

wxIMPLEMENT_APP(App);

using namespace std;

bool App::OnInit()
{
    // Initialise the Database - Using Smart Pointers here to prevent memory leaks.
    unique_ptr<Database> database = make_unique<Database>();
    database->InitializeDatabase();

    // Setup the GUI
    unique_ptr<MainFrame> mainFrame = make_unique<MainFrame>("Emergency Services Routing");
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

    MainFrame *rawMainFrame = mainFrame.release();

    return true;
}
