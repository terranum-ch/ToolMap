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


#ifndef NAME_H
#define NAME_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


/***************************************************************************//**
 @brief Message used by #tmProgressIndicator
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
static wxString TMPROGRESS_MESSAGES_STRING[] = 
{
	_("Loading project. Please wait"),
	_("Loading data. Please wait"),
	_("Drawing data. Please wait")
};
enum TMPROGRESS_MESSAGE_ID
{
	TMPROGRESS_LOAD_PRJ = 0,
	TMPROGRESS_LOAD_DATA,
	TMPROGRESS_DRAW_DATA
};
static const int TMPROGRESS_MESSAGE_NUMBER = 3;


static const int TMPROGRESS_STATUS_FIELD = 2;


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
		
		// init function
		void InitMembers();
		
	public:
		// ctor - dtor
		tmProgressIndicator();
		tmProgressIndicator(wxStatusBar * status);
		~tmProgressIndicator();
		
		// setting target
		bool SetProgressTarget (wxStatusBar * status);
	};




#endif
