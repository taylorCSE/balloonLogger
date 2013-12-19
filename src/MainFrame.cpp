/**
 * \file MainFrame.cpp
 * \brief Main Main Class
 */
 
#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame,wxFrame)
    EVT_MENU(ID_NEWSETTINGS, MainFrame::NewSettingsFrame)
    EVT_MENU(ID_ABOUT, MainFrame::NewAboutFrame)
    EVT_TIMER(UPDATE_TIMER, MainFrame::OnTimer)
    //EVT_MENU(-1, MainFrame::OnMenu)
    EVT_CLOSE(MainFrame::OnClose)
END_EVENT_TABLE()

/**
   Constructor
*/

MainFrame::MainFrame()
: wxFrame(NULL,-1, wxT(""), wxDefaultPosition, wxSize(360,300), 
          wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | 
          wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX) {
    CreateGUIControls();
    
    COMM_open("com6");

    updateTimer = new wxTimer(this, UPDATE_TIMER);

    updateTimer->Start(1000);
              
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
    SetTitle(wxT(wxString("BalloonGraph ") + wxString(VERSION)));
    SetIcon(wxNullIcon);
    
    // Create main panel and configure sizer
    mainPanel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);
    
    logPanel = new wxHtmlWindow(mainPanel);
    mainSizer->Add(logPanel, 1, wxEXPAND | wxALL);
    
    // Update and arrange
    CreateMenu();
    
    Update();    
}

/**
    Checks for new serial data and updates the display
*/

void MainFrame::Update() {
    SetStatusText(wxString(DB_STATUS), 0);
    SetStatusText(wxString(DB_FLIGHT_ID), 1);
    SetStatusText(wxString(COMM_PORT), 2);
    
    LOGGER_storeAvailablePackets();
    
    logPanel->SetPage("Bytes read: " + wxString::Format(wxT("%i"),LOGGER_state.bytesRead) + "<br>" + 
        "Packets read: " + wxString::Format(wxT("%i"),LOGGER_state.packetsRead) + "<br>" +
        "Last packet type: " + wxString::Format(wxT("%x"),LOGGER_state.lastCmd) + "<br>" +
        "Last device ID: " + wxString::Format(wxT("%u"),LOGGER_state.lastId) + "<br>" +
        "GPS Status: " + wxString::Format(wxT("%d"),LOGGER_state.gpsStatus) + "<br>" +
        "Altitude: " + wxString::Format(wxT("%d"),LOGGER_state.altitude) + "<br>" +
        "Rate: " + wxString::Format(wxT("%d"),LOGGER_state.rate) + "<br>"
    );
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
    
    window_menu->Append(ID_NEWSETTINGS, wxT("Settings..."));

    help_menu = new wxMenu;
    menubar->Append(help_menu, wxT("&Help"));
        
    help_menu->Append(ID_ABOUT, "About");
    
    SetMenuBar(menubar);

    if(old_menubar) delete old_menubar;

    CreateStatusBar(3);
}

void MainFrame::OnClose(wxCloseEvent& event) {
    Destroy();
}

/**
    Handles timer firing
*/

void MainFrame::OnTimer(wxTimerEvent& event) {
    Update();
}

void MainFrame::NewSettingsFrame( wxCommandEvent& event ) {
    wxFrame* frame = new SettingsFrame();
    frame->Show();     
}

void MainFrame::NewAboutFrame( wxCommandEvent& event ) {
    wxFrame* frame = new AboutFrame();
    frame->Show();     
}

wxFrame* NewMainFrame() {
    return (wxFrame*)(new MainFrame());
}



