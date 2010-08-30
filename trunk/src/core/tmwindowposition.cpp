/***************************************************************************
								tmwindowposition.cpp
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

#include "tmwindowposition.h"


tmWindowPosition::tmWindowPosition()
{
	WP_LoadScreenSize();
	//InitMemberValues();
}



/*tmWindowPosition::tmWindowPosition(wxSize screensize)
{
	InitMemberValues();
	Create(screensize);
}*/


/*void tmWindowPosition::Create (wxSize screensize)
{
	m_ScreenSize = screensize;
}*/


/*void tmWindowPosition::InitMemberValues()
{
	//m_ScreenSize = wxDefaultSize;
}*/


tmWindowPosition::~tmWindowPosition()
{
	//WP_SaveScreenSize();
}


/*bool tmWindowPosition::WP_IsInited()
{
	if (m_ScreenSize != wxDefaultSize)
		return true;
	
	wxLogDebug(_T("Windows position not correctly inited"));
	
	return false;
}*/

bool tmWindowPosition::SaveScreenPosition()
{
	return WP_SaveScreenSize();
}

bool tmWindowPosition::LoadPosition(const wxString & wndname, wxRect & pos)
{
	wxString readedpos = wxEmptyString;
	if (LoadPosition(wndname, readedpos)==false)
		return false;
	
	pos =  WP_StringToPosition(readedpos);
	
	wxRect myActualSize = WP_GetActualScreenSize();
	if (wndname != _T("SCREEN") && HasScreenChanged()==true)
	{
		if (Intersects(pos, wxSize(myActualSize.GetWidth(), myActualSize.GetHeight()))==false)
		{
			wxLogDebug(_T("Windows %s was outside screen"), wndname.c_str());
			return false;
		}
	}
	return true;
}


bool tmWindowPosition::LoadPosition(const wxString & wndname, wxString & postext)
{	
	wxFileConfig myConfig;
	myConfig.SetPath(_T("WINDOW_POSITION"));	
	if (myConfig.Read(wndname, &postext)==false)
	{
		wxLogMessage(_("No position stored for windows %s"), wndname.c_str());
		return false;
	}
	
	return true;
}



wxString tmWindowPosition::WP_PositionToString(wxRect pos)
{
	wxString mySPos = wxString::Format(_T("%d;%d;%d;%d"),
									   pos.GetX(),
									   pos.GetY(),
									   pos.GetWidth(),
									   pos.GetHeight());
	return mySPos;
}



wxRect tmWindowPosition::WP_StringToPosition(const wxString & posstring)
{
	wxStringTokenizer tokenizer(posstring, _T(";"));
	wxASSERT(tokenizer.CountTokens() == 4);

	long myTemp[] = {wxNOT_FOUND, wxNOT_FOUND, 
					wxNOT_FOUND, wxNOT_FOUND};
	wxRect myRect;
	
	for (int i = 0; i<4; i++)
    {
        tokenizer.GetNextToken().ToLong(&(myTemp[i]));
    }
	
	myRect.SetX(myTemp[0]);
	myRect.SetY(myTemp[1]);
	myRect.SetWidth(myTemp[2]);
	myRect.SetHeight(myTemp[3]);
	return myRect;
}




bool tmWindowPosition::SavePosition(const wxString & wndname, wxRect pos)
{
	return SavePosition(wndname, WP_PositionToString(pos));
}


bool tmWindowPosition::SavePosition(const wxString & wndname, const wxString & postext)
{
	wxString myStringPosition = wxEmptyString;
	wxFileConfig myConfig;
	
	myConfig.SetPath(_T("WINDOW_POSITION"));
	if (myConfig.Write(wndname, postext)==false)
	{
		wxLogDebug(_T("Unable to write to config file"));
		return false;
	}
	
	return true;
	
}



bool tmWindowPosition::WP_SaveScreenSize ()
{
	wxRect myWndSize = WP_GetActualScreenSize();
	return SavePosition(_T("SCREEN"), myWndSize);
}



bool tmWindowPosition::WP_LoadScreenSize ()
{
	wxRect myWndSize = WP_GetActualScreenSize();
	if (LoadPosition(_T("SCREEN"), myWndSize)==false)
	{
		m_OldScreenSize = wxSize(myWndSize.GetWidth(), myWndSize.GetHeight());
		return false;
	}
	
	m_OldScreenSize = wxSize(myWndSize.GetWidth(), myWndSize.GetHeight());
	return true;
}


bool tmWindowPosition::HasScreenChanged()
{
	wxRect myActualSize = WP_GetActualScreenSize();
	if (m_OldScreenSize != wxSize(myActualSize.GetWidth(), myActualSize.GetHeight()))
	{
		wxLogDebug(_T("Screen size has changed"));
		return true;
	}
	
	return false;
}




wxRect tmWindowPosition::WP_GetActualScreenSize()
{
	return wxRect(0,0,
				  wxSystemSettings::GetMetric(wxSYS_SCREEN_X , NULL),
				  wxSystemSettings::GetMetric(wxSYS_SCREEN_Y , NULL));
}
	

bool tmWindowPosition::Intersects (wxRect wndpos, wxSize screensize)
{
	wxRect myScreenSize = wxRect(wxPoint(0,0),screensize);
	return myScreenSize.Contains(wndpos);
}


		
