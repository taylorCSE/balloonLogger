/**
 * \file MainFrame.h
 * \brief Main window header file
 */

#ifndef __MAINFRAME_h__
#define __MAINFRAME_h__

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include <wx/wx.h>
    #include <wx/frame.h>
#else
    #include <wx/wxprec.h>
#endif

#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/statusbr.h>
#include <wx/slider.h>
#include <wx/stattext.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/progdlg.h>
#include <wx/dcbuffer.h>
#include <wx/html/htmlwin.h>

#include "database.h"
#include "version.h"
#include "SettingsFrame.h"
#include "AboutFrame.h"

class MainFrame : public wxFrame
{
    public:
        MainFrame();
        virtual ~MainFrame();
        void CreateMenu();
    
        wxMenuBar *menubar;

    private:
        // Functions
        void CreateGUIControls();
        void Update();
        void OnOk( wxCommandEvent& event );
        void OnCancel( wxCommandEvent& event );
        void OnClose( wxCloseEvent& event );
        void OnTimer(wxTimerEvent& event);
        
        wxTimer *updateTimer;
    
        // wxWidgets gui objects
        wxPanel *mainPanel; 
        wxHtmlWindow *logPanel;
        wxBoxSizer *mainSizer;
        
        wxTextCtrl* dbHost;
        wxTextCtrl* dbName;
        wxTextCtrl* dbUser;
        wxTextCtrl* dbPass;
    
        wxMenu *window_menu;
        wxMenu *help_menu;

        // IDs fro various GUI elements
        DECLARE_EVENT_TABLE();
        /// IDs for various controls
        enum {
            ID_OK = 2000,
            ID_CANCEL,
            ID_NEWSETTINGS,
            ID_ABOUT,
            UPDATE_TIMER,
        };
};

#endif
