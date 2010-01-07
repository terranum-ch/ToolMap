/***************************************************************************
								wxflatbutton.h
                    Creation of a flat button on every plateform
					uses wxButton flat or wxToggleButton (on mac)
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


#ifndef WXFLATBUTTON_H
#define WXFLATBUTTON_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/tglbtn.h>


#define wxFLATBUTTON_STYLE wxBORDER_NONE 
#define wxFLATBUTTON_SIZE wxSize(30,-1)
#define wxFLATBUTTON_TEXT_ADD _T("+")
#define wxFLATBUTTON_TEXT_REMOVE _T("-")

//#define ID_WXFLATBUTTON 10219

#if (__WXMAC__)
#define EVT_FLATBUTTON EVT_TOGGLEBUTTON
#define EVT_FLATBUTTON_CLICKED wxEVT_COMMAND_TOGGLEBUTTON_CLICKED

class wxFlatButton : public wxToggleButton
	{		
		
	public:
		
		wxFlatButton();
		wxFlatButton(wxWindow * parent, wxWindowID id, 
					 const wxString & label = wxEmptyString, 
					 wxSize size = wxFLATBUTTON_SIZE);
		~wxFlatButton();
		
	private:
		void OnClickToggleButton(wxCommandEvent & event);
		void InitwxFlatButton(wxWindowID id= wxID_ANY);
		DECLARE_DYNAMIC_CLASS(wxFlatButton)
		
		
	};
#else
#define EVT_FLATBUTTON EVT_BUTTON
#define EVT_FLATBUTTON_CLICKED wxEVT_COMMAND_BUTTON_CLICKED

class wxFlatButton : public wxButton
	{
	private:
		DECLARE_DYNAMIC_CLASS(wxFlatButton)
		void InitwxFlatButton();
		
	public:
		wxFlatButton();
		wxFlatButton(wxWindow * parent, wxWindowID id,
					 const wxString & label = wxEmptyString,
					 wxSize size = wxFLATBUTTON_SIZE);
		~wxFlatButton();
	};
#endif






#endif
