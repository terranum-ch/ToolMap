/***************************************************************************
							tmwindowposition.h
						Save and load windows position
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

// comment doxygen


#ifndef _TM_WINDOWS_POSITION_H_
#define _TM_WINDOWS_POSITION_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/fileconf.h>
#include <wx/tokenzr.h>



class tmWindowPosition : public wxObject
	{
	private:
		wxSize m_OldScreenSize;
		wxString m_ConfigAppName;
		//wxSize m_ScreenSize;
		//wxFileConfig m_Config;
		
		//void InitMemberValues();
		//bool WP_IsInited();
		wxString WP_PositionToString(wxRect pos);
		wxRect WP_StringToPosition(const wxString & posstring);
		bool WP_SaveScreenSize ();
		bool WP_LoadScreenSize ();
		wxRect WP_GetActualScreenSize();
		
	protected:
	public:
		tmWindowPosition(const wxString & appname = wxEmptyString);
		//tmWindowPosition(wxSize screensize);
		//void Create (wxSize screensize);
		~tmWindowPosition();
		
		bool LoadPosition(const wxString & wndname, wxRect & pos);
		bool SavePosition(const wxString & wndname, wxRect pos);
		bool SavePosition(const wxString & wndname, const wxString & postext);
		bool LoadPosition(const wxString & wndname, wxString & postext);
		
		bool SaveScreenPosition();
		
		bool HasScreenChanged();
		bool Intersects (wxRect wndpos, wxSize screensize);
};






#endif
