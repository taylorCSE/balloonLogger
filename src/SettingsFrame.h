/**
 * \file SettingsFrame.h
 * \brief Settings window header file
 */

#ifndef __SETTINGSFRAME_h__
#define __SETTINGSFRAME_h__

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

#include "BaseFrame.h"
#include "database.h"

class SettingsFrame : public wxFrame
{
    public:
        SettingsFrame();
        virtual ~SettingsFrame();

    private:
        // Event handlers
        DECLARE_EVENT_TABLE();

        // Functions
        void CreateGUIControls();
        void Update();
        void OnOk( wxCommandEvent& event );
        void OnCancel( wxCommandEvent& event );
        
        // wxWidgets gui objects
        wxPanel *mainPanel; 
        wxBoxSizer *mainSizer;
        
        wxTextCtrl* dbHost;
        wxTextCtrl* dbName;
        wxTextCtrl* dbUser;
        wxTextCtrl* dbPass;
        
    private:
        // IDs fro various GUI elements
        enum {
            ID_OK = 2000,
            ID_CANCEL,
        };
};

#endif
