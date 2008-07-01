/***************************************************************************
								tmprogress.cpp
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
 @file tmprogress.cpp
 @brief GIS progress indicator
 @details This file contain the class used for describing the progress indicator
 (#tmProgressIndicator) This indicator is threaded and mainly used during all
 GIS jobs taking times like opening files, drawing files,...
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/


#include "tmprogress.h"


/***************************************************************************//**
 @brief Init class members with default values
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
void tmProgressIndicator::InitMembers()
{
	m_MessageTarget = NULL;
	m_MessageID = TMPROGRESS_LOAD_PRJ;
}


/***************************************************************************//**
 @brief Default constructor
 @details Init internal members. 
 @note Don't forget to call the tmProgressIndicator::SetProgressTarget()
 function, otherwise nothing will be possible
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
tmProgressIndicator::tmProgressIndicator()
{
	InitMembers();
}


/***************************************************************************//**
 @brief Advanced constructor
 @details Init internal members and specify a target.
 @param status pointer to a valid Statusbar containing enought fields, see
 #TMPROGRESS_STATUS_FIELD
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
tmProgressIndicator::tmProgressIndicator(wxStatusBar * status)
{
	InitMembers();
	SetProgressTarget(status);
}


/***************************************************************************//**
 @brief Default destructor
 @details do nothing for the moment
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
tmProgressIndicator::~tmProgressIndicator()
{

}


/***************************************************************************//**
 @brief Setting target for progress indicator
 @details This function is checking internally if the status bar passed exists
 and contain the number of required fields (#TMPROGRESS_STATUS_FIELD)
 @param status Pointer to a wxStatusBar, checks are done to ensure validity of
 target
 @return  TRUE if target was changed, false if target isn't correct. In this
 case, the target isn't changed.
 @author Lucien Schreiber (c) CREALP 2008
 @date 01 July 2008
 *******************************************************************************/
bool tmProgressIndicator::SetProgressTarget (wxStatusBar * status)
{
	// status must not be null
	if (!status)
	{
		wxLogDebug(_T("Pointer to status is empty, error"));
		return FALSE;
	}
	
	// do we have enought seperator in the status
	if (status->GetFieldsCount() >= TMPROGRESS_STATUS_FIELD)
	{
		wxLogDebug(_T("Not enought fields in the status bar"));
		return FALSE;
	}
	
	m_MessageTarget = status;
	return TRUE;
}


