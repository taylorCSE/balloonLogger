/**
 * \file BalloonLogger.cpp
 * \brief Main Application Code
 *
 * This is the entry point for the application.
 */

#include "BalloonLogger.h"
#include "MainFrame.h"

IMPLEMENT_APP(BalloonLogger)

/**
    Creates the main frame and shows it
*/

bool BalloonLogger::OnInit() {
    MainFrame* frame = new MainFrame();
    SetTopWindow(frame);
    frame->Show();
    return true;
}
 
/**
   Closes the application
*/

int BalloonLogger::OnExit() {
    return 0;
}
