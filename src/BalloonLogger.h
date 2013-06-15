/**
 * \file App.h
 * \brief Main Application Header File
 */

#ifndef __BALLOONLOGGER_h__
#define __BALLOONLOGGER_h__

#ifdef __BORLANDC__
	#pragma hdrstop
#endif

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif

#define APP_NAME "BalloonLogger"

class BalloonLogger : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
