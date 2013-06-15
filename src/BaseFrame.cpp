/**
 * \file BaseFrame.cpp
 * \brief Base frame Class
 */
 
#include "BaseFrame.h"

BEGIN_EVENT_TABLE(BaseFrame,wxFrame)
    EVT_MENU(ID_NEWSETTINGS, BaseFrame::NewSettingsWindow)
    EVT_MENU(ID_ABOUT, BaseFrame::NewAboutWindow)
    EVT_TIMER(UPDATE_TIMER, BaseFrame::OnTimer)
    EVT_MENU(-1, BaseFrame::OnMenu)
    EVT_CLOSE(BaseFrame::OnClose)
END_EVENT_TABLE()

/**
    Constructor
*/

BaseFrame::BaseFrame()
: wxFrame(NULL, -1, wxT("AppName"), wxDefaultPosition, wxSize(360,600), 
          wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | 
          wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX) {
    flightId = "";

    SetIcon(wxIcon(wxT("icon.xpm")));
    
    updateTimer = new wxTimer(this, UPDATE_TIMER);
    updateTimer->Start(5000);
    
    // Nullify the pointers
    menubar = 0x00;
    flight_menu = 0x00;
    view_menu = 0x00;
    window_menu = 0x00;

    view = VIEW_BASIC;
}

/**
    Deconstructor
*/

BaseFrame::~BaseFrame() {
}

/**
    Creates the default menus
    
    Options can be provided to disable specific menus.
*/

void BaseFrame::CreateMenu(bool show_view, 
                           bool show_window, 
                           bool show_flights,
                           bool show_help,
                           bool show_data) {
    wxMenuBar* old_menubar = menubar;
    
    menubar = new wxMenuBar;

    if(show_window) {
        window_menu = new wxMenu;
        menubar->Append(window_menu, wxT("&Options"));
        
        window_menu->Append(ID_NEWSETTINGS, wxT("Connection Settings..."));
    }

    if(show_help) {
        help_menu = new wxMenu;
        menubar->Append(help_menu, wxT("&Help"));
        
        help_menu->Append(ID_ABOUT, "About");
    }
    
    SetMenuBar(menubar);

    if(old_menubar) delete old_menubar;

    CreateStatusBar(2);
    SetStatusText(wxString(DB_STATUS), 0);
    SetStatusText(wxString("Version:")+wxString(VERSION), 1);
}

/** 
    Launches a new settings window
*/
     
void BaseFrame::NewSettingsWindow( wxCommandEvent& event ) {
    wxFrame* frame = NewSettingsFrame();
    frame->Show();     
}

/** 
    Launches a new about window
*/
     
void BaseFrame::NewAboutWindow( wxCommandEvent& event ) {
    wxFrame* frame = NewAboutFrame();
    frame->Show();     
}

/**
    Handles new flight selection event
*/

void BaseFrame::OnMenu( wxCommandEvent& event ) {
    int id = event.GetId();
    
    if(id >= 10000) {
        /// We're selecting a device
        flightId = flightIds[id - 10000];
    }

    if(id >= VIEW_BASIC && id <= VIEW_BEACON) {
        view = id;
    }
    
    Update();
}

/**
    Handles timer firing
*/

void BaseFrame::OnTimer(wxTimerEvent& event) {
    Update();
}


/**
   Exit the Program
*/

void BaseFrame::OnClose(wxCloseEvent& event) {
    if(updateTimer) {
        delete updateTimer;
        updateTimer = 0x00;
    }
    Destroy();
}
