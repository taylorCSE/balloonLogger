/**
 * \file SettingsFrame.cpp
 * \brief Settings Settings Class
 */
 
#include "SettingsFrame.h"

BEGIN_EVENT_TABLE(SettingsFrame,wxFrame)
    EVT_BUTTON(ID_OK,  SettingsFrame::OnOk)
    EVT_BUTTON(ID_CANCEL,  SettingsFrame::OnCancel)
END_EVENT_TABLE()

/**
   Constructor
*/

SettingsFrame::SettingsFrame()
: wxFrame(NULL,-1, wxT(""), wxDefaultPosition, wxSize(360,300), 
          wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | 
          wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX) {
    CreateGUIControls();
    
    SetTransparent(245);
}

/** 
   Destructor
*/

SettingsFrame::~SettingsFrame() {
}

/**
   Creates all of the GUI controls
*/
    
void SettingsFrame::CreateGUIControls() {
    // Set window properties and title bar
    SetTitle(wxT("Settings"));
    SetIcon(wxNullIcon);
    
    // Get port list
    COMM_GetAvailablePorts(availablePorts);
    
    wxString ports[64];
    
    for(unsigned int i = 0; i < availablePorts.size(); i++) {
        ports[i] = availablePorts[i];
    }
    
    // Create main panel and configure sizer
    mainPanel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    mainPanel->SetSizer(mainSizer);
    
    // Create text entry controls
    dbHost = new wxTextCtrl(mainPanel,-1,DB_HOST);
    dbName = new wxTextCtrl(mainPanel,-1,DB_NAME);
    dbUser = new wxTextCtrl(mainPanel,-1,DB_USER);
    dbPass = new wxTextCtrl(mainPanel,-1,DB_PASS);
    
    commPort = new wxChoice(mainPanel, -1, wxDefaultPosition, wxDefaultSize, availablePorts.size(), ports);
    
    // Create static text labels
    wxStaticText* db_host_label = 
        new wxStaticText(mainPanel,-1,wxT("Database Hostname"));
    wxStaticText* db_name_label = 
        new wxStaticText(mainPanel,-1,wxT("Database Name"));
    wxStaticText* db_user_label = 
        new wxStaticText(mainPanel,-1,wxT("Database Username"));
    wxStaticText* db_pass_label = 
        new wxStaticText(mainPanel,-1,wxT("Database Password"));
    wxStaticText* comm_port_label = 
        new wxStaticText(mainPanel,-1,wxT("Serial Port"));
    
    // Create the buttons
    wxButton* ok_button = 
        new wxButton(mainPanel,ID_OK,wxT("Ok"));
    wxButton* cancel_button = 
        new wxButton(mainPanel,ID_CANCEL,wxT("Cancel"));
    
    // Place everything in the sizer
    mainSizer->Add(db_host_label, 0);
    mainSizer->Add(dbHost, 0);
    mainSizer->Add(db_name_label, 0);
    mainSizer->Add(dbName, 0);
    mainSizer->Add(db_user_label, 0);
    mainSizer->Add(dbUser, 0);
    mainSizer->Add(db_pass_label, 0);
    mainSizer->Add(dbPass, 0);
    mainSizer->Add(comm_port_label, 0);
    mainSizer->Add(commPort, 0);
    mainSizer->Add(buttonSizer, 0);
    buttonSizer->Add(ok_button, 0);
    buttonSizer->Add(cancel_button, 0);

    // Update and arrange
    Update();
    mainSizer->Fit(this);
}

/**
    Update
    
    This doesn't do anything at the moment, but must exist in order for 
    this class to properly extend BaseFrame. It still gets called by a
    timer event.
*/

void SettingsFrame::Update() {
}

/**
    Handle OK button
*/

void SettingsFrame::OnOk( wxCommandEvent& event ) {
    // Set global information based on the field values
    
    DB_HOST = dbHost->GetValue();
    DB_NAME = dbName->GetValue();
    DB_USER = dbUser->GetValue();
    DB_PASS = dbPass->GetValue();
    
    COMM_open(availablePorts[commPort->GetCurrentSelection()]);

    // Reconnect to the database
    DB_connect();
    
    // Close the window
    Destroy();
}

/**
    Handle Cancel
*/

void SettingsFrame::OnCancel( wxCommandEvent& event ) {
    Destroy();
}