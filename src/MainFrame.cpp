/**
 * \file MainFrame.cpp
 * \brief Main Main Class
 */
 
#include "MainFrame.h"

BEGIN_EVENT_TABLE(MainFrame,wxFrame)
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
    
    logPanel->SetPage(wxString("Real time logging goes here"));
}

/**
    Create a new Main frame.
    
    This allows BaseFrame to create a new MainFrame without knowing
    all the details of the class.
*/

wxFrame* NewMainFrame() {
    return (wxFrame*)(new MainFrame());
}

