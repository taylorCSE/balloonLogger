/**
 * \file AboutFrame.cpp
 * \brief Main Windows Class
 */
 
#include "AboutFrame.h"

BEGIN_EVENT_TABLE(AboutFrame,wxFrame)
END_EVENT_TABLE()

/**
   Constructor for the Main frame.
*/

AboutFrame::AboutFrame()
: wxFrame(NULL,-1, wxT(""), wxDefaultPosition, wxSize(360,300), 
          wxCAPTION | wxRESIZE_BORDER | wxSYSTEM_MENU | 
          wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX) {
    CreateGUIControls();
    
    SetTransparent(245);
}

/** 
   Destructor for the Main form.
*/

AboutFrame::~AboutFrame() {
}

/**
   Creates all of the GUI controls on the main form.
*/
    
void AboutFrame::CreateGUIControls() {

    
    // Set window properties and title bar
    SetTitle(wxT("About"));
    SetIcon(wxNullIcon);
    
    mainPanel = new wxPanel(this, wxID_ANY);
    mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);
    
    info = new wxHtmlWindow(mainPanel);
    mainSizer->Add(info, 1, wxEXPAND | wxALL);

    info->SetPage(wxString::Format(wxT(""
        "<b>BalloonLogger</b>\n<br />"
        "Version: %s<br />"
        "Build %d <br />"
        "Commit %s<br />"),
        VERSION,
        VERSION_BUILD,
        VERSION_COMMIT
        ));

}

/**
    Create a new status frame
*/

wxFrame* NewAboutFrame() {
    return (wxFrame*)(new AboutFrame());
}

