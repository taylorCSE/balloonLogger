/**
 * \file MainFrame.cpp
 * \brief Main Main Class
 */
 
#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame,wxFrame)
    EVT_MENU(ID_NEWSETTINGS, BaseFrame::NewSettingsWindow)
    EVT_MENU(ID_ABOUT, BaseFrame::NewAboutWindow)
    EVT_TIMER(UPDATE_TIMER, BaseFrame::OnTimer)
    EVT_MENU(-1, BaseFrame::OnMenu)
    EVT_CLOSE(BaseFrame::OnClose)
END_EVENT_TABLE()

/**
   Constructor
*/

MainFrame::MainFrame()
: wxFrame(NULL,-1, wxT(""), wxDefaultPosition, wxSize(360,300), 
          wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | 
          wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX) {
    CreateGUIControls();
    
    SetTransparent(245);
}

/** 
   Destructor
*/

MainFrame::~MainFrame() {
}

/**
   Creates all of the GUI controls
*/
    
void MainFrame::CreateGUIControls() {
    // Set window properties and title bar
    SetTitle(wxT("Main"));
    SetIcon(wxNullIcon);
    
    // Create main panel and configure sizer
    mainPanel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);
    
    logPanel = new wxHtmlWindow(mainPanel);
    mainSizer->Add(logPanel, 1, wxEXPAND | wxALL);
    
    // Update and arrange
    Update();
}

/**
    Update
    
    This doesn't do anything at the moment, but must exist in order for 
    this class to properly extend BaseFrame. It still gets called by a
    timer event.
*/

void MainFrame::Update() {
    CreateMenu();
    
    logPanel->SetPage(wxString("Hello, World!"));
}

/**
    Creates the default menus
    
    Options can be provided to disable specific menus.
*/

void MainFrame::CreateMenu() {
    wxMenuBar* old_menubar = menubar;
    
    menubar = new wxMenuBar;

    window_menu = new wxMenu;
    menubar->Append(window_menu, wxT("&Options"));
    
    window_menu->Append(ID_NEWSETTINGS, wxT("Connection Settings..."));

    help_menu = new wxMenu;
    menubar->Append(help_menu, wxT("&Help"));
        
    help_menu->Append(ID_ABOUT, "About");
    
    SetMenuBar(menubar);

    if(old_menubar) delete old_menubar;

    CreateStatusBar(2);
    SetStatusText(wxString(DB_STATUS), 0);
    SetStatusText(wxString("Version:")+wxString(VERSION), 1);
}

void MainFrame::OnClose(wxCommandEvent& event) {
    Destroy();
}

/**
    Create a new Main frame.
    
    This allows BaseFrame to create a new MainFrame without knowing
    all the details of the class.
*/

wxFrame* NewMainFrame() {
    return (wxFrame*)(new MainFrame());
}

