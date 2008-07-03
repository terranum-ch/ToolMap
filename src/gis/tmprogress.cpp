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
	m_Thread = NULL;
}


/***************************************************************************//**
 @brief Internal checking function for Target
 @details This function checks that :
 - Target isn't NULL
 - Target contain enought fields
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
bool tmProgressIndicator::CheckTarget()
{
	// target not null
	if (!m_MessageTarget)
	{
		wxLogDebug(_T("Pointer to target is empty, error"));
		return FALSE;
	}
	
	// do we have enought seperator in the status
	if (m_MessageTarget->GetFieldsCount() < TMPROGRESS_STATUS_FIELD + 1)
	{
		wxLogDebug(_T("Not enought fields in the status bar : need %d, has %d"),
		TMPROGRESS_STATUS_FIELD + 1, m_MessageTarget->GetFieldsCount());
		return FALSE;
	}
	
	return TRUE;	
}

/***************************************************************************//**
 @brief Internal checking function for Messages
 @details This function checks that :
 - Message is in the limits
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
bool tmProgressIndicator::CheckMessage()
{
	
	if (m_MessageID < 0 || m_MessageID >= TMPROGRESS_MESSAGE_NUMBER -1)
	{
		wxLogDebug(_T("Message is out of the limits : %d"), m_MessageID);
		return FALSE;
	}
	return TRUE;
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
	// delete the thread if running
	StopProgress();
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
	m_MessageTarget = status;
	
	if(!CheckTarget())
		return FALSE;
		
	return TRUE;
}


/***************************************************************************//**
 @brief Select wich message to display
 @param messageid One of the message ID, see #TMPROGRESS_MESSAGE_NUMBER for
 options
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
void tmProgressIndicator::SetMessage (TMPROGRESS_MESSAGE_ID messageid)
{
	if(!CheckMessage())
		return;
	
	m_MessageID = messageid;
}


/***************************************************************************//**
 @brief Display the current progress message
 @details Verifications are done before displaying the progress message :
 - Target is valid
 - Message isn't empty
 One call this function securely
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
void tmProgressIndicator::DisplayProgress ()
{
	// only checking message, checking target is done in DisplayProgress()
	if (!CheckMessage())
		return;
	
	DisplayProgress(TMPROGRESS_MESSAGES_STRING[m_MessageID]);

}

/***************************************************************************//**
 @brief Display a personnal progress message
 @note Use preferably stock message (#TMPROGRESS_MESSAGES_STRING) to
 allow constant UI design
 @param message Message used for the progess
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
void tmProgressIndicator::DisplayProgress (const wxString & message)
{
	if (!CheckTarget())
		return;
	
		
	m_Thread = new tmProgressIndicatorThread(message, m_MessageTarget);
	if (m_Thread->Create() != wxTHREAD_NO_ERROR)
	{
		wxLogError(_T("Can't create thread for progress"));
		return;
	}
	m_Thread->Run();
}


/***************************************************************************//**
 @brief Stop the progress
 @note If no progress is running, this function is doing nothing.
 @author Lucien Schreiber (c) CREALP 2008
 @date 03 July 2008
 *******************************************************************************/
void tmProgressIndicator::StopProgress ()
{
	if(!m_Thread)
	{
		wxLogDebug(_T("No progress is running, not able to stop anything"));
		return;
	}
	
	m_Thread->StopThread();
	// thread will be deleted itself.
	m_MessageTarget->SetStatusText(TMPROGRESS_MESSAGES_STRING[TMPROGRESS_EMPTY_MSG],
								   TMPROGRESS_STATUS_FIELD);
		
}





/******************************* THREAD FONCTIONS *******************************/

/***************************************************************************//**
 @brief Entry function for the thread
 @author Lucien Schreiber (c) CREALP 2008
 @date 02 July 2008
 *******************************************************************************/
void * tmProgressIndicatorThread::Entry()
{
	wxString sProgress;
	while (1)
	{
		sProgress = _T("");
		
		for (int i= 0; i<TMPROGRESS_NUMBER_PROGRESS_INDICATOR; i++)
		{
			// exit thread
			if (m_Stop == TRUE)
				return NULL;
			
			sProgress.Append(TMPROGRESS_INDICATOR_CHAR); 
			m_Status->SetStatusText(m_Message + sProgress,
									TMPROGRESS_STATUS_FIELD);
			Sleep(TMPROGRESS_WAIT_BETWEEN_INDICATOR);
		}
	}
}


