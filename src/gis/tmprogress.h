/***************************************************************************
								tmprogress.h
                    GIS : Used to display progress during work
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/***************************************************************************//**
 @file tmprogress.h
 @brief GIS progress indicator
 @details This file contain the class used for describing the progress indicator
 (#tmProgressIndicator) This indicator is threaded and mainly used during all
 GIS jobs taking times like opening files, drawing files,...
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/


#ifndef _TMPROGRESS_H
#define _TMPROGRESS_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

// forward declaration
class tmProgressIndicatorThread;


// PARAMETERS 
static const int TMPROGRESS_STATUS_FIELD = 2;
static const wxString TMPROGRESS_INDICATOR_CHAR = _T("."); 
static const int TMPROGRESS_NUMBER_PROGRESS_INDICATOR = 10;
static const int TMPROGRESS_WAIT_BETWEEN_INDICATOR = 200;

/***************************************************************************//**
 @brief Message used by #tmProgressIndicator
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
static wxString TMPROGRESS_MESSAGES_STRING[] = 
{
	_T(""),
	_("Loading project. Please wait"),
	_("Loading data. Please wait"),
	_("Drawing data. Please wait")
};
enum TMPROGRESS_MESSAGE_ID
{
	TMPROGRESS_EMPTY_MSG = 0,
	TMPROGRESS_LOAD_PRJ,
	TMPROGRESS_LOAD_DATA,
	TMPROGRESS_DRAW_DATA
};
static const int TMPROGRESS_MESSAGE_NUMBER = 4;




/***************************************************************************//**
 @brief Used to display the progress
 @details When a long job is in progress, call this class for displaying the
 progress. Actually this class only shows informations in the status bar but
 she may be enhanced later for displaying progress bar or other informations
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
class tmProgressIndicator : public wxObject
	{
	private:
		wxStatusBar * m_MessageTarget;
		TMPROGRESS_MESSAGE_ID m_MessageID;
		tmProgressIndicatorThread * m_Thread;
		
		// init function
		void InitMembers();
		
		// internal check function
		bool CheckTarget();
		bool CheckMessage();
		
	public:
		// ctor - dtor
		tmProgressIndicator();
		tmProgressIndicator(wxStatusBar * status);
		~tmProgressIndicator();
		
		// setting parameters 
		bool SetProgressTarget (wxStatusBar * status);
		void SetMessage (TMPROGRESS_MESSAGE_ID messageid);
		
		// displaying progress
		void DisplayProgress ();
		void DisplayProgress (const wxString & message);
		void StopProgress ();
	};


/***************************************************************************//**
 @brief Internal thread class for tmProgressIndicator
 @details @note Don't call this class directly, use instead tmProgressIndicator
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
class tmProgressIndicatorThread : public wxThread
	{
	private:
		wxString m_Message;
		wxStatusBar * m_Status;
		bool m_Stop;
		
	public:
		tmProgressIndicatorThread(const wxString & message, wxStatusBar * status) : 
		m_Message(message), m_Status(status), m_Stop(FALSE) {}
		virtual void * Entry();
		void StopThread (){m_Stop = TRUE;}
	};

#endif
