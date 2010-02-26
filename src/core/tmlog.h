/***************************************************************************
 tmlog.h
 Display log message for specified severity (even if chained)
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber 
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



#ifndef _TM_LOG_H_
#define _TM_LOG_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/version.h>


class tmLogGuiSeverity : public wxLogGui {
private:
	wxLogLevel m_LogLevel;
	
public:
	tmLogGuiSeverity(wxLogLevel minlevel = wxLOG_Warning){m_LogLevel = minlevel;}
	
	
#if wxCHECK_VERSION(2, 9, 0)
	virtual void DoLogRecord(wxLogLevel level,
                             const wxString& msg,
                             const wxLogRecordInfo& info){
		if (level <= m_LogLevel) { 
			wxLogGui::DoLogRecord(level, msg, info);
		}
	}
	
	
#else
	// support version 2.8.x
	virtual void DoLog(wxLogLevel level, const wxChar *msg, time_t timestamp) { 
		if (level <= m_LogLevel) { 
			// display wxLogWarning in a friendly way to the user. 
			wxLogGui::DoLog(level, msg, timestamp); 
		}
	}
#endif
};
	
	
	
	
	
#endif
